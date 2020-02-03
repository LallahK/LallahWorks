def isbn13_to_10(isbn) :
    ISBN = isbn[3:]
    sigma = 0
    vals = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, "X"]
    for i in range(1, 10) :
        sigma = sigma + (i + 1) * (int(ISBN[9 - i]))
    print(str(11 - sigma % 11))
    ISBN = ISBN[:10] + str(vals[- sigma % 11])
    return ISBN
