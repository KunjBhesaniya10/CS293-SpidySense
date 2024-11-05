#include <array>
#include <iostream>
#include <span>
#include <vector>
#include <cmath>
// -----------------------------------------------------------------------------
#include <utility>
#include <unordered_map>
#include <algorithm>
#define ll long long
#define THRESHOLD 0.85
#define PRIME 1000000007LL

// -----------------------------------------------------------------------------

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here

// Struct to store a matching subsequence
struct Match
{
    int start_file1;
    int start_file2;
    int end_file1;
    int end_file2;
    int length;
};

bool sortMatches(Match &a, Match &b)
{
    if (a.start_file1 == b.start_file1)
    {
        return a.start_file2 < b.start_file2;
    }
    return a.start_file1 < b.start_file1;
}

//-----------------------------------------------------------------------------
// Function to evaluate the boolean plagiarism
bool evaluate_plag(int short_match_length, int large_match_length, int total_length)
{
    if (float(short_match_length) / total_length >= 0.6)
    {
        return true;
    }
    else if (float(large_match_length) / total_length >= 0.5)
    {
        return true;
    }
    else
        return false;
}

//-----------------------------------------------------------------------------
//code to find short matching patterns

// calculating hash for first window of submission.
std::pair<ll, int> hashing(std::vector<int> text, int len)
{
    ll x = 1;
    ll hashed = 0;
    for (ll i = 0; i < len; i++, x = ((x % PRIME) * 33))
    {
        hashed = (hashed % PRIME + (((x % PRIME) * text[len - i - 1]) % PRIME)) % PRIME;
    }
    x /= 33;
    hashed = hashed % PRIME;
    return {hashed, x};
}

// calculating hash for all the windows of submission by rolling hash technique.
void calculate_hashes(std::unordered_multimap<ll, int> &hash_set, std::vector<int> &text, ll len)
{
    std::pair<ll, int> first_hash = hashing(text, len);
    ll hashed = first_hash.first, x = first_hash.second;
    hash_set.insert({hashed, 0});
    for (int i = 1; i <= text.size() - len; i++)
    {
        hashed = (hashed - (text[i - 1] * x % PRIME) + PRIME) % PRIME;
        hashed = (hashed * 33 + text[i + len - 1]) % PRIME;

        hash_set.insert({hashed, i});
    }
}

// Function to find first non overlapping match after the current match
Match *upper_nonoverlap_match(std::vector<Match> &matches, Match &m, int start)
{
    int end = matches.size() - 1;
    int i = start;
    int curr_end1 = m.end_file1;
    int curr_end2 = m.end_file2;
    while (start <= end)
    {
        int mid = (start+end)/2;
        if (matches[mid].start_file1 > curr_end1 && matches[mid].start_file2 > curr_end2)
        {
            end = mid - 1;
        }
        else
        {
            start = mid + 1;
        }
    }
    if (start == matches.size() || end == i - 1)
    {
        return nullptr;
    }
    else
        return &matches[start];
}

// Function to remove overlapping matches from the total matches.
// logic adapted from below citation
/* CITATION : https://stackoverflow.com/questions/24026073/algorithm-to-find-maximum-coverage-of-non-overlapping-sequences-i-e-the-weig*/
int remove_overlap(std::vector<Match> &matches)
{
    sort(matches.begin(), matches.end(), sortMatches);
    int n = matches.size();
    std::vector<int> dp(n + 1, 0);
    dp[n] = 0;
    for (int i = n - 1; i >= 0; i--)
    {
        Match *firstNonOverlap = upper_nonoverlap_match(matches, matches[i], i + 1);
        if (firstNonOverlap == nullptr)
        {
            dp[i] = std::max(matches[i].length, dp[i + 1]);
        }
        else
        {
            dp[i] = std::max(dp[i + 1], matches[i].length + dp[firstNonOverlap - matches.data()]);
        }
    }
    return dp[0];
}

// Function to find the short matching patterns using rolling hash technique.
int rolling_hash(std::vector<int> &submission1, std::vector<int> &submission2)
{
    std::vector<Match> sub1_match;
    std::vector<Match> final_matches;
    int total_length=0; // total length of the non overlaping matching subsequence.
    bool is_plagiarised = false;
    for (int match_len = 20; match_len >= 10; match_len--)
    {
        std::unordered_multimap<ll, int> hash_set;
        if (match_len > submission1.size() || match_len > submission2.size()) continue;
    
        calculate_hashes(hash_set, submission1, match_len);

        std::pair<ll, int> v = hashing(submission2, match_len);
        ll hashed = v.first, x = v.second;
        auto it = hash_set.find(hashed); 
        if (it != hash_set.end())
        {
            Match m = {it->second, 0, it->second + match_len - 1, match_len - 1, match_len};
            sub1_match.push_back(m);
            hash_set.erase(it);  
        }
        for (int i = 1; i <= submission2.size() - match_len; i++)
        {
            //calculating the hash for the next window.
            hashed = (hashed - (submission2[i - 1] * x % PRIME) + PRIME) % PRIME;
            hashed = (hashed * 33 + submission2[i + match_len - 1]) % PRIME;
            hashed = hashed % PRIME;
            it = hash_set.find(hashed); //searching for the match in the hash_set.
            if (it != hash_set.end())
            {
                Match m = {it->second, i, it->second + match_len - 1, i + match_len - 1, match_len};
                sub1_match.push_back(m);
                hash_set.erase(it); //erasing to ensure no other pattern matches to the same pattern.
            }
        }
    }
    total_length = remove_overlap(sub1_match);

    return total_length;
}

