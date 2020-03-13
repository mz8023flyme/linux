# -*- coding:utf-8 -*-
''' 匿名函数
L = list(filter(lambda x :x % 2 == 1, range(1, 20)))
print(L)
'''
''' 按某项进行排序i
L = [('Bob', 75), ('Adam', 92), ('Bart', 66), ('Lisa', 88)]
def by_name(t):
    return t[1]
L2 = sorted(L, key=by_name,reverse=True)
print(L2)
'''
''' 累乘
from functools import reduce
def prod(L):
    def fn (x,y):
        return x * y
    return reduce(fn,L)
print('3 * 5 * 7 * 9 =', prod([3, 5, 7, 9]))
if prod([3, 5, 7, 9]) == 945:
    print('测试成功!')
else:
    print('测试失败!')
'''
''' map 运算将首字母大写其余小写
def normalize(name):
    return name.capitalize()
    
# 测试:
L1 = ['adam', 'LISA', 'barT']
L2 = list(map(normalize, L1))
print(L2)
'''

'''列表生成式
L1 = ['Hello', 'World', 18, 'Apple', None]
L2 = [s.lower() for s in L1 if isinstance(s,str) == True]
print(L2)
'''
'''遍历数组且找出最大最小值
def findMinAndMax(L):
    if len(L) >= 1:
        MIN = MAX = L[0]
        for i in L:
            if i > MAX:
                MAX = i
            elif i < MIN:
                MIN = i
        return (MIN,MAX)
    return (None, None)
'''
''' 切片去除首位空格
def trim(s):
    while s[0:1]==' ':
        s=s[1:]
    while s[(len(s)-1):len(s)]==' ':
        s=s[:-1]
    return s
'''

'''
def product(*argc):
    sum = 1
    for n in range(len(argc)):
        sum = sum * argc[n]
    print(sum)

product(1,2,4,5)
'''

''' 打印小数
s1 = 72
s2 = 85
r = s1/s2
r = r*100
print("小明成绩提升了%.2f %%"%r)
'''

''' 字符转换
n1 = 255
n2 = 1000

print('%s'%str(hex(n2)))
'''
