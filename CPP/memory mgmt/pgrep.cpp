#include <iostream>
#include <vector>
#include <unordered_map>
#include <climits>
using namespace std;

const int numframes = 4;

int fifo(string ref, vector<char> frames){
    int pf = 0, idx = 0;
    cout << "\n\nFIFO:\n\tC\tF0\tF1\tF2\tF3\tPF" << endl;

    for(char c:ref){
        bool absent = true;

        for(int i = 0; i<numframes; i++) if(frames[i] == c) absent = false;

        if(absent){
            frames[idx] = c;
            idx = (idx+1)%numframes;
            pf++;
        }

        cout << "\t" << c << "\t" << frames[0] << "\t" << frames[1] << "\t" << frames[2] << "\t" << frames[3] << "\t" << (absent ? 1 : 0) << endl;
    }

    cout << "\nNo. of page faults that occured using FIFO: = " << pf;
    cout << "\n--------------------------------------------------------------------------\n";

    return pf;
}

int lru(string ref, vector<char> frames){
    int pf = 0;
    cout << "\n\nLRU:\n\tC\tF0\tF1\tF2\tF3\tPF" << endl;

    unordered_map<char, int> timestamps;

    for(int i = 0; i<ref.length(); i++){
        bool absent = true;

        for(int j = 0; j<numframes; j++) if(frames[j] == ref[i]) absent = false;

        if(absent){
            pf++;
            int replace = -1;

            for(int j = 0; j<numframes; j++) if(!frames[j]) {replace = j; break;}

            if(replace == -1){
                int min = INT_MAX;
                for(int j = 0; j<numframes; j++){
                    if(timestamps[frames[j]] < min) min = timestamps[frames[j]], replace = j;
                }

                timestamps[frames[replace]] = -1;
            }

            frames[replace] = ref[i];
        }

        cout << "\t" << ref[i] << "\t" << frames[0] << "\t" << frames[1] << "\t" << frames[2] << "\t" << frames[3] << "\t" << (absent ? 1 : 0) <<endl;
        timestamps[ref[i]] = i;
    }

    cout << "\nNo. of page faults that occured using LRU: = " << pf;
    cout << "\n--------------------------------------------------------------------------\n";

    return pf;
}

int opt(string ref, vector<char> frames){
    int pf = 0;
    cout << "\n\nOPT:\n\tC\tF0\tF1\tF2\tF3\tPF" << endl;

    for(int i = 0; i<ref.length(); i++){
        bool absent = true;

        for(int j = 0; j<numframes; j++) if(frames[j] == ref[i]) absent = false;

        if(absent){
            pf++;
            int replace = -1;

            for(int j = 0; j<numframes; j++) if(!frames[j]) {replace = j; break;}

            if(replace == -1){
                int far = -1;
                for(int j = 0; j<numframes; j++){
                    int nextocc = INT_MAX;

                    for(int k = i+1; k<ref.length(); k++){
                        if(ref[k] == frames[j]) {nextocc = k; break;} 
                    }

                    if(nextocc>far) far = nextocc, replace = j;
                }
            }

            frames[replace] = ref[i];
        }

        cout << "\t" << ref[i] << "\t" << frames[0] << "\t" << frames[1] << "\t" << frames[2] << "\t" << frames[3] << "\t"<< (absent ? 1 : 0) << endl;
    }

    cout << "\nNo. of page faults that occured using OPTIMAL: = " << pf;
    cout << "\n--------------------------------------------------------------------------\n";

    return pf;
}

int main(){
    vector<char> frames(numframes);
    string ref;
    cout << "\nEnter the reference string: ";
    cin >> ref;

    cout << "\nKey:\n C = Current Process\n Fi = Frame i\n PF = Page Fault Flag\n\n";
    int fpf = fifo(ref, frames);
    int lpf = lru(ref, frames);
    int opf = opt(ref, frames);

    cout << "\n\nSUMMARY:\n-> FIFO: " << fpf << " page faults\n-> LRU: " << lpf << " page faults\n-> OPTIMAL: " << opf << " page faults\n";
    cout << "\nThus, for the entered string, " << ref;

    if(fpf < min(lpf, opf)) cout << ", FIFO is most suitable!";
    else if(lpf < min(fpf, opf)) cout << ", LRU is most suitable!";
    else cout << ", OPTIMAL is most suitable!";

    return 0;
}