//-----------------------------------------------------------------------------
// Code to find the longest matching pattern

// Struct to store the LCS element
struct LCSelement
{
    int element;
    int start_file1;
    int start_file2;
};

// Function to calculate the similarity score
double similarity_score(int window_size, int matching_len)
{
    return ((double)matching_len) / window_size;
}

// finding large pattern from the lcs which has similarity score greater than threshold.
std::array<int,3> Large_pattern(std::vector<LCSelement> &lcs)
{
    int max_window_size = 0;
    int idx1, idx2;
    for (int i = 0; i < lcs.size(); i++)
    {
        for (int j = i + 1; j < lcs.size(); j++)
        {
            int window_size = std::max(lcs[j].start_file1 - lcs[i].start_file1 + 1,
             lcs[j].start_file2 - lcs[i].start_file2 + 1);
            
            int matching_len = j - i + 1;
            if (similarity_score(window_size, matching_len) >= THRESHOLD){
                if (max_window_size < window_size)
                {
                    max_window_size = window_size;
                    idx1 = lcs[i].start_file1;
                    idx2 = lcs[i].start_file2;
                }
            }
        }
    }
    // if no large pattern found then setting the values to 0.
    if (max_window_size < 30){
        max_window_size = 0;
        idx1 = -1;
        idx2 = -1;
    }
    return {max_window_size,idx1,idx2};
}

// flitering lcs such that there is continous sequence of at least 10 elements.
void filter_lcs(std::vector<LCSelement> &lcs)
{
    int i = 0;
    int j = 0;
    while (i < lcs.size() && j < lcs.size())
    {
        while (j < lcs.size() && lcs[j].start_file1 == lcs[i].start_file1 + j - i 
        && lcs[j].start_file2 == lcs[i].start_file2 + j - i)
        {
            j++;
        }

        if (j - i >= 10){
            i = j;
        }
        else if (j - i <= 9){
            do
            {
                lcs[i].element = -1;
                i++;
            } while (i < lcs.size() && i != j);
        }
    }
    for (int k = 0; k < lcs.size(); k++)
    {
        if (lcs[k].element == -1){
            lcs.erase(lcs.begin() + k);
            k--;
        }
    }
}

// finding longest pattern by obtaining LCS using dynamic programming and extracting
// out pattern from it which satisfies the similarity threshold.
// LCS finding code is adapted from below citation
// CITATION - https://www.geeksforgeeks.org/printing-longest-common-subsequence/
std::array<int,3> longestPattern(std::vector<int> &sub1, std::vector<int> &sub2)
{
    
    std::vector<std::vector<int>> dp(sub1.size() + 1, std::vector<int>(sub2.size() + 1, 0));

    for (int i = 1; i <=sub1.size(); ++i)
    {
        for (int j = 1; j <= sub2.size(); ++j)
        {
            if (sub1[i - 1] == sub2[j - 1]){
                dp[i][j] = dp[i - 1][j - 1] + 1;
            }
            else{
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    std::vector<LCSelement> lcs;   // to store longest common subsequence of both file.
    int i = sub1.size(), j = sub2.size();
    while (i > 0 && j > 0)
    {
        if (sub1[i - 1] == sub2[j - 1]){
            lcs.push_back({sub1[i - 1], i - 1, j - 1});
            i--;j--;
        }
        else if (dp[i - 1][j] > dp[i][j - 1]) {
            i--;
        }
        else{
            j--;
        }
    }
    // lcs is in reverse order so reversing it.
    std::reverse(lcs.begin(), lcs.end());
    filter_lcs(lcs); 
    return Large_pattern(lcs);
}

//-------------------------------------------------------------------------------   
// Main function to match the submissions
std::array<int, 5> match_submissions(std::vector<int> &submission1, std::vector<int> &submission2)
{
    // TODO: Write your code here
    std::array<int, 5> result = {0, 0, 0, 0, 0};
    result[1] = rolling_hash(submission1, submission2);
    auto res = longestPattern(submission1, submission2);
    result[2] = res[0];
    result[3] = res[1];
    result[4] = res[2];
    result[0] = evaluate_plag(result[1], result[2], std::min(submission1.size(), submission2.size()));
    return result;
    // End TODO
}