#include "structures.hpp"
// -----------------------------------------------------------------------------
#include <chrono>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <memory>
#include <functional>
#include <condition_variable>
#include<queue>
#include <iostream>
#include <fstream>
#include<future>

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and classes here
struct Match{;
    int subID;
    int MatchingTo;
    int start;
    int end;

    Match(int subID, int MatchingTo, int start, int end): subID(subID), MatchingTo(MatchingTo), start(start), end(end){}
};
class ThreadPool {
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop = false;

public:
    ThreadPool(size_t num_threads) {
        for (size_t i = 0; i < num_threads; i++) {
            threads.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queue_mutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) {
                            return;
                        }
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    template<class F>
    std::future<void> enqueue(F f) {
        auto task = std::make_shared<std::packaged_task<void()>>(f);

        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace([task] { (*task)(); });        
        }
            condition.notify_one();
        return task->get_future();
    }
    std::future<std::vector<Match>> enqueue1(std::function<std::vector<Match>()> f) {
        auto task = std::make_shared<std::packaged_task<std::vector<Match>()>>(f);

        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace([task] { (*task)(); });        
        }
            condition.notify_one();
        return task->get_future();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        {
        std::unique_lock<std::mutex> lock(queue_mutex);
        std::cerr << "thread size : "<< threads.size() << std::endl;
        }
        condition.notify_all();
        for (std::thread &worker : threads) {
        std::cerr<<"Destructor called\n";
            if(worker.joinable())
            worker.join();
        }
    }
};

class plagiarism_checker_t {
    // You should NOT modify the public interface of this class.
public:
    plagiarism_checker_t(void);
    plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions);
    ~plagiarism_checker_t(void);
    void add_submission(std::shared_ptr<submission_t> __submission);

protected:
    // TODO: Add members and function signatures here
    std::vector<std::thread> threads;
    std::mutex mtx;
    std::vector<std::pair<std::shared_ptr<submission_t>,std::chrono::time_point<std::chrono::system_clock>>> submissions;
    std::unordered_map<int,std::vector<int>> tokenized_submissions;
    std::unordered_map<int,bool> is_flagged;
    ThreadPool pool;

    std::vector<Match> check_plagiarism(std::shared_ptr<submission_t> submission1, std::shared_ptr<submission_t> submission2, bool flag_both);
    void individual_plag(std::shared_ptr<submission_t>submission,std::chrono::time_point<std::chrono::system_clock> curr_time);
    void flag_them(std::shared_ptr<submission_t> submission1, std::shared_ptr<submission_t> submission2, bool flag_both, std::ofstream &curr_file);
    // End TODO
};
