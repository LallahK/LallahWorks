def is_valid_usnum(su) :
    SUnum = int(su)
    sum = 0
    for i in range(9) :
        sum = sum + (i + 1) * (SUnum % 10)
        SUnum = SUnum // 10
    sum = sum % 11
    return sum == 0
    
def is_valid_usnum2(su) :
    SUnum = str(su)
    sum = 0
    for i in range(8) :
        sum = sum + (i + 1) * (int(SUnum[7 - i]))
    sum = sum % 11
    return sum == 0
