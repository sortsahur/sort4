#include <stdio.h>
#include <stdlib.h>

void sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1]) {
                int t = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = t;
            }
}

int main() {
    int n, head, maxtrack;
    char dir;
    printf("\nEnter number of track requests: ");
    scanf("%d", &n);
    printf("Enter initial head position: ");
    scanf("%d", &head);
    printf("Enter max track number: ");
    scanf("%d", &maxtrack);
    printf("Enter direction (l/r): ");
    scanf(" %c", &dir);

    int tracks[n];
    printf("\nEnter track requests:\n");
    for (int i = 0; i < n; i++) scanf("%d", &tracks[i]);

    sort(tracks, n);

    int order[n + 2], oidx = 0;
    int totaldist = 0, from = head, moves = 0;

    if (dir == 'l' || dir == 'L') {
        for (int i = n - 1; i >= 0; i--)
            if (tracks[i] <= head) order[oidx++] = tracks[i];
        order[oidx++] = 0;
        for (int i = 0; i < n; i++)
            if (tracks[i] > head) order[oidx++] = tracks[i];
    } else {
        for (int i = 0; i < n; i++)
            if (tracks[i] >= head) order[oidx++] = tracks[i];
        order[oidx++] = maxtrack;
        for (int i = n - 1; i >= 0; i--)
            if (tracks[i] < head) order[oidx++] = tracks[i];
    }

    printf("\n%-10s%-10s%-10s%-10s\n", "S.No.", "From", "To", "Distance");
    for (int i = 0; i < oidx; i++) {
        int dist = abs(order[i] - from);
        moves++;
        printf("%-10d%-10d%-10d%-10d\n", moves, from, order[i], dist);
        totaldist += dist;
        from = order[i];
    }

    printf("\nOrder: %d", head);
    for (int i = 0; i < oidx; i++) printf(" -> %d", order[i]);
    printf("\nTotal seek distance: %d", totaldist);
    printf("\nAverage seek length: %.2f\n", (float)totaldist / moves);

    return 0;
}
