#include "structures.hpp"
// -----------------------------------------------------------------------------
#include <chrono>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <memory>
#include <functional>
#include <condition_variable>
#include <queue>
#include <stack>
#include <iostream>
#include <fstream>
#include <future>

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and classes here

// Match structure to store the matches
struct Match
{
    long subID;
    long MatchingTo;
    int start;
    int end;
    int start2;
    int end2;
    Match(long subID, long MatchingTo, int start, int end, int start2, int end2) : subID(subID), MatchingTo(MatchingTo), start(start), end(end), start2(start2), end2(end2) {}
};

// thread pool class to handle the threads. It is initialized with 16 threads. Implementing thread pool
// saves from the overhead of creating and destroying threads. A queue is maintained to store the tasks
// and threads are assigned tasks from the queue. when thread is done with one task, it waits until queue notifies it.
// when the queue is empty and stop is true, the threads exits.
// following references are used to implement thread pool.
// REFERENCE: https://medium.com/@bhushanrane1992/getting-started-with-c-thread-pool-b6d1102da99a
// https://www.youtube.com/watch?v=u7ouCuieBhI
class ThreadPool
{
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop = false;

public:
    ThreadPool(size_t num_threads)
    {
        for (size_t i = 0; i < num_threads; i++)
        {
            threads.emplace_back([this]
                                {
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
#ifdef PRINT_LOGS
                    {
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    std::cerr<<"doing some task"<<std::endl;
                    }
#endif
                    task();
                } });
        }
    }

    template <class F>
    auto enqueue(F f) -> std::future<typename std::invoke_result<F>::type>
    {
        using return_type = typename std::invoke_result<F>::type;
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::move(f));
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.emplace([task]()
                        { (*task)(); });
        }
        condition.notify_one();
        return task->get_future();
    }

    void join_threads()
    {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread &worker : threads)
        {
            if (worker.joinable())
            {
                worker.join();
            }
        }
    }
};

class plagiarism_checker_t
{
    // You should NOT modify the public interface of this class.
public:
    plagiarism_checker_t(void);
    plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions);
    ~plagiarism_checker_t(void);
    void add_submission(std::shared_ptr<submission_t> __submission);

protected:
    // TODO: Add members and function signatures here
    std::unordered_map<int, std::vector<int>> tokenized_submissions;
    std::mutex mtx;
    std::vector<std::pair<std::shared_ptr<submission_t>, std::chrono::time_point<std::chrono::system_clock>>> submissions;
    std::unordered_map<int, bool> is_flagged;
    ThreadPool pool; // pool gets destructed first

    std::vector<Match> check_plagiarism(std::shared_ptr<submission_t> submission1, std::shared_ptr<submission_t> submission2, bool flag_both);
    void process_plagcheck_for_submission(std::shared_ptr<submission_t> submission, std::chrono::time_point<std::chrono::system_clock> curr_time);
    void flag_them(std::shared_ptr<submission_t> submission1, std::shared_ptr<submission_t> submission2, bool flag_both);
    // End TODO
};
