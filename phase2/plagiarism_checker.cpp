#include "plagiarism_checker.hpp"
#include <iostream>
#include <fstream>
#define PRIME 1000000007LL
#define ll long long int 
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


//------------------------------------------------------------------------->
plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions): pool(16) {
    std::chrono::time_point<std::chrono::system_clock> curr_time{}; // Initialize to Epoch Time
    for(auto submission : __submissions){
        std::cerr << "Submission added: " << submission->id <<" "<< submission->student->get_name()<< " time:"<<curr_time<< std::endl;
        tokenizer_t tokenizer(submission->codefile);
        auto tokens = tokenizer.get_tokens();
        tokenized_submissions[submission->id] = tokens;
        this->submissions.push_back(std::make_pair(submission, curr_time));
    }
}

plagiarism_checker_t::plagiarism_checker_t(void): pool(16){}

void plagiarism_checker_t::check_plagiarism(std::shared_ptr<submission_t> submission1, std::shared_ptr<submission_t> submission2, bool flag_both)
{
    // {
    //     std::lock_guard<std::mutex> lock(mtx);
    // std::cerr << "Checking plagiarism between " << submission1->id << " and " << submission2->id << std::endl;
        
    // }   
    std::ofstream curr_file("./fstreams/"+submission1->student->get_name()+"_"+std::to_string(submission1->id)+"_"+submission2->student->get_name()+"_"+std::to_string(submission2->id)+".txt");
    std::vector<Match> matches;
    std::vector<bool> matched(tokenized_submissions[submission2->id].size(), false); // To store which hashes are already matched (indices of submission2)
    int count =0;
    auto tokens1 = tokenized_submissions[submission1->id];
    auto tokens2 = tokenized_submissions[submission2->id];
    std::unordered_multimap<ll, int> hash_set; // Stores Hash with index
    calculate_hashes(hash_set, tokens2, 15);
    std::pair<ll, int> hashed = hashing(tokens1, 15);
    ll hash = hashed.first, x = hashed.second;
    int i=0;
    int cns_size = 0; // Size of continuous match
    curr_file<<"CNS SIZE: ";
    while(i <= tokens1.size()-15){
        if(i!=0){
            hash  = (hash - (tokens1[i - 1] * x % PRIME) + PRIME) % PRIME;
            hash = (hash * 33 + tokens1[i + 14]) % PRIME;
        }
        auto it = hash_set.find(hash);
        if(it != hash_set.end()){ // Match Found
            if(matched[it->second]) continue;
            Match m(submission1->id, submission2->id,i, i+14);
            matches.push_back(m);
            for(int j = it->second; j < it->second+15; j++){
                matched[j] = true;
            }
            count++;
            if(count >= 10) { // Check for patchwork Plag
                std::lock_guard<std::mutex> lock(mtx);
                submission1->student->flag_student(submission1);
                submission1->professor->flag_professor(submission1);
                return;
            }
            i+=15; // Advance to next window
            cns_size+=15;
            curr_file<<cns_size<<" ";
            if (cns_size >= 75){ // Plag with submission2
                std::lock_guard<std::mutex> lock(mtx);
                if(flag_both){
                    submission1->student->flag_student(submission1);
                    submission2->student->flag_student(submission2);
                    submission1->professor->flag_professor(submission1);
                    submission2->professor->flag_professor(submission2);
                }
                else{
                    submission1->student->flag_student(submission1);
                    submission1->professor->flag_professor(submission1);
                }
                return;
            }
        }
        else {
            i++; // Advance to next index if match not found at current
            cns_size = 0;
        }
    }
    curr_file<<submission2->student->get_name()+"_"+std::to_string(submission2->id)+"\n";
    curr_file.close();
}


// Multiple threads can read and write a shared pointer https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2010/c9ceah3b(v=vs.100)?redirectedfrom=MSDN
void plagiarism_checker_t::individual_plag(std::shared_ptr<submission_t> __submission, std::chrono::time_point<std::chrono::system_clock> curr_time){
    for (auto other_submission : submissions){
        if (other_submission.first != __submission && other_submission.second < curr_time - std::chrono::seconds(1)){
            pool.enqueue([this, __submission, other_submission]() {
                this->check_plagiarism(__submission, other_submission.first, false);
            });
            // Enqueue the task to the thread pool
        }
        else if (other_submission.second < curr_time){
            pool.enqueue([this, __submission, other_submission]() {
                this->check_plagiarism(other_submission.first, __submission, true);
            });
            // Enqueue the task to the thread pool
        }
    }
}


void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission){
    // lock(): Locks access of the submissions vector until unlocked
    auto curr_time = std::chrono::system_clock::now();
    std::cerr << "Submission added: " << __submission->id <<" "<< __submission->student->get_name()<< " time:"<<curr_time<< std::endl;
    tokenizer_t tokenizer(__submission->codefile);
    auto tokens = tokenizer.get_tokens();
    if (__submission->id == 109) 
    tokenized_submissions[__submission->id] = tokens;
    this->submissions.push_back(std::make_pair(__submission, curr_time));
    pool.enqueue([this, __submission,curr_time]() {
        this->individual_plag(__submission,curr_time); // Ensure 'this' is captured for the method call
    });    
}

plagiarism_checker_t::~plagiarism_checker_t(void) {
}
// End TODO