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

void sjf(vector<process> &p, int n){
    int time = 0, done = 0;
    vector<int> order, timestamps = {0};

    while(done<n){
        int selected = -1, shortest = INT_MAX;

        for(int i = 0; i<n; i++){
            if(p[i].arrival <= time && !p[i].executed){
                if(p[i].burst<shortest) selected = i, shortest = p[i].burst;
            }
        }

        if(selected == -1){
            int first = INT_MAX;
            for(int i = 0; i<n; i++){
                if(!p[i].executed){
                    if(p[i].arrival<first && p[i].burst<shortest) 
                       selected = i, first = p[i].arrival, shortest = p[i].burst;
                }
            }

            time = first;
            if(timestamps.size()==1) timestamps[0] = first;
            else{timestamps.push_back(time); order.push_back(-1);}
        }
        else{
            time += p[selected].burst;
            p[selected].completion = time;
            p[selected].turnaround = p[selected].completion - p[selected].arrival;
            p[selected].waiting = p[selected].turnaround - p[selected].burst;
            p[selected].executed = true;
            order.push_back(selected+1);
            timestamps.push_back(time);
            done++;
        }
    }

    cout << "\nExecution information (using SJF):\n";
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
    for(int t:timestamps) cout << t << "      "; 
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
        p[i].executed = false; 
    }

    sjf(p, n);

    return 0;
}