def parallelogram(s):
    n = len(s);
    s = (n - 1) * " " + s + (n - 1) * " ";
    strin = "";
    for i in range(n * 2 - 1):
        strin = s[i:i + n];
        print(strin[0:n]);
