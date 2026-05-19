#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

struct process{
    int id;
    int size;
    int partid;
    int hole;
};

void print(vector<int> &parts, vector<process> &procs, int pr){
    cout << "\n" << left << setw(12) << "Process ID" << setw(15) << "Process Size" << setw(18) << "Partition ID" << setw(12) << "Hole" << "Allocated?";
    for(int i = 0; i<pr; i++){
        cout << "\n" << left << setw(12) << procs[i].id << setw(15) << procs[i].size;
        if(procs[i].partid == -1) cout << setw(18) << "N/A" << setw(12) << "N/A";
        else cout << setw(18) << procs[i].partid << setw(12) << procs[i].hole;
        cout << (procs[i].partid != -1);
    }
    cout << endl;
}

void first(vector<int> parts, vector<process> procs, int pa, int pr){
    for(int i = 0; i<pr; i++){
        for(int j = 0; j<pa; j++){
            if(procs[i].size <= parts[j]){
                procs[i].partid = j+1;
                parts[j] -= procs[i].size;
                procs[i].hole = parts[j];
                break;
            }
        }
    }
    
    cout << "\n========================== FIRST FIT ==========================\n";
    print(parts, procs, pr);
}

void best(vector<int> parts, vector<process> procs, int pa, int pr){
    for(int i = 0; i<pr; i++){
        int selected = -1, minsize = INT_MAX;
        for(int j = 0; j<pa; j++){
            if(procs[i].size < parts[j]){
                if(parts[j] < minsize) selected = j, minsize = parts[j];
            }
        }

        if(selected != -1){
            procs[i].partid = selected+1;
            parts[selected] -= procs[i].size;
            procs[i].hole = parts[selected];
        }
    }

    cout << "\n========================== BEST FIT =========================\n";
    print(parts, procs, pr);
}

void next(vector<int> parts, vector<process> procs, int pa, int pr){
    int lastidx = 0;
    for(int i = 0; i<pr; i++){
        int count = 0;
        while(count < pa){
            if(procs[i].size < parts[lastidx]){
                procs[i].partid = lastidx+1;
                parts[lastidx] -= procs[i].size;
                procs[i].hole = parts[lastidx];
                break;
            }
            lastidx = (lastidx + 1)%pa;
            count++;
        }
    }

    cout << "\n========================== NEXT FIT =========================\n";
    print(parts, procs, pr);
}

void worst(vector<int> parts, vector<process> procs, int pa, int pr){
    for(int i = 0; i<pr; i++){
        int selected = -1, maxsize = INT_MIN;
        for(int j = 0; j<pa; j++){
            if(procs[i].size < parts[j]){
                if(parts[j] > maxsize) selected = j, maxsize = parts[j];
            }
        }

        if(selected != -1){
            procs[i].partid = selected+1;
            parts[selected] -= procs[i].size;
            procs[i].hole = parts[selected];
        }
    }

    cout << "\n========================== WORST FIT =========================\n";
    print(parts, procs, pr);
}

int main(){
    int pa, pr;
    cout << "\nEnter number of memory partitions: ";
    cin >> pa;

    vector<int> parts(pa);
    cout << "\nEnter size of each partition:\n";
    for(int i = 0; i<pa; i++) cin >> parts[i];

    cout << "\nEnter number of processes: ";
    cin >> pr;

    vector<process> procs(pr);
    cout << "\nEnter size of each process:\n";
    for(int i = 0; i<pr; i++){
        procs[i].id = i+1; 
        cin >> procs[i].size;
        procs[i].partid = -1;
    }

    cout << "\nMemory partitions in system: \nID.\tSize\n";
    for(int i = 0; i<pa; i++) cout << i+1 << "\t" << parts[i] << endl;
    
    first(parts, procs, pa, pr);
    best(parts, procs, pa, pr);
    next(parts, procs, pa, pr);
    worst(parts, procs, pa, pr);
}