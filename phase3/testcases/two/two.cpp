#include <iostream>
#include <vector>
#include <string>

int longestCommonSubsequence(const std::string &text1, const std::string &text2) {
    int m = text1.size();
    int n = text2.size();
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}

int main() {
    std::string text1 = "ABCBDAB";
    std::string text2 = "BDCAB";
    int length = longestCommonSubsequence(text1, text2);
    std::cout << "Length of Longest Common Subsequence: " << length << std::endl;
    return 0;
}