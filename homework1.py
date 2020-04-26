#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Sep 14 14:02:06 2019

@author: luojierong
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
df = pd.read_csv("russell_prices.txt", header=None, skiprows=2, sep=' ').iloc[:, :-1].T
df.columns = [f"asset_{i}" for i in range(df.shape[1])]
rtn = df.pct_change().dropna(how="all")


#1. Compute sample mean returns and sample covariance matrix.
mean_return = rtn.mean(axis=0).to_list()
covariance_matrix = rtn.cov().values
print(mean_return)
print(covariance_matrix)

#2. Implement the algorithm for mean-variance portfolio optimization problem described in "qp.pdf".  
#Here each asset can have weight between 0 and 1.  You can use Python, Java, C or C++.

#a.set the initial wieght of assets, i.e., all to the first one
w=[1]+[0]*946

#b. implement the algorithm decribed in "qp.pdf"
#define a function, input lamda, weight of portfolio, mean and covariance, and return a optimized weight of portfolio(list)
def optimization(lamda,weight,mean,covariance):
    l=weight.copy()
    
#exclude the trival condition
    if lamda==0:
        i=mean.index(max(mean))
        l[i]=1
        l[0]=0
        return l
    
#calculate the epsilon for each pair of asset, and the stop condition is set as: the decrease of target function is less than 0.001
    for i in range(len(mean)-1):
        for j in range(i+1,len(mean)):
            if l[i]==0 and l[j]==0:
                continue
            a=(covariance[i][i]+covariance[j][j]-2*covariance[i][j])*lamda
            b=-covariance[i][i]*l[i]+covariance[j][j]*l[j]
            sum_item=0
            for k in range(len(mean)):
                if k==i or k==j:
                    continue
                else:
                    sum_item+=(covariance[k][j]-covariance[k][i])*l[k]
            b=(b+sum_item)*2*lamda+(mean[i]-mean[j])
            if a!=0:
                eps=-b/(2*a)
            elif b<0:
                eps=min(1-l[j],l[i])
            else:
                eps=max(l[i]-1,-l[j])
            eps_low=min(1-l[i],l[j])
            eps_up=min(l[i],1-l[j])
            if eps>eps_up:
                eps=eps_up
            elif eps<(-1*eps_low):
                eps=-eps_low
            l[i]-=eps
            l[j]+=eps
            
    return l

#3. For lambda = 0, how many assets do you have in your optimal basket, and what risk (total variance) do you have?

#define a function which calculate the total variance of assets given a portfolio
def totalvariance(vector):
    result=0
    for i in range(len(vector)):
        for j in range(len(vector)):
            result+=covariance_matrix[i][j]*vector[i]*vector[j]
    return result

optimal_weight=optimization(0,w,mean_return,covariance_matrix)
the_asset=optimal_weight.index(max(optimal_weight))
print("If lamda=0, we only have one asset in my optimal basket,it is the {}th asset".format(the_asset+1))
the_total_variance=covariance_matrix[the_asset][the_asset]
print("The risk (total variance) I have is {}".format(the_total_variance))

#4. Plot an approximate risk (variance) v. return curve by running the optimization over different choices of lambda.
#   What is the smallest variance you see
#   in any of these optimizations?

#define a function which calculate the total return of assets given a portfolio        

def totalreturn(vector):
    res=0
    for i in range(len(vector)):
        res+=vector[i]*mean_return[i]
    return res
 
#Get value of "small" lamda       
lammda_1=np.linspace(0.06,0.08,10,endpoint=True)
#lammda=lammda.tolist()
x_axis=[]
y_axis=[]
#Compute the return and variance related to each lamda, and put them in lists
for item in lammda_1:
    l_var=totalvariance(optimization(item,w,mean_return,covariance_matrix))
    l_re=totalreturn(optimization(item,w,mean_return,covariance_matrix))
    
    x_axis.append(l_var)
    y_axis.append(l_re)
 #Get value of "not to small" lamda  
lammda_2=np.linspace(0.08,0.15,10,endpoint=True)

for item in lammda_2:
    l_var=totalvariance(optimization(item,w,mean_return,covariance_matrix))
    l_re=totalreturn(optimization(item,w,mean_return,covariance_matrix))

    x_axis.append(l_var)
    y_axis.append(l_re)
    
 #Get value of "medium" lamda     
lammda_3=np.linspace(0.15,0.3,6,endpoint=True)

for item in lammda_3:
    l_var=totalvariance(optimization(item,w,mean_return,covariance_matrix))
    l_re=totalreturn(optimization(item,w,mean_return,covariance_matrix))
    x_axis.append(l_var)
    y_axis.append(l_re)
    
 #Get value of "more than medium" lamda     
lammda_4=np.linspace(0.3,1,3,endpoint=True)

for item in lammda_4:
    l_var=totalvariance(optimization(item,w,mean_return,covariance_matrix))
    l_re=totalreturn(optimization(item,w,mean_return,covariance_matrix))
    
    x_axis.append(l_var)
    y_axis.append(l_re)
    
#Get value of "large" lamda     
lammda_4=np.linspace(1,100,10,endpoint=True)

for item in lammda_4:
    l_var=totalvariance(optimization(item,w,mean_return,covariance_matrix))
    l_re=totalreturn(optimization(item,w,mean_return,covariance_matrix))
    x_axis.append(l_var)
    y_axis.append(l_re)
    
l_var=totalvariance(optimization(500,w,mean_return,covariance_matrix))
l_re=totalreturn(optimization(500,w,mean_return,covariance_matrix))
x_axis.append(l_var)
y_axis.append(l_re)

#plot the Variance_return curve
plt.plot(x_axis,y_axis,color='b',linewidth=1.0)

plt.xlabel("Variance")
plt.ylabel("Return")
plt.title("Variance -Return curve")
plt.grid(color="k", linestyle=":")
plt.savefig('/Users/luojierong/Documents/application of programming/materials/Variance - Return curve')
plt.show()
plt.close()

#find the smallest variance(set lamda large enough)
print("the smallest variance I see is {}".format(l_var))
target_variance=(l_var+the_total_variance)/2

#5. Let s2_0 be the variance you reported in #3, and let s2_min be the variance
#you reported in #4.  Define s2_mid = 0.5(s2_0 + s2_min).  Estimate the value of
#lambda needed so that your optimal portfolio has variance s2_mid.

note=lammda_2[2]
print("The estimate value of lamda that my optimal portfolio has variance of s2_mid = 0.5(s2_0 + s2_min) is {}".format(note))