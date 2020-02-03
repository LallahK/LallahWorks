def sort3(a,b,c):
    mini = min(a,b,c)
    maxi = max(a,b,c)
    mid = a + b + c - mini - maxi
    return mini, mid, maxi
