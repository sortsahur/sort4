#include <iostream>
#include <vector>
#include <climits>
using namespace std;

struct process{
    int pid;
    int arrival;
    int burst;
    int remaining;
    int priority;
    int completion;
    int turnaround;
    int waiting;
};

void priorityp(vector<process> &p, int n){
    int time = 0, done = 0, prev = -1;
    vector<int> order, timestamp = {0};
    
    while(done<n){
        int top = INT_MAX, selected = -1;
        for(int i = 0; i<n; i++){
            if(p[i].arrival <= time && p[i].remaining > 0){
                if(p[i].priority < top || (p[i].priority == top && p[i].arrival < p[selected].arrival)){
                    selected = i;
                    top = p[i].priority;
                }
            }
        }

        if(selected == -1){
            int first = INT_MAX;
            for(int i = 0; i<n; i++){
                if(p[i].arrival > time && p[i].arrival < first && p[i].remaining > 0)
                    selected = i, first = p[i].arrival;
            }

            time = first;
            if(timestamp.size() == 1) timestamp[0] = time;
            else {timestamp.push_back(time); order.push_back(-1);}
        }
        else{
            if(selected != prev){
                order.push_back(p[selected].pid);
                if(timestamp.back() != time) timestamp.push_back(time);
            }

            p[selected].remaining--;
            time++;
            prev = selected;

            if(p[selected].remaining == 0){
                p[selected].completion = time;
                p[selected].turnaround = p[selected].completion - p[selected].arrival;
                p[selected].waiting = p[selected].turnaround - p[selected].burst;
                timestamp.push_back(time);
                done++;
            }
        }
    }

    cout << "\nExecution information (using PRIORITY P):\n";
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

    cout << "\nFor each proccess, enter the following details: \n";
    vector<process> p(n);
    for(int i = 0; i<n; i++){
        p[i].pid = i+1;
        cout << "\nProcess " << p[i].pid << ": ";
        cout << "\nArrival Time: ";
        cin >> p[i].arrival;
        cout << "Burst Time: ";
        cin >> p[i].burst;
        cout << "Priority: ";
        cin >> p[i].priority;
        p[i].remaining = p[i].burst;
    }

    priorityp(p, n);

    return 0;
}