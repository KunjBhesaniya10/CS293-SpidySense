#include <bits/stdc++.h>
using namespace std;
double similarity(std::vector<int> &a, std::vector<int> &b,int i, int j, int size) {
    if (i+size > a.size() || j+size > b.size()) {
        return 0;
    }
    std::unordered_set<int> set1(a.begin()+i, a.begin()+i+size);
    std::unordered_set<int> set2(b.begin()+j, b.begin()+j+size);
    
    std::unordered_set<int> intersection;
    std::unordered_set<int> union_set(set1);
    
    for (const auto& token : set2) {
        union_set.insert(token);
        if (set1.find(token) != set1.end()) {
            intersection.insert(token);
        }
    }

    return static_cast<double>(intersection.size()) / union_set.size();
}
int main(){
    std::vector<int> a = {1,2,3,4,123,6,7,8,9,10};
    std::vector<int> b = {1,2,3,4,5,1,2,3,9,10};
    std::cout<<similarity(a,b,0,3,5)<<std::endl;
}