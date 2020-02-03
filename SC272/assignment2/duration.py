def end_time():
    start = input("Start time:")
    startT = int(start)
    duration = int(input("Duration:"))
    
    hours = (duration // 60) * 100
    tim = (startT + hours)
    duration = duration % 60
    mins = tim % 100 + duration
    tim = tim + (mins // 60) * 100 + (mins - (mins // 60) * 60) - tim % 100
    strTime = str(tim % 2400)
    strTime = '0'*(4 - len(strTime)) + strTime
    return strTime

