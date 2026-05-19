#include <iostream>
#include <vector>
#include <cmath>
#include <cctype>
using namespace std;

struct block{
    char id;
    int size;
    bool free;
};

vector<block> mem = {{' ', 1024, true}};
char id = 'A';

void print(){
    cout << "\nMemory state: \n";
    for(auto& m:mem) cout << "[ " << m.size << "K (" << m.id << ") ]\t"; 
    cout << endl;
}

void request(){
    int req;
    cout << "\nEnter size of requested space for process " << id << ": ";
    cin >> req;

    int target = pow(2, ceil(log2(req)));

    for(int i = 0; i < mem.size(); i++){
        if(mem[i].free && mem[i].size >= target){
            while(mem[i].size > target){
                mem[i].size /= 2;
                mem.insert(mem.begin()+i+1, {' ', mem[i].size, true});
            }

            mem[i].free = false;
            mem[i].id = id;

            cout << "Memory block of size " << mem[i].size << "K allocated to " << id << "(" << req << "K)\n";
            print();
            id++;
            return; 
        }
    }

    cout << "[ERROR] Insufficient memory. Unable to allocate space.\n";
}

void release(){
    char relid;
    cout << "\nEnter ID of process to release space: ";
    cin >> relid;
    relid = toupper(relid);

    bool found = false;
    for(int i = 0; i<mem.size()-1; i++) if(mem[i].id == relid) mem[i].id = ' ', mem[i].free = true, found = true;
    
    if(!found){
        cout << "[ERROR] Process not found in the memory.\n";
        return;
    }
    
    bool merged = true;
    while(merged){
        merged = false;
        for(int i = 0; i<mem.size()-1; i++){
            if(mem[i].size == mem[i+1].size && mem[i].free && mem[i+1].free){
                mem[i].size *= 2;
                mem.erase(mem.begin()+i+1);
                merged = true;
                break;
            }
        }
    }

    cout << "Space for process " << relid << " released.\n";
    print();
}

int main(){
    int c;
    cout << "\n========== BUDDY SYSTEM ==========\n\nInitial memory block size = 1024kb\n";
    
    do{
        cout << "\nChoose an operation: \n 1. Request space\n 2. Release space\n 3. Exit\nYour choice: ";
        cin >> c;

        switch(c){
            case 1: request(); break;
            case 2: release(); break;
            case 3: cout << "\nExiting. Thank you!"; break;
            default: cout << "\nInvalid choice.";
        }
    } while(c!=3);
    
}