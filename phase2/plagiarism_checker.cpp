#include "plagiarism_checker.hpp"
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// TODO: Implement the methods of the plagiarism_checker_t class
plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions){
    auto curr_time = std::chrono::system_clock::now();
    for(auto submission : __submissions){
        this->submissions.push_back(std::make_pair(submission, curr_time));
    }
    check_plagiarism_init();
}

void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission){
    this->submissions.push_back(__submission);
    
}

void plagiarism_checker_t::check_plagiarism_init(){
    for (auto submission1: this->submissions){
        for (auto submission2: this->submissions){
            if (submission1.first != submission2.first){
                std::thread{check_plagiarism, submission1.first, submission2.first}.detach();
            }
        }
    }
}

void check_plagiarism(std::shared_ptr<submission_t> submission1, std::shared_ptr<submission_t> submission2){
    
}

// End TODO