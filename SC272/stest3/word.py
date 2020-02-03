def diamond(string):
    length = len(string);
    b, t = 0, 0;
    a = length;
    if (length % 2 == 0) :
        b = (int) (length / 2 - 1);
        t = (int) (length / 2 + 1);
        a = length - 1;
    else :
        b = (int) (length / 2.0);
        t = (int) (length / 2.0 + 1);
    for k in range(a):
        l = (int) ((len(string) - len(string[b:t])) / 2);
        print(" " * l + string[b:t])
        if k < length / 2 - 1 :
            b -= 1;
            t += 1;
        else :
            b += 1;
            t -= 1;
