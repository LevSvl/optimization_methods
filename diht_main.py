
# ДИХОТОМИЧЕСКИЙ МЕТОД

def f(x):
    return 2*x

if __name__ == '__main__':
    a = 10
    b = 50
    e = 0.01

    while (b-a >2*e):
        mid = (a+b)/2

        x1 = mid - e/2
        x2 = mid + e/2

        f1 = f(x1)
        f2 = f(x2)

        if (f1>f2):
            a = x1
        elif (f2>f1):
            b = x2
        else:
            break

    print(mid)



