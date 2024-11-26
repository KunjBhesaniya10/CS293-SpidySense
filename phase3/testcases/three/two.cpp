#include <iostream>
#include <vector>

void swapElements(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void selectionSortVerbose(std::vector<int>& array) {
    int length = array.size();
    for (int currentIndex = 0; currentIndex < length - 1; currentIndex++) {
        int minIndex = currentIndex;
        for (int searchIndex = currentIndex + 1; searchIndex < length; searchIndex++) {
            if (array[searchIndex] < array[minIndex]) {
                minIndex = searchIndex;
            }
        }
        swapElements(array[currentIndex], array[minIndex]);
    }
}
