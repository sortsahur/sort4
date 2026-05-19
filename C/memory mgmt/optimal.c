#include <stdio.h>
#include <limits.h>

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

    int pf = 0;

    printf("\n%-10s", "Page");
    for (int i = 0; i < nf; i++) printf("F%-8d", i);
    printf("PF\n");

    for (int i = 0; i < n; i++) {
        int found = 0;
        for (int j = 0; j < nf; j++)
            if (frames[j] == ref[i]) { found = 1; break; }

        int fault = 0;
        if (!found) {
            int replace = -1;
            for (int j = 0; j < nf; j++)
                if (frames[j] == -1) { replace = j; break; }

            if (replace == -1) {
                int farthest = -1;
                for (int j = 0; j < nf; j++) {
                    int nextocc = INT_MAX;
                    for (int k = i + 1; k < n; k++) {
                        if (ref[k] == frames[j]) { nextocc = k; break; }
                    }
                    if (nextocc > farthest) { farthest = nextocc; replace = j; }
                }
            }

            frames[replace] = ref[i];
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

    printf("\nTotal page faults (Optimal): %d\n", pf);
    return 0;
}
