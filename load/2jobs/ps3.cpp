#include <iostream>
#include <fstream>
#include <string>
using namespace std;

char mem[200][4] = {};
char R[4] = {};
char IR[4] = {};
int IC = 0;
bool C = false;
int SI = 0;

char buffer[40] = {};

void init() {
    IC = 0;
    C = false;
    SI = 0;
    for(int i = 0; i < 4; i++) R[i] = 0;
    for(int i = 0; i < 4; i++) IR[i] = 0;
    for(int i = 0; i < 200; i++)
        for(int j = 0; j < 4; j++) mem[i][j] = '-';
    for(int i = 0; i < 40; i++) buffer[i] = '-';
}

void displaymem() {
    for(int i = 0; i < 200; i++) {
        if(i % 10 == 0 && i != 0) cout << "\n---\n";
        cout << i << "\t" << mem[i][0] << mem[i][1] << mem[i][2] << mem[i][3] << endl;
    }
}
void load() {
    ifstream file("input.txt");
    if(!file.is_open()) { cerr << "Error opening file" << endl; return; }
    
    string line;
    int lineptr = 0;
    bool dataPhase = false;
    int progcount = 0;
    
    while(getline(file, line)) {
        string word = line.substr(0, 4);
        
        if(word == "$AMJ") {
            init();
            lineptr = 0;
            progcount++;
            cout << "\n=== Loading Program " << progcount << " ===\n";
            dataPhase = false;
        }
        else if(word == "$DTA") {
            dataPhase = true;
            //executeprog();
        }
        else if(word == "$END") {
            cout << "Program " << progcount << " ended\n";
            displaymem();
            continue;
        }
        else if(!dataPhase) {
            // Load instructions
            int instcnt = 0;
            for(int i = 0; i < line.length(); i++) buffer[i] = line[i];
            
            for(int i = 0; i < line.length(); i++) {
                if(instcnt != 0 && instcnt % 4 == 0) lineptr++;
                
                if(line[i] == 'H') {
                    mem[lineptr][instcnt % 4] = 'H';
                    instcnt = ((instcnt / 4) + 1) * 4;
                }
                else {
                    mem[lineptr][instcnt % 4] = buffer[i];
                    instcnt++;
                }
            }
        }
    }
    file.close();
}

int main() {
    load();
    return 0;
}
