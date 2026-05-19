#include <stdio.h>
#include <limits.h>

struct process {
    int pid, arrival, burst, completion, turnaround, waiting, executed;
};

void sjf(struct process p[], int n) {
    int time = 0, done = 0;
    int order[100], timestamp[101];
    int oidx = 0, tidx = 0;
    timestamp[tidx++] = 0;

    while (done < n) {
        int minburst = INT_MAX, selected = -1;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && !p[i].executed) {
                if (p[i].burst < minburst || (p[i].burst == minburst && p[i].arrival < p[selected].arrival)) {
                    selected = i;
                    minburst = p[i].burst;
                }
            }
        }

        if (selected == -1) {
            int earliest = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (!p[i].executed && p[i].arrival < earliest) {
                    earliest = p[i].arrival;
                    selected = i;
                }
            }
            time = p[selected].arrival;
            if (tidx == 1) timestamp[0] = time;
            else { timestamp[tidx++] = time; order[oidx++] = -1; }
        }

        time += p[selected].burst;
        p[selected].completion = time;
        p[selected].turnaround = p[selected].completion - p[selected].arrival;
        p[selected].waiting = p[selected].turnaround - p[selected].burst;
        p[selected].executed = 1;
        order[oidx++] = p[selected].pid;
        timestamp[tidx++] = time;
        done++;
    }

    printf("\nExecution information (SJF Non-Preemptive):\n");
    printf("\n%-10s%-10s%-10s%-15s%-15s%-10s\n", "Process", "Arrival", "Burst", "Completion", "Turnaround", "Waiting");
    for (int i = 0; i < n; i++)
        printf("P%-9d%-10d%-10d%-15d%-15d%-10d\n", p[i].pid, p[i].arrival, p[i].burst, p[i].completion, p[i].turnaround, p[i].waiting);

    float avgtat = 0, avgwt = 0;
    for (int i = 0; i < n; i++) { avgtat += p[i].turnaround; avgwt += p[i].waiting; }
    printf("\nAverage Turnaround Time: %.2f", avgtat / n);
    printf("\nAverage Waiting Time: %.2f\n", avgwt / n);

    printf("\nGantt Chart:\n");
    for (int i = 0; i < oidx; i++) {
        if (order[i] == -1) printf("|  IDLE  ");
        else printf("|  P%-4d ", order[i]);
    }
    printf("|\n");
    for (int i = 0; i < tidx; i++) printf("%-9d", timestamp[i]);
    printf("\n");
}

int main() {
    int n;
    printf("\nEnter number of processes: ");
    scanf("%d", &n);

    struct process p[n];
    printf("\nEnter data for each process:\n");
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("\nProcess %d:\n", p[i].pid);
        printf("Arrival Time: ");
        scanf("%d", &p[i].arrival);
        printf("Burst Time: ");
        scanf("%d", &p[i].burst);
        p[i].executed = 0;
    }

    sjf(p, n);
    return 0;
}
