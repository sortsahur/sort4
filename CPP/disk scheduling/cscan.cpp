#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define last 199
#define start 0
#define head 67

void scan(vector<int> &tracks, int n){
    int moves = 0, totaldist = 0, curdist = 0;
    vector<int> lt, gt;

    cout << "\nNo. of tracks: " << last-start+1;
    cout << "\nR/W head is initially at: " << head;
    cout << "\nRequest queue: ";
    for(int t:tracks){
        cout << t << " ";
        if(t>head) gt.push_back(t);
        else lt.push_back(t);
    }
    cout << "\nStarting direction: Right";

    sort(gt.begin(), gt.end());
    sort(lt.begin(), lt.end());

    int from = head;
    cout << "\n\n\tS.no.\tFrom\tTo\tDistance\n";
    
    for(int i = 0; i<gt.size(); i++){
        moves++;
        curdist = abs(gt[i] - from);
        cout << "\t" << moves << ".\t" << from << "\t" << gt[i] << "\t" << curdist << "\n";
        from = gt[i];
        totaldist += curdist;
    }

    moves++;
    curdist = abs(last-from);
    cout << "\t" << moves << ".\t" << from << "\t" << last << "\t" << curdist << "\n";
    from = last;
    totaldist += curdist;

    moves++;
    curdist = abs(start-last);
    cout << "\t" << moves << ".\t" << from << "\t" << start << "\t" << curdist << "\n";
    from = start;
    totaldist += curdist;

    for(int i = 0; i<lt.size(); i++){
        moves++;
        curdist = abs(lt[i] - from);
        cout << "\t" << moves << ".\t" << from << "\t" << lt[i] << "\t" << curdist << "\n";
        from = lt[i];
        totaldist += curdist;
    }

    cout << "\nOrder of execution: " << head;
    for(int i:gt) cout << " -> " << i;
    cout << " -> " << last << " -> " << start;
    for(int i = 0; i<lt.size(); i++) cout << " -> " << lt[i];
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

    scan(tracks, n);

    return 0;
}