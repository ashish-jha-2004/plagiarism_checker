#include <array>
#include <iostream>
#include <span>
#include <vector>
#include <cmath>
// -----------------------------------------------------------------------------

#include <unordered_set>
#include <algorithm>

// Constants defining match criteria
#define MIN_PERFECT_MATCH 10
#define MIN_APPROX_MATCH 30
#define MATCH_THRESHOLD 0.8


// RollingHash class to compute rolling hash values for a sequence
class RollingHash {
private:
    std::vector<long long> hash, power;
    static constexpr int BASE = 257;
    static constexpr int MOD = 1e9 + 7;
public:
    // Initializes rolling hash values for the input sequence
    RollingHash(const std::vector<int>& str) {
        int n = str.size();
        hash.resize(n + 1);
        power.resize(n + 1);

        hash[0] = 0;
        power[0] = 1;

        for (int i = 0; i < n; i++) {
            hash[i + 1] = ((hash[i] * BASE) + str[i]) % MOD;
            power[i + 1] = (power[i] * BASE) % MOD;
        }
    }

    // Computes hash for a specified subsequence
    long long getHash(const int start_index, const int length) const {
        long long h = (hash[start_index + length] - (hash[start_index] * power[length] % MOD) + MOD) % MOD;
        return h;
    }

    // Generates hashes for all substrings of a given length
    std::vector<long long> generateHashes(const int length = MIN_PERFECT_MATCH) const {
        std::vector<long long> hashes;
        if (hash.size() < length) return hashes;
        for (int i = 0; i <= hash.size() - length - 1; i++) {
            hashes.push_back(getHash(i, length));
        }
        return hashes;
    }
};

// Checks the length of a perfect match starting from two indices in separate sequences
int checkMatchLength(const std::vector<int>& vec1, int start1,
                     const std::vector<int>& vec2, int start2) {
    int matchLength = 0;
    while (start1 + matchLength < vec1.size() && start2 + matchLength < vec2.size() &&
           vec1[start1 + matchLength] == vec2[start2 + matchLength]) {
        matchLength++;
    }
    return matchLength;
}


// Finds exact matches between two sequences and returns the total matched length
int findExactMatches(const std::vector<int>& submission1, const std::vector<int>& submission2,
                     const int min_length = MIN_PERFECT_MATCH) {
    RollingHash rolling_hash1(submission1), rolling_hash2(submission2);
    std::vector<long long> hashes1 = rolling_hash1.generateHashes(min_length);
    std::vector<long long> hashes2 = rolling_hash2.generateHashes(min_length);

    int total_matched_length = 0;
    std::unordered_set<int> matched_indices1, matched_indices2;

    for (int i = 0; i < hashes1.size(); i++) {
        for (int j = 0; j < hashes2.size(); j++) {
            if (hashes1[i] == hashes2[j] && matched_indices1.count(i) == 0 &&
                matched_indices2.count(j) == 0) {
                int match_length = checkMatchLength(submission1, i, submission2, j);
                if (match_length >= min_length) {
                    for (int k = 0; k < match_length; k++) {
                        matched_indices1.insert(i + k);
                        matched_indices2.insert(j + k);
                    }
                    i += match_length - 1;
                    total_matched_length += match_length;
                    break;
                }
            }
        }
    }

    return total_matched_length;
}



// Counts mismatches within a specified length from given starting indices in two sequences
int countMismatches(const std::vector<int>& vec1, const std::vector<int>& vec2,
                    int start1, int start2, int length) {
    int mismatches = 0;
    for (int k = 0; k < length; k++) {
        if (vec1[start1 - k] != vec2[start2 - k]) {
            mismatches++;
        }
    }
    return mismatches;
}


// Finds the longest fuzzy match allowing limited mismatches within 80% and returns the starting indices as well
std::array<int, 3> findLongestFuzzyMatch(const std::vector<int>& vec1, 
                                         const std::vector<int>& vec2) {
    int n = vec1.size();
    int m = vec2.size();
    // Creating a 2D DP array
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(m + 1, 0));
    int max_length = 0;
    int start_index1 = -1;
    int start_index2 = -1;

    // Iterate through each position in both vectors
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (vec1[i - 1] == vec2[j - 1]) {
                // If elements match, extend the match length from the previous position
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                // If elements do not match, consider a "fuzzy" match with mismatches
                int length = dp[i - 1][j - 1] + 1;
                int max_mismatches = static_cast<int>(length * (1 - MATCH_THRESHOLD));
                int mismatches = countMismatches(vec1, vec2, i - 1, j - 1, length);

                if (mismatches <= max_mismatches) {
                    dp[i][j] = length;
                } else {
                    dp[i][j] = 0;
                }
            }

            // Update longest match details if the current match length is the largest found
            if (dp[i][j] > max_length) {
                max_length = dp[i][j];
                start_index1 = i - max_length;
                start_index2 = j - max_length;
            }
        }
    }

    if(max_length<MIN_APPROX_MATCH){
        max_length = 0;
        start_index1 = -1;
        start_index2 = -1;
    }
    return {max_length, start_index1, start_index2};
}

std::array<int, 5> match_submissions(std::vector<int>& submission1, std::vector<int>& submission2) {
    auto [fuzzy_match_length, start_index1, start_index2] = findLongestFuzzyMatch(submission1, submission2);
    size_t min_size = std::min(submission1.size(), submission2.size());
    int total_match_length = findExactMatches(submission1, submission2);

    bool is_plagiarized = (total_match_length >= (0.15 * min_size) && fuzzy_match_length >= (0.3 * min_size)) || 
                         total_match_length >= 300 || fuzzy_match_length >= 250;

    return {is_plagiarized ? 1 : 0, total_match_length, fuzzy_match_length, start_index1, start_index2};
}