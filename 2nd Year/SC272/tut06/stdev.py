def stddev(vals):
    sqsums = 0;
    sums = 0;
    for i in vals:
        sqsums += i * i;
        sums += i;
    sums = sums / (1.0 * len(vals));
    stdev = sqsums / (1.0 * len(vals)) - sums * sums;
    return stdev
