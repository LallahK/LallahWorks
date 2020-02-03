import math

def category(m) :
    mag = (round(m * 10)) / 10.0
    if (mag < 3.0) : return "micro"
    elif ((mag >= 3) and (mag < 4)) : return "minor"
    elif ((mag >= 4) and (mag < 5)) : return "light"
    elif ((mag >= 5) and (mag < 6)) : return "moderate"
    elif ((mag >= 6) and (mag < 7)) : return "strong"
    elif ((mag >= 7) and (mag < 8)) : return "major"
    else : return "great"
