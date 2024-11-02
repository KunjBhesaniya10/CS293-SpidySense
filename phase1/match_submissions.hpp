#include <array>
#include<chrono>
#include <iostream>
#include <span>
#include <vector>
#include<cmath>
#include<map>
#include <utility>
#include<unordered_set>
#include<unordered_map>
#include<set>
#include<queue>
#include<algorithm>
#define ll long long 
#define THRESHOLD 0.85

#include <iostream>

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
    ll start_file1;
    ll start_file2;
    ll end_file1;
    ll end_file2;
    ll length;
};


bool sortMatches(Match &a, Match &b){
    if(a.start_file1 == b.start_file1){
        return a.start_file2 < b.start_file2;
    }
    return a.start_file1 < b.start_file1;
}



//-----------------------------------------------------------------------------
// Jaccard Similarity
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

void filter_to_check(std::vector<int> &submission, std::vector<int> &to_check, int size_to_match){
    for (int start_it = 0; start_it < to_check.size(); start_it++){
        for (int it = start_it+1; it < to_check.size(); it++){
            if (similarity(submission,submission,to_check[start_it],to_check[it],size_to_match) < THRESHOLD){
                to_check.erase(to_check.begin()+it);
            }
        }
    }
}

void find_longest(std::vector<int> &submission1, std::vector<int> &submission2, int &start_idx1, int &start_idx2, int &pattern_size){

    std::vector<Match> matches;
    int max_window_size = std::min(submission1.size(),submission2.size());
    int min_window_size = 20;
    int mid;

    while (max_window_size >= min_window_size){
        mid = (min_window_size+max_window_size)/2;
        std::cout<<"mid: "<<mid<<std::endl;
        bool update = 0;
        for (int i = 0; i < submission1.size()-mid+1; i++){
            for (int j = 0; j < submission2.size()-mid+1; j++){
                if (similarity(submission1,submission2,i,j,mid) > THRESHOLD){
                    Match m = {i,j,i+mid-1,j+mid-1,mid};
                    matches.push_back(m);
                    update= 1;
                    break;
                }
            }
            if (update){
                break;
            }
        }
        if (update){
            min_window_size = mid+1;
        }
        else{
            max_window_size = mid-1;
        }
    }

    Match longest_match = matches[matches.size()-1];
    pattern_size = longest_match.length;
    start_idx1 = longest_match.start_file1;
    start_idx2 = longest_match.start_file2;
    std::cerr<< "Sub1"<<std::endl;
    for (int  i = start_idx1; i < pattern_size; i++){
        std::cerr<<submission1[i]<<" ";
    }
    std::cerr<<std::endl;
    std::cerr<< "Sub2"<<std::endl;
    for (int  i = start_idx2; i < pattern_size; i++){
        std::cerr<<submission2[i]<<" ";
    }
    std::cerr<<std::endl;
}

//-----------------------------------------------------------------------------
bool evaluate_plag(int short_match_length, int large_match_length, int total_length){
    if(float(short_match_length)/total_length >= 0.6){
        return true;
    }
    else if(float(large_match_length)/total_length >= 0.5){
        return true;
    }
    else return false;
}
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

/// @brief  Function to calculate the hash of the text
/// @param hash_set 
/// @param text 
/// @param len 
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

Match* upper_nonoverlap_match(std::vector<Match> &matches, Match &m, ll start){
    ll end = matches.size()-1;
    ll i = start;
    ll curr_end1 = m.end_file1; ll  curr_end2 = m.end_file2;
    while(start<=end){
        ll mid = start + (end-start)/2;
        if(matches[mid].start_file1 > curr_end1 && matches[mid].start_file2  > curr_end2){
            end = mid-1;
        }
        else{
            start = mid+1;
        }
    }
    if (start == matches.size() || end == i-1){
        return nullptr;
    }
    else return &matches[start];
}

ll remove_overlap(std::vector<Match> &matches){
    sort(matches.begin(), matches.end(), sortMatches);
    ll n = matches.size();
    std::vector<ll> dp(n+1,0);
    dp[n] = 0;
    for(ll i = n-1; i>=0; i--){
        Match *firstNonOverlap = upper_nonoverlap_match(matches, matches[i], i+1);
        if(firstNonOverlap == nullptr){
            dp[i] = std::max(matches[i].length,dp[i+1]);
        }
        else{
            dp[i] = std::max(dp[i+1], matches[i].length + dp[firstNonOverlap-matches.data()]);
        }        
    }
    return dp[0];
}

