#include "plagiarism_checker.hpp"

#define PRIME 1000000007LL
// Use PRINT_LOGS and OUTPUT_TOKENS for debugging and testing purposes
// Add -DPRINT_LOGS -DOUTPUT_TOKENS to Makefile

// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// TODO: Implement the methods of the plagiarism_checker_t class
// Add Original Submissions without checking plagiarism between them

//------------------------------------------------------------------------------------------------>
// Using 3 threads per instance: each thread runs an instance of 'process_plagcheck_for_submission' function
plagiarism_checker_t::plagiarism_checker_t(void) : pool(3) {}

plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions) : pool(3)
{
    std::chrono::time_point<std::chrono::system_clock> curr_time{}; // Initialize to Epoch Time
    for (auto submission : __submissions)
    {
#ifdef PRINT_LOGS
        std::cerr << "Submission added: " << submission->id << " " << submission->student->get_name() << " time:" << curr_time << std::endl;
#endif

        tokenizer_t tokenizer(submission->codefile);
        auto tokens = tokenizer.get_tokens();
        tokenized_submissions[submission->id] = tokens;
        is_flagged[submission->id] = false;
        this->submissions.push_back(std::make_pair(submission, curr_time));
        // Original submissions's tokens generated and added to global vector with timestamp
    }
}

// pattern matching ------------------------------------------------------------------------------->
// comparator for sorting the intervals
bool cmp(Match &a, Match &b)
{
    return a.end < b.end;
}

// Function to check for patchwork plagiarism by checking if for a particular submission,
// there are more than 20 matches with different submissions.
bool patchWork(std::vector<Match> &intervals)
{
    if (intervals.size() == 0)
        return 0;
    sort(intervals.begin(), intervals.end(), cmp);
    int lastEnd = intervals[0].end;
    int matchID = intervals[0].MatchingTo;
    int match_count = 1;
    int file_count = 1;
    for (int i = 1; i < intervals.size(); i++)
    {
        if (intervals[i].start > lastEnd)
        {
            match_count+= (intervals[i].end - intervals[i].start + 1)/15;
            if (intervals[i].MatchingTo != matchID)
            {
                file_count++;
            }
            lastEnd = intervals[i].end;
        }
    }
    if (match_count >= 20 && file_count != 1)
        return 1;
    else
        return 0;
}

// Calculates the hash of given text and return {hash, hash_param:x}
std::pair<long long, int> hashing(std::vector<int> &text, int len)
{
    long long x = 1;
    long long hashed = 0;
    for (long long i = 0; i < len; i++, x = ((x % PRIME) * 33))
    {
        hashed = (hashed % PRIME + (((x % PRIME) * text[len - i - 1]) % PRIME)) % PRIME;
    }
    x /= 33;
    hashed = hashed % PRIME;
    return {hashed, x};
}

// Calculates the hashes of all the substrings of length len using rolling hash technique.
void calculate_hashes(std::unordered_map<long long, std::vector<int>> &hash_set, std::vector<int> &text, long long len)
{
    std::pair<long long, int> first_hash = hashing(text, len);
    long long hashed = first_hash.first, x = first_hash.second;
    hash_set[hashed].push_back(0);

#ifdef OUTPUT_TOKENS
    std::ofstream rise("fstreams/0_hashes.txt");
    rise << hashed << " ";
#endif

    for (int i = 1; i <= text.size() - len; i++)
    {
        hashed = (hashed - (text[i - 1] * x % PRIME) + PRIME) % PRIME;
        hashed = (hashed * 33 + text[i + len - 1]) % PRIME;
        hash_set[hashed].push_back(i);

#ifdef OUTPUT_TOKENS
        rise << hashed << " ";
#endif
    }

#ifdef OUTPUT_TOKENS
    rise << std::endl;
#endif
}

