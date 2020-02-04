def phi(d):
    fn = 1.0;
    fnext = 1.0;
    temp = 0.0;
    r1, phi = 1.0, 0.0;
    while (abs(phi - r1)> 1e-10) :
        r1 = fnext / fn;
        temp = fn;
        fn = fnext;
        fnext = fnext + temp;
        phi = fnext / fn;
    print(("{:." + str(d) + "}").format(phi));
