#include <array>
#include <iostream>
#include <span>
#include <vector>
#include <cmath>
#include<unordered_map>
#include<algorithm>
#include<unordered_set>


// -----------------------------------------------------------------------------

// You are free to add any STL includes above this comment, below the --line--.
// DO NOT add "using namespace std;" or include any other files/libraries.
// Also DO NOT add the include "bits/stdc++.h"

// OPTIONAL: Add your helper functions and data structures here
//this is used to generate polynomial hashes.
std::size_t polynomial_hash(const std::vector<int>& vec, int base = 31, int mod = 1e9 + 9) {
    std::size_t hash_value = 0;
    std::size_t power = 1;
    for (const auto& num : vec) {
        hash_value = (hash_value + num * power) % mod;
        power = (power * base) % mod;
    }
    return hash_value;
}
//winnowing function is used to check if there is a significant match this function returns number of matched elements in first vector to second vector.
//for smaller lengths i am taking k gram length 5 and doing directly inside the function total winnowing process.
int winnowing_for_smaller_elements(const std::vector<int>& vector1, const std::vector<int>& vector2){
    int window_size=5;
    std::vector<int>winnowingmap1;
    std::vector<int>winnowingmap2;
    for(int i=0;i+5<vector1.size();i++)
{
std::vector<int>subseq(vector1.begin()+i,vector1.begin()+i+5);
winnowingmap1.push_back(polynomial_hash(subseq));
}
for(int i=0;i+5<vector2.size();i++){
    std::vector<int>subseq(vector2.begin()+i,vector2.begin()+i+5);
    winnowingmap2.push_back(polynomial_hash(subseq));}
std::vector<int> minwinnowingmap1;
std::vector<int> minwinnowingmap2;
for (std::size_t i = 0; i + 5 <= winnowingmap1.size(); ++i) {
    int min_value = *std::min_element(winnowingmap1.begin() + i, winnowingmap1.begin() + i + 5);
    minwinnowingmap1.push_back(min_value);       
    
}
for(int i=0;i+5<winnowingmap2.size();i++){
    int min_value = *std::min_element(winnowingmap2.begin() + i, winnowingmap2.begin() + i + 5);
    minwinnowingmap2.push_back(min_value);  
}
std::unordered_map<std::size_t, int> hash_map;
for(int i=0;i<minwinnowingmap1.size();i++){ 
        hash_map[minwinnowingmap1[i]]++;
    }
    for(int i=0;i<minwinnowingmap2.size();i++){
        if(hash_map.find(minwinnowingmap2[i])!=hash_map.end()&&hash_map[minwinnowingmap2[i]]>0){
           
            hash_map[minwinnowingmap2[i]]--;
        }
    }
    int counthere=0;
    for(auto it:hash_map){
        counthere+=it.second;
    }
    
    return minwinnowingmap1.size()-counthere; // No matching subsequences found

}
//for larger cases to decrease time i calculated k gram hashes and min sliding window hashes in the main function(int main) and passed to this function.
//and then i am passing minimum sliding window hashes to this fuction to significantly decrease time.
//this function returns number of matched elements in first vector to second vector.
int winnowing(const std::vector<int>& vector1, const std::vector<int>& vector2) {
    int window_size = 10;
    std::unordered_map<std::size_t, int> hash_map;
    
  
    //i am using a vector map to store the values of vector1 in hashmap.
    for(int i=0;i<vector1.size();i++){ 
        hash_map[vector1[i]]++;
    }
    //here i am checking if there is a match in the vector2 with the vector1 and if there is a match i will decrease the value in he hashmap by 1.
    for(int i=0;i<vector2.size();i++){
        if(hash_map.find(vector2[i])!=hash_map.end()&&hash_map[vector2[i]]>0){
           
            hash_map[vector2[i]]--;
        }
    }
    int counthere=0;
    //finally i am counting number of unmatched.
    for(auto it:hash_map){
        counthere+=it.second;
    }
    
    return vector1.size()-counthere; // No matching subsequences found
}

