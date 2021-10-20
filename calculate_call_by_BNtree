# -*- coding: utf-8 -*-
"""
Created on Wed Oct 20 19:19:27 2021

@author: user
"""
import math
import numpy as np

def BNTcall(S,T,r,K,vol,N):#建樹
    dt = T/N
    u = math.exp(vol*math.sqrt(dt))
    d = math.exp(-vol*math.sqrt(dt))
    p = (math.exp(r*dt)-d)/(u-d)
    priceT = np.zeros((N+1,N+1))
    probT = np.zeros((N+1,N+1))
    priceT[0][0] = S
    probT[0][0] = 1
    for c1 in range(N):
        priceT[0][c1+1] = priceT[0][c1]*u
        for r1 in range(c1+1):
            priceT[r1+1][c1+1] = priceT[r1][c1]*d  #[r1+1][c1+1](右下角)是自己*d      
    for c1 in range(N):#probability tree
        for r1 in range(c1+1):
            probT[r1][c1+1] += probT[r1][c1]*p
            probT[r1+1][c1+1] += probT[r1][c1]*(1-p)
    call = 0
    for r1 in range(N+1):
        if(priceT[r1][N]>K):
            call += (priceT[r1][N] - K)*probT[r1][N]#獲利率*獲利量 算期望值
    return call*math.exp(-r*T)#折現
print(BNTcall(50,2,0.08,40,0.2,1000))
