def elapsed_minutes(time1,time2):
    t2 = int(time2)
    t1 = int(time1)
    min2 = t2 % 100
    min1 = t1 % 100
    
    return ( (t2 // 100) - (t1 // 100) )*60 + (min2 - min1)
    
