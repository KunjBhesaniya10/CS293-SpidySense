#include "plagiarism_checker.hpp"
#include <iostream>
#define PRIME 1000000007LL
#define ll long long int 
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// TODO: Implement the methods of the plagiarism_checker_t class
// Add Original Submissions without checking plagiarism between them

// pattern matching -------------------------------------------------->
struct Match{;
    int subID;
    int MatchingTo;
    int start;
    int end;
    Match(int subID, int MatchingTo, int start, int end): subID(subID), MatchingTo(MatchingTo), start(start), end(end){}
};

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
plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions): pool(4) {
    std::chrono::time_point<std::chrono::system_clock> curr_time{}; // Initialize to Epoch Time
    for(auto submission : __submissions){
        tokenizer_t tokenizer(submission->codefile);
        auto tokens = tokenizer.get_tokens();
        tokenized_submissions[submission->id] = tokens;
        this->submissions.push_back(std::make_pair(submission, curr_time));
    }
}

void plagiarism_checker_t::check_plagiarism(std::shared_ptr<submission_t> submission1, std::shared_ptr<submission_t> submission2, bool flag_both)
{
    // {
    //     std::lock_guard<std::mutex> lock(mtx);
    // std::cerr << "Checking plagiarism between " << submission1->id << " and " << submission2->id << std::endl;
        
    // }   
    std::vector<Match> matches;
    std::vector<bool> matched(tokenized_submissions[submission2->id].size(), false);
    int count =0;
    auto tokens1 = tokenized_submissions[submission1->id];
    auto tokens2 = tokenized_submissions[submission2->id];
    std::unordered_multimap<ll, int> hash_set;
    calculate_hashes(hash_set, tokens2, 15);
    std::pair<ll, int> hashed = hashing(tokens2, 15);
    ll hash = hashed.first, x = hashed.second;
    int i=0;
    while(i <= tokens1.size()-15){
        if(i!=0){
        hash  = (hash - (tokens1[i - 1] * x % PRIME) + PRIME) % PRIME;
        hash = (hash * 33 + tokens1[i + 14]) % PRIME;
        }
        auto it = hash_set.find(hash);
        if(it != hash_set.end()){
            if(matched[it->second]) continue;
            Match m(submission1->id, submission2->id,i, i+14);
            matches.push_back(m);
            for(int j = it->second; j < it->second+15; j++){
                matched[j] = true;
            }
            count++;
            if(count >= 10) {
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
            i+=15;
        }
        else i++;
    }
    
}


    // Multiple threads can read and write a shared pointer https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2010/c9ceah3b(v=vs.100)?redirectedfrom=MSDN
    void plagiarism_checker_t::individual_plag(std::shared_ptr<submission_t> __submission,
     std::chrono::time_point<std::chrono::system_clock> curr_time){

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
    std::cerr << "Submission added: " << __submission->id << std::endl;
    auto curr_time = std::chrono::system_clock::now();
    tokenizer_t tokenizer(__submission->codefile);
    auto tokens = tokenizer.get_tokens();
    tokenized_submissions[__submission->id] = tokens;
    this->submissions.push_back(std::make_pair(__submission, curr_time));
    pool.enqueue([this, __submission,curr_time]() {
        this->individual_plag(__submission,curr_time); // Ensure 'this' is captured for the method call
    });    
}

plagiarism_checker_t::~plagiarism_checker_t(void) {
}
// End TODO