// Function to flag the submissions because plagiarism is detected.
// flag_both = 1 if both submissions are to be flagged, 0 if only submission1 is to be flagged.
void plagiarism_checker_t::flag_them(std::shared_ptr<submission_t> submission1, std::shared_ptr<submission_t> submission2, bool flag_both)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (!is_flagged[submission2->id] && flag_both)
    {
        if (submission2->student)
            submission2->student->flag_student(submission2);
        if (submission2->professor)
            submission2->professor->flag_professor(submission2);
        is_flagged[submission2->id] = true;
    }
    if (!is_flagged[submission1->id])
    {
        if (submission1->student)
            submission1->student->flag_student(submission1);
        is_flagged[submission1->id] = true;
        if (submission1->professor)
            submission1->professor->flag_professor(submission1);
    }
    return;
}

// Function to handle and merge continuous matches when new match is found.
void handle_and_merge_matches(std::vector<Match> &final_matches, std::stack<Match> &matches_stack, int &continuous_size, int &match_count, std::vector<bool> &matched, bool flag_both, std::shared_ptr<submission_t> submission1, std::shared_ptr<submission_t> submission2, int &i, int &start_idx_file2)
{
    if (matches_stack.empty()) // If empty, add current match without merging
    {
        matches_stack.push({submission1->id, submission2->id, i, i + 14, start_idx_file2, start_idx_file2 + 14});
        continuous_size += 15;
        match_count++;
    }
    else if (!matches_stack.empty()) // Possibility to extend a match
    {
        Match m = matches_stack.top();
        int prev_start1 = m.start;
        int prev_start2 = m.start2;
        int prev_end1 = m.end;
        int prev_end2 = m.end2;
        if ((m.end + 1) == i && (m.end2 + 1) == start_idx_file2) // If continuous match found, extend it
        {
            continuous_size += 15;
            match_count++;
            matches_stack.pop();
            matches_stack.push({submission1->id, submission2->id, prev_start1, i + 14, prev_start2, start_idx_file2 + 14});
        }
        else // If continuous match not found, add the current match and update the stack
        {
            continuous_size = 15;
            match_count++;
            final_matches.push_back({submission1->id, submission2->id, m.start, m.end, m.start2, m.end2});
            matches_stack.pop();
            matches_stack.push({submission1->id, submission2->id, i, i + 14, start_idx_file2, start_idx_file2 + 14});
        }
    }
    for (int j = start_idx_file2; j <= start_idx_file2 + 14; j++)
    {
        // Mark the matched hashes for current window size
        matched[j] = true;
    }
}

// Function to handle small matches of length less than 15 and merge them with the previous matches. if the current
//  continuous match > 0 and next window of 15 does not matches then, this function check if less than 15 length match can
//  be added to the previous continuous match.
int handle_small_match(std::vector<Match> &final_matches, std::stack<Match> &matches_stack, int &continuous_size, int &match_count, std::vector<bool> &matched, std::shared_ptr<submission_t> submission1, std::shared_ptr<submission_t> submission2, int &i, std::vector<int> &tokens1, std::vector<int> &tokens2)
{
    if (continuous_size > 0) // Check for small matches to merge with continuous match
    {
#ifdef PRINT_LOGS
        std::cerr << "continuous size: " << continuous_size << std::endl;
#endif

        int match_last_len = 0;
        Match m = matches_stack.top();
        int prev_start1 = m.start;
        int prev_start2 = m.start2;
        int prev_end1 = m.end;
        int prev_end2 = m.end2;
        matches_stack.pop();
        for (int j = i; j < i + 14; j++)
        {
            if (m.end2 + j - i + 1 < tokens2.size() && tokens1[j] == tokens2[m.end2 + j - i + 1]) // Match found
            {
                match_last_len++;
                matched[m.end2 + j - i + 1] = true;
            }
            else
                break;
        }
#ifdef PRINT_LOGS
        std::cerr << "match_last_len: " << match_last_len << std::endl;
#endif

        final_matches.push_back({submission1->id, submission2->id, prev_start1, i + match_last_len - 1, prev_start2, m.end2 + match_last_len});

        if (match_last_len == 0)
            i++;
        else
            i += match_last_len;

        continuous_size = 0;
        return match_last_len;
    }
    else
    {
        i++;
        return 0;
    }
}

