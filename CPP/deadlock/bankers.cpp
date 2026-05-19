#include <iostream>
#include <vector>
using namespace std;

#define R 3

struct process{
    int alloc[R];
    int max[R];
    int need[R];
    bool finished;
};

int n;
int total[R], taken[R];
int work[R], available[R];

void bankers(){
    bool deadlock = false;
    cout << "\nEnter number of processes: ";
    cin >> n;

    vector<process> p(n);
    cout << "\nEnter details for each process: \n";

    for(int i = 0; i<n; i++){
        cout << "\n ===== Process " << i+1 << " ===== \n";

        for(int j = 0; j<R; j++){
            char type = 'A' + j;
            cout << "Resources Allocated [" << type << "]: ";
            cin >> p[i].alloc[j];

            if(taken[j] + p[i].alloc[j] > total[j]){
                cout << "\nERROR: Instances allocated of type " << type << " exceed the total number of instances.";
                return;
            }
        }

        for(int j = 0; j<R; j++) taken[j] += p[i].alloc[j];

        for(int j = 0; j<R; j++){
            char type = 'A' + j;
            cout << "Max Requirement [" << type << "]: ";
            cin >> p[i].max[j];

            if(p[i].alloc[j] > p[i].max[j]){
                cout << "\nERROR: Instances allocated of type " << type << " exceed the max requirement.";
                return;
            }
        }
        
        for(int j = 0; j<R; j++) p[i].need[j] = p[i].max[j] - p[i].alloc[j];

        p[i].finished = false;
    }

    for(int i = 0; i<R; i++){
        available[i] = total[i] - taken[i];
        work[i] = available[i];
    }

    cout << "\nProcess\t\t   Allocated\t\t      \tMax\t\t     \tNeed\t\t  Finish\n";
    cout << "\t\tA\tB\tC\tA\tB\tC\tA\tB\tC\n";
    for(int i = 0; i < n; i++) {
        cout << "   P" << i << "\t        " << p[i].alloc[0] << "\t" << p[i].alloc[1] << "\t"
             << p[i].alloc[2] << "\t" << p[i].max[0] << "\t" << p[i].max[1] << "\t"
             << p[i].max[2] << "\t" << p[i].need[0] << "\t" << p[i].need[1] << "\t"
             << p[i].need[2] << "\t   " << p[i].finished << "\n";
    }

    cout << "\n\n\nAvailable : " << available[0] << " " << available[1] << " " << available[2] << "\n";

    int c = 0;
    vector<int> order;

    while(c!=n){
        int prev = c;

        for(int i = 0; i<n; i++){
            int ok = 1;
            for(int j = 0; j<R; j++) if(p[i].need[j] > work[j]) {ok = 0; break;}
            if(p[i].finished) ok = 0;

            if(ok){
                p[i].finished = true;

                cout << "\nExecuting P" << i+1 << ": \n";
                order.push_back(i);
                c++;
                
                for(int j = 0; j<R; j++) {
                    work[j] += p[i].alloc[j];
                    p[i].alloc[j] = 0;
                }

                cout << "\nWork : " << work[0] << " " << work[1] << " " << work[2] << "\n";
                cout << "\nProcess\t\t   Allocated\t\t      \tMax\t\t     \tNeed\t\t  Finish\n";
                cout << "\t\tA\tB\tC\tA\tB\tC\tA\tB\tC\n";
                for(int i = 0; i < n; i++) {
                    cout << "   P" << i << "\t        " << p[i].alloc[0] << "\t" << p[i].alloc[1] << "\t"
                        << p[i].alloc[2] << "\t" << p[i].max[0] << "\t" << p[i].max[1] << "\t"
                        << p[i].max[2] << "\t" << p[i].need[0] << "\t" << p[i].need[1] << "\t"
                        << p[i].need[2] << "\t   " << p[i].finished << "\n";
                }
                cout << "\n============================================================================================================\n\n";
            }
        }

        if(prev == c){
            deadlock = 1;
            cout << "\n\nSYSTEM IS IN DEADLOCK STATE. TERMINATING THE PROGRAM\n\n";
            break;
        }
    }

    cout << "\n\n";
    if(deadlock == 0) {
        cout << "Safe order for execution: ";
        for(int i = 0; i < n; i++) cout << "P" << order[i] << "\t";
    }
}

int main(){
    cout << "\nEnter total instance of each resource in system:\n";

    for(int i = 0; i<R; i++){
        char type = 'A' + i;
        cout << type << ": ";
        cin >> total[i];
        taken[i] = 0;
    }

    bankers();
    return 0;
}