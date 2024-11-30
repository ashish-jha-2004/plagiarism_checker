#include <array>
#include <iostream>
#include <span>
#include <vector>
#include <cmath>
// -----------------------------------------------------------------------------
// #include <stdexcept>
#include <algorithm> 
#include <map>
#define k 10 // for exact match

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here
class Exact_Match{
    public:
        // returns hash values of the text
        // Hash computation is inspired from GeeksForGeeks - "https://www.geeksforgeeks.org/rabin-karp-algorithm-for-pattern-searching/"
        static std::map<int, std::vector<int>> hash_text(std::vector<int> text)
        {
            std::map<int, std::vector<int>> hash_map;

            int n = text.size();
            int p = 5381, d = 33;
            int h = 1;

            for (int i = 0; i < 10 - 1; i++) h = (h * d) % p;

            int t = 0;
            for (int i = 0; i < 10; i++) t = (d * t + text[i]) % p;

            if (hash_map.find(t) == hash_map.end()) hash_map[t] = {0};
            else hash_map[t].push_back(0);

            // Uses rolling hash function for hash computation efficiency
            for (int i = 1; i < n - 10; i++){
                t = (d * (t - text[i - 1] * h) + text[i + 9]) % p;
                if (t < 0) t = t + p;
                if (hash_map.find(t) == hash_map.end()) hash_map[t] = {i};
                else hash_map[t].push_back(i);
            }

            return hash_map;
        }

        // returns hash values of the pattern
        // Hash computation is inspired from GeeksForGeeks - "https://www.geeksforgeeks.org/rabin-karp-algorithm-for-pattern-searching/"
        static std::vector<int> hash_pattern(std::vector<int> &pattern)
        {
            int m = pattern.size();
            int h = 1;
            int p = 5381, d = 33;
            std::vector<int> hashes(m - k + 1);

            for (int i = 0; i < 10 - 1; i++) h = (h * d) % p;

            int t = 0;
            for (int i = 0; i < 10; i++) t = (d * t + pattern[i]) % p;

            hashes[0] = t;
            // Uses rolling hash function for hash computation efficiency
            for (int i = 1; i < m - 10; i++){
                t = (d * (t - pattern[i - 1] * h) + pattern[i + 9]) % p;
                if (t < 0)
                    t = t + p;
                hashes[i] = t;
            }

            return hashes;
        }

        // returns the length of the exact match
        static int exact_match(std::vector<int> &text, std::vector<int> &pattern)
        {
            // calls the hash functions
            std::map<int, std::vector<int>> text_hash = hash_text(text);
            std::vector<int> pattern_hash = hash_pattern(pattern);
            int p = pattern.size();
            int t = text.size();

            std::vector<bool> visited(t, false);

            int hashp;
            int total_length = 0;

            // iterates over the pattern index by index if there is no match
            for (int i = 0; i < p - k; i++)
            {   
                // matches the hash value of pattern and text
                hashp = pattern_hash[i];
                if (text_hash.find(hashp) != text_hash.end())
                {
                    std::vector<int> indices = text_hash[hashp];
                    int max = k;
                    int index = -1;

                    // runs for maximum match length over each match of hash value
                    for (auto j : indices)
                    {
                        int len = k;
                        bool ismatch = true;

                        // checks for feasible matches (considering overlap conditions)
                        for (int z = 0; z < 10; z++)
                        {
                            if (text[j + z] != pattern[i + z]) ismatch = false;
                            if (visited[j + z] == true) ismatch = false;
                            if(ismatch == false) break;
                        }
                        
                        // if match is feasible, checks for further indices 
                        if(ismatch){
                            while ((j + len < t) && (text[j + len] == pattern[i + len]) && !(visited[j + len]))
                            {
                                len++;
                                if (len == 20)
                                    break;
                            }

                            if ((max <= len))
                            {
                                max = len;
                                index = j;
                            }
                        }   
                    }
                    // Updates if the feasible match has maximum length and adds to the total_length
                    if (index != -1)
                    {
                        for (int y = 0; y < max; y++) visited[index + y] = true;
                        total_length = total_length + max;
                        i = i + max - 1;
                    }
                }
            }
            return total_length;
        }
};

