#include "plagiarism_checker.hpp"

#define PRIME 1000000007LL
#define ll long long int
// You should NOT add ANY other includes to this file.
// Do NOT add "using namespace std;".

// TODO: Implement the methods of the plagiarism_checker_t class
// Add Original Submissions without checking plagiarism between them


//------------------------------------------------------------------------------------------------>

plagiarism_checker_t::plagiarism_checker_t(void) : pool(3) {}

plagiarism_checker_t::plagiarism_checker_t(std::vector<std::shared_ptr<submission_t>> __submissions) : pool(3)
{
    std::chrono::time_point<std::chrono::system_clock> curr_time{}; // Initialize to Epoch Time
    for (auto submission : __submissions)
    {
        std::cerr << "Submission added: " << submission->id << " " << submission->student->get_name() << " time:" << curr_time<< std::endl;
        tokenizer_t tokenizer(submission->codefile);
        auto tokens = tokenizer.get_tokens();
        tokenized_submissions[submission->id] = tokens;
        is_flagged[submission->id] = false;
        this->submissions.push_back(std::make_pair(submission, curr_time));
    }
}

// pattern matching -------------------------------------------------->
// comparator for sorting the intervals
bool cmp(Match &a, Match &b)
{
    return a.end < b.end;
}

/// @brief  Function to check for patchwork plagiarism by checking if for a particular submission,
// there are more than 20 matches with different submissions.
/// @param intervals
/// @return true if patchwork plagiarism is detected, false otherwise.
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
            match_count++;
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
/// @brief calculates the hashes of all the substrings of length len using rolling hash technique.
/// @param hash_set
/// @param text
/// @param len
void calculate_hashes(std::unordered_map<ll, std::vector<int>> &hash_set, std::vector<int> &text, ll len)
{
    std::ofstream rise("fstreams/0_hashes.txt");

    std::pair<ll, int> first_hash = hashing(text, len);
    ll hashed = first_hash.first, x = first_hash.second;
    rise << hashed << " ";
    hash_set[hashed].push_back(0);
    for (int i = 1; i <= text.size() - len; i++)
    {
        hashed = (hashed - (text[i - 1] * x % PRIME) + PRIME) % PRIME;
        hashed = (hashed * 33 + text[i + len - 1]) % PRIME;
        hash_set[hashed].push_back(i);
        rise << hashed << " ";
    }
    rise << std::endl;
}

/// @brief Function to flag the submissions if plagiarism is detected.
/// @param submission1
/// @param submission2
/// @param flag_both flag_both = 1 if both submissions are to be flagged, 0 if only submission1 is to be flagged.
/// @param curr_file
void plagiarism_checker_t::flag_them(std::shared_ptr<submission_t> submission1, std::shared_ptr<submission_t> submission2, bool flag_both)
{
    std::lock_guard<std::mutex> lock(mtx);
    if (!is_flagged[submission2->id] && flag_both)
    {
        std::cerr << "Flagging both: " << submission2->id << std::endl;
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

/// @brief Function to handle and merge continuous matches when new match is found.
void handle_and_merge_matches(std::vector<Match> &final_matches, std::stack<Match> &matches_stack, int &continuous_size,
                              int &match_count, std::vector<bool> &matched, bool flag_both, std::shared_ptr<submission_t> submission1,
                              std::shared_ptr<submission_t> submission2, int &i, int &start_idx_file2)
{
    if (matches_stack.empty())
    {
        matches_stack.push({submission1->id, submission2->id, i, i+14, start_idx_file2, start_idx_file2+ 14});
        continuous_size += 15;
        match_count++;
    }
    else if (!matches_stack.empty())
    {
        Match m = matches_stack.top();
        int prev_start1 = m.start; int prev_start2 = m.start2;
        int prev_end1 = m.end; int prev_end2 = m.end2;
        if ((m.end + 1) == i && (m.end2 + 1) == start_idx_file2)
        {
            std::cerr << "match extended" << std::endl;
            continuous_size += 15;
            match_count++;
            matches_stack.pop();
            matches_stack.push({submission1->id, submission2->id, prev_start1,i+14, prev_start2, start_idx_file2+14});
        }
        else
        {
            continuous_size = 15;
            match_count++;
            final_matches.push_back({submission1->id, submission2->id, m.start, m.end, m.start2, m.end2});
            matches_stack.pop();
            matches_stack.push({submission1->id, submission2->id,i,i+14,start_idx_file2,start_idx_file2+14});
        }
    }
    for (int j = start_idx_file2 ; j <= start_idx_file2+14; j++){
        matched[j] = true;
    }

}

/// @brief Function to handle small matches of length less than 15 and merge them with the previous matches. if the current
//  continuous match > 0 and next window of 15 does not matches then, this function check if less than 15 length match can
//  be added to the previous continuous match.
int handle_small_match(std::vector<Match> &final_matches, std::stack<Match> &matches_stack, int &continuous_size,
                        int &match_count, std::vector<bool> &matched, std::shared_ptr<submission_t> submission1,
                        std::shared_ptr<submission_t> submission2, int &i, std::vector<int> &tokens1, std::vector<int> &tokens2)
{
    if (continuous_size > 0)
    {
        std::cerr<<"continuous size: "<<continuous_size<<std::endl;
        int match_last_len = 0;
        Match m = matches_stack.top();
        std::cerr<<"m.end2: "<<m.end2<<std::endl;
        int prev_start1 = m.start; int prev_start2 = m.start2;
        int prev_end1 = m.end; int prev_end2 = m.end2;
        matches_stack.pop();
        for (int j = i; j < i + 14; j++)
        {
            if ( m.end2 +j-i+1 < tokens2.size() && tokens1[j] == tokens2[m.end2 + j - i +1])
            {
                match_last_len++;
                matched[m.end2 + j - i+1] = true;
            }
            else
                break;
        }
        std::cerr<<"match_last_len: "<<match_last_len<<std::endl;
        final_matches.push_back({submission1->id, submission2->id, prev_start1, i+match_last_len-1, prev_start2, m.end2 + match_last_len});
        if(match_last_len==0) i++;
        else i += match_last_len;
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
std::vector<Match> plagiarism_checker_t::check_plagiarism(std::shared_ptr<submission_t> submission1,
                                                          std::shared_ptr<submission_t> submission2, bool flag_both)
{
    std::stack<Match> matches_stack;
    std::vector<Match> final_matches;
    std::vector<bool> matched(tokenized_submissions[submission2->id].size(), false); // To store which hashes are already matched (indices of submission2)
    int match_count = 0;
    auto tokens1 = tokenized_submissions[submission1->id];
    auto tokens2 = tokenized_submissions[submission2->id];
    std::unordered_map<ll, std::vector<int>> hash_set; // Stores Hash with index
    calculate_hashes(hash_set, tokens2, 15);   // Calculate hashes of all substrings of length 15.
    
    std::ofstream fall("fstreams/100000_hashes.txt");
    
    std::pair<ll, ll> hashed = hashing(tokens1, 15);
    ll hash = hashed.first, x = hashed.second;
    fall << hash << " ";
    int i = 0;               // Start of window
    int continuous_size = 0; // Size of continuous match

    while (i <= tokens1.size() - 15)
    {
        // std::cerr << "i: " << i << std::endl;
        if (i != 0 && continuous_size == 0)
        {
            hash = (hash - (tokens1[i - 1] * x % PRIME) + PRIME) % PRIME;
            hash = (hash * 33 + tokens1[i + 14]) % PRIME;
            fall << hash << " ";
        }
        else if (i != 0)
        {
            for (int k = i - 14; k <= i; k++)
            {
                hash = (hash - (tokens1[k - 1] * x % PRIME) + PRIME) % PRIME;
                hash = (hash * 33 + tokens1[k + 14]) % PRIME;
                fall << hash << " ";
            }
        }

        auto it = hash_set.find(hash);
        int start_idx_file2 = -1;
        if(it != hash_set.end()){
            std::vector<int> idxs = it->second;
            for(auto idx: idxs){
                std::cerr << "idx: " << idx << std::endl;
                if(matched[idx] || matched[idx+14] ) continue;
                else { 
                    start_idx_file2 = idx;
                    break;
                }
            }
        }
        
        if (it != hash_set.end() && start_idx_file2 != -1)
        { // Match Found
           
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cerr << "Match Found: " << submission1->id << " " << submission2->id << " " << i << " " << start_idx_file2 << std::endl;
        }
            handle_and_merge_matches(final_matches, matches_stack, continuous_size, match_count,
                                     matched, flag_both, submission1, submission2,i, start_idx_file2);
            if (match_count >= 10){
                flag_them(submission1, submission2, flag_both);
            }
            if (continuous_size >= 75){
                flag_them(submission1, submission2, flag_both);
            }
            i += 15; // Advance to next window
        }
        else
        {
            int match_len = handle_small_match(final_matches, matches_stack, continuous_size, match_count, matched, submission1, submission2, i,tokens1,tokens2);
            for (int k = i - match_len + 1; k < i; k++)
            {
                hash = (hash - (tokens1[k - 1] * x % PRIME) + PRIME) % PRIME;
                hash = (hash * 33 + tokens1[k + 14]) % PRIME;
            }
            // i++;
            // continuous_size = 0;
        }
    }
    while(!matches_stack.empty()){
        Match m = matches_stack.top();
        final_matches.push_back(m);
        matches_stack.pop();
    }
    return final_matches;
}

// Multiple threads can read and write a shared pointer https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2010/c9ceah3b(v=vs.100)?redirectedfrom=MSDN
/// @brief Function to process plagiarism check for a particular submission.
void plagiarism_checker_t::process_plagcheck_for_submission(std::shared_ptr<submission_t> __submission, std::chrono::time_point<std::chrono::system_clock> curr_time)
{
    std::vector<Match> MasterMatch;  // store all the matches with all other submissions.
    std::vector<std::future<std::vector<Match>>> futures;
    for (auto other_submission : submissions)
    {
        if(other_submission.first->id == __submission->id)
            continue;

        std::ofstream out("fstreams/"+std::to_string(__submission->id)+"_"+std::to_string(other_submission.first->id)+".txt");
        
        for (auto token : tokenized_submissions[__submission->id])
        {
            out << token << " ";
        }
        out << std::endl;
        for (auto token : tokenized_submissions[other_submission.first->id])
        {
            out << token << " ";
        }
        out << std::endl;

        if ( (curr_time - other_submission.second) > std::chrono::seconds(1))
        {
            std::lock_guard<std::mutex> lock(mtx);
            futures.push_back(pool.enqueue([this, __submission, other_submission]()
                                            { return this->check_plagiarism(__submission, other_submission.first, false); }));
        }
        else if ( other_submission.second < curr_time)
        {
            std::lock_guard<std::mutex> lock(mtx);
            futures.push_back(pool.enqueue([this, __submission, other_submission]()
                                            { return this->check_plagiarism(__submission,other_submission.first, true); }));
        } // Enqueue the task to the thread pool
    }
    for (auto &f : futures)
    {
        auto tmp = f.get(); // Wait for all the futures to finish.
        MasterMatch.insert(MasterMatch.end(), tmp.begin(), tmp.end());
    }
    std::cerr << "id: " << __submission->id << "MasterMatch size: " << MasterMatch.size() << std::endl;
    {
        std::lock_guard<std::mutex> lock(mtx);
        for(auto match: MasterMatch){
            std::cerr << "id: " << __submission->id << "Match: " << match.end-match.start+1 << " start1: " <<match.start<<" start2: " <<match.start2<< std::endl;
        }
    }
    if (patchWork(MasterMatch))
    {
        if (!is_flagged[__submission->id])
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
    std::cerr << "Submission added: " << __submission->id << " " << __submission->student->get_name() << " time:" <<curr_time<< std::endl;
    tokenizer_t tokenizer(__submission->codefile);
    auto tokens = tokenizer.get_tokens();
    tokenized_submissions[__submission->id] = tokens;
    is_flagged[__submission->id] = false;
    this->submissions.push_back(std::make_pair(__submission, curr_time));
    pool.enqueue([this, __submission, curr_time]()
                 {
                    this->process_plagcheck_for_submission(__submission, curr_time);
                 });
}

plagiarism_checker_t::~plagiarism_checker_t(void)
{
}
// End TODO