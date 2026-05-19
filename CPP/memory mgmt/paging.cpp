#include <iostream>
#include <vector>
using namespace std;

int main(){
    int memorysize = 120;
    int blocksize = 10;
    int VA, PA;

    vector<int> pgtable = {4, 8, 7, 3, 6, 5, 1, 9, 2, 10};
    
    cout << "\nSize of memory = " << memorysize << " words";
    cout << "\nSize of each block (page/frame) = " << blocksize << " words";
    cout << "\n\nPage Table Contents: \nLocation\tValue\n";
    for(int i = 0; i<blocksize; i++) cout << i << "\t\t" << pgtable[i] << endl;

    cout << "\n==========================================================\n\nEnter virtual address: ";
    cin >> VA;

    int pageno = VA/10;

    if(pageno >= pgtable.size()) {
        cout << "\n[ERROR] Invalid virtual address!";
        return 0;
    }

    int offset = VA%10;
    int frameno = pgtable[pageno];
    PA = frameno*blocksize + offset;

    cout << "\nPage Size: " << blocksize;
    cout << "\nPage Number: " << pageno;
    cout << "\nOffset: " << offset;
    cout << "\nFrame Number: " << frameno;
    cout << "\n\n==========================================================\n\nReal Address: " << PA << "\n";

    return 0;
}