std::array<int, 5> match_submissions(std::vector<int> &submission1, 
        std::vector<int> &submission2) {
    // TODO: Write your code here
    
    std::array<int, 5> result = {0, 0, 0, 0, 0};
    std::vector<bool> visitedin1(submission1.size(), false);
    std::vector<bool> visitedin2(submission2.size(), false);
    std::unordered_map<std::size_t, std::vector<int>> hash_map;
    // Compute hashes for all subsequences of length 10 in submission1(contigious subseq)
    for (std::size_t i = 0; i + 10 <= submission1.size(); ++i) {
        std::vector<int> subsequence(submission1.begin() + i, submission1.begin() + i + 10);
        std::size_t hash = polynomial_hash(subsequence);
        hash_map[hash].push_back(i);
    }
    int subseq_length = 10;
    // here i am computing hashes for subsequences of length 10 in submission2(contigious subseq)
    //and checking if there is a same hash in the submission1 hashes
    //if there is a subsequence match i am filling visitedin1 vector with true for matched 10 indices in both vectors visitedin1 and visitedin2.
    //i am using visitedin1 and visitedin2 because we should not do overcounting as mentioned in question.
    //here all the multiples of 10 subsequences will be captured and subsequences of lenth 13/14/15...will not be matched.for that i am doind in next part.
    for (int i = 0; i+subseq_length <= submission2.size(); ++i) {
       if(!visitedin2[i]){ std::vector<int> subseq(submission2.begin() + i, submission2.begin() + i + subseq_length);
        std::size_t hash_value = polynomial_hash(subseq);
        
        if (hash_map.find(hash_value) != hash_map.end()) {
            for (int start_index : hash_map[hash_value]) {
                if (!visitedin1[start_index] &&!visitedin1[start_index+subseq_length-1]&& !visitedin2[i]&&!visitedin2[i+subseq_length-1]) {
                    for(int pp=0;pp<subseq_length;pp++){visitedin1[start_index+pp] = true;
                    visitedin2[i+pp] = true;}
                    // Increment the count of matched subsequences
                }
            }
        }
    }}
    
//take a 15 length subseq we want to match all the 15 but from before step we only match first 10 and the next 5 will be left .
//now the basic idea is to travese the submission1 and submission2 reversely and match.
//them in reverse travesal visitedin1 and visitedin2 will be false for the wanted match so we can match it here.
//so i reversed all submission1,submission2,visitedin1 and visitedin2.
std::vector<int> reversed_submission1(submission1.rbegin(), submission1.rend());
std::vector<int> reversed_submission2(submission2.rbegin(), submission2.rend());
std::unordered_map<std::size_t, std::vector<int>> newhashmap;
std::vector<bool> revvisitedin1(visitedin1.rbegin(), visitedin1.rend());
std::vector<bool> revvisitedin2(visitedin2.rbegin(), visitedin2.rend());

//i am computing hashes for the reverse subsequences.
for (std::size_t i = 0; i + 10 <= reversed_submission1.size(); ++i) {
    std::vector<int> subsequence(reversed_submission1.begin() + i, reversed_submission1.begin() + i + 10);
    std::size_t hash = polynomial_hash(subsequence);
    newhashmap[hash].push_back(i);
}

//now i am checking if a match is there.
//here there is a different case take a subseq of length 10 A in subm1 
//and there is other subseq of length 10 in subm1.here A and B are in such
//a way that last 4 numbers of a are first 4 numbers of B and the next 6 numbers after A are last 6 numbers of B.
//If we naively traverse like before we count this case which should not be counted.
//so before making visited true. I am checking if this case is excluded by traversing forward and checking all the between hashes are matched.
for (int i = 0; i+subseq_length<= submission2.size() ; ++i) {
    if(!revvisitedin2[i]){
    std::vector<int> subseq(reversed_submission2.begin() + i, reversed_submission2.begin() + i + subseq_length);
    std::size_t hash_value = polynomial_hash(subseq);
    if(newhashmap.find(hash_value)!=newhashmap.end()){
        for(int start_index:newhashmap[hash_value]){
            if(!revvisitedin1[start_index]&&!revvisitedin2[i]){
                int startindexin1=start_index;
                int startindexin2=i;
                while(startindexin1 < reversed_submission1.size() && startindexin2 < reversed_submission2.size() && startindexin1 + subseq_length - 1 < reversed_submission1.size() && startindexin2 + subseq_length - 1 < reversed_submission2.size()){
                    std::vector<int> subseq1(reversed_submission2.begin() + startindexin2, reversed_submission2.begin() + startindexin2 + subseq_length);
                    std::size_t hash_valuenow = polynomial_hash(subseq1);
                     if(revvisitedin1[startindexin1]&&revvisitedin2[startindexin2]){
                        
                        for(int j=start_index;j<=startindexin1;j++){revvisitedin1[j]=true;}
                        for(int j=i;j<=startindexin2;j++){revvisitedin2[j]=true;}
                        //here found match should modify visited
                        ;break;
                     }
                     if(revvisitedin1[startindexin1]&&!revvisitedin2[startindexin2]){break;}
                     if(!revvisitedin1[startindexin1]&&revvisitedin2[startindexin2]){break;}
                     if(newhashmap.find(hash_valuenow)!=newhashmap.end()){
                            startindexin1+=1;
                            startindexin2+=1;
                     }

                }
            }
        }
    }
    }
}

int count4=0;


//finally i am counting total number of exact matches by counting total visited indexes in revvisitedin1.
for (bool visited : revvisitedin1) {
    if (visited) {
        count4++;
    }
}

//for approximate matching i am using "winnowing" which is one of the idea in actual moss.
//i read about winnowing from this the algorithm and why the algorithm produces good results is expalained in 
//this under section winnowing and expected density "https://theory.stanford.edu/~aiken/publications/papers/sigmod03.pdf"
//The process starts by breaking the sequence into small overlapping chunks called shingles. 
//Each shingle is then converted into a numeric representation using hashing.
// Next, a sliding window moves across the sequence of hashes,
// and only the smallest hash (known as the min-hash) from each window is selected as part of the fingerprint.
// This selection process reduces the total data and helps focus on the sequence's most representative features.
//
//first i am computing all the hashes of all the k grams in submission1 and submission2 and store them in winnowinmap1 and winnowingmap2.
//next i am using a sliding window of size 10 and storing the minimum hash value in the window in minwinnowingmap1 and minwinnowingmap2.


// Using winnowing to find the longest matching subsequence between two sequences

// Step 1: First, we generate hashes for all subsequences of length 10 using a polynomial hash function.
//         This gives each 10-character chunk of the sequence a unique identifier.

// Step 2: Next, we slide a window of size 10 over each sequence to find the smallest hash value in each window.
//         We store these minimum values in vectors `minWinnowingMap1` and `minWinnowingMap2` for each sequence.
//         By keeping only the smallest values, we make it faster and easier to compare sequences later on.

// Step 3: Now, we go through each subsequence length (i) in `minWinnowingMap1` and `minWinnowingMap2`
//         to look for matches. This helps us spot similar parts between the two sequences.

// Step 4: If we find enough matches between `minWinnowingMap1` and `minWinnowingMap2`, we consider the sequences
//         to have an approximate match. 

// Step 5:i am using the "winnowing" fuction to return number of unmatched elements in the first vector passed to it.

// Step 6: Finally, we use binary search to zero in on the longest matching subsequence length.
//         By adjusting the subsequence length we're comparing, binary search helps us quickly find the longest match.

// Overall, this method identifies approximate matches by comparing only key hash values, 
// making it a lot faster to find the longest shared subsequence between sequences.




//winnowingmap1 one for calculating hashes of all k grams from submission1 and storing similarly winnowingmap2 for submission2. 
//minwinnowingmap1 and minwinnowingmap2 are used to store the minimum hash values of the k grams in the window of 10.
std::vector<int>winnowingmap1;
std::vector<int>winnowingmap2;
for(int i=0;i+10<submission1.size();i++)
{
std::vector<int>subseq(submission1.begin()+i,submission1.begin()+i+10);
winnowingmap1.push_back(polynomial_hash(subseq));
}
for(int i=0;i+10<submission2.size();i++){
    std::vector<int>subseq(submission2.begin()+i,submission2.begin()+i+10);
    winnowingmap2.push_back(polynomial_hash(subseq));   
}
std::vector<int> minwinnowingmap1;
for (std::size_t i = 0; i + 10 <= winnowingmap1.size(); ++i) {
    int min_value = *std::min_element(winnowingmap1.begin() + i, winnowingmap1.begin() + i + 10);
    minwinnowingmap1.push_back(min_value);
}
std::vector<int> minwinnowingmap2;
for (std::size_t i = 0; i + 10 <= winnowingmap2.size(); ++i) {
    int min_value = *std::min_element(winnowingmap2.begin() + i, winnowingmap2.begin() + i + 10);
    minwinnowingmap2.push_back(min_value);
}

int final_longest_approxmatch_length=0;int final_start_index_in_submission1=0;int final_start_index_in_submission2=0;


int first=30;
int last=minwinnowingmap1.size();
int mid=(first+last)/2;

while(first<last){
    
    bool found=false;
    int i=mid;
    
       if(i>1000){
        for(int p=0;p+i<=minwinnowingmap1.size();p=p+50){
        std::vector<int>subseq1(minwinnowingmap1.begin()+p,minwinnowingmap1.begin()+p+i);
        for(int q=0;q+i<=minwinnowingmap2.size();q=q+50){
            
           std::vector<int>subseq2(minwinnowingmap2.begin()+q,minwinnowingmap2.begin()+q+i);
            int numofmatches=winnowing(subseq1,subseq2);
            double similarity=numofmatches*100.0/(subseq1.size()+subseq2.size()-numofmatches);
            if(similarity>80){
                
                found=true;
            final_longest_approxmatch_length=i;final_start_index_in_submission2=p;final_start_index_in_submission1=q;
              break;
            }
        }if(found){break;}
    }
       }
       else if(i>150){for(int p=0;p+i<=minwinnowingmap1.size();p=p+10){
        std::vector<int>subseq1(minwinnowingmap1.begin()+p,minwinnowingmap1.begin()+p+i);
        for(int q=0;q+i<=minwinnowingmap2.size();q=q+10){
            
           std::vector<int>subseq2(minwinnowingmap2.begin()+q,minwinnowingmap2.begin()+q+i);
            int numofmatches=winnowing(subseq1,subseq2);
            double similarity=numofmatches*100.0/(subseq1.size()+subseq2.size()-numofmatches);
            if(similarity>85){
                
                found=true;
            final_longest_approxmatch_length=i;final_start_index_in_submission2=p;final_start_index_in_submission1=q;
              break;
            }
        }if(found){break;}
    }}
    else{
        for(int p=0;p+i<=submission1.size();p=p+10){
        std::vector<int>subseq1(submission1.begin()+p,submission1.begin()+p+i);
        for(int q=0;q+i<=submission2.size();q=q+10){
            
           std::vector<int>subseq2(submission2.begin()+q,submission2.begin()+q+i);
            int numofmatches=winnowing(subseq1,subseq2);
            double similarity=numofmatches*100.0/(subseq1.size()+subseq2.size()-numofmatches);
            if(similarity>92){
                
                found=true;
            final_longest_approxmatch_length=i;final_start_index_in_submission2=p;final_start_index_in_submission1=q;
              break;
            }
        }if(found){break;}
    }

    }

    if(found){
        first=mid+1;
    }
    else {
        last=mid;
    }
    mid=(first+last)/2;
}

//final plag check
result[0]=0;
//i am dividing int five cases that are below 200 tokens below 500 tokens,below 1000 tokens below 2000 tokens above 2000 tokens for treshholds for final plag check.
//submissions below 200 tokens 60% winnowing or 85 %exact match
//submissions below 500 tokens 40% winnowing or 75% exact match
//submissions below 1000 tokens 35% winnowing or 65% exact match
//submissions below 2000 tokens 30% winnowing or 50% exact match
//submissions above 2000 tokens 25% winnowing or 42% exact match 
if(submission1.size()==0||submission2.size()==0){
    result[0]=0;
}
else if(submission1.size()<200||submission2.size()<200){
    
int finalmatches=winnowing_for_smaller_elements(submission1,submission2);
double similarity=finalmatches*100.0/(submission1.size()+submission2.size()-finalmatches);
int minimumofsubmissions=std::min(submission1.size(),submission2.size());
if(similarity>60||count4>0.85*minimumofsubmissions){
    result[0]=1;

}

}
else if(submission1.size()<500||submission2.size()<500){
    int finalmatches=winnowing(minwinnowingmap1,minwinnowingmap2);
    double similarity=finalmatches*100.0/(minwinnowingmap1.size()+minwinnowingmap2.size()-finalmatches);
    int minimumofsubmissions=std::min(submission1.size(),submission2.size());
    if(similarity>40||count4>0.75*minimumofsubmissions){
        result[0]=1;
    }

}
else if(submission1.size()<1000||submission2.size()<1000){
int finalmatches=winnowing(minwinnowingmap1,minwinnowingmap2);
    double similarity=finalmatches*100.0/(minwinnowingmap1.size()+minwinnowingmap2.size()-finalmatches);
    int minimumofsubmissions=std::min(submission1.size(),submission2.size());
    if(similarity>30||count4>0.65*minimumofsubmissions){
        result[0]=1;
    }

}
else if(submission1.size()<2000||submission2.size()<2000){
    int finalmatches=winnowing(minwinnowingmap1,minwinnowingmap2);
    double similarity=finalmatches*100.0/(minwinnowingmap1.size()+minwinnowingmap2.size()-finalmatches);
    int minimumofsubmissions=std::min(submission1.size(),submission2.size());
    if(similarity>27||count4>0.5*minimumofsubmissions){
        result[0]=1;
    }

}
else{
int finalmatches=winnowing(minwinnowingmap1,minwinnowingmap2);
    double similarity=finalmatches*100.0/(minwinnowingmap1.size()+minwinnowingmap2.size()-finalmatches);
    int minimumofsubmissions=std::min(submission1.size(),submission2.size());
    if(similarity>24||count4>0.42*minimumofsubmissions){
        result[0]=1;
    }
 
}














result[1]=count4;
result[2]=final_longest_approxmatch_length;
result[3]=final_start_index_in_submission2;
result[4]=final_start_index_in_submission1;
    

    
    return result; // dummy return
    // End TODO
}