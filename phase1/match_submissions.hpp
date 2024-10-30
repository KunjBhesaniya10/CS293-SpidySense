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

/// @brief Calculating Hashes
std::pair<ll,ll> hashing(std::vector<int> text, ll len){
    ll prime = 1.0e9+7;
    // std::cout<<"prime: "<<prime<<std::endl;
    ll x = 1;
    ll hashed = 0;
    for(ll i =0; i<len; i++,x=((x%prime)*33)){
        hashed = (hashed%prime+ (((x%prime)*text[len-i-1]) % prime))%prime;
    }
    x/=33;
    hashed=hashed%prime;
    return {hashed, x};
}

void calculate_hashes(std::unordered_multimap<ll,ll> &hash_set, std::vector<int> &text, ll len){
    ll prime = 1.0e9+7;
    ll x = 1;
    ll hashed = 0;
    for(ll i =0; i<len; i++,x = ((x%prime)*33)){
        hashed= (hashed%prime+(((x%prime)*text[len-i-1]) % prime))%prime;
    }
    hashed = hashed % prime;
    x/=33;
    
    hash_set.insert({hashed,0});
    for(int i = 1; i<=text.size()-len; i++){
        hashed = (hashed - (text[i - 1] * x % prime) + prime) % prime;
        hashed = (hashed * 33 + text[i + len - 1]) % prime;
    
        hash_set.insert({hashed,i});
    }
}

std::pair<std::priority_queue<Match, isLessthan1>,std::priority_queue<Match, isLessthan2>> rolling_hash(std:: vector<int> &submission1, std::vector<int> &submission2){

    std::priority_queue<Match, isLessthan1> sub_1_matchings;
    std::priority_queue<Match, isLessthan2> sub_2_matchings;

    for(int match_len =10; match_len <= 20; match_len++){
        
        std::unordered_multimap<ll,ll> hash_set;
        if(match_len > submission1.size() || match_len > submission2.size()){
            return {0,0,0,0,0};
        }
        calculate_hashes(hash_set, submission1, match_len);
            // initialising the hash set for submission 2
        std::pair<ll,ll> v = hashing(submission2,match_len);
        ll hashed = v.first, x= v.second;
        ll prime= 1.0e9+7;
        auto it = hash_set.find(hashed);
        if( it != hash_set.end()){
            // std::cout << "Match found :"<<" "<<match_len<<" "<<hashed<<" "<<it->first<<'\n';
            // std::cout << "sub1 idx "<<it->second<<" sub2 idx "<<0<<'\n';
            Match m = {match_len, it->second, 0, it->second+match_len, match_len};
            sub_1_matchings.push(m);
            sub_2_matchings.push(m);
            hash_set.erase(it);
        }
        
        for(int i = 1; i<submission2.size()-match_len; i++){
            hashed = (hashed - (submission2[i - 1] * x % prime) + prime) % prime;
            hashed = (hashed * 33 + submission2[i + match_len - 1]) % prime;
            hashed=hashed%prime;
            it = hash_set.find(hashed);
            if( it != hash_set.end()){
                // std::cout << "Match found :"<<" "<<match_len<<" "<<hashed<<" "<<it->first<<'\n';
                // std::cout << "sub1 idx "<<it->second<<" sub2 idx "<<i<<'\n';
                Match m = {match_len, it->second, i, it->second+match_len, i+match_len};
                sub_1_matchings.push(m);
                sub_2_matchings.push(m);
                hash_set.erase(it);
            }
        }
    }
    return std::make_pair(sub_1_matchings, sub_2_matchings);
}
/// Hash Calculated and Values Matched

// -----------------------------------------------------------------------------

std::pair<std::priority_queue<Match, isLessthan1>,std::priority_queue<Match, isLessthan2>> remove_overlaps(std::pair<std::priority_queue<Match, isLessthan1> ,std::priority_queue<Match, isLessthan2>> &all_matchings){
    auto sub_1_matchings = all_matchings.first;
    auto sub_2_matchings = all_matchings.second;
    
}

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