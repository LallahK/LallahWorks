import math

def polar_to_cartesian(r, theta, d) :
  rad = (math.pi / 180) * theta
  x = r * math.cos(rad)
  y = r * math.sin(rad)
  
  return ('({0:.' + str(d) + 'f}, {1:.' + str(d) + 'f})').format(x, y)
