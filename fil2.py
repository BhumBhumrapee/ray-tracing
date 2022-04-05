from functools import *


def minPathSum(A):
    m = len(A)
    n = len(A[0])

    sum_arr = [[float('inf') for i in range(n + 1)] for i in range(m + 1)]
    sum_arr[0][1] = 0
    sum_arr[1][0] = 0
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            sum_arr[i][j] = min(sum_arr[i][j - 1], sum_arr[i - 1][j]) + A[i - 1][j - 1]

    print(f"{len(sum_arr)}, {len(sum_arr[0])}")
    print(f"m: {m}, n: {n}")

    return sum_arr[m][n]


a = [
  [6, 4, 63],
  [14, 95, 20],
  [31, 85, 6]
]

print(minPathSum(a))