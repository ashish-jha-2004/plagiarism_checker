#include <array>
#include <iostream>
#include <span>
#include <vector>
#include <cmath>
#include <map>
#include <memory>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <unordered_map>
#include <cassert>
#include <chrono>

// -----------------------------------------------------------------------------

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here

class TrieNode {
public:
    std::unordered_map<int, TrieNode*> children;
    int start;
    std::shared_ptr<int> end;
    int suffixIndex;
    TrieNode* suffixLink;
    bool isLeaf; 
    int children_count;

    TrieNode(int start, std::shared_ptr<int> end) 
        : start(start), end(end), suffixIndex(-1), suffixLink(nullptr), isLeaf(false) {}

    TrieNode(int start, std::shared_ptr<int> end, bool isLeaf) 
        : start(start), end(end), suffixIndex(-1), suffixLink(nullptr), isLeaf(isLeaf) {}

    int edgeLength() const {
        return *end - start + 1;
    }
};

class SimpleNode{
public:
    int value;
    std::unordered_map<int, SimpleNode*> children;

    SimpleNode(int v){
        value = v;
    }

    SimpleNode(){
        value = -1;
    }
};

// My implementation of the SuffixTree class is such that we should only ever insert codes of length 10 into this Trie,
// to keep track of which codes have already been seen once.
class NotAcceptingTrie{
public:
    SimpleNode* root;

    NotAcceptingTrie(){
        root = new SimpleNode();
    }

    void insert(const std::vector<int>& sequence, int count){
        SimpleNode* current = root;
        for (int i = 0; i < sequence.size(); i++){
            if (current->children.find(sequence[i]) == current->children.end()){
                current->children[sequence[i]] = new SimpleNode();
            }
            current = current->children[sequence[i]];
        }
        current->value = count;
    }

    bool check_if_present(const std::vector<int>& sequence){
        SimpleNode* current = root;
        for (int i = 0; i < sequence.size(); i++){
            if (current->children.find(sequence[i]) == current->children.end()){
                return false;
            }
            current = current->children[sequence[i]];
        }
        if (current->value == 0){
            return true;
        }
        current->value--;
        return false;
    }

    void freeTrieByPostOrder(SimpleNode* node){
        if (node == nullptr){
            return;
        }
        for (auto& child : node->children){
            freeTrieByPostOrder(child.second);
        }
        delete node;
    }

    ~NotAcceptingTrie(){
        freeTrieByPostOrder(root);
    }
};

class SuffixTree {
    // Implementation of this is based on Ukkonen's algorithm. I have referred to the implementation specified in https://www.geeksforgeeks.org/ukkonens-suffix-tree-construction-part-6/ and https://cp-algorithms.com/string/suffix-tree-ukkonen.html
    // Longest Common Substring is based on the algorithm explained in Wikipedia and on https://www.geeksforgeeks.org/suffix-tree-application-5-longest-common-substring-2/
    public:

    TrieNode* root;
    TrieNode* activeNode;
    TrieNode* lastNewNode;

    NotAcceptingTrie* not_accepted_trie;

    int activeEdge;
    int activeLength;

    int remainingSuffixCount;
    std::shared_ptr<int> leafEnd;
    std::unordered_map<int, int> children;

    std::shared_ptr<int> rootEnd;
    std::shared_ptr<int> splitEnd;
    int count;
    int size;

    std::vector<int> sequence;

    SuffixTree(const std::vector<int>& submission);

    ~SuffixTree();

    TrieNode* generateNewNode(int start, std::shared_ptr<int> end);
    
    bool walkDown(TrieNode* node);

    void extendSuffixTree(int position);

    void freeSuffixTreeByPostOrder(TrieNode* node);

    void setSuffixIndexByDFS(TrieNode* node, int label_height);

    void setNumberOfOccurencesByDFS(TrieNode* node);

