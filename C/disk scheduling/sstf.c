#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
    int n, head;
    printf("\nEnter number of track requests: ");
    scanf("%d", &n);
    printf("Enter initial head position: ");
    scanf("%d", &head);

    int tracks[n], done[n];
    printf("\nEnter track requests:\n");
    for (int i = 0; i < n; i++) { scanf("%d", &tracks[i]); done[i] = 0; }

    int totaldist = 0, from = head, moves = 0;
    int order[n];

    printf("\n%-10s%-10s%-10s%-10s\n", "S.No.", "From", "To", "Distance");

    for (int k = 0; k < n; k++) {
        int mindist = INT_MAX, selected = -1;
        for (int i = 0; i < n; i++) {
            if (!done[i] && abs(tracks[i] - from) < mindist) {
                mindist = abs(tracks[i] - from);
                selected = i;
            }
        }
        done[selected] = 1;
        order[k] = tracks[selected];
        moves++;
        printf("%-10d%-10d%-10d%-10d\n", moves, from, tracks[selected], mindist);
        totaldist += mindist;
        from = tracks[selected];
    }

    printf("\nOrder: %d", head);
    for (int i = 0; i < n; i++) printf(" -> %d", order[i]);
    printf("\nTotal seek distance: %d", totaldist);
    printf("\nAverage seek length: %.2f\n", (float)totaldist / n);

    return 0;
}
