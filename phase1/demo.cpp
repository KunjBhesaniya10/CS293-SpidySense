#include <bits/stdc++.h>
using namespace std;
#define THRESHOLD 0.85
struct LCSelement{
    int element;
    int start_file1;
    int start_file2;
};

double similarity_score(int window_size, int matching_len){
    return ((double)matching_len)/window_size;
}

void Large_pattern(std::vector<LCSelement> &lcs){
    int max_window_size = 0;
    int idx1,idx2;
    for(int i=0; i< lcs.size(); i++){
        for(int j=i+1; j <lcs.size(); j++){
            int window_size = std::max(lcs[i].start_file1-lcs[j].start_file1+1,lcs[i].start_file2-lcs[j].start_file2+1);
            int matching_len = j-i+1;
            // std::cerr<<"window_size: "<<window_size<<" matching_len: "<<matching_len<<'\n';
            if(similarity_score(window_size,matching_len) >= THRESHOLD){
                // std::cerr <<window_size<<" "<<matching_len<<" "<<float(matching_len/window_size)<<'\n';
                if(max_window_size < window_size){
                    max_window_size = window_size;
                    idx1 = lcs[j].start_file1;
                    idx2 = lcs[j].start_file2;
                }
                // std::cerr<<"window_size: "<<window_size<<" matching_len: "<<matching_len<<'\n';
                // for(int k = i; k<=j; k++){
                //     std::cerr<<lcs[k].element<<" ";
                // }
                // std::cerr<<'\n';
        }
    }
    }
    std::cerr<<"max_window_size: "<<max_window_size<<'\n';
    std::cerr<<"idx1: "<<idx1<<" idx2: "<<idx2<<'\n';

}
void longestCommonSubsequence(std::vector<int> &X, std::vector<int> &Y) {
    int m = X.size();
    int n = Y.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (X[i - 1] == Y[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    std::vector<LCSelement> lcs;

    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcs.push_back({X[i - 1], i - 1, j - 1});
            --i;
            --j;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            --i;
        } else {
            --j;
        }
    }
    Large_pattern(lcs);
    std::cerr<<"lcs size: "<<lcs.size()<<'\n';
}

int main(){
    std::vector<int> a = {1,3,5,4,6,2,2};
    std::vector<int> b = {1,2,6,1,3,5,4};
    longestCommonSubsequence(a,b);
}