    std::pair<int, int> deterministic_check(const std::vector<int>& submission2, std::vector<bool> plag_flags){
        // The algorithm works as follows: For every substring of length 10, if it matches with a substring in the base suffix tree, 
        // If it does match, we add the substring to a trie containing the sequences (and their respective counts) which should not be matched anymore
        // If the same sequence is seen again, decrement its count in the trie. For as long as the count is non-zero, we can "accept" the sequence
        // If the count becomes 0, reject the sequence whenever it occurs again
        // Here, on a rejection, the double-counting array comes into play, to ensure that we don't skip valid sequences but also that if an element is forced to map to a pattern which has already been used up,
        // we don't consider it as a new pattern but as a "double count"
        std::vector<bool> double_count_flags(plag_flags.size(), false);
        int start=0;
        int end=0;
        int gap=10;
        for (int i = 0; i + gap <= submission2.size(); i++){
            end=deterministic_check_from_starting_pos(submission2, i);
            if (end == 0) continue;
            // std::cout << end << "\n";
            if (!not_accepted_trie->check_if_present(std::vector<int>(submission2.begin()+i, submission2.begin()+i+gap))){
                // std::cout << "Accepted: ";
                // for (int j=i; j<i+gap; j++){
                //     std::cout << submission2[j] << " ";
                // }
                // std::cout << "\n";
                not_accepted_trie->insert(std::vector<int>(submission2.begin()+i, submission2.begin()+i+gap), end-1);
                for (int j=i; j<i+gap; j++){
                    plag_flags[j]=true;
                }
            }
            else {
                for (int j = i; j < i+gap; j++){
                    plag_flags[j]=true;
                    double_count_flags[j]=true;
                }
                // std::cout << "Rejected: ";
                // for (int j=i; j<i+gap; j++){
                //     std::cout << submission2[j] << " ";
                // }
                // std::cout << "\n";
            }
        }
        int plag_count=0;
        int longest=0;
        int max_longest=0;
        for (int i=0; i<plag_flags.size(); i++){
            // Toggle the commentedness of the line below for different logics
            // double_count_flags[i]=false;
            if (plag_flags[i] && !double_count_flags[i]){
                plag_count++;
            }
            if (plag_flags[i]){
                longest++;
            }
            else {
                max_longest=std::max(max_longest, longest);
                longest=0;
            }
        }
        max_longest=std::max(max_longest, longest);
        // std::cout << "Longest non-exact match found: " << max_longest << "\n";
        return {plag_count, max_longest};
    }

    int deterministic_check_from_starting_pos(const std::vector<int>& submission2, int starting_pos){
        TrieNode* current=root;
        if (current->children.find(submission2[starting_pos])==current->children.end()){
            return 0;
        }
        current=current->children[submission2[starting_pos]];
        int i=starting_pos;
        int start=current->start;
        int end=*current->end;
        while(true){
            while (i<submission2.size() && start<=end && submission2[i]==sequence[start]){
                i++;
                if (i==starting_pos+10){
                    return current->children_count;
                }
                start++;
            }
            if (i==submission2.size() || i==starting_pos+10){
                return current->children_count;
            }
            if (start <= end || current->children.find(submission2[i])==current->children.end()){
                return 0;
            }
            current=current->children[submission2[i]];
            start=current->start;
            end=*current->end;
        }
    }

    int traverse(TrieNode* node, int labelHeight, int* max_lenght){
        // Recursion to set flags (stored in the suffixIndex variable) to determine whether a particular node has children corresponding to both vector 1 and vector 2, only vector 1, only vector 2 or no children.
        if (node == nullptr){
            return 0;
        }
        int i = 0;
        int ret=-1;
        if (node->suffixIndex < 0){
            for (auto child : node->children){
                ret=traverse(child.second, labelHeight + child.second->edgeLength(), max_lenght);
                if (node->suffixIndex == -1){
                    node->suffixIndex = ret;
                }
                else if ((node->suffixIndex == -2 && ret == -3) || (node->suffixIndex == -3 && ret == -2) || (node->suffixIndex == -4)){
                    node->suffixIndex = -4;
                    if (labelHeight > *max_lenght){
                        *max_lenght = labelHeight;
                    }
                }
            }
        }
        else if (node->suffixIndex > -1 && node->suffixIndex < size){
            return -2;
        }
        else if (node->suffixIndex >= size){
            return -3;
        }
        return node->suffixIndex;
    }