/// @brief Function to check plagiarism between two submissions.
/// @param submission1 this is the main submission for which plagiarism is to be checked.
/// @param submission2 this is the submission to be checked against.
/// @param flag_both  flag_both = 1 if both submissions are to be flagged, 0 if only submission1 is to be flagged.
/// @return final_matches - vector of total matches found between the two submissions is returned for further patchwork check.
std::vector<Match> plagiarism_checker_t::check_plagiarism(std::shared_ptr<submission_t> submission1, std::shared_ptr<submission_t> submission2, bool flag_both)
{
    std::stack<Match> matches_stack;                                                 // Enables merging of matches
    std::vector<Match> final_matches;                                                // Merged Matches
    std::vector<bool> matched(tokenized_submissions[submission2->id].size(), false); // To store which hashes are already matched (indices of submission2)
    int match_count = 0;
    auto tokens1 = tokenized_submissions[submission1->id];
    auto tokens2 = tokenized_submissions[submission2->id];
    std::unordered_map<long long, std::vector<int>> hash_set; // Stores Hash with index
    calculate_hashes(hash_set, tokens2, 15);                  // Calculate hashes of all substrings of length 15.

    std::pair<long long, long long> hashed = hashing(tokens1, 15);
    long long hash = hashed.first, x = hashed.second;
    int i = 0;               // Start of window
    int continuous_size = 0; // Size of continuous match

    while (i <= tokens1.size() - 15)
    {
        if (i != 0 && continuous_size == 0)
        {
            hash = (hash - (tokens1[i - 1] * x % PRIME) + PRIME) % PRIME;
            hash = (hash * 33 + tokens1[i + 14]) % PRIME;
        }
        else if (i != 0) // If continuous match found, recalculate hash of last 15 indices
        {
            for (int k = i - 14; k <= i; k++)
            {
                hash = (hash - (tokens1[k - 1] * x % PRIME) + PRIME) % PRIME;
                hash = (hash * 33 + tokens1[k + 14]) % PRIME;
            }
        }
        auto it = hash_set.find(hash);
        int start_idx_file2 = -1;
        if (it != hash_set.end())
        { // Match Found, take the first unmatched index
            std::vector<int> idxs = it->second;
            for (auto idx : idxs)
            {
                if (matched[idx] || matched[idx + 14])
                    continue;
                else
                {
                    start_idx_file2 = idx;
                    break;
                }
            }
        }

        if (it != hash_set.end() && start_idx_file2 != -1)
        { // Use unmatched index
#ifdef PRINT_LOGS
            {
                std::lock_guard<std::mutex> lock(mtx);
                std::cerr << "Match Found: " << submission1->id << " " << submission2->id << " " << i << " " << start_idx_file2 << std::endl;
            }
#endif
            handle_and_merge_matches(final_matches, matches_stack, continuous_size, match_count, matched, flag_both, submission1, submission2, i, start_idx_file2);
            if (match_count >= 10)
            {
                flag_them(submission1, submission2, flag_both);
                break;
            }
            if (continuous_size >= 75)
            {
                flag_them(submission1, submission2, flag_both);
                break;
            }
            i += 15; // Advance to next window
        }
        else
        { // 15 length match not found, look for smaller or increment is no continuous match
            int match_len = handle_small_match(final_matches, matches_stack, continuous_size, match_count, matched, submission1, submission2, i, tokens1, tokens2);
            for (int k = i - match_len + 1; k < i; k++)
            {
                hash = (hash - (tokens1[k - 1] * x % PRIME) + PRIME) % PRIME;
                hash = (hash * 33 + tokens1[k + 14]) % PRIME;
            }
        }
    }
    while (!matches_stack.empty())
    { // Add remaining matches to final matches
        Match m = matches_stack.top();
        final_matches.push_back(m);
        matches_stack.pop();
    }
    return final_matches;
}

// Multiple threads can read and write a shared pointer https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2010/c9ceah3b(v=vs.100)?redirectedfrom=MSDN

