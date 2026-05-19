#include <iostream>
#include <vector>
using namespace std;

struct segment{
    int base;
    int offset;
};

int main(){
    vector<segment> segtable = {
        {1000, 875},
        {4550, 960},
        {2050, 1000},
        {6200, 220},
        {3220, 180}
    };

    cout << "\nSegment Table: \nSeg. No.\tBase Addr.\tOffset\n";
    for(int i = 0; i<segtable.size(); i++) cout << i << "\t\t" << segtable[i].base << "\t\t" << segtable[i].offset << endl;
    
    int segno, offset, PA;

    cout << "\n==========================================================\n\nEnter virtual address: ";
    cout << "\n-> Segment number: ";
    cin >> segno;
    
    if(segno > segtable.size()-1 || segno < 0){
        cout << "\n[ERROR]: SEGMENTATION FAULT. Invalid segment number.";
        return 1;
    }

    cout << "-> Offset: ";
    cin >> offset;

    if(offset > segtable[segno].offset){
        cout << "\n[ERROR]: SEGMENTATION FAULT. Offset exceeds size of segment.";
        return 1;
    }

    cout << "\nBase address for selected segment: " << segtable[segno].base;
    cout << "\nSize of selected segment: " << segtable[segno].offset;

    PA = segtable[segno].base + offset;

    cout << "\n==========================================================\n\nReal Address: " << PA << "\n";

    return 0;
}