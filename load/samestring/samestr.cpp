#include <iostream>
#include <fstream>
#include <string>
using namespace std;

char buffer[40] = {};
char mem[200][4] {};

void displaymem();

void load(){
    ifstream file("input.txt");
    if(!file.is_open()) { cerr << "Error opening file" << endl; return; }
    
    string line;
    int lineptr = 0;
    bool takedata = false;
    int instcnt = 0;

    while(getline(file, line)){
        string word = line.substr(0,4);

        if(word == "$AMJ"){
            //call init()
        }
        else if(word == "$DTA") {takedata = true ;//executeprogram()
        }
        else if(word == "$END"){
            displaymem(); //terminate
        }
        else if(!takedata){
            string instructions = line;

            for(int i = 0; i<instructions.length(); i++) buffer[i] = instructions[i];
            
            for(int i = 0; i<instructions.length(); i++){
                if(instcnt != 0 && instcnt%4 == 0) lineptr++;

                if(instructions[i] == 'H') {mem[lineptr][0] = 'H'; instcnt = ((instcnt/4)+1)*4;}
                else {mem[lineptr][instcnt%4] = buffer[i]; instcnt++;}
            }

            for(int i = 0; i<40; i++) buffer[i] = '-';
        }
    }
}

void displaymem(){
    for(int i = 0; i<200; i++){
        if(i%10 == 0 && i!=0) cout << "\n-------------------------------------------------------\n";
        cout << i << "\t" << mem[i][0] << mem[i][1] << mem[i][2] << mem[i][3] << endl;
    }
}

int main(){
    load();
    return 0;
}