// Function to process plagiarism check for a particular submission.
void plagiarism_checker_t::process_plagcheck_for_submission(std::shared_ptr<submission_t> __submission, std::chrono::time_point<std::chrono::system_clock> curr_time)
{
    auto instance_time = std::chrono::system_clock::now();
    auto time_elapsed = std::chrono::duration_cast<std::chrono::seconds>(instance_time - curr_time);
    if (time_elapsed < std::chrono::seconds(1))
    {
        usleep(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::seconds(1) - time_elapsed).count());
    }
    std::vector<std::pair<std::shared_ptr<submission_t>, std::chrono::time_point<std::chrono::system_clock>>> copy_submissions;
    { // Compute tokens for the submission, using lock_guard to lock the mutex because we're using tokenized_submissions
        std::lock_guard<std::mutex> lock(mtx);
        if (tokenized_submissions[__submission->id].size() == 0) {
            tokenizer_t tokenizer(__submission->codefile);
            auto tokens = tokenizer.get_tokens();
            tokenized_submissions[__submission->id] = tokens;
            copy_submissions = std::vector(submissions.begin(), submissions.end());
        }
    }
    std::vector<Match> MasterMatch; // store all the matches with all other submissions.

    for (auto other_submission : copy_submissions)
    {
        if (other_submission.first->id == __submission->id || ((curr_time - other_submission.second) > std::chrono::seconds(1) && is_flagged[__submission->id]))
            continue;

        // Enqueue the task to the thread pool
        if ((curr_time - other_submission.second) > std::chrono::seconds(1))
        { // flag only the current submission
            auto m = (check_plagiarism(__submission, other_submission.first, false));
            MasterMatch.insert(MasterMatch.end(), m.begin(), m.end());
        }
        else if (other_submission.second - curr_time < std::chrono::seconds(1) && other_submission.second - curr_time > -std::chrono::seconds(1))
        { // flag both
            if (tokenized_submissions[other_submission.first->id].size() == 0)
            {
                std::lock_guard<std::mutex> lock(mtx);
                {
                    tokenizer_t tokenizer(other_submission.first->codefile);
                    auto tokens = tokenizer.get_tokens();
                    tokenized_submissions[other_submission.first->id] = tokens;
                }
            }
            auto m = (check_plagiarism(__submission, other_submission.first, true));
            MasterMatch.insert(MasterMatch.end(), m.begin(), m.end());
        }
    }

#ifdef PRINT_LOGS
    std::cerr << "id: " << __submission->id << "MasterMatch size: " << MasterMatch.size() << std::endl;
    {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto match : MasterMatch)
        {
            std::cerr << "id: " << __submission->id << "Match: " << match.end - match.start + 1 << " start1: " << match.start << " start2: " << match.start2 << std::endl;
        }
    }
#endif
    if (!is_flagged[__submission->id])
    {
        if (patchWork(MasterMatch))
        // Check for PatchWork Plagiarism based on master matches
        {
            std::lock_guard<std::mutex> lock(mtx);
            __submission->professor->flag_professor(__submission);
            __submission->student->flag_student(__submission);
            is_flagged[__submission->id] = true;
        }
    }

    return;
}

void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission)
{
    auto curr_time = std::chrono::system_clock::now();
#ifdef PRINT_LOGS
    std::cerr << "Submission added: " << __submission->id << " " << __submission->student->get_name() << " time:" << curr_time << std::endl;
#endif

    {
        std::lock_guard<std::mutex> lock(mtx);
        this->submissions.push_back(std::make_pair(__submission, curr_time));
        is_flagged[__submission->id] = false;
    }
    pool.enqueue([this, __submission, curr_time]()
                 { this->process_plagcheck_for_submission(__submission, curr_time); });
}

plagiarism_checker_t::~plagiarism_checker_t(void)
{
#ifdef PRINT_LOGS
    std::cerr << "Destructor called" << std::endl;
#endif
    pool.join_threads();
}
// End TODO