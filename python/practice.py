# -*- coding:utf-8 -*-
import os
path = os.getcwd()#获取当前路径
print(path)
all_files = [f for f in os.listdir(path )]#输出根path下的所有文件名到一个列表中
#对各个文件进行处理
print(all_files)
'''
class Screen(object):
    @property
    def width(self):
        return self._width
    @width.setter
    def width(self,value):
        self._width = value

    @property
    def height(self):
        return self._height
    @height.setter
    def height(self,value):
        self._height = value

    @property
    def resolution(self):
        return self._height * self._width
# 测试:
s = Screen()
s.width = 1024
s.height = 768
print('resolution =', s.resolution)
if s.resolution == 786432:
    print('测试通过!')
else:
    print('测试失败!')
'''
'''
class Student(object):
     count = 0
     def __init__(self, name):
         self.name = name
         Student.count +=  1
 # 测试:
 if Student.count != 0:
  print('测试失败1!')
  else:
     bart = Student('Bart')
     if Student.count != 1:
         print('测试失败2!')
     else:
         lisa = Student('Bart')
         if Student.count != 2:
             print('测试失败3!')
         else:
                print('Students:', Student.count)
                print('测试通过!')
'''
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