    int longest_perfect_match(const std::vector<int>& submission2){
        // Creates the generalized suffix tree and calls traverse() on the same, assuming that -1 and -2 are distinguished values which can never be part of the tokens
        int original_size=sequence.size();
        sequence.push_back(-1);
        for (int i=0; i<submission2.size(); i++){
            sequence.push_back(submission2[i]);
        }
        sequence.push_back(-2);
        for (int i=original_size; i<sequence.size(); i++){
            extendSuffixTree(i);
        }
        setSuffixIndexByDFS(root, 0);

        int max_length=0;

        traverse(root, 0, &max_length);
        // std::cout << "Longest perfect match: " << max_length << "\n";
        return max_length;
    }
};

    TrieNode* SuffixTree::generateNewNode(int start, std::shared_ptr<int> end) {
        count++;
        TrieNode* node = new TrieNode(start, end);
        node->suffixLink = root;
        node->suffixIndex = -1;
        return node;
    }

    bool SuffixTree::walkDown(TrieNode* node) {
        if (activeLength >= node->edgeLength()) {
            activeEdge += node->edgeLength(); 
            activeLength -= node->edgeLength();
            activeNode = node;
            return true;
        }
        return false;
    }

    void SuffixTree::extendSuffixTree(int position) {
        // std::cout << std::make_shared<int>(leafEnd) << " " << &leafEnd << "\n";
        *leafEnd = position; 
        remainingSuffixCount++;
        lastNewNode = nullptr;
        
        while (remainingSuffixCount > 0) {
            if (activeLength == 0) {
                activeEdge = position;
            }

            int edgeKey = sequence[activeEdge];
            if (activeNode->children.find(edgeKey) == activeNode->children.end()) {
                activeNode->children[edgeKey] = generateNewNode(position, leafEnd); 
                if (lastNewNode != nullptr) {
                    lastNewNode->suffixLink = activeNode;
                    lastNewNode = nullptr;
                }
            } else {
                TrieNode* next = activeNode->children[edgeKey];
                if (walkDown(next)) {
                    continue;
                }

                if (sequence[next->start + activeLength] == sequence[position]) {
                    if (lastNewNode != nullptr && activeNode != root) {
                        lastNewNode->suffixLink = activeNode;
                        lastNewNode = nullptr;
                    }
                    activeLength++;
                    break;
                }

                auto splitEndPtr = std::make_shared<int>(next->start + activeLength - 1);
                TrieNode* split = generateNewNode(next->start, splitEndPtr);
                activeNode->children[edgeKey] = split;

                split->children[sequence[position]] = generateNewNode(position, leafEnd); 
                next->start += activeLength;
                split->children[sequence[next->start]] = next;

                if (lastNewNode != nullptr) {
                    lastNewNode->suffixLink = split;
                }
                lastNewNode = split;
            }

            remainingSuffixCount--;
            if (activeNode == root && activeLength > 0) {
                activeLength--;
                activeEdge = position - remainingSuffixCount + 1;
            } else if (activeNode != root) {
                activeNode = activeNode->suffixLink;
            }
        }
    }

    void SuffixTree::freeSuffixTreeByPostOrder(TrieNode* node) {
        if (node == nullptr) {
            return;
        }
        for (auto& child : node->children) {
            freeSuffixTreeByPostOrder(child.second);
        }
        delete node;
    }

    SuffixTree::~SuffixTree() {
        freeSuffixTreeByPostOrder(root);
        delete not_accepted_trie;
    }

    SuffixTree::SuffixTree(const std::vector<int>& submission) {
        for (int a : submission) {
            sequence.push_back(a);
        }
        activeEdge = -1;
        activeLength = 0;
        remainingSuffixCount = 0;
        leafEnd = std::make_shared<int>(-1);
        lastNewNode = nullptr;
        count = 0;

        rootEnd = std::make_shared<int>(-1);
        root = generateNewNode(-1, rootEnd); 
        activeNode = root;
        size=sequence.size();
        
        for (int i = 0; i < sequence.size(); i++) {
            extendSuffixTree(i);
        }
        setSuffixIndexByDFS(root, 0);
        setNumberOfOccurencesByDFS(root);
        not_accepted_trie = new NotAcceptingTrie();
    }

    void SuffixTree::setSuffixIndexByDFS(TrieNode* node, int label_height) {
        if (node == nullptr) {
            return;
        }
        int leaf = 1;
        for (auto child : node->children) {
            leaf = 0;
            setSuffixIndexByDFS(child.second, label_height + child.second->edgeLength());
        }
        if (leaf == 1) {
            node->suffixIndex = sequence.size() - label_height;
            // std::cout << node->suffixIndex << "\n";
        }
        else {
            node->suffixIndex = -1;
        }
    }

    void SuffixTree::setNumberOfOccurencesByDFS(TrieNode* node) {
        if (node == nullptr) {
            return;
        }
        int leaf = 1;
        for (auto child : node->children) {
            leaf = 0;
            setNumberOfOccurencesByDFS(child.second);
        }
        if (leaf == 1) {
            node->children_count = 1;
        } else {
            node->children_count = 0;
            for (auto child : node->children) {
                node->children_count += child.second->children_count;
            }
        }
    }

