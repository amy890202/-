# -*- coding: utf-8 -*-
"""
Created on Thu Sep 30 19:46:11 2021
@author: user
"""
total_debt = int(input("貸款總額: (單位:萬)"))*10000
time_duration = int(input("貸款期限: (單位:月)"))
time_allow = int(input("寬限期: (單位:年)")) *12
pay_time = time_duration - time_allow
interest_rate = 0.0133
month_rate = interest_rate/12
#本期平均攤還
start_interest = round(total_debt*month_rate)
tmp =  (((1+month_rate)**pay_time )*month_rate)/(((1+month_rate)**pay_time) -1)
interest_principle = round(total_debt * tmp)
print('本息平均攤還:' ,interest_principle )


for i in range(time_allow):
    print(0,',',start_interest,',',start_interest)
debt = total_debt
for i in range(pay_time):
    interest = round(debt * month_rate)
    principle = interest_principle -  interest
    print( principle, ',',interest , ',', principle+interest)
    debt = debt - principle




#本金平均攤還


avg_principle = round(total_debt / pay_time)

print('本金平均攤還:',round(total_debt * month_rate)+avg_principle)


for i in range(time_allow):
     print(0,',',start_interest,',',start_interest)
debt = total_debt
for i in range(pay_time):
    interest = round(debt * month_rate)
    total_pay = avg_principle+interest
    debt = debt - avg_principle
    print( avg_principle, ',',interest , ',', total_pay)
