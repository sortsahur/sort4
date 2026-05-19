#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char R[4], IR[4];
int IC;
int C;
int SI;
char mem[200][4];
char buffer[41];
int jobcount = 0;
FILE *inputfp, *outputfp;

void init() {
    IC = 0; C = 0; SI = 0;
    memset(R, 0, 4);
    memset(IR, 0, 4);
    for (int i = 0; i < 200; i++)
        for (int j = 0; j < 4; j++)
            mem[i][j] = '-';
    memset(buffer, '-', 40);
    buffer[40] = '\0';
}

void readcard() {
    int loc = (IR[2] - '0') * 10 + (IR[3] - '0');
    char line[81];
    if (!fgets(line, 80, inputfp)) return;
    int len = strlen(line);
    if (len > 0 && line[len-1] == '\n') line[--len] = '\0';
    if (len > 0 && line[len-1] == '\r') line[--len] = '\0';

    int i = 0, m = 0;
    while (i < len) {
        if (m != 0 && m % 4 == 0) loc++;
        mem[loc][m % 4] = line[i];
        i++; m++;
    }
}

void writecard() {
    int loc = (IR[2] - '0') * 10 + (IR[3] - '0');
    for (int i = loc; i < loc + 10; i++) {
        for (int j = 0; j < 4; j++) {
            if (mem[i][j] != '-') fputc(mem[i][j], outputfp);
            else fputc(' ', outputfp);
        }
    }
    fputc('\n', outputfp);
}

void terminate() {
    fputc('\n', outputfp);
}

void mastermode() {
    switch (SI) {
        case 1: readcard(); break;
        case 2: writecard(); break;
        case 3: terminate(); break;
    }
}

void executeprog() {
    int addr;
    while (1) {
        for (int i = 0; i < 4; i++) IR[i] = mem[IC][i];

        if (IR[0] == 'H') { SI = 3; IC++; mastermode(); break; }

        char op[3] = {IR[0], IR[1], '\0'};

        if (strcmp(op, "GD") == 0) { SI = 1; IC++; mastermode(); }
        else if (strcmp(op, "PD") == 0) { SI = 2; IC++; mastermode(); }
        else if (strcmp(op, "LR") == 0) {
            IC++;
            addr = (IR[2] - '0') * 10 + (IR[3] - '0');
            for (int i = 0; i < 4; i++) R[i] = mem[addr][i];
        }
        else if (strcmp(op, "SR") == 0) {
            IC++;
            addr = (IR[2] - '0') * 10 + (IR[3] - '0');
            for (int i = 0; i < 4; i++) mem[addr][i] = R[i];
        }
        else if (strcmp(op, "CR") == 0) {
            IC++;
            addr = (IR[2] - '0') * 10 + (IR[3] - '0');
            C = 1;
            for (int i = 0; i < 4; i++)
                if (R[i] != mem[addr][i]) { C = 0; break; }
        }
        else if (strcmp(op, "BT") == 0) {
            if (C) {
                addr = (IR[2] - '0') * 10 + (IR[3] - '0');
                IC = addr;
            } else IC++;
        }
        else { printf("Invalid opcode: %s\n", op); break; }
    }
}

void displaymem() {
    printf("\n===== MEMORY DUMP (Job %d) =====\n", jobcount);
    for (int i = 0; i < 200; i++) {
        if (i % 10 == 0 && i != 0) printf("--------------------------------------------\n");
        printf("%3d    %c%c%c%c\n", i, mem[i][0], mem[i][1], mem[i][2], mem[i][3]);
    }
    printf("\n");
}

void load() {
    char line[81];
    int lineptr = 0, instcnt = 0;

    while (fgets(line, 80, inputfp)) {
        int len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[--len] = '\0';
        if (len > 0 && line[len-1] == '\r') line[--len] = '\0';

        char word[5] = {0};
        strncpy(word, line, 4);

        if (strcmp(word, "$AMJ") == 0) {
            lineptr = 0; instcnt = 0;
            jobcount++;
            printf("Loading Job %d...\n", jobcount);
            init();
        }
        else if (strcmp(word, "$DTA") == 0) { executeprog(); }
        else if (strcmp(word, "$END") == 0) { displaymem(); }
        else {
            for (int i = 0; i < len; i++) {
                if (instcnt != 0 && instcnt % 4 == 0) lineptr++;
                if (line[i] == 'H') {
                    mem[lineptr][0] = 'H';
                    instcnt = ((instcnt / 4) + 1) * 4;
                } else {
                    mem[lineptr][instcnt % 4] = line[i];
                    instcnt++;
                }
            }
        }
    }

    printf("\nTotal jobs processed: %d\n", jobcount);
}

int main() {
    inputfp = fopen("input.txt", "r");
    outputfp = fopen("output.txt", "w");
    if (!inputfp) { printf("Cannot open input.txt\n"); return 1; }

    load();

    fclose(inputfp);
    fclose(outputfp);
    return 0;
}