std::vector<std::tuple<int, double, int, int>> smith_waterman_80_similarity(const std::vector<int>& s1, const std::vector<int>& s2, int match = 3, int gap = -2, const std::vector<double>& thresholds={0.8}) {
    // This algorithm, commonly used in bioinformatics, is used to find a location with high local alignment.
    // I am using extra 2D vectors in order to carry out the check for a particular location having a greater than 80% match before potentially changing the max_index.
    assert(std::abs(thresholds[0]-0.8)<1e-3);
    int m = s1.size(), n = s2.size();
    std::vector<std::vector<int>> H(m + 1, std::vector<int>(n + 1, 0));
    std::vector<std::vector<int>> L1(m + 1, std::vector<int>(n + 1, 0));
    std::vector<std::vector<int>> H2(m + 1, std::vector<int>(n + 1, 0));
    std::vector<std::vector<int>> L2(m + 1, std::vector<int>(n + 1, 0));
    std::vector<int> max_scores(thresholds.size(), 0);
    std::vector<std::pair<int, int>> end_pos(thresholds.size(), {-1, -1});

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            int score_diag = (s1[i - 1] == s2[j - 1]) ? H[i - 1][j - 1] + match : 0;

            int score_up = H[i - 1][j] + gap;

            int score_left = H[i][j - 1] + gap;

            H[i][j] = std::max({0, score_diag, score_up, score_left});

            if (H[i][j] == 0){
                H2[i][j] = 0;
                L1[i][j] = 0;
                L2[i][j] = 0;
            }
            else {
                if (H[i][j] == score_diag){
                    H2[i][j] = H2[i - 1][j - 1] + 1;
                    L1[i][j] = L1[i - 1][j - 1] + 1;
                    L2[i][j] = L2[i - 1][j - 1] + 1;
                }
                else if (H[i][j] == score_up){
                    H2[i][j] = H2[i-1][j];
                    L1[i][j] = L1[i-1][j] + 1;
                    L2[i][j] = L2[i-1][j];
                }
                else {
                    H2[i][j] = H2[i][j-1];
                    L1[i][j] = L1[i][j-1];
                    L2[i][j] = L2[i][j-1]+1;
                }
            }
            double similarity = static_cast<double>(H2[i][j]) / std::max(L1[i][j], L2[i][j]);
            if (H[i][j] == 0) {
                continue;
            }
            for (int k = 0; k < thresholds.size(); k++){
                double threshold = thresholds[k];
                if (similarity >= threshold && std::max(L1[i][j], L2[i][j]) >= max_scores[k]) {
                    max_scores[k] = std::max(L1[i][j], L2[i][j]);
                    end_pos[k] = {i, j};
                }
            }
        }
    }
    std::unordered_map<int, std::pair<int, int>> end_pos_map;
    std::vector<double> similarities(thresholds.size(), 0.0);
    for (int k = 0; k < thresholds.size(); k++){
        int matches=0;
        int alignment_length=0;
        int num_gaps1 = 0;
        int num_gaps2 = 0;

        int i = end_pos[k].first, j = end_pos[k].second;
        if (end_pos_map.find(end_pos[k].first*(n+1)+end_pos[k].second) != end_pos_map.end()){
            max_scores[k] = std::max(L1[end_pos[k].first][end_pos[k].second], L2[end_pos[k].first][end_pos[k].second]);
            similarities[k] = static_cast<double>(H2[end_pos[k].first][end_pos[k].second]) / std::max(L1[end_pos[k].first][end_pos[k].second], L2[end_pos[k].first][end_pos[k].second]);
            end_pos[k] = end_pos_map[end_pos[k].first*(n+1)+end_pos[k].second];
            continue;
        }
        while (i > 0 && j > 0 && H[i][j] > 0) {
            if (s1[i - 1] == s2[j - 1]) {
                i--; j--;
                matches++;
            } else if (H[i - 1][j] + gap == H[i][j]) {
                i--;
                num_gaps1++;
            } else {
                j--;
                num_gaps2++;
            }
        }
        alignment_length = matches + std::max(num_gaps1, num_gaps2);
        max_scores[k] = alignment_length;
        double similarity = alignment_length > 0 ? static_cast<double>(matches) / alignment_length : 0.0;
        similarities[k] = similarity;
        end_pos_map[end_pos[k].first*(n+1)+end_pos[k].second] = {i, j};
        // std::cout << "Alignment length: " << alignment_length << " Matches: " << matches << " Gaps: " << num_gaps1 << " " << num_gaps2 << " " << std::max(L1[end_pos[k].first][end_pos[k].second], L2[end_pos[k].first][end_pos[k].second]) << " " << H2[end_pos[k].first][end_pos[k].second] << "\n";
        end_pos[k] = {i, j};
    }
    std::vector<std::tuple<int, double, int, int>> res;
    for (int k = 0; k < thresholds.size(); k++){
        res.push_back({max_scores[k], similarities[k], end_pos[k].first, end_pos[k].second});
    }
    return res;
}

