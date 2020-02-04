def triangle(i):
    count = 1;
    n = 0
    while (count < i + 1) :
        n = 0;
        while (n < count) :
            print("X", end = "");
            n +=1;
        count += 1;
        print();
