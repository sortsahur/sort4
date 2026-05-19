#include <stdio.h>
#include <limits.h>

struct process {
    int pid, arrival, burst, priority, remaining, completion, turnaround, waiting;
};

void priority_p(struct process p[], int n) {
    int time = 0, done = 0;
    int order[1000], timestamp[1001];
    int oidx = 0, tidx = 0;
    int prev = -1;

    for (int i = 0; i < n; i++) p[i].remaining = p[i].burst;

    while (done < n) {
        int highpri = INT_MAX, selected = -1;
        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && p[i].remaining > 0) {
                if (p[i].priority < highpri || (p[i].priority == highpri && p[i].arrival < p[selected].arrival)) {
                    selected = i;
                    highpri = p[i].priority;
                }
            }
        }

        if (selected == -1) {
            if (prev != -2) {
                timestamp[tidx++] = time;
                order[oidx++] = -1;
                prev = -2;
            }
            time++;
            continue;
        }

        if (selected != prev) {
            timestamp[tidx++] = time;
            prev = selected;
            order[oidx++] = p[selected].pid;
        }

        p[selected].remaining--;
        time++;

        if (p[selected].remaining == 0) {
            done++;
            p[selected].completion = time;
            p[selected].turnaround = p[selected].completion - p[selected].arrival;
            p[selected].waiting = p[selected].turnaround - p[selected].burst;
            prev = -1;
        }
    }
    timestamp[tidx++] = time;

    printf("\nExecution information (Priority Preemptive):\n");
    printf("\n%-10s%-10s%-10s%-10s%-15s%-15s%-10s\n", "Process", "Arrival", "Burst", "Priority", "Completion", "Turnaround", "Waiting");
    for (int i = 0; i < n; i++)
        printf("P%-9d%-10d%-10d%-10d%-15d%-15d%-10d\n", p[i].pid, p[i].arrival, p[i].burst, p[i].priority, p[i].completion, p[i].turnaround, p[i].waiting);

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
        printf("Priority (lower = higher): ");
        scanf("%d", &p[i].priority);
    }

    priority_p(p, n);
    return 0;
}
