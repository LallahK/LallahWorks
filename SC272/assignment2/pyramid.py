def volume(A,h,d):
    V = (A * h)/3.0
    dec = int(d)
    strin = "{:." + str(dec) + "f}"
    return strin.format(V)
