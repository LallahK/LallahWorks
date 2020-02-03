import math

def prime_factorise(n) :
    prime = 1;
    tups = [];
    while n != 1 :
        prime = prime_greater(prime);
        count = 0;
        while (n % prime == 0) :
            n = (int) (n / prime);
            count += 1;
        if count != 0 :
            tup = (prime, count);
            tups.append(tup);
    return tups;
        
def prime_greater(n) :
    t = n + 1;
    count = 0;
    end = 0;
    while 1 :
        count = 0;
		end = sqrt(t + 1);
        for k in range(1, end):
            if (t % k == 0) :
                count = count + 1;
        if count == 2 :
            return t;
        else :
            t += 1;
              
