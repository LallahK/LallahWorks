def isbn10to13(isbn10) :
    """Converts an isbn10 numeric code to an isbn13 numeric code.
    Takes a string as the initial parameter and returns a string"""
    sums = 0
    for i in range(0, 9, 2) :
        sums = sums + 3 * int(isbn10[i])
    for i in range(1, 9, 2) :
        sums = sums + int(isbn10[i])
    print(sums)
    sums = sums + 9 * 1 + 7 * 3 + 8 * 1
    print("978" + isbn10[:9] + str(sums % 10))
    return ("978" + isbn10[:9] + str((10 - sums) % 10))
