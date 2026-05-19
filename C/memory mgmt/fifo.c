#include <stdio.h>

int main() {
    int nf, n;
    printf("\nEnter number of frames: ");
    scanf("%d", &nf);
    printf("Enter length of reference string: ");
    scanf("%d", &n);

    int ref[n], frames[nf];
    printf("Enter reference string:\n");
    for (int i = 0; i < n; i++) scanf("%d", &ref[i]);
    for (int i = 0; i < nf; i++) frames[i] = -1;

    int pf = 0, idx = 0;

    printf("\n%-10s", "Page");
    for (int i = 0; i < nf; i++) printf("F%-8d", i);
    printf("PF\n");

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < nf; j++)
            if (frames[j] == ref[i]) { found = 1; break; }

        int fault = 0;
        if (!found) {
            frames[idx] = ref[i];
            idx = (idx + 1) % nf;
            pf++;
            fault = 1;
        }

        printf("%-10d", ref[i]);
        for (int j = 0; j < nf; j++) {
            if (frames[j] == -1) printf("%-9s", "-");
            else printf("%-9d", frames[j]);
        }
        printf("%s\n", fault ? "Yes" : "No");
    }

    printf("\nTotal page faults (FIFO): %d\n", pf);
    return 0;
}
