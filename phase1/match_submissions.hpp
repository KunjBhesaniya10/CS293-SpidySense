#include <array>
#include<chrono>
#include <iostream>
#include <span>
#include <vector>
#include<cmath>
#include<map>
#include <utility>
#include<unordered_map>
#include<set>
#include<queue>
#include<algorithm>
#define ll long long 
#define THRESHOLD 0.7

// -----------------------------------------------------------------------------

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here


/// @brief Match struct to store interval of matching subsequence [start,start+length)
/// @param length: length of matching subsequence
/// @param start_file1: start index of matching subsequence in submission1
/// @param start_file2: start index of matching subsequence in submission2
/// @param end_file1: end index of matching subsequence in submission1
/// @param end_file2: end index of matching subsequence in submission2
struct Match{
    ll length;
    ll start_file1;
    ll start_file2;
    ll end_file1;
    ll end_file2;
};
/// @brief  Custom comparator for priority queue to sort Match objects based on start_file2
struct isLessthan2{
    bool operator()( Match &a,  Match &b){
        return a.start_file2 < b.start_file2;
    }
};

/// @brief  Custom comparator for priority queue to sort Match objects based on start_file1
struct isLessthan1{
    bool operator()( Match &a,  Match &b){
        return a.start_file1 < b.start_file1;
    } 
};

// -----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
// Jaccard Similarity
double similarity(std::vector<int>::iterator a, std::vector<int>::iterator b, int size) {
    std::unordered_set<int> set1(a, a+size);
    std::unordered_set<int> set2(b, b+size);
    
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

void filter_to_check(std::vector<int> &submission, std::vector<std::vector<int>::iterator> &to_check, int size_to_match){
    for (auto start_it = to_check.begin(); start_it != to_check.end(); start_it++){
        for (auto it = start_it+1; it != to_check.end(); it++){
            if (similarity(*start_it,*it,size_to_match) < 0.9){
                to_check.erase(it);
            }
        }
    }
}

void find_longest(std::vector<int> &submission1, std::vector<int> &submission2, int &start_idx1, int &start_idx2, int &pattern_size){
    for (int size_to_match = 20; size_to_match < min(submission1.size(),submission2.size()); size_to_match+=10){
        /// @brief  vector to store the indices of the elements to check for similarity
        std::vector<std::vector<int>::iterator> to_check = submission1.begin();
        for(int i=1; i<submission1.size()-size_to_match+1;i++){
            to_check.emplace_back(to_check[0]+i);
        }
        filter_to_check(submission1, to_check, size_to_match);//filtering out the elements which are similar in submission1

        // Checking for similarity in submission2
        for (auto it_to_check = to_check.begin(); it_to_check != to_check.end(); it_to_check++){
            for (int i = 0; i < submission2.size()-size_to_match; i++){
                if (similarity(*it_to_check,submission2.begin()+i,size_to_match) > 0.9){
                    start_idx1 = (int)(*it_to_check - submission1.begin());
                    start_idx2 = i;
                    if (size_to_match > pattern_size){
                        pattern_size = size_to_match;
                    }
                    int j = i + size_to_match;
                    while (j < min(submission1.size(),submission2.size()) && similarity((*it_to_check)+j-i,submission2.begin()+j, size_to_match) > 0.9){
                        j+=size_to_match;
                    }
                    if (j-i > pattern_size){
                        pattern_size = j-i;
                    }
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------

std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2) {
    // TODO: Write your code here

    // std::cerr<<hashing(submission1, submission1.size()).first<<'\n';;
    // std::cerr << hashing(submission2, submission2.size()).second<<'\n';
    std::array<int, 5> result = {0, 0, 0, 0, 0};
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto A = rolling_hash(submission1, submission2);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cerr << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;
    // for(Match a : A){
    //     std::cout << a.length <<" "<<a.start<<" "<<a.submission<<'\n';
    // }


    int idx1,idx2,longest_approx_pattern_size;
    find_longest(submission1,submission2,idx1,idx2,longest_approx_pattern_size);
    result[2] = longest_approx_pattern_size;
    result[3] = idx1;
    result[4] = idx2;

    return result; // dummy return
    // End TODO
}

// int main(){
//     std::vector<int> submission1 = {1,2,3,258,5,6,7,8,9,10,11};
//     std::vector<int> submission2 = {1,2,3,258,5,6,7,8,9,10,12};
//     std::array<int, 5> result = match_submissions(submission1, submission2);
//     for(int i = 0; i<5; i++){
//         std::cout << result[i]<<" ";
//     }
//     std::cout << '\n';
//     return 0;
// }