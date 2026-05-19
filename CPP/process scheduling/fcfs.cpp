#include <iostream>
#include <vector>
#include <climits>
using namespace std;

struct process{
    int pid;
    int arrival;
    int burst;
    int completion;
    int turnaround;
    int waiting;
    bool executed;
};

void fcfs(vector<process> &p, int n){
    int time = 0, done = 0;
    vector<int> order, timestamp = {0};

    while(done<n){
        int firsttime = INT_MAX, selected = -1;
        for(int i = 0; i<n; i++){
            if(p[i].arrival <= time && !p[i].executed){
                if(p[i].arrival < firsttime) selected = i, firsttime = p[i].arrival;
            }
        }

        if(selected == -1){
            for(int i = 0; i<n; i++){
                if(!p[i].executed){
                    if(p[i].arrival < firsttime) selected = i, firsttime = p[i].arrival;
                }
            }

            time = p[selected].arrival;
            if(timestamp.size() == 1) timestamp[0] = time;
            else {timestamp.push_back(time); order.push_back(-1);}
        }
        else{
            time += p[selected].burst;
            p[selected].completion = time;
            p[selected].turnaround = p[selected].completion - p[selected].arrival;
            p[selected].waiting = p[selected].turnaround - p[selected].burst;
            p[selected].executed = true;
            order.push_back(p[selected].pid);
            timestamp.push_back(time);
            done++;
        }
    }

    cout << "\nExecution information (using FCFS):\n";
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
    int n;
    cout << "\nEnter number of processes: ";
    cin >> n;

    vector<process> p(n);

    cout << "\nEnter data for each process:\n";
    for(int i = 0; i<n; i++){
        p[i].pid = i+1;
        cout << "\nProcess " << p[i].pid << ": ";
        cout << "\nArrival Time: ";
        cin >> p[i].arrival;
        cout << "Burst Time: ";
        cin >> p[i].burst;
        p[i].executed = false;
    }

    fcfs(p, n);

    return 0;
}