class Approx_Match
{
public:
    // returns the starts indices of both the submissions where the largest approx match is found along with its length
    // returns as a vector (start1, start2, max_approx_length)
    static std::vector<int> longest_80(std::vector<int> ind1, std::vector<int> ind2)
    {
        int s = 0;
        int e = ind1.size()-1;
        int max = 0;
        int sub1i = ind1[s];
        int sub2i = ind2[s];
        // Iterates over all possible aprrox length sub vectors and finds the vector with maximum length
        for(int i=s;i<ind1.size();i++){
            for(int j=e;j>i;j--){
                if((j-i+1)>0.8*(ind1[j]-ind1[i]+1)) {
                    if(max < ind1[j]-ind1[i]+1){
                        sub1i = ind1[i];
                        sub2i = ind2[i];
                        max = ind1[j]-ind1[i]+1;
                    }
                }
            }
        }
        return {sub1i,sub2i,max};
    }

    // This algorithm is inspired from GeeksForGeeks website "https://www.geeksforgeeks.org/longest-common-subsequence-dp-4/"
    static std::vector<int> findLCS(const std::vector<int> &vec1, const std::vector<int> &vec2)
    {
        int m = vec1.size();
        int n = vec2.size();

        // dp[i][j] stores the length of LCS for vec1[0..i-1] and vec2[0..j-1]
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

        // Fill dp table
        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (vec1[i - 1] == vec2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                }
                else
                {
                    dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }

        // Reconstruct the LCS from dp table
        std::vector<int> lcs;
        int i = m, j = n;
        std::vector<int> text_indices;
        std::vector<int> pat_indices;

        while (i > 0 && j > 0)
        {
            // If current characters match, add to result
            if (vec1[i - 1] == vec2[j - 1])
            {   
                text_indices.push_back(i-1);
                pat_indices.push_back(j-1);
                i--;
                j--;
            }
            // Move in direction of larger value
            else if (dp[i - 1][j] > dp[i][j - 1]) i--;
            else j--;
        }
        std::reverse(text_indices.begin(), text_indices.end());
        std::reverse(pat_indices.begin(), pat_indices.end());
        return longest_80(text_indices,pat_indices);
    }
};

//returns result[0] analysing result[1],result[2] and token vector sizes
int determine_plag_flag(int result1, int result2, int n1, int n2) {
    // Calculate percentage of matched content relative to the size of the smaller file
    double match_percentage = static_cast<double>(result1) / std::min(n1, n2) * 100;
    
    // Calculate the ratio of file sizes
    double size_ratio = static_cast<double>(std::max(n1, n2)) / std::min(n1, n2);
    
    // DEclaring three kinds of conditions for determining plagiarism flag
    bool plag_conditions[3];
    
    if (std::min(n1, n2) < 150) {
        // Handle smaller files differently (less than 150 tokens or so)
        plag_conditions[0] = (match_percentage > 70);                     // High match percentage
        plag_conditions[1] = false;          // Long continuous match (Not so relevant for smaller codes )
        plag_conditions[2] = false;                                       // Not needed for small files
    } else {
        // Conditions for larger files
        plag_conditions[0] = (match_percentage > 50 );    // High match percentage with enough length
        plag_conditions[1] = (result2 > std::min(n1, n2) * 0.8); // Very long continuous match
        plag_conditions[2] = (match_percentage > 45 && size_ratio < 1.3 && result1 > 150); // Moderate match with similar size
    }
    
    // Set plag flag based on each of the plag_conditions analysed above
    return (plag_conditions[0] || plag_conditions[1] || plag_conditions[2]) ? 1 : 0;
}

std::array<int, 5> match_submissions(std::vector<int> &submission1, std::vector<int> &submission2) {
    // TODO: Write your code here
    std::array<int, 5> result = {0, 0, 0, 0, 0};
    std::vector<int> approx_reply;
    int n1 = submission1.size();
    int n2 = submission2.size();

    if(n1>n2) {
        result[1]=Exact_Match::exact_match(submission1,submission2);
        approx_reply = Approx_Match::findLCS(submission1,submission2);
        result[2] = approx_reply[2];
        result[3] = approx_reply[0];
        result[4] = approx_reply[1];
        
    } 
    else {
        result[1]=Exact_Match::exact_match(submission2,submission1);
        approx_reply = Approx_Match::findLCS(submission2,submission1);
        result[2] = approx_reply[2];
        result[3] = approx_reply[1];
        result[4] = approx_reply[0];
        
    }
    result[0] = determine_plag_flag(result[1], result[2], n1, n2);
    return result; 
    // End TODO
}












