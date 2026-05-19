#include <stdio.h>

int main() {
    int n, r;
    printf("\nEnter number of resource types: ");
    scanf("%d", &r);
    printf("Enter number of processes: ");
    scanf("%d", &n);

    int total[r], taken[r], available[r], work[r];
    int alloc[n][r], max[n][r], need[n][r];
    int finished[n], order[n];
    int oidx = 0;

    printf("\nEnter total instances of each resource type:\n");
    for (int i = 0; i < r; i++) {
        printf("Resource %c: ", 'A' + i);
        scanf("%d", &total[i]);
        taken[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        printf("\n===== Process %d =====\n", i);
        for (int j = 0; j < r; j++) {
            printf("Allocated [%c]: ", 'A' + j);
            scanf("%d", &alloc[i][j]);
            taken[j] += alloc[i][j];
        }
        for (int j = 0; j < r; j++) {
            printf("Max [%c]: ", 'A' + j);
            scanf("%d", &max[i][j]);
            need[i][j] = max[i][j] - alloc[i][j];
        }
        finished[i] = 0;
    }

    for (int i = 0; i < r; i++) {
        available[i] = total[i] - taken[i];
        work[i] = available[i];
    }

    /* print initial state */
    printf("\n%-10s", "Process");
    for (int j = 0; j < r; j++) printf("Alloc_%c  ", 'A' + j);
    for (int j = 0; j < r; j++) printf("Max_%c    ", 'A' + j);
    for (int j = 0; j < r; j++) printf("Need_%c   ", 'A' + j);
    printf("Finish\n");

    for (int i = 0; i < n; i++) {
        printf("P%-9d", i);
        for (int j = 0; j < r; j++) printf("%-9d", alloc[i][j]);
        for (int j = 0; j < r; j++) printf("%-9d", max[i][j]);
        for (int j = 0; j < r; j++) printf("%-9d", need[i][j]);
        printf("%d\n", finished[i]);
    }

    printf("\nAvailable: ");
    for (int i = 0; i < r; i++) printf("%d ", available[i]);
    printf("\n");

    int c = 0;
    while (c != n) {
        int prev = c;
        for (int i = 0; i < n; i++) {
            if (finished[i]) continue;
            int ok = 1;
            for (int j = 0; j < r; j++)
                if (need[i][j] > work[j]) { ok = 0; break; }

            if (ok) {
                finished[i] = 1;
                order[oidx++] = i;
                c++;
                printf("\nExecuting P%d:\n", i);
                for (int j = 0; j < r; j++) {
                    work[j] += alloc[i][j];
                    alloc[i][j] = 0;
                }
                printf("Work: ");
                for (int j = 0; j < r; j++) printf("%d ", work[j]);
                printf("\n");
            }
        }

        if (prev == c) {
            printf("\n\nSYSTEM IS IN UNSAFE STATE. NO SAFE SEQUENCE EXISTS.\n\n");
            return 0;
        }
    }

    printf("\nSystem is in SAFE state.\nSafe sequence: ");
    for (int i = 0; i < n; i++) printf("P%d ", order[i]);
    printf("\n");

    return 0;
}
