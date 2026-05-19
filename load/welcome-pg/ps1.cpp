#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
using namespace std;

// 400 words memory with paging (40 frames * 10 words per frame)
char mem[400][4] = {};
char R[4] = {};
char IR[4] = {};
int IC = 0;
bool C = false;
int SI = 0;
int pagesize = 10;
int totalframes = 40;

char buffer[40] = {};
int PTR = 0; // Page Table Register
vector<int> frametable; // Tracks which frames are allocated

void init() {
    IC = 0;
    C = false;
    SI = 0;
    PTR = 0;
    for(int i = 0; i < 4; i++) R[i] = 0;
    for(int i = 0; i < 4; i++) IR[i] = 0;
    for(int i = 0; i < 400; i++)
        for(int j = 0; j < 4; j++) mem[i][j] = '-';
    
    frametable.clear();
    for(int i = 0; i < totalframes; i++) frametable.push_back(i);
}

int allocateFrame() {
    if(frametable.empty()) return -1;
    int frame = frametable.back();
    frametable.pop_back();
    return frame;
}

void displaymem() {
    for(int i = 0; i < 400; i++) {
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
    int currentpage = 0;
    
    while(getline(file, line)) {
        string word = line.substr(0, 4);
        
        if(word == "$AMJ") {
            init();
            int frame = allocateFrame();
            if(frame != -1) PTR = frame * pagesize;
        }
        else if(word == "$DTA") {
            dataPhase = true;
        }
        else if(word == "$END") {
            displaymem();
            break;
        }
        else if(!dataPhase) {
            // Load instructions with paging
            int instcnt = 0;
            for(int i = 0; i < line.length(); i++) buffer[i] = line[i];
            
            for(int i = 0; i < line.length(); i++) {
                if(instcnt != 0 && instcnt % 4 == 0) {
                    lineptr++;
                    // Check if we need a new frame
                    if(lineptr % pagesize == 0) {
                        int newframe = allocateFrame();
                        if(newframe != -1) lineptr = newframe * pagesize;
                    }
                }
                
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
