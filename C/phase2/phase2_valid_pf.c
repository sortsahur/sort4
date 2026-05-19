#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MEMSIZE 400
#define PAGESIZE 10
#define NFRAMES (MEMSIZE / PAGESIZE)

char R[4], IR[4];
int IC, C, SI, TI, PI;
char mem[MEMSIZE][4];
char buffer[41];
int PTR;
int TLC, TTL, LLC, TLL;
int frames[NFRAMES], frameidx;
FILE *inputfp, *outputfp;

void shuffleframes() {
    for (int i = 0; i < NFRAMES; i++) frames[i] = i;
    for (int i = NFRAMES - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int t = frames[i]; frames[i] = frames[j]; frames[j] = t;
    }
    frameidx = 0;
}

int allocate() {
    if (frameidx >= NFRAMES) { printf("ERROR: Out of frames\n"); return -1; }
    return frames[frameidx++];
}

void init() {
    IC = 0; C = 0; SI = 0; TI = 0; PI = 0;
    TLC = 0; LLC = 0;
    memset(R, 0, 4);
    memset(IR, 0, 4);
    for (int i = 0; i < MEMSIZE; i++)
        for (int j = 0; j < 4; j++)
            mem[i][j] = '-';

    shuffleframes();
    int ptframe = allocate();
    PTR = ptframe * PAGESIZE;
    printf("PTR = %d (Frame %d)\n", PTR, ptframe);
}

void setpagetable(int pageno, int frameno) {
    int entry = PTR + pageno;
    mem[entry][0] = '1';
    mem[entry][1] = ' ';
    mem[entry][2] = '0' + frameno / 10;
    mem[entry][3] = '0' + frameno % 10;
}

int getframe(int pageno) {
    int entry = PTR + pageno;
    if (mem[entry][0] != '1') return -1;
    return (mem[entry][2] - '0') * 10 + (mem[entry][3] - '0');
}

int addressmap(int va) {
    int pageno = va / PAGESIZE;
    int offset = va % PAGESIZE;
    int frameno = getframe(pageno);
    if (frameno == -1) return -1;
    return frameno * PAGESIZE + offset;
}

void displaymem() {
    printf("\n===== MEMORY DUMP =====\n");
    for (int i = 0; i < MEMSIZE; i++) {
        if (i % 10 == 0 && i != 0) printf("--------------------------------------------\n");
        printf("%3d    %c%c%c%c\n", i, mem[i][0], mem[i][1], mem[i][2], mem[i][3]);
    }
    printf("\n");
}

void terminateprog(const char *msg) {
    fprintf(outputfp, "%s\n", msg);
    fprintf(outputfp, "IC=%d SI=%d TI=%d PI=%d\n\n", IC, SI, TI, PI);
    printf("TERMINATE: %s\n", msg);
}

int handlePageFault(int va, int iswrite) {
    int pageno = va / PAGESIZE;
    if (iswrite || SI == 1) {
        int newframe = allocate();
        if (newframe == -1) { terminateprog("Out of memory"); return -1; }
        setpagetable(pageno, newframe);
        printf("Valid page fault handled: Page %d -> Frame %d\n", pageno, newframe);
        return newframe * PAGESIZE + (va % PAGESIZE);
    } else {
        terminateprog("Invalid Page Fault");
        return -1;
    }
}

void readcard() {
    char line[81];
    if (!fgets(line, 80, inputfp)) return;
    int len = strlen(line);
    if (len > 0 && line[len-1] == '\n') line[--len] = '\0';
    if (len > 0 && line[len-1] == '\r') line[--len] = '\0';

    char subword[5] = {0};
    strncpy(subword, line, 4);
    if (strcmp(subword, "$END") == 0) {
        terminateprog("Out of Data");
        IC = 9999;
        return;
    }

    int va = (IR[2] - '0') * 10 + (IR[3] - '0');
    int ra = addressmap(va);
    if (ra == -1) {
        ra = handlePageFault(va, 1);
        if (ra == -1) { IC = 9999; return; }
    }

    int loc = ra;
    int i = 0, m = 0;
    while (i < len) {
        if (m != 0 && m % 4 == 0) loc++;
        mem[loc][m % 4] = line[i];
        i++; m++;
    }
}

void writecard() {
    LLC++;
    if (LLC > TLL) { terminateprog("Line Limit Exceeded"); IC = 9999; return; }

    int va = (IR[2] - '0') * 10 + (IR[3] - '0');
    int ra = addressmap(va);
    if (ra == -1) {
        terminateprog("Invalid Page Fault");
        IC = 9999;
        return;
    }

    int loc = ra;
    for (int i = loc; i < loc + 10; i++) {
        for (int j = 0; j < 4; j++) {
            if (mem[i][j] != '-') fputc(mem[i][j], outputfp);
            else fputc(' ', outputfp);
        }
    }
    fputc('\n', outputfp);
}

