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
int PTR, TLC, TTL, LLC, TLL;
int frames[NFRAMES], frameidx;
FILE *inputfp, *outputfp;

void shuffleframes() {
    for (int i = 0; i < NFRAMES; i++) frames[i] = i;
    for (int i = NFRAMES-1; i > 0; i--) {
        int j = rand()%(i+1);
        int t = frames[i]; frames[i] = frames[j]; frames[j] = t;
    }
    frameidx = 0;
}

int allocate() {
    if (frameidx >= NFRAMES) return -1;
    return frames[frameidx++];
}

void init() {
    IC=0; C=0; SI=0; TI=0; PI=0; TLC=0; LLC=0;
    memset(R,0,4); memset(IR,0,4);
    for (int i=0; i<MEMSIZE; i++)
        for (int j=0; j<4; j++) mem[i][j] = '-';
    shuffleframes();
    int ptf = allocate();
    PTR = ptf * PAGESIZE;
    printf("PTR = %d (Frame %d)\n", PTR, ptf);
}

void setpt(int pn, int fn) {
    int e = PTR+pn;
    mem[e][0]='1'; mem[e][1]=' ';
    mem[e][2]='0'+fn/10; mem[e][3]='0'+fn%10;
}

int getframe(int pn) {
    int e = PTR+pn;
    if (mem[e][0]!='1') return -1;
    return (mem[e][2]-'0')*10 + (mem[e][3]-'0');
}

int addrmap(int va) {
    int fn = getframe(va/PAGESIZE);
    if (fn==-1) return -1;
    return fn*PAGESIZE + va%PAGESIZE;
}

void terminateprog(const char *msg) {
    fprintf(outputfp, "%s\nIC=%d SI=%d TI=%d PI=%d\n\n", msg, IC, SI, TI, PI);
    printf("TERMINATE: %s\n", msg);
}

void displaymem() {
    printf("\n===== MEMORY DUMP =====\n");
    for (int i=0; i<MEMSIZE; i++) {
        if (i%10==0 && i!=0) printf("------------------------------------------\n");
        printf("%3d    %c%c%c%c\n", i, mem[i][0], mem[i][1], mem[i][2], mem[i][3]);
    }
}

void readcard() {
    char line[81];
    if (!fgets(line,80,inputfp)) return;
    int len=strlen(line);
    while (len>0 && (line[len-1]=='\n'||line[len-1]=='\r')) line[--len]='\0';
    if (strncmp(line,"$END",4)==0) { terminateprog("Out of Data"); IC=9999; return; }

    int va = (IR[2]-'0')*10 + (IR[3]-'0');
    int ra = addrmap(va);
    if (ra==-1) { PI=3; terminateprog("Invalid Page Fault"); IC=9999; return; }

    int i=0, m=0;
    while (i<len) { if (m && m%4==0) ra++; mem[ra][m%4]=line[i]; i++; m++; }
}

void writecard() {
    LLC++;
    if (LLC>TLL) { terminateprog("Line Limit Exceeded"); IC=9999; return; }
    int va = (IR[2]-'0')*10 + (IR[3]-'0');
    int ra = addrmap(va);
    if (ra==-1) { PI=3; terminateprog("Invalid Page Fault"); IC=9999; return; }
    for (int i=ra; i<ra+10; i++)
        for (int j=0; j<4; j++)
            fputc(mem[i][j]!='-' ? mem[i][j] : ' ', outputfp);
    fputc('\n', outputfp);
}

