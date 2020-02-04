from math import log, sin, pi

def mercator(lambd, longitude, latitude):
  x = lambd - longitude;
  rad = (pi / 180) * latitude
  y = (1 / 2) * log((1 + sin(rad))/(1 - sin(rad)))
  return x, y
