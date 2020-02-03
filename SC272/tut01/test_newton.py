import nose
import newton

def test_final_vel() :
  assert newton.final_vel(10, 3, 2) == 16

def test_final_dist() :
  assert newton.final_dist(10, 2, 3, 4) == 34
  
if __name__ == '__main__':
  nose.runmodule()
