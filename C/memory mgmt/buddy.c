#include <stdio.h>
#include <math.h>
#include <ctype.h>

#define MAX 100

struct block {
    char id;
    int size;
    int free;
};

struct block mem[MAX];
int memcount = 1;
char nextid = 'A';

void printmem() {
    printf("\nMemory state:\n");
    for (int i = 0; i < memcount; i++)
        printf("[ %dK (%c) ]  ", mem[i].size, mem[i].id);
    printf("\n");
}

void request() {
    int req;
    printf("\nEnter size of requested space for process %c: ", nextid);
    scanf("%d", &req);

    int target = 1;
    while (target < req) target *= 2;

    for (int i = 0; i < memcount; i++) {
        if (mem[i].free && mem[i].size >= target) {
            while (mem[i].size > target) {
                mem[i].size /= 2;
                for (int j = memcount; j > i + 1; j--) mem[j] = mem[j - 1];
                mem[i + 1].id = ' ';
                mem[i + 1].size = mem[i].size;
                mem[i + 1].free = 1;
                memcount++;
            }
            mem[i].free = 0;
            mem[i].id = nextid;
            printf("Block of %dK allocated to %c (%dK requested)\n", mem[i].size, nextid, req);
            printmem();
            nextid++;
            return;
        }
    }
    printf("ERROR: Insufficient memory.\n");
}

void release() {
    char relid;
    printf("\nEnter ID of process to release: ");
    scanf(" %c", &relid);
    relid = toupper(relid);

    int found = 0;
    for (int i = 0; i < memcount; i++) {
        if (mem[i].id == relid) {
            mem[i].id = ' ';
            mem[i].free = 1;
            found = 1;
        }
    }

    if (!found) { printf("ERROR: Process not found.\n"); return; }

    int merged = 1;
    while (merged) {
        merged = 0;
        for (int i = 0; i < memcount - 1; i++) {
            if (mem[i].size == mem[i + 1].size && mem[i].free && mem[i + 1].free) {
                mem[i].size *= 2;
                for (int j = i + 1; j < memcount - 1; j++) mem[j] = mem[j + 1];
                memcount--;
                merged = 1;
                break;
            }
        }
    }

    printf("Space for process %c released.\n", relid);
    printmem();
}

int main() {
    int c;
    mem[0].id = ' ';
    mem[0].size = 1024;
    mem[0].free = 1;

    printf("\n========== BUDDY SYSTEM ==========\nInitial memory block size = 1024K\n");

    do {
        printf("\n1. Request space\n2. Release space\n3. Exit\nChoice: ");
        scanf("%d", &c);
        switch (c) {
            case 1: request(); break;
            case 2: release(); break;
            case 3: printf("Exiting.\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (c != 3);

    return 0;
}
