from math import sin, pi, log
import random

def gaussrand() :
  u = random.random()
  v = random.random()
  w = sin(2 * pi * v) * ((-2) * log(u)) ** (1 / 2)
  print('{0:.4f}'.format(w))
