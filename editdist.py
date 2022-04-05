def minDistance(word1: str, word2: str) -> int:
    n = len(word1)
    m = len(word2)
    dp = [[0 for i in range(m + 1)] for i in range(n + 1)]
    for i in range(n + 1):
        dp[i][0] = i
    for i in range(m + 1):
        dp[0][i] = i

    for i in range(1, n + 1):
        for j in range(1, m + 1):
            dp[i][j] = min(dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + (0 if word1[i] == word2[j] else 1))

    return dp[n][m]


word1 = "horse"
word2 = "ros"

print(minDistance(word1, word2))