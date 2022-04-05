def unboundedKnapsack(k, arr):
    n = len(arr)
    arr.sort()
    dp = [[False for i in range(k + 1)] for i in range(n + 1)]
    for i in range(len(dp)):
        dp[i][0] = True
    for i in range(1, n + 1):
        for j in range(k + 1):
            if j - arr[i - 1] >= 0:
                dp[i][j] = dp[i][j] or dp[i - 1][j] or dp[i - 1][j - arr[i - 1]]
            if j - arr[i - 1] >= 0 and dp[i][j - arr[i - 1]] == True:
                dp[i][j] = True
    for l in dp:
        print(l)

    cur_max = 0
    for i in range(1, n + 1):
        for j in range(k + 1):
            if dp[i][j] == True and j > cur_max:
                cur_max = j

    return cur_max

inp = [9, 4 ,4 ,9, 4, 9, 9, 9, 9]
k = 10

print(unboundedKnapsack(k, inp))