#include <stdio.h>
#include <limits.h>

struct process {
    int pid, arrival, burst, remaining, completion, turnaround, waiting;
};

void rr(struct process p[], int n, int quantum) {
    int time = 0, done = 0;
    int order[1000], timestamp[1001];
    int oidx = 0, tidx = 0;
    int queue[1000], front = 0, rear = 0;
    int inqueue[100] = {0};

    for (int i = 0; i < n; i++) p[i].remaining = p[i].burst;

    /* add processes arriving at time 0 */
    for (int i = 0; i < n; i++) {
        if (p[i].arrival == 0) {
            queue[rear++] = i;
            inqueue[i] = 1;
        }
    }

    timestamp[tidx++] = 0;

    while (done < n) {
        if (front == rear) {
            /* no process in queue, find next arrival */
            int earliest = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (p[i].remaining > 0 && p[i].arrival < earliest)
                    earliest = p[i].arrival;
            }
            timestamp[tidx++] = time;
            order[oidx++] = -1;
            time = earliest;
            for (int i = 0; i < n; i++) {
                if (p[i].arrival <= time && p[i].remaining > 0 && !inqueue[i]) {
                    queue[rear++] = i;
                    inqueue[i] = 1;
                }
            }
            continue;
        }

        int idx = queue[front++];
        timestamp[tidx++] = time;
        order[oidx++] = p[idx].pid;

        int exec = (p[idx].remaining < quantum) ? p[idx].remaining : quantum;
        p[idx].remaining -= exec;
        time += exec;

        /* add newly arrived processes to queue */
        for (int i = 0; i < n; i++) {
            if (p[i].arrival <= time && p[i].remaining > 0 && !inqueue[i]) {
                queue[rear++] = i;
                inqueue[i] = 1;
            }
        }

        if (p[idx].remaining == 0) {
            done++;
            p[idx].completion = time;
            p[idx].turnaround = p[idx].completion - p[idx].arrival;
            p[idx].waiting = p[idx].turnaround - p[idx].burst;
        } else {
            queue[rear++] = idx;
        }
    }
    timestamp[tidx++] = time;

    printf("\nExecution information (Round Robin, Quantum = %d):\n", quantum);
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
    int n, quantum;
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
    }

    printf("\nEnter time quantum: ");
    scanf("%d", &quantum);

    rr(p, n, quantum);
    return 0;
}
