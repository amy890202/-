# -*- coding: utf-8 -*-
"""
Spyder Editor
This is a temporary script file.
"""
import math
from scipy.stats import norm
import numpy as np
import matplotlib.pyplot as plt

def blscall(S,K,T,r,vol):#K履約價
    d1 = (math.log(S/K)+(r+vol*vol/2)*T)/(vol*math.sqrt(T))
    d2 = d1-vol*math.sqrt(T)
    call = S*norm.cdf(d1)-K*math.exp(-r*T)*norm.cdf(d2)
    return call

def MCcall(S,K,T,r,vol,N):
    dt = T/N
    St = np.zeros((N+1))
    St[0]=S
    for i in range(N):
        St[i+1] = St[i]*math.exp((r-0.5*vol*vol)*dt+vol*np.random.normal()*math.sqrt(dt))
    return St

def BisectioniBLS(S,K,T,r,call):#用bisection求vol
    left = 0.00000001
    right = 1
    while(right-left>0.00001):
        middle = (left+right)/2
        if((blscall(S,K,r,T,middle)-call)*(blscall(S,K,r,T,left)-call)<0):
            right = middle
        else:
            left = middle
    return (left+right)/2

S = 50
K = 40
T = 2
r = 0.08
vol = 0.2
print('blscall:',blscall(S,K,T,r,vol))

N=100
M=1000
mcresult = MCcall(S,K,T,r,vol,N)


def calcall_plot(M,N,K):
    call = 0
    for i in range(M):
        Sa = MCcall(S,K,T,r,vol,N)
        plt.plot(Sa)
        if(Sa[-1]-K>0):
            call += (Sa[-1]-K)
    plt.show()
    return call/M*math.exp(-r*T)
print('MCcall',calcall_plot(M,N,K))


def calcall(M,N,K):
    call = 0
    for i in range(M):
        Sa = MCcall(S,K,T,r,vol,N)
        #plt.plot(Sa)
        if(Sa[-1]-K>0):
            call += (Sa[-1]-K)
    plt.show()
    return call/M*math.exp(-r*T)

def calavgdef(M,N,K):
    difsum = 0
    for i in range(10):
        bls = blscall(S,K,T,r,vol)
        cal = calcall(M,N,K)
        difsum += abs(bls-cal)
    return difsum/10
print('Monte Carlo Simulationc和公式解的平均誤差:',calavgdef(M,N,K))


    

Na = [10, 100,1000]
Ma = [10, 100,1000]


#%%

for a in Na:
    for b in Ma:
        print('N:',a,'M:',b)
        #tmp = calcall(b,a,K)
        print(calavgdef(b,a,K))#abs(tmp - blscall(S,K,T,r,vol))
#%%
        
S = 19393.16
r = 0.00825
T = 10/252#一年252個交易日
call =[655,585,499,434,372,306,248,199,156,117,86,60,41,28.5,20.5,14.5,10]
Ka = np.arange(15800,17500,100)
vola = np.zeros(len(Ka))
for i in range(len(Ka)):
    vola[i] = BisectioniBLS(S,Ka[i],T,r,call[i])
    print('K:',Ka[i],'vol:', vola[i])
plt.plot(Ka,vola)#測微笑曲線

    
