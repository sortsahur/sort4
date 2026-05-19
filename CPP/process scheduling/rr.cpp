#include <iostream>
#include <vector>
#include <climits>
#include <queue>
#include <algorithm>
using namespace std;

struct process{
    int pid;
    int arrival;
    int burst;
    int remaining;
    int completion;
    int turnaround;
    int waiting;
};

void rr(vector<process> &p, int n, int q){
    sort(p.begin(), p.end(), [](const process &a, const process &b){
        return a.arrival < b.arrival;
    });

    queue<int> ready;
    int time = 0, done = 0, next = 0;
    vector<int> order, timestamp = {0};

    while(done<n){
        while(next < n && p[next].arrival <= time){
            ready.push(next);
            next++;
        }

        if(ready.empty()){
            time = p[next].arrival;
            ready.push(next);
            next++;
        }
        else{
            int selected = ready.front();
            ready.pop();

            if(p[selected].remaining < q){
                time += p[selected].remaining;
                p[selected].remaining = 0;
            }
            else{
                time += q;
                p[selected].remaining -= q;
            }

            order.push_back(p[selected].pid);
            timestamp.push_back(time);

            while(next < n && p[next].arrival <= time){
                ready.push(next);
                next++;
            }

            if(p[selected].remaining == 0){
                p[selected].completion = time;
                p[selected].turnaround = p[selected].completion - p[selected].arrival;
                p[selected].waiting = p[selected].turnaround - p[selected].burst;
                done++;
            }
            else ready.push(selected);
        }
    }

    cout << "\nExecution information (using RR):\n";
    cout << "\nProcess\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << p[i].pid << "\t" << p[i].arrival << "\t" << p[i].burst << "\t"
             << p[i].completion << "\t\t" << p[i].turnaround << "\t\t" << p[i].waiting << "\n";
    }

    cout << "\nGantt Chart:\n";
    for(int o:order){
        if(o == -1) cout << "|   X   ";
        else cout << "|  P" << o << "  ";
    }
    cout << "| " << endl;
    for(int t:timestamp) cout << t << "      "; 

}

int main(){
    int q, n;
    cout << "\nEnter the time quanta: ";
    cin >> q;
    cout << "Enter the number of processes: ";
    cin >> n;

    cout << "\nFor each proccess, enter the following details: \n";
    vector<process> p(n);
    for(int i = 0; i<n; i++){
        p[i].pid = i+1;
        cout << "\nProcess " << p[i].pid << ": ";
        cout << "\nArrival Time: ";
        cin >> p[i].arrival;
        cout << "Burst Time: ";
        cin >> p[i].burst;
        p[i].remaining = p[i].burst;
    }

    rr(p, n, q);

    return 0;
}