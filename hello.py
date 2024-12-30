import math

a = 1
b = 2
sum = 0

for i in range (3, 10**12+1):
    # a and b to store two previous numbers
    if i % 2 == 0:
        c = 2*a
    else:
        c = a - 3*b
    a = b
    b = c

print(c)