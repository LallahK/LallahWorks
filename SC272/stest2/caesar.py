def cipher(w, n) :
    """Takes a string alphabet parameter w and returns the alphabet shifted
    n positions to the right, where the last n letters are wrapped around"""
    caeser = ""
    for i in range(len(w)) :
        print((ord(w[i]) + n) % 65)
        caeser = caeser + chr((ord(w[i]) - 65 + n) % 26 + 65)
    return caeser   
