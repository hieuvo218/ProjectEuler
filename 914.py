import math

max = 0
for i in range (541190000, 541200000):
    for j in range (math.floor(math.sqrt(2*pow(10, 18)-1 - i*i)), 0, -1):
        if math.gcd(i, j) == 1 and (i%2==0 or j%2==0):
            break
    if max < i*(j-i):
        max = i*(j-i)

print(max)
