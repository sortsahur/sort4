#include <iostream>
#include <vector>
#include <climits>
using namespace std;

#define last 199
#define start 0
#define head 67

void sstf(vector<int> &tracks, int n){
    int moves = 0, totaldist = 0, curdist = 0;

    cout << "\nNo. of tracks: " << last-start+1;
    cout << "\nR/W head is initially at: " << head;
    cout << "\nRequest queue: ";
    for(int t:tracks) cout << t << " ";
    
    vector<int> order(n), visited(n, 0);
    int from = head;
    cout << "\n\n\tS.no.\tFrom\tTo\tDistance\n";

    for(int i = 0; i<n; i++){
        int closest = -1, mindist = INT_MAX;

        for(int j = 0; j<n; j++){
            if(!visited[j]){
                if(abs(tracks[j]-from) < mindist) 
                    mindist = abs(tracks[j]-from), closest = j; 
            }
        }

        moves++;
        curdist = mindist;
        visited[closest] = 1;
        order[i] = tracks[closest];
        cout << "\t" << moves << ".\t" << from << "\t" << tracks[closest] << "\t" << curdist << "\n";
        totaldist += curdist;
        from = tracks[closest];
    }

    cout << "\nOrder of execution: " << head;
    for(int o:order) cout << " -> " << o;
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

    sstf(tracks, n);

    return 0;
}