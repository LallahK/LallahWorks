def final_vel(v,a,t) :
    return v + a * t
    
def final_dist(s,v,t,a) :
    return s + v*t + (1 / 2)*(a * t * t)
    
def vel_square(v,a,s) :
    return (v*v + 2*a*s)**(0.5)
