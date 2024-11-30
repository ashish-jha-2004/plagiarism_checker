
#include "structures.hpp"
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <set>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>


class plagiarism_checker_t {
public:
    plagiarism_checker_t();
    plagiarism_checker_t(const std::vector<std::shared_ptr<submission_t>>& pre_existing_submissions);
    ~plagiarism_checker_t();
    void add_submission(std::shared_ptr<submission_t> __submission);

private:
    // Background processing thread
    void process_submissions();

    // Process a single submission
    void process_submission(std::shared_ptr<submission_t> __submission);

    // Helper methods for plagiarism detection and flagging
    bool check_exact_match(const std::vector<int>& tokens1, const std::vector<int>& tokens2, int length);
    int count_pattern_matches(const std::vector<int>& tokens1, const std::vector<int>& tokens2, 
                int length, std::set<int> &unique);
    void flag_submission(std::shared_ptr<submission_t> existing, 
                        std::shared_ptr<submission_t> new_submission, bool is_bidirectional);

    // Data members
    std::vector<std::shared_ptr<submission_t>> submissions; // Processed submissions
    std::map<int, int64_t> timestamps; // Timestamps
    std::queue<std::shared_ptr<submission_t>> submission_queue; // Submissions to process
    std::map<int, bool> flagged_submissions; // Flagged submissions
    std::mutex queue_mutex;              // Mutex for submission queue
    std::mutex submissions_mutex;        // Mutex for processed submissions
    std::condition_variable queue_cv;    // Condition variable for the queue
    std::atomic<bool> stop_processing;   // Signal to stop processing
    int64_t time_offset;                     // Time offset
    std::thread processing_thread;       // Background processing thread
};
