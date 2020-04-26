import math
def maximizeRevenue(P0, alpha, pi, N, T):
    # calculate decay function
    decay = [1 - alpha * math.pow(i, pi) for i in range(N + 1)]
    
    # calculate maximal value for day T - 1
    dp = [decay[i] * i for i in range(N + 1)]

    # calculate maximal value from day T - 2 back to day 0
    for t in range(T - 2, -1, -1):
        for k in range(N, -1, -1):
            dp[k] = max([decay[i] * (i + dp[k - i]) for i in range(k + 1)])

    # calculate optimal value for whole strategy
    return P0 * dp[N]

ans = maximizeRevenue(1, 1e-3, 0.5, 10000, 20)
print(ans)