void mastermode() {
    if (TLC>=TTL) TI=2;
    if (TI==0 && SI==1) { SI=0; readcard(); }
    else if (TI==0 && SI==2) { SI=0; writecard(); }
    else if (TI==0 && SI==3) terminateprog("Program terminated normally");
    else if (TI==2 && SI==1) terminateprog("Time Limit Exceeded");
    else if (TI==2 && SI==2) { writecard(); terminateprog("Time Limit Exceeded"); }
    else if (TI==2 && SI==3) terminateprog("Program terminated normally");
    else if (TI==0 && PI==1) terminateprog("Opcode Error");
    else if (TI==0 && PI==2) terminateprog("Operand Error");
    else if (TI==0 && PI==3) terminateprog("Invalid Page Fault");
    else if (TI==2 && PI==1) terminateprog("TLE + Opcode Error");
    else if (TI==2 && PI==2) terminateprog("TLE + Operand Error");
    else if (TI==2 && PI==3) terminateprog("TLE + Invalid Page Fault");
}

void executeprog() {
    while (1) {
        if (TLC>=TTL) TI=2;
        if (IC>=9999) break;
        int ra = addrmap(IC);
        if (ra==-1) { PI=3; terminateprog("Invalid Page Fault fetch"); break; }
        for (int i=0;i<4;i++) IR[i]=mem[ra][i];
        if (IR[0]=='H') { SI=3; IC++; mastermode(); break; }
        char op[3]={IR[0],IR[1],'\0'};
        if (!strcmp(op,"GD")) { SI=1; IC++; TLC++; mastermode(); }
        else if (!strcmp(op,"PD")) { SI=2; IC++; TLC++; mastermode(); }
        else if (!strcmp(op,"LR")) {
            IC++; TLC++;
            int a=addrmap((IR[2]-'0')*10+(IR[3]-'0'));
            if (a==-1){PI=3;terminateprog("Invalid Page Fault");break;}
            for(int i=0;i<4;i++) R[i]=mem[a][i];
        }
        else if (!strcmp(op,"SR")) {
            IC++; TLC++;
            int a=addrmap((IR[2]-'0')*10+(IR[3]-'0'));
            if (a==-1){PI=3;terminateprog("Invalid Page Fault");break;}
            for(int i=0;i<4;i++) mem[a][i]=R[i];
        }
        else if (!strcmp(op,"CR")) {
            IC++; TLC++;
            int a=addrmap((IR[2]-'0')*10+(IR[3]-'0'));
            if (a==-1){PI=3;terminateprog("Invalid Page Fault");break;}
            C=1; for(int i=0;i<4;i++) if(R[i]!=mem[a][i]){C=0;break;}
        }
        else if (!strcmp(op,"BT")) {
            TLC++;
            if(C) IC=(IR[2]-'0')*10+(IR[3]-'0'); else IC++;
        }
        else { PI=1; mastermode(); break; }
        if (IC>=9999) break;
    }
}

void load() {
    char line[81]; int pageno=0;
    while (fgets(line,80,inputfp)) {
        int len=strlen(line);
        while(len>0&&(line[len-1]=='\n'||line[len-1]=='\r')) line[--len]='\0';
        char word[5]={0}; strncpy(word,line,4);
        if (!strcmp(word,"$AMJ")) {
            pageno=0;
            char ts[5]={0},ls[5]={0};
            strncpy(ts,line+8,4); strncpy(ls,line+12,4);
            TTL=atoi(ts); TLL=atoi(ls); init();
        }
        else if (!strcmp(word,"$DTA")) executeprog();
        else if (!strcmp(word,"$END")) displaymem();
        else {
            int nf=allocate();
            if (nf==-1) continue;
            setpt(pageno, nf);
            int base=nf*PAGESIZE, ic=0;
            for (int i=0;i<len;i++) {
                int row=base+ic/4;
                if(line[i]=='H'){mem[row][0]='H';ic=((ic/4)+1)*4;}
                else{mem[row][ic%4]=line[i];ic++;}
            }
            pageno++;
        }
    }
}

int main() {
    srand(time(NULL));
    inputfp=fopen("input.txt","r"); outputfp=fopen("output.txt","w");
    if (!inputfp){printf("Cannot open input.txt\n");return 1;}
    load(); fclose(inputfp); fclose(outputfp);
    return 0;
}