std::tuple<int, bool, int, int> levensthein_after_smith_waterman(const std::vector<int>& s1, const std::vector<int>& s2){
    // After calling the smith_waterman algorithm with different treshholds, I am using the approx. starting indices identified by the algorithm to find the longest length match, starting from those indices
    // These heuristics together reduce the search space enough for my algorithm to be O(n^2), as opposed to O(n^4).
    std::vector<std::tuple<int, double, int, int>> res=smith_waterman_80_similarity(s1, s2, 3, -2, {0.8, 0.9, 0.95});
    // for (auto r: res){
    //     std::cout << std::get<0>(r) << " " << std::get<1>(r) << " " << std::get<2>(r) << " " << std::get<3>(r) << "\n";
    // }
    int start_i=std::get<2>(res[0]);
    int start_j=std::get<3>(res[0]);   
    if (start_i == -1 || start_j == -1){
        return {0,0,0,0};
    }
    int m=s1.size()-start_i;
    int n=s2.size()-start_j;
    std::vector<std::vector<int>> dp(m+1, std::vector<int>(n+1, 0));
    for (int i=0; i<=m; i++){
        dp[i][0]=0;
    }
    for (int i=0; i<=n; i++){
        dp[0][i]=0;
    }
    int max_length=0;
    for (int i=1; i<=m; i++){
        for (int j=1; j<=n; j++){
            if (s1[start_i+i-1]==s2[start_j+j-1]){
                dp[i][j]=dp[i-1][j-1]+1;
            }
            else {
                dp[i][j]=std::max(dp[i-1][j], dp[i][j-1]);
            }
            if (static_cast<double>(dp[i][j])/std::max(i, j) >= 0.8 && std::max(i, j) >= max_length){
                max_length=std::max(i, j);
            }
        }
    }
    if (max_length < 30){
        // std::cout << 0 << " " << 0 << " " << 0 << "\n";
        // std::cout << 0 << " " << 0 << " " << 0 << " " << "\n";
        return {0,0,0,0};
    }
    // std::cout << max_length << " " << std::get<0>(res[1]) << " " << std::get<0>(res[2]) << "\n";
    double percentage_overlap_80=static_cast<double>(max_length)/std::max(s1.size(), s2.size());
    double percentage_overlap_90=static_cast<double>(std::get<0>(res[1]))/std::max(s1.size(), s2.size());
    double percentage_overlap_95=static_cast<double>(std::get<0>(res[2]))/std::max(s1.size(), s2.size());
    bool result=((percentage_overlap_80>0.6)&&(percentage_overlap_90>0.15));
    // std::cout << percentage_overlap_80 << " " << percentage_overlap_90 << " " << percentage_overlap_95 << " " << result << "\n";
    return {max_length, result, start_i, start_j};
}


