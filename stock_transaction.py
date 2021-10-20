# -*- coding: utf-8 -*-
"""
Created on Thu Sep 23 01:36:19 2021

@author: user
"""
T = int(input())

for t in range(T):
    N = int(input())
    buy = []
    sell = []
    stockprice = -1
    for n in range(N):
        str = input()
        strlist = str.split()
        share = int(strlist[1])
        price = int(strlist[-1])
        # 遇到買單
        if strlist[0] == 'buy':
            while len(sell) > 0:
                order = sell[0]
                if order[0] > price:
                    break
                # 成交
                dealno = min(share, order[1])
                stockprice = order[0]
                # 更新張數
                order[1] -= dealno
                share -= dealno
                # 委託單已結束
                if order[1] == 0:
                    del sell[0]
                if share == 0:
                    break
            # 如果還有剩餘要結算
            if share > 0:
                i = 0
                # 如果有一樣的要疊加                    
                while i < len(buy) and price < buy[i][0]:
                    i += 1
                if i < len(buy) and price == buy[i][0]:
                    buy[i][1] += share
                # 找插的位置
                else:
                    buy.insert(i, [price, share])
                # sorted(buy.append([price, share]), key=lambda l:l[0])

        # 處理賣
        else:
            while len(buy) > 0:
                order = buy[0]
                if order[0] < price:
                    break
                # 成交 
                dealno = min(share, order[1])
                stockprice = price
                # 更新張數
                order[1] -= dealno
                share -= dealno
                # 委託單已結束
                if order[1] == 0:
                    del buy[0]
                if share == 0:
                    break
            # 如果還有剩餘要結算
            if share > 0:
                i = 0
                # 如果有一樣的要疊加                    
                while i<len(sell) and price > sell[i][0]:
                    i+=1
                if i < len(sell) and price == sell[i][0]:
                    sell[i][1]+=share
                # 找插的位置
                else:
                    sell.insert(i, [price, share])

        # 處理列印
        if not sell: # 如果空了
            print('-', end=' ')
        else:
            print(sell[0][0], end=' ')
        if not buy:
            print('-', end=' ')
        else:
            print(buy[0][0], end=' ')
        if stockprice == -1:
            print('-')
        else:
            print(stockprice)
