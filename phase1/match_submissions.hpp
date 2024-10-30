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
#define ll long long 

// -----------------------------------------------------------------------------

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here


//for submission 2
// Match struct to store interval of matching subsequence [start,start+length)
struct Match{
    ll start;
    ll end;
};

bool overlap(Match a, Match b){
  return a.start < b.end && b.start < a.end;
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

std::array<int,5> rolling_hash(std:: vector<int> &submission1, std::vector<int> &submission2){
    
        std::vector<Match> final_matches;
        std::set<Match> existing_matches_sub1;
        std::set<Match> existing_matches_sub2;

        for(int match_len =10; match_len <= 20; match_len++){
            
            std::unordered_multimap<ll,ll> hash_set;
            if(match_len > submission1.size() || match_len > submission2.size()){
                return {0,0,0,0,0};
             }
            calculate_hashes(hash_set, submission1, match_len);
        
            std::pair<ll,ll> v = hashing(submission2,match_len);
            ll hashed = v.first, x= v.second;
            ll prime= 1.0e9+7;
            auto it = hash_set.find(hashed);
            if( it != hash_set.end()){
                    std::cout << "Match found :"<<" "<<match_len<<" "<<hashed<<" "<<it->first<<'\n';
                    std::cout << "sub1 idx "<<it->second<<" sub2 idx "<<0<<'\n';
                    
                    // checking for overlap with existing matches in submission 1
                    // Match sub1_match = {it->second, it->second+match_len};
                    // bool overlap_flag = false;
                    // do{
                    // auto it = existing_matches_sub1.lower_bound(sub1_match);
                    // auto it2 = existing_matches_sub1.lower_bound(sub1_match);
                    // }
                    // Match sub2_match = {match_len,0};
                    // f
                    
                    // hash_set.erase(it);
                    // }
            }
            
            for(int i = 1; i<submission2.size()-match_len; i++){
                hashed = (hashed - (submission2[i - 1] * x % prime) + prime) % prime;
                hashed = (hashed * 33 + submission2[i + match_len - 1]) % prime;
                hashed=hashed%prime;
                it = hash_set.find(hashed);
                if( it != hash_set.end()){
                    std::cout << "Match found :"<<" "<<match_len<<" "<<hashed<<" "<<it->first<<'\n';
                    std::cout << "sub1 idx "<<it->second<<" sub2 idx "<<i<<'\n';

                    hash_set.erase(it);
                }
            }

        }
    
return {0,0,0,0,0};
}

std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2) {
    // TODO: Write your code here

//    std::cout<<"\n\n\n\n\n\n\n";
//     std::cerr << submission1.size()<<" "<<submission2.size()<<'\n';
//     for(int i=0; i<submission1.size(); i++){
//         std::cerr <<"("<<i<<","<<submission1[i]<<") ";
//     }
//     std::cerr << '\n';
//     std::cerr<< '\n';
//     for(int i=0; i<submission2.size(); i++){
//         std::cerr <<"("<<i<<","<<submission2[i]<<") ";
//     }

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