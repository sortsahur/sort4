#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, head;
    printf("\nEnter number of track requests: ");
    scanf("%d", &n);
    printf("Enter initial head position: ");
    scanf("%d", &head);

    int tracks[n];
    printf("\nEnter track requests:\n");
    for (int i = 0; i < n; i++) scanf("%d", &tracks[i]);

    int totaldist = 0, from = head;

    printf("\n%-10s%-10s%-10s%-10s\n", "S.No.", "From", "To", "Distance");
    for (int i = 0; i < n; i++) {
        int dist = abs(tracks[i] - from);
        printf("%-10d%-10d%-10d%-10d\n", i + 1, from, tracks[i], dist);
        totaldist += dist;
        from = tracks[i];
    }

    printf("\nOrder: %d", head);
    for (int i = 0; i < n; i++) printf(" -> %d", tracks[i]);
    printf("\nTotal seek distance: %d", totaldist);
    printf("\nAverage seek length: %.2f\n", (float)totaldist / n);

    return 0;
}
