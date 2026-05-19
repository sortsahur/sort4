#include <iostream>
#include <vector>
using namespace std;

#define last 199
#define start 0
#define head 67

void fifo(vector<int> &tracks, int n){
    int moves = 0, totaldist = 0, curdist = 0;

    cout << "\nNo. of tracks: " << last-start+1;
    cout << "\nR/W head is initially at: " << head;
    cout << "\nRequest queue: ";
    for(int t:tracks) cout << t << " ";
    cout << "\nStarting direction: ";
    if(tracks[0]>head) cout << "Right"; else cout << "Left";

    cout << "\n\n\tS.no.\tFrom\tTo\tDistance\n";
    int from = head;
    
    for(int i = 0; i<n; i++){
        moves++;
        curdist = abs(tracks[i] - from);
        cout << "\t" << moves << ".\t" << from << "\t" << tracks[i] << "\t" << curdist << "\n";
        from = tracks[i];
        totaldist += curdist;
    }

    cout << "\nOrder of execution: " << head;
    for(int t:tracks) cout << " -> " << t;
    cout << "\nTotal track distance: " << totaldist;
    cout << "\nAverage seek length: " << (float)totaldist/moves << endl;
}

int main(){
    int n;
    cout << "\nEnter number of track requests: ";
    cin >> n;

    vector<int> tracks(n);

    cout << "\nEnter track indices: \n";
    for(int i = 0; i<n; i++) cin >> tracks[i];

    fifo(tracks, n);

    return 0;
}