def sqrt(a) :
    eps = 10.0
    for i in range(15):
        eps = eps / 10
    print(eps)
    xn = a
    xn1 = xn - (xn * xn - a) / (2 * xn)
    while (xn1 - xn) * (xn1 - xn) > eps * eps :
        xn = xn1
        xn1 = xn - (xn * xn - a) / (2 * xn)
    return xn1 
