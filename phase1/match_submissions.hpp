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


//for submission 2
// Match struct to store interval of matching subsequence [start,start+length)
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

//for submission 1
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

std::array<int,5> rolling_hash(std:: vector<int> &submission1, std::vector<int> &submission2){
        std::vector<Match> sub1_match;
        std::vector<Match> final_matches;
        int total_length ; // total length of the non overlaping matching subsequence.
        bool is_plagiarised = false;
        for(int match_len =20; match_len >= 10; match_len--){
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
             total_length = remove_overlap(sub1_match);
           
             std::cerr << total_length <<'\n';
            if ( float(total_length)/std::min(submission1.size(),submission2.size()) > 0.6){
                is_plagiarised = true;
            }   
        std::cerr<<"is_plagiarised: "<<is_plagiarised<<'\n';
    
return {is_plagiarised,total_length,0,0,0};
}

// longest common subsequence
int longestCommonSubsequence(std::vector<int> &X, std::vector<int> &Y) {
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

    return dp[m][n];
}

std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2) {
    // TODO: Write your code here
    std::cerr << "submission1 size: "<<submission1.size()<<'\n';
    std::cerr << "submission2 size: "<<submission2.size()<<'\n';
    // std::cerr<<hashing(submission1, submission1.size()).first<<'\n';;
    // std::cerr << hashing(submission2, submission2.size()).second<<'\n';
    for(int i = 0; i<submission1.size(); i++){
        std::cerr << submission1[i]<<" ";
    }
    std::cerr << '\n';
    for(int i = 0; i<submission2.size(); i++){
        std::cerr << submission2[i]<<" ";
    }   
    std::cerr << '\n';
    std::array<int, 5> result = {0, 0, 0, 0, 0};
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto A = rolling_hash(submission1, submission2);
    auto len = longestCommonSubsequence(submission1, submission2);
    std::cout << "LCS: " << len << std::endl;
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;

    return A; // dummy return
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