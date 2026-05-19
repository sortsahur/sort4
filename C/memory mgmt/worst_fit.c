#include <stdio.h>
#include <limits.h>

int main() {
    int np, npart;
    printf("\nEnter number of memory partitions: ");
    scanf("%d", &npart);

    int parts[npart];
    printf("Enter size of each partition:\n");
    for (int i = 0; i < npart; i++) scanf("%d", &parts[i]);

    printf("\nEnter number of processes: ");
    scanf("%d", &np);

    int psize[np], assigned[np], hole[np];
    printf("Enter size of each process:\n");
    for (int i = 0; i < np; i++) { scanf("%d", &psize[i]); assigned[i] = -1; }

    for (int i = 0; i < np; i++) {
        int selected = -1, maxsize = -1;
        for (int j = 0; j < npart; j++) {
            if (psize[i] <= parts[j] && parts[j] > maxsize) {
                selected = j;
                maxsize = parts[j];
            }
        }
        if (selected != -1) {
            assigned[i] = selected + 1;
            hole[i] = parts[selected] - psize[i];
            parts[selected] -= psize[i];
        }
    }

    printf("\n========== WORST FIT ==========\n");
    printf("%-12s%-15s%-15s%-12s%-12s\n", "Process", "Process Size", "Partition", "Hole", "Allocated");
    for (int i = 0; i < np; i++) {
        printf("P%-11d%-15d", i + 1, psize[i]);
        if (assigned[i] == -1) printf("%-15s%-12s", "N/A", "N/A");
        else printf("%-15d%-12d", assigned[i], hole[i]);
        printf("%s\n", assigned[i] != -1 ? "Yes" : "No");
    }

    return 0;
}
