def find_min(L, i) :
    iMIN = i
    for x in range(i, len(L)) :
        if (L[x] < L[iMIN]) :
            iMIN = x
    return iMIN
