#include <array>
#include <iostream>
#include <span>
#include <vector>
#include <cmath>
// -----------------------------------------------------------------------------
#include <set>
#include <unordered_map>
#include <string>
#include<algorithm>
// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here



// OPTIONAL: Add your helper functions and data structures here
 // FOR THE FOLLOWING FUNCTION IMPLEMENTATION we adapted a part of code from geeksandgeeks. DP programming and LCS : https://www.geeksforgeeks.org/longest-common-subsequence-dp-4/

struct Result {
    int start1, end1, start2, end2, length;
    bool valid;
};

class SequenceMatcher {
public:
    // Main function to find a valid pair of sequences with LCS length meeting the threshold criteria
    static Result findValidSequences(const std::vector<int>& v1, const std::vector<int>& v2, double threshold = 0.8) {
        // Initialize an invalid Result by default
        Result result = {-1, -1, -1, -1, 0, false};

        // If either vector is empty, return the invalid result immediately
        if (v1.empty() || v2.empty()) return result;

        // Obtain the longest common subsequence as pairs of matching indices
        std::vector<std::pair<int, int>> lcs = getLCS(v1, v2);
        if (lcs.empty()) return result;  // Return if no common subsequence exists

        int maxLength = 0;  // Variable to track the maximum sequence length found

        // Explore pairs of indices in the LCS list to find the longest valid sequence within the threshold
        for (int i = 0; i < lcs.size(); ++i) {
            for (int j = i; j < lcs.size(); ++j) {
                int len1 = lcs[j].first - lcs[i].first + 1;  // Length of subsequence in v1
                int len2 = lcs[j].second - lcs[i].second + 1; // Length of subsequence in v2
                int matches = j - i + 1;  // Number of matching elements

                // Check if the matches meet the threshold for both v1 and v2 subsequence lengths
                if (matches >= threshold * len1 && matches >= threshold * len2) {
                    int currentLength = std::max(len1, len2);  // Determine the longer sequence length
                    // Update result if this sequence is the longest one found that meets the criteria
                    if (currentLength > maxLength) {
                        maxLength = currentLength;
                        result = {lcs[i].first, lcs[j].first, lcs[i].second, lcs[j].second, currentLength, true};
                    }
                }
            }
        }

        return result;  // Return the best found sequence or an invalid result if none met the criteria
    }

private:
    // Function to compute the LCS as pairs of indices (index from v1 and v2 for each matching element)
    static std::vector<std::pair<int, int>> getLCS(const std::vector<int>& a, const std::vector<int>& b) {
        int m = a.size(), n = b.size();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));  // DP table for LCS length calculation

        // Fill the DP table based on matching elements between a and b
        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (a[i-1] == b[j-1]) {
                    dp[i][j] = dp[i-1][j-1] + 1;  // Extend LCS if elements match
                } else {
                    dp[i][j] = std::max(dp[i-1][j], dp[i][j-1]);  // Otherwise, take the longer of previous results
                }
            }
        }

        // Backtrack through the DP table to reconstruct the LCS as index pairs
        std::vector<std::pair<int, int>> lcs;
        int i = m, j = n;
        while (i > 0 && j > 0) {
            if (a[i-1] == b[j-1]) {
                lcs.push_back({i-1, j-1});  // Add matching indices to the LCS list
                --i; --j;  // Move diagonally in the DP table
            } else if (dp[i-1][j] > dp[i][j-1]) {
                --i;  // Move up in DP table if upper cell has greater LCS value
            } else {
                --j;  // Move left in DP table if left cell has greater LCS value
            }
        }
        
        std::reverse(lcs.begin(), lcs.end());  // Reverse to get indices in the correct order
        return lcs;
    }
};




//Function to find total length of matching substrings. FOR result[1]
std::unordered_map<std::string, std::vector<int>> get_n_length_substrings(const std::vector<int>& tokens, int n, std::set<int>& used_indices) {
    std::unordered_map<std::string, std::vector<int>> substrings;
    for (size_t i = 0; i + n <= tokens.size(); ++i) {
        // Check if this substring overlaps with already used indices
        bool overlap = false;
        for (int j = 0; j < n; ++j) {
            if (used_indices.count(i + j)) {
                overlap = true;
                break;
            }
        }
        if (overlap) continue;

        // Build a string representation of the n-length substring
        std::string pattern;
        for (int j = 0; j < n; ++j) {
            pattern += std::to_string(tokens[i + j]) + ",";
        }

        // Store the starting index for each unique pattern
        substrings[pattern].push_back(i);
    }
    return substrings;
}

// Function to find the total length of non-overlapping exact pattern matches between lengths 10 and max_match_len
int find_total_match_length(const std::vector<int>& submission1, const std::vector<int>& submission2, int min_match_len= 10, int max_match_len = 29) {
    int total_length = 0;
    std::set<int> used_indices1, used_indices2; // Track used indices in each vector

    // Loop through each length from max_match_len to min_match_len
    for (int n = max_match_len; n >= min_match_len; --n) {
        // Get all non-overlapping n-length substrings for both submissions
        std::unordered_map<std::string, std::vector<int>> patterns1 = get_n_length_substrings(submission1, n, used_indices1);
        std::unordered_map<std::string, std::vector<int>> patterns2 = get_n_length_substrings(submission2, n, used_indices2);

        // Find common patterns and accumulate their lengths
        for (const auto& entry : patterns1) {
            const std::string& pattern = entry.first;

            // Check if the pattern exists in both submissions
            if (patterns2.find(pattern) != patterns2.end()) {
                // Process each occurrence of the pattern in submission1
                for (int idx1 : patterns1[pattern]) {
                    // If indices overlap in submission1, skip this match
                    bool overlap1 = false;
                    for (int j = 0; j < n; ++j) {
                        if (used_indices1.count(idx1 + j)) {
                            overlap1 = true;
                            break;
                        }
                    }
                    if (overlap1) continue;

                    // Process each occurrence of the pattern in submission2
                    for (int idx2 : patterns2[pattern]) {
                        bool overlap2 = false;
                        for (int j = 0; j < n; ++j) {
                            if (used_indices2.count(idx2 + j)) {
                                overlap2 = true;
                                break;
                            }
                        }
                        if (overlap2) continue;

                        // If no overlap, count this match and mark indices as used
                        total_length += n;
                        for (int j = 0; j < n; ++j) {
                            used_indices1.insert(idx1 + j);
                            used_indices2.insert(idx2 + j);
                        }

                        // Break after finding one match to avoid multiple counting for the same pattern
                        break;
                    }
                }
            }
        }
    }

    return total_length;
}


std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2) {
    // TODO: Write your code here
    std::array<int, 5> result = {-1, 0, -1, -1, -1};
    result[1] = find_total_match_length(submission1,submission2);
    Result longest = SequenceMatcher::findValidSequences(submission1, submission2);  //class RESULT IS DEFINED EARLIER 
    // result[2] = longest.maxLength;
    // result[3] = longest.index1;
    // result[4] = longest.index2;
    if(longest.length > 0) {
        result[2] = longest.length;
        result[3] = longest.start1;
        result[4] = longest.start2;
    }

    int min  = (submission1.size()>submission2.size())? submission2.size() : submission1.size();


    if( result[1] / static_cast<double>(min) >= 0.2   || result[2]/static_cast<double>(min) >= 0.3){
        result[0] = 1;

    }
    else{
        result[0] = 0;
    }
    return result; 
    // End TODO  
}