int rolling_hash(std:: vector<int> &submission1, std::vector<int> &submission2){
        std::vector<Match> sub1_match;
        std::vector<Match> final_matches;
        int total_length ; // total length of the non overlaping matching subsequence.
        bool is_plagiarised = false;
        for(int match_len =20; match_len >= 10; match_len--){
            std::unordered_multimap<ll,ll> hash_set;
            if(match_len > submission1.size() || match_len > submission2.size()){
                return 0;
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
                    Match m = {it->second, 0, it->second+match_len-1,match_len-1, match_len};
                    sub1_match.push_back(m);
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
                    Match m = {it->second, i, it->second+match_len-1, i+match_len-1,match_len};
                    sub1_match.push_back(m);
                    hash_set.erase(it);
                }
            }
        }
        short_total_length = remove_overlap(sub1_match);

    return short_total_length;
}

// longest common subsequence
struct LCSelement{
    int element;
    int start_file1;
    int start_file2;
};

double similarity_score(int window_size, int matching_len){
    return ((double)matching_len)/window_size;
}

void Large_pattern(std::vector<LCSelement> &lcs, int &start_idx1, int &start_idx2, int &pattern_size){
    int max_window_size = 0;
    int idx1,idx2;
    for(int i=0; i< lcs.size(); i++){
        for(int j=i+1; j <lcs.size(); j++){
            int window_size = std::max(lcs[i].start_file1-lcs[j].start_file1+1,lcs[i].start_file2-lcs[j].start_file2+1);
            int matching_len = j-i+1;
            // std::cerr<<"window_size: "<<window_size<<" matching_len: "<<matching_len<<'\n';
            if(similarity_score(window_size,matching_len) >= THRESHOLD){
                // std::cerr <<window_size<<" "<<matching_len<<" "<<float(matching_len/window_size)<<'\n';
                if(max_window_size < window_size){
                    max_window_size = window_size;
                    idx1 = lcs[j].start_file1;
                    idx2 = lcs[j].start_file2;
                }
                // std::cerr<<"window_size: "<<window_size<<" matching_len: "<<matching_len<<'\n';
                // for(int k = i; k<=j; k++){
                //     std::cerr<<lcs[k].element<<" ";
                // }
                // std::cerr<<'\n';
        }
    }
    }
    std::cerr<<"max_window_size: "<<max_window_size<<'\n';
    std::cerr<<"idx1: "<<idx1<<" idx2: "<<idx2<<'\n';
    pattern_size = max_window_size;
    start_idx1 = idx1;
    start_idx2 = idx2;
}
void longestCommonSubsequence(std::vector<int> &X, std::vector<int> &Y, int &start_idx1, int &start_idx2, int &pattern_size) {
    int m = X.size();
    int n = Y.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (X[i - 1] == Y[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    std::vector<LCSelement> lcs;

    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcs.push_back({X[i - 1], i - 1, j - 1});
            --i;
            --j;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }
    Large_pattern(lcs,start_idx1,start_idx2,pattern_size);
    std::cerr<<"lcs size: "<<lcs.size()<<'\n';
}

// -----------------------------------------------------------------------------

std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2) {
    // TODO: Write your code here
    std::cerr << "submission1 size: "<<submission1.size()<<'\n';
    std::cerr << "submission2 size: "<<submission2.size()<<'\n';
    std::cerr << '\n';
    std::array<int, 5> result = {0, 0, 0, 0, 0};
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    result[1]= rolling_hash(submission1, submission2);

    int start_idx1, start_idx2, pattern_size;
    longestCommonSubsequence(submission1,submission2,start_idx1,start_idx2,pattern_size);
    result[2] = pattern_size;
    result[3] = start_idx1;
    result[4] = start_idx2;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cerr << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;

    result[0] = evaluate_plag(result[1],result[2],std::min(submission1.size(),submission2.size()));

    return result;
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