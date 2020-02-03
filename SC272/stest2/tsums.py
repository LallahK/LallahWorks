def time_sum(t1, t2) :
    """Takes 2 time parameters as integers in military format and
    returns their value as a string lead by zeros"""
    min1 = t1 % 100
    min2 = t2 % 100
    hours = (t1 // 100) + (t2 // 100) + (min1 + min2) // 60
    mins = (min1 + min2) % 60
    fT = hours * 100 + mins
    
    sFinalTime = str(fT)
    return "0" * (4 - len(sFinalTime)) + (sFinalTime)
