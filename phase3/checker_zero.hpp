#include <array>
#include <cmath>
#include <iostream>
#include <span>
#include <vector>
// -----------------------------------------------------------------------------

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here
namespace match_detector {
    std::vector<int> KMPtable(std::span<int> pattern);
    std::vector<std::pair<int, int>> KMPsearch(std::span<int> vec, 
            std::span<int> pattern, int min_match_len);
    int net_match_len(std::span<int> submission1, std::span<int> submission2);
    bool is_approx_match(std::span<int> sequence1, 
            std::span<int> sequence2);
    std::array<int, 3> find_longest_approx_match(std::span<int> submission1, 
            std::span<int> submission2);
};

std::vector<int> match_detector::KMPtable(std::span<int> pattern) {
    int len = pattern.size();
    std::vector<int> table(len, 0);
    int i = 1;
    int j = 0;
    while (i < len) {
        if (pattern[i] == pattern[j]) {
            j++;
            table[i] = j;
            i++;
        } else {
            if (j != 0) {
                j = table[j - 1];
            } else {
                table[i] = 0;
                i++;
            }
        }
    }
    return table;
}

std::vector<std::pair<int, int>> match_detector::KMPsearch(std::span<int> vec, 
        std::span<int> pattern, int min_match_len) {
    std::vector<std::pair<int, int>> result;
    std::vector<int> table = match_detector::KMPtable(pattern);
    int len = vec.size();
    int len_pattern = pattern.size();
    int i = 0;
    int j = 0;
    while (i < len) {
        if (vec[i] == pattern[j]) {
            i++;
            if (++j < min_match_len) {
                continue;
            }
            if (result.empty() || i - j > result.back().first) {
                result.push_back({i - j, j});
            } else {
                result.back().second = j;
            }
            j = table[j - 1];
        } else {
            if (j != 0) {
                j = table[j - 1];
            } else {
                i++;
            }
        }
    }
    return result;
}

int match_detector::net_match_len(std::span<int> submission1, 
        std::span<int> submission2) {
    int len1 = submission1.size();
    int len2 = submission2.size();
    std::vector<int> best_match_sub1_dp(len1 - 9, 0);
    for (int pos1 = len1 - 10; pos1 >= 0; pos1--) {
        if (pos1 + 10 < len1) {
            best_match_sub1_dp[pos1] = best_match_sub1_dp[pos1 + 1];
        }
        for (decltype(auto) match : match_detector::KMPsearch(submission2, 
                submission1.subspan(pos1, 10), 10)) {
            if (pos1 + match.second > len1 - 10) continue;
            best_match_sub1_dp[pos1] = std::max(best_match_sub1_dp[pos1], 
                    match.second + best_match_sub1_dp[pos1 + match.second]);
        }
    }
    return best_match_sub1_dp[0];
}

bool match_detector::is_approx_match(
        std::span<int> sequence1, std::span<int> sequence2) {
    int len1 = sequence1.size();
    int len2 = sequence2.size();
    if (std::max(len1, len2) > 1.1 * std::min(len1, len2)) {
        return false;
    }
    std::vector<std::vector<int>> longest_subseq(len1 + 1, 
            std::vector<int>(len2 + 1, 0));
    for (int i = len1; i >= 0; i--) for (int j = len2; j >= 0; j--) {
        if (i == len1 || j == len2) {
            longest_subseq[i][j] = 0;
        } else if (sequence1[i] == sequence2[j]) {
            longest_subseq[i][j] = longest_subseq[i + 1][j + 1] + 1;
        } else {
            longest_subseq[i][j] = std::max(longest_subseq[i + 1][j], 
                    longest_subseq[i][j + 1]);
        }
    }
    if (longest_subseq[0][0] < 0.8 * std::min(len1, len2)) {
        return false;
    }
    return true;
}

std::array<int, 3> match_detector::find_longest_approx_match(
        std::span<int> sequence1, std::span<int> sequence2) {
    int len1 = sequence1.size();
    int len2 = sequence2.size();
    std::array<int, 3> result = {0, 0, 0};
    std::vector<std::vector<bool>> all_matches(len1 / 10,
            std::vector<bool>(len2 / 10, false));
    for (int i = 0; i < (len1 - 30) / 10; i++) {
        for (int j = 0; j < (len2 - 30) / 10; j++) {
            if (match_detector::is_approx_match(
                    sequence1.subspan(i * 10, 30), 
                    sequence2.subspan(j * 10, 30))) {
                all_matches[i][j] = true;
            }
        }
    }
    for (int i = 0; i < len1 / 10; i++) for (int j = 0; j < len2 / 10; j++) {
        if (!all_matches[i][j]) {
            continue;
        }
        int match_len = 30;
        for (int k = 1; k < std::min(len1 / 10 - i, len2 / 10 - j); k++) {
            if (!all_matches[i + k][j + k]) {
                break;
            }
            match_len += 10;
        }
        if (match_len <= result[0]) {
            continue;
        }
        result[0] = match_len;
        result[1] = i * 10;
        result[2] = j * 10;
    }
    return result;
}

std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2) {
    // TODO: Write your code here
    std::array<int, 5> result = {0, 0, 0, 0, 0};
    std::span<int> sub1_span = std::span(submission1);
    std::span<int> sub2_span = std::span(submission2);

    result[1] = std::min(match_detector::net_match_len(sub1_span, sub2_span), 
            match_detector::net_match_len(sub2_span, sub1_span));
    std::array<int, 3> approx_match = match_detector::find_longest_approx_match(
            sub1_span, sub2_span);
    result[2] = approx_match[0];
    result[3] = approx_match[1];
    result[4] = approx_match[2];
    result[0] = (result[1] >= 100 || result[2] >= 75) ? 1 : 0;
    return result;
    // End TODO
}
