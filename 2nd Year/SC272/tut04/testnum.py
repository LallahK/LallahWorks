import validUsnum
import sys

if __name__ == '__main__' :
    SUnum = int(sys.argv[1])
    print(validUsnum.is_valid_usnum(SUnum))
