def find_max(L, i) :
    iMAX = i;
    for x in range(i, len(L)) :
        if (L[iMAX] < L[x]) :
            iMAX = x
    return iMAX
