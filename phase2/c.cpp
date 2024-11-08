#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <functional>
#include <chrono>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <memory>

#define PRIME 1000000007LL
#define ll long long int 
#include "../tokenizer.hpp"
// You should NOT modify ANYTHING in this file.

tokenizer_t::tokenizer_t(std::string __file_name) {
    this->file_name = __file_name;
    this->index = clang_createIndex(0, 0);
    const char* args[] = {"-std=c++20"};
    this->unit = clang_parseTranslationUnit(
        index,
        this->file_name.c_str(), 
        args, 1,
        nullptr, 0,
        CXTranslationUnit_None);
    if (this->unit == nullptr) {
        std::string inv = "Unable to parse file: " + std::string(file_name);
        throw std::invalid_argument(inv.c_str());
    }
}

tokenizer_t::~tokenizer_t(void) {
    clang_disposeTranslationUnit(this->unit);
    clang_disposeIndex(this->index);
}

std::vector<int> tokenizer_t::get_tokens(void) {
    struct tokenizer_data_t data = {std::vector<int>(), this};
    clang_visitChildren(clang_getTranslationUnitCursor(this->unit), 
            [](CXCursor c, CXCursor parent, CXClientData client_data) {
                tokenizer_data_t* data = 
                    reinterpret_cast<tokenizer_data_t*>(client_data);
                if (data->tokenizer->is_from_main_file(c)) {
                    int token = static_cast<int>(clang_getCursorKind(c));
                    data->tokens.push_back(token);
                }
                return CXChildVisit_Recurse;
            }, reinterpret_cast<CXClientData>(&data));
    return data.tokens;
}

bool tokenizer_t::is_from_main_file(CXCursor __cursor) {
    CXFile cursor_file;
    unsigned line, column, offset;
    clang_getSpellingLocation(clang_getCursorLocation(__cursor),
            &cursor_file, &line, &column, &offset);
    CXFile main_file = clang_getFile(this->unit, this->file_name.c_str());
    return clang_File_isEqual(cursor_file, main_file);
}

std::string get_cursor_kind_spelling(int kind) {
    return clang_getCString(clang_getCursorKindSpelling(
        static_cast<CXCursorKind>(kind)));
} 

// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// TODO: Implement the methods of the plagiarism_checker_t class
// Add Original Submissions without checking plagiarism between them

// pattern matching -------------------------------------------------->
/// @brief To store matchings
struct Match{;
    int subID;
    int MatchingTo;
    int start;
    int end;

    Match(int subID, int MatchingTo, int start, int end): subID(subID), MatchingTo(MatchingTo), start(start), end(end){}
};
/// @brief Calculates the hash of given text
/// @param text 
/// @param len 
/// @return {hash, hash_param}
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

int main(){
    // take input tokens from fstreams/1_tokens.txt and fstreams/109_tokens.txt
    std::ifstream file1("./fstreams/1_tokens.txt");
    std::ifstream file2("./fstreams/109_tokens.txt");
    std::vector<int> tokens1, tokens2;
    int token;
    while(file1 >> token){
        tokens1.push_back(token);
    }
    while(file2 >> token){
        tokens2.push_back(token);
    }
    std::ofstream curr_file("./fstreams/1_109.txt");
    std::vector<Match> matches;
    std::vector<bool> matched(tokens2.size(), false); // To store which hashes are already matched (indices of submission2)
    int count =0;
    std::unordered_multimap<ll, int> hash_set; // Stores Hash with index
    for(int i = 0; i < tokens2.size(); i++){
        std::cerr << tokens2[i] << " ";
    }
    std::cerr<<std::endl;
    for(int i=0; i < tokens1.size(); i++){
        std::cerr << tokens1[i] << " ";
    }
    std::cerr<<std::endl;

    //------------------------------------>
    std::unordered_multimap<ll, int> hash_set1;
    calculate_hashes(hash_set1, tokens2, 15);
    for(auto x : hash_set1){
        std::cerr << x.first << " ";
    }
    std::cerr<<std::endl;
    calculate_hashes(hash_set, tokens1, 15);
    for(auto x : hash_set){
        std::cerr << x.first << " ";
    }

    std::cerr<<std::endl;
    std::pair<ll, ll> hashed = hashing(tokens1, 15);
    ll hash = hashed.first, x = hashed.second;
    int i=0;
    int cns_size = 0; // Size of continuous match
    while(i <= tokens1.size()-15){
        if(i!=0){
            hash  = (hash - (tokens1[i - 1] * x % PRIME) + PRIME) % PRIME;
            hash = (hash * 33 + tokens1[i + 14]) % PRIME;
        }
        auto it = hash_set.find(hash);
        if(it != hash_set.end()){ // Match Found
        std::cerr << "i: " << i << std::endl;
            
            if(matched[it->second]) {i++;continue;}
            Match m(109, 1,i, i+14);
            matches.push_back(m);
            for(int j = it->second; j < it->second+15; j++){
                matched[j] = true;
            }
            count++;
            if(count >= 10) { // Check for patchwork Plag
            std::cout << "more than 10 matches found\n";
                return 0;
            }
            i+=15; // Advance to next window
            cns_size+=15;
            curr_file<<cns_size<<" ";
            if (cns_size >= 75){ // Plag with submission2
                std::cout<< "75 or more matches found\n";
            }
        }
        else {
            i++; // Advance to next index if match not found at current
            cns_size = 0;
        }
    }


std::cout << "No matches found\n";
    return 0;

}