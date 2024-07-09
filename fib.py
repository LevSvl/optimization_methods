

def fib(n):
	if n == 0: 
		return 1
	elif n == 1:
		return 1
	else:
		return fib(n - 1) + fib(n - 2)

def f(x): 
	return 2 * x

a,b = 5.00000, 100.0000000

n = int(input("N = "))
	
fibArr = []

for i in range(0,n+1,1):
	fibArr.append(fib(i))
print(fibArr)

x1 = a + ((fibArr[n - 1] / fibArr[n]) * (b - a))
x2 = b - ((fibArr[n - 1] / fibArr[n]) * (b - a))

xOpt = x1

while (n>2):
    f1 = f(x1)
    f2 = f(x2)

    if (f1 > f2):
        xOpt = x2
        b = x1
        x2 = b - (fibArr[n - 2] / fibArr[n - 1]) * (b - a)
    elif (f1 < f2):
        xOpt = x1
        a = x2
        x1 = a + (fibArr[n - 2] / fibArr[n - 1]) * (b - a)
    else:
        xOpt = x1

    n = n - 1

print(f(xOpt))