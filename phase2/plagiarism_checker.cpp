#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// TODO: Implement the methods of the plagiarism_checker_t class
// Add Original Submissions without checking plagiarism between them
plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions){
    std::chrono::time_point<std::chrono::system_clock> curr_time{}; // Initialize to Epoch Time
    for(auto submission : __submissions){
        this->submissions.push_back(std::make_pair(submission, curr_time));
    }
}

void plagiarism_checker_t::check_plagiarism(std::shared_ptr<submission_t> submission1, std::shared_ptr<submission_t> submission2, bool flag_both)
{

}


void plagiarism_checker_t::individual_plag(std::shared_ptr<submission_t>__submission){
    //Multiple threads can read and write a shared pointer https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2010/c9ceah3b(v=vs.100)?redirectedfrom=MSDN
    // for (auto other_submission: submissions){
    //     if (other_submission.first!=__submission && other_submission.second < curr_time - std::chrono::seconds(1)){
    //         // Reference: https://stackoverflow.com/questions/20340750/pass-multiple-arguments-into-stdthread
    //         std::thread t(&plagiarism_checker_t::check_plagiarism,__submission, other_submission.first,false);
    //         // Reference: https://stackoverflow.com/questions/30768216/c-stdvector-of-independent-stdthreads
    //         threads.push_back(std::move(t));
    //     }
    //     else if (other_submission.second < curr_time){
    //         std::thread t(&plagiarism_checker_t::check_plagiarism, other_submission.first, __submission, true);
    //         threads.push_back(std::move(t));
    //     }
    // }
}


void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission){
    // lock(): Locks access of the submissions vector until unlocked
    std::lock_guard<std::mutex> lock(mtx);
    auto curr_time = std::chrono::system_clock::now();
    this->submissions.push_back(std::make_pair(__submission, curr_time));
    std::thread t([this, __submission]() {
        this->individual_plag(__submission); // Ensure 'this' is captured for the method call
    });    
}

plagiarism_checker_t::~plagiarism_checker_t(void) {
    for (auto& thread : this->threads) {
            thread.join();
    }
}
// End TODO