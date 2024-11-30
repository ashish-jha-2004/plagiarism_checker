#include "plagiarism_checker.hpp"
#include "../tokenizer.hpp"
#include <chrono>
#include <cmath>
#include <algorithm>

// constants for thresholds
const int EXACT_MATCH_THRESHOLD = 75;
const int PATTERN_MATCH_LENGTH = 15;
const int MIN_PATTERN_MATCHES = 10;
const int TIME_DIFF_THRESHOLD = 1;
const int MINIMUM_PATCHWORK_MATCHES = 20;

// Constructor
plagiarism_checker_t::plagiarism_checker_t(const std::vector<std::shared_ptr<submission_t>>& pre_existing_submissions)
    : submissions(pre_existing_submissions), stop_processing(false) {

    for (const auto& submission : submissions) {
        timestamps[submission->id] = 0; // Assuming current time for all pre-existing submissions
        flagged_submissions[submission->id] = false;
    }
    time_offset = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
    // Start the processing thread
    processing_thread = std::thread(&plagiarism_checker_t::process_submissions, this);
}

// Constructor
plagiarism_checker_t::plagiarism_checker_t()
    : stop_processing(false) {
    // Start the processing thread
    processing_thread = std::thread(&plagiarism_checker_t::process_submissions, this);
}

// Destructor
plagiarism_checker_t::~plagiarism_checker_t() {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        stop_processing = true;
    }
    queue_cv.notify_all();
    if (processing_thread.joinable()) {
        processing_thread.join();
    }
}

// Add a submission to the processing queue
void plagiarism_checker_t::add_submission(std::shared_ptr<submission_t> __submission) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        
        // Update the timestamp for the submission being added
        auto current_time = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
        timestamps[__submission->id] = current_time - time_offset;

        // Add submission to the processing queue
        submission_queue.push(__submission);
    }
    queue_cv.notify_one();
    return;
}


// Check for exact match
bool plagiarism_checker_t::check_exact_match(const std::vector<int>& tokens1, const std::vector<int>& tokens2, int length) {
    for (size_t i = 0; i + length <= tokens1.size(); ++i) {
        for (size_t j = 0; j + length <= tokens2.size(); ++j) {
            if (std::equal(tokens1.begin() + i, tokens1.begin() + i + length, tokens2.begin() + j)) {
                return true;
            }
        }
    }
    return false;
}

// Count pattern matches
int plagiarism_checker_t::count_pattern_matches(const std::vector<int>& tokens1, 
        const std::vector<int>& tokens2, int length, std::set<int> &unique) {
    int count = 0;
    for (size_t i = 0; i + length <= tokens1.size(); ++i) {
        for (size_t j = 0; j + length <= tokens2.size(); ++j) {
            if (std::equal(tokens1.begin() + i, tokens1.begin() + i + length, tokens2.begin() + j)) {
                unique.insert(j);
                count++;
                j += length - 1;
                i += length - 1;
                if (count >= MIN_PATTERN_MATCHES) return count;
            }
        }
    }
    return count;
}


void plagiarism_checker_t::flag_submission(std::shared_ptr<submission_t> existing, 
    std::shared_ptr<submission_t> new_submission, bool is_bidirectional) {

    auto time_diff = std::abs(timestamps[existing->id] - timestamps[new_submission->id]);

    // Check if the new submission is already flagged, and if so, return early
    if (flagged_submissions[new_submission->id]) {
        return;  // Skip further flagging if it's already flagged
    }

    if (new_submission->student != nullptr) {
        new_submission->student->flag_student(new_submission);
        flagged_submissions[new_submission->id] = true; 
    }

    if (new_submission->professor != nullptr) {
        new_submission->professor->flag_professor(new_submission);
        flagged_submissions[new_submission->id] = true; 
    }

    if (timestamps[existing->id] == 0) return;  // Skip if the existing submission timestamp is 0

    // Handle flagging of the existing submission if time difference is less than TIME_DIFF_THRESHOLD
    if (time_diff < TIME_DIFF_THRESHOLD && !flagged_submissions[existing->id]) {
        if (existing->student != nullptr) {
            existing->student->flag_student(existing);
            flagged_submissions[existing->id] = true;  
        }
        if (existing->professor != nullptr) {
            existing->professor->flag_professor(existing);
            flagged_submissions[existing->id] = true;  
        }
    }
}


// Background thread to process submissions
void plagiarism_checker_t::process_submissions() {
    while (true) {
        std::shared_ptr<submission_t> submission;

        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            // Wait until there is a submission in the queue or stop_processing is set to true
            queue_cv.wait(lock, [this]() { return !submission_queue.empty() || stop_processing; });

            if (stop_processing && submission_queue.empty()) {
                break;
            }

            submission = submission_queue.front();
            submission_queue.pop();
        }

        this->process_submission(submission);
    }
    return;
}

// Process a single submission
void plagiarism_checker_t::process_submission(std::shared_ptr<submission_t> __submission) {
    auto tokenizer = tokenizer_t(__submission->codefile);
    auto tokens = tokenizer.get_tokens();
    std::set<int> unique;
    // Compare with all existing submissions
    int64_t total_count = 0;
    for (const auto& existing_submission : submissions) {
        auto existing_tokens = tokenizer_t(existing_submission->codefile).get_tokens();
        bool is_bidirectional = std::abs(timestamps[existing_submission->id] - timestamps[__submission->id]) < TIME_DIFF_THRESHOLD;
        
        if (check_exact_match(existing_tokens, tokens, EXACT_MATCH_THRESHOLD)) {
            flag_submission(existing_submission, __submission, is_bidirectional);
            continue;
        }
        auto it = count_pattern_matches(existing_tokens, tokens, PATTERN_MATCH_LENGTH, unique);
        if (it >= MIN_PATTERN_MATCHES) {
            flag_submission(existing_submission, __submission, is_bidirectional);
        }
        total_count += it;
    }
    // patchwork plagiarism
    if (total_count >= MINIMUM_PATCHWORK_MATCHES and unique.size() >= MINIMUM_PATCHWORK_MATCHES) {
        if (!flagged_submissions[__submission->id]) {
            if (__submission->student != nullptr) {
                __submission->student->flag_student(__submission);
                flagged_submissions[__submission->id] = true;
            }
            if (__submission->professor != nullptr) {
                __submission->professor->flag_professor(__submission);
                flagged_submissions[__submission->id] = true;
            }
        }
    }
    {
        std::lock_guard<std::mutex> lock(submissions_mutex);
        submissions.push_back(__submission);
    }
    return;
}



