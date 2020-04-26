from gurobipy import *
import numpy as np
from datetime import datetime

# set the partition for step size analysis as a global variable
M = 1000


# define the function of frank-wolfe method, taking gradient, current point, total asset and time as input
def optimization(delta_S, X, N, T):
    m = Model("4500_HW4")
    y = [m.addVar(lb=-GRB.INFINITY, ub=GRB.INFINITY, name=f"day{i}") for i in range(T)]
    m.setObjective(np.dot(delta_S, y), GRB.MAXIMIZE)
    m.addConstr(np.sum(y) == 0, "C0")
    for i in range(T):
        m.addConstr(X[i] + y[i] >= 0, f"C{i + 1}")
    m.optimize()
    return [v.x for v in m.getVars()]


# define main function, taking T, N, alpha and pi as input
def optimized_solve(T, N, alpha, pi):
    # initialize strategy
    X = np.array([N] + [0] * (T - 1))
    # X = np.array([N / T] * T)
    
    # initialize value of target function
    last_obj = 0
    for t in range(10000):
        # initialize gradient array as delta_S
        delta_S = np.zeros(T)
        # preprocessing of data using base list, reduce its time complexity
        # product basis
        pre_prod = np.cumprod(1 - alpha * np.power(X, pi))
        all_term = X * pre_prod
        # sum basis
        pre_sum_all_term = all_term.cumsum()
        # a certain part in derivative term
        deriv = -alpha * pi * np.array([np.power(X[i], pi - 1) if X[i] > 0 else 0 for i in range(T)])
        # renew value of target function
        last_obj = np.sum(all_term)

        # since derivative function is huge, we compute it separately in two parts
        for i in range(T):
            delta_S[i] = pre_prod[i]\
                + (pre_sum_all_term[-1] - pre_sum_all_term[i]) / (1 - alpha * X[i] ** pi) * deriv[i]
            if i > 0:
                delta_S[i] += X[i] * deriv[i] * pre_prod[i - 1]
            else:
                delta_S[i] += X[i] * deriv[i]

        # get our optimal direction
        y = np.array(optimization(delta_S, X, N, T))
        best_X = X
        # initialize maximal value in terms of step size choice
        best_obj = np.sum(X * pre_prod)
        for i in range(1, M + 1):
            # get a partition of [0,1] to M part
            cur_X = X + y * (i * 1. / M)
            # compute current value of target function
            pre_prod = np.cumprod(1 - alpha * np.power(cur_X, pi))
            obj = np.sum(cur_X * pre_prod)
            # renew the best step size
            if obj > best_obj:
                best_X = cur_X
                best_obj = obj
        # renew our current strategy
        X = best_X

        # stopping rule
        if best_obj-last_obj < 1e-7:
            break
        else:
            # renew current target function
            last_obj = best_obj
    return best_obj, X


# test the time to run the whole process
st = datetime.now()
setParam("LogToConsole", 0)

# compute our answer as required by assignment
ans = optimized_solve(T=20, N=10000, alpha=0.001, pi=0.5)
print("The optimal execution in each period is {}".format(ans[1]))
print("The optimal value we got is {}".format(ans[0]))

print("Time elapsed:", datetime.now() - st)