void mastermode() {
    if (TLC >= TTL) TI = 2;

    if (TI == 0 && SI == 1) { SI = 0; readcard(); }
    else if (TI == 0 && SI == 2) { SI = 0; writecard(); }
    else if (TI == 0 && SI == 3) { terminateprog("Program terminated normally"); }
    else if (TI == 2 && SI == 1) { terminateprog("Time Limit Exceeded"); }
    else if (TI == 2 && SI == 2) { writecard(); terminateprog("Time Limit Exceeded"); }
    else if (TI == 2 && SI == 3) { terminateprog("Program terminated normally"); }
    else if (TI == 0 && PI == 1) { terminateprog("Opcode Error"); }
    else if (TI == 0 && PI == 2) { terminateprog("Operand Error"); }
    else if (TI == 2 && PI == 1) { terminateprog("Time Limit Exceeded + Opcode Error"); }
    else if (TI == 2 && PI == 2) { terminateprog("Time Limit Exceeded + Operand Error"); }
}

void executeprog() {
    int addr, ra;
    while (1) {
        if (TLC >= TTL) TI = 2;
        if (IC >= 9999) break;

        ra = addressmap(IC);
        if (ra == -1) {
            terminateprog("Invalid Page Fault during fetch");
            break;
        }

        for (int i = 0; i < 4; i++) IR[i] = mem[ra][i];

        if (IR[0] == 'H') { SI = 3; IC++; mastermode(); break; }

        char op[3] = {IR[0], IR[1], '\0'};

        if (strcmp(op, "GD") == 0) { SI = 1; IC++; TLC++; mastermode(); }
        else if (strcmp(op, "PD") == 0) { SI = 2; IC++; TLC++; mastermode(); }
        else if (strcmp(op, "LR") == 0) {
            IC++; TLC++;
            int va = (IR[2] - '0') * 10 + (IR[3] - '0');
            addr = addressmap(va);
            if (addr == -1) { terminateprog("Invalid Page Fault"); break; }
            for (int i = 0; i < 4; i++) R[i] = mem[addr][i];
        }
        else if (strcmp(op, "SR") == 0) {
            IC++; TLC++;
            int va = (IR[2] - '0') * 10 + (IR[3] - '0');
            addr = addressmap(va);
            if (addr == -1) {
                addr = handlePageFault(va, 1);
                if (addr == -1) break;
            }
            for (int i = 0; i < 4; i++) mem[addr][i] = R[i];
        }
        else if (strcmp(op, "CR") == 0) {
            IC++; TLC++;
            int va = (IR[2] - '0') * 10 + (IR[3] - '0');
            addr = addressmap(va);
            if (addr == -1) { terminateprog("Invalid Page Fault"); break; }
            C = 1;
            for (int i = 0; i < 4; i++)
                if (R[i] != mem[addr][i]) { C = 0; break; }
        }
        else if (strcmp(op, "BT") == 0) {
            TLC++;
            if (C) {
                int va = (IR[2] - '0') * 10 + (IR[3] - '0');
                IC = va;
            } else IC++;
        }
        else { PI = 1; mastermode(); break; }

        if (IC >= 9999) break;
    }
}

void load() {
    char line[81];
    int pageno = 0;

    while (fgets(line, 80, inputfp)) {
        int len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[--len] = '\0';
        if (len > 0 && line[len-1] == '\r') line[--len] = '\0';

        char word[5] = {0};
        strncpy(word, line, 4);

        if (strcmp(word, "$AMJ") == 0) {
            pageno = 0;
            char ttlstr[5] = {0}, tllstr[5] = {0};
            strncpy(ttlstr, line + 8, 4);
            strncpy(tllstr, line + 12, 4);
            TTL = atoi(ttlstr);
            TLL = atoi(tllstr);
            init();
            printf("Job loaded. TTL=%d, TLL=%d\n", TTL, TLL);
        }
        else if (strcmp(word, "$DTA") == 0) { executeprog(); }
        else if (strcmp(word, "$END") == 0) { displaymem(); }
        else {
            int newframe = allocate();
            if (newframe == -1) { printf("Out of frames during load\n"); continue; }
            setpagetable(pageno, newframe);

            int base = newframe * PAGESIZE;
            int instcnt = 0;
            for (int i = 0; i < len; i++) {
                int row = base + instcnt / 4;
                if (line[i] == 'H') {
                    mem[row][0] = 'H';
                    instcnt = ((instcnt / 4) + 1) * 4;
                } else {
                    mem[row][instcnt % 4] = line[i];
                    instcnt++;
                }
            }
            pageno++;
        }
    }
}

int main() {
    srand(time(NULL));
    inputfp = fopen("input.txt", "r");
    outputfp = fopen("output.txt", "w");
    if (!inputfp) { printf("Cannot open input.txt\n"); return 1; }

    load();

    fclose(inputfp);
    fclose(outputfp);
    return 0;
}
