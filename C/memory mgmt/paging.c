#include <stdio.h>

int main() {
    int npages, pagesize, nframes;
    printf("\nEnter page size (words): ");
    scanf("%d", &pagesize);
    printf("Enter number of pages: ");
    scanf("%d", &npages);

    int pgtable[npages];
    printf("\nEnter page table (frame number for each page):\n");
    for (int i = 0; i < npages; i++) {
        printf("Page %d -> Frame: ", i);
        scanf("%d", &pgtable[i]);
    }

    printf("\nPage Table:\n%-15s%-15s\n", "Page Number", "Frame Number");
    for (int i = 0; i < npages; i++)
        printf("%-15d%-15d\n", i, pgtable[i]);

    int VA;
    printf("\nEnter virtual address: ");
    scanf("%d", &VA);

    int pageno = VA / pagesize;
    int offset = VA % pagesize;

    if (pageno >= npages) {
        printf("\nERROR: Invalid virtual address! Page %d does not exist.\n", pageno);
        return 0;
    }

    int frameno = pgtable[pageno];
    int PA = frameno * pagesize + offset;

    printf("\nPage Size: %d", pagesize);
    printf("\nPage Number: %d", pageno);
    printf("\nOffset: %d", offset);
    printf("\nFrame Number: %d", frameno);
    printf("\n\nPhysical Address: %d\n", PA);

    return 0;
}
