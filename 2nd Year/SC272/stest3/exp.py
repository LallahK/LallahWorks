def exponential(x):
    val, fact = -1, 0;
    sn, snext = 0, 1;
    k = 1;
    while (sn != snext) :
        sn = snext;
        
        val = 1.0;
        fact = 1.0;
        
        for i in range(1, k + 1) :
            val = val*x;
            fact = fact * (i * 1.0);
        
        k += 1;
        snext = snext + (val)/(fact);
    return sn;
