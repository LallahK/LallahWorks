import nose
import math
import temperature

def test_freezing() :
  e = 10 ** (-15)
  assert (temperature.c2f(0) - 32) < e
  
def test_boiling() :
  e = 10 ** (-15)
  assert (temperature.c2f(100) - 212) < e
  
def test_body() :
  e = 10 ** (-15)
  assert (temperature.c2f(37) - 98.6) < e
  
def test_fever() :
  e = 10 ** (-15)
  assert (temperature.c2f(37 + 7/9) - 100) < e    
  
if __name__ == '__main__' :
  nose.runmodule()

