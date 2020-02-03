def qsolve(a, b, c):
    discr = b * b - 4 * a * c
    return (-1 * b + discr ** (1 / 2) ) / (2 * a), (-1 * b - discr ** (1 / 2) ) / (2 * a)
