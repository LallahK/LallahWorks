def rock_paper_sciccors(p1, p2) :
    if ((p1 == "R") and (p2 == "S")) :
        print("Player 1 wins")
    elif ((p1 == "P") and (p2 == "R")) :
        print("Player 1 wins")
    elif ((p1 == "S") and (p2 == "P")) :
        print("Player 1 wins")
    elif ((p2 == "R") and (p1 == "S")) :
        print("Player 1 wins")
    elif ((p2 == "P") and (p1 == "R")) :
        print("Player 1 wins")
    elif ((p2 == "S") and (p1 == "P")) :
        print("Player 1 wins")
    else :
        print("Draw")