std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2) {
    // std::ios_base::sync_with_stdio(false);
    // std::cin.tie(NULL);
    double start_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    // for (int i = 0; i < submission1.size(); i++) {
    //     std::cout << submission1[i] << " ";
    // }
    // std::cout << "\n";
    // for (int i = 0; i < submission2.size(); i++) {
    //     std::cout << submission2[i] << " ";
    // }
    // std::cout << "\n";
    // std::cout << "Submission Sizes: " << submission1.size() << " " << submission2.size() << "\n";
    std::pair<int, int> res1;
    std::pair<int, int> res2;
    int longest_match=0;
    int longest_non_exact_match=0;
    {
        // Moving this to a separate scope so that immediately once the scope ends, the suffix trees can be destructed, thus saving memory.
        SuffixTree tree1(submission1);
        res1=tree1.deterministic_check(submission2, std::vector<bool>(submission2.size(), false));
        // std::cout << "Deterministic Check results: " << res1.first << " ";
        SuffixTree tree2(submission2);
        res2=tree2.deterministic_check(submission1, std::vector<bool>(submission1.size(), false));
        // std::cout << res2.first << "\n";
        longest_non_exact_match=std::min(res1.second, res2.second);
        // std::cout << "Longest non-exact match: " << std::min(res1.second, res2.second) << "\n";
        longest_match=tree1.longest_perfect_match(submission2); 
        // tree2.longest_perfect_match(submission1);
    }
    std::tuple<int, bool, int, int> res3=levensthein_after_smith_waterman(submission1, submission2);
    // std::cout << std::get<0>(res3) << " " << std::get<1>(res3) << " " << std::get<2>(res3) << " " << std::get<3>(res3) << "\n";
    std::array<int, 5> result = {(std::get<1>(res3) || (longest_match >= 80 && longest_non_exact_match >= 90)), std::min(res1.first, res2.first), std::get<0>(res3), std::get<2>(res3), std::get<3>(res3)};
    double end_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    // std::cout << "Time taken: " << end_time-start_time << " Milliseconds\n";
    // For setting the flag, we are checking for either 60% of the length of the codes having 80% overlap and 15% of the length of the codes having 90% overlap, 
    // or exact matches of length >= 80 (along with an approx. match of length >= 90, with this "approx" match accounting for some degree of transpositions of code block).
    // This heuristic accounts for both the cases when a student copies a significant portion of another code but then modifies it (to a 20% extent), or when he copies one single function or so, and does not make many modifications.
    // std::cout << "Final result: " << result[0] << " " << result[1] << " " << result[2] << " " << result[3] << " " << result[4] << "\n";
    return result; // dummy return
    // End TODO
}
