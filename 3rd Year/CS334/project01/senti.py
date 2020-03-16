import string
#from vaderSentiment.vaderSentiment import SentimentIntensityAnalyzer

neg_words = []
pos_words = []

def senti_tree_constr() :
    global pos_words
    global neg_words 

    f = open(r"positive.txt","r")
    pos_words = f.readlines()
    for i in range(len(pos_words)) :
        pos_words[i] = pos_words[i].rstrip()
    
    f = open(r"negative.txt", "r")
    neg_words = f.readlines()
    for i in range(len(neg_words)) :
        neg_words[i] = neg_words[i].rstrip()

def analyse(review) :
    rev_as_list = list(review.split(" "))
    positive = 0
    negative = 0
    for i in range(len(rev_as_list)) :
        if (binarySearch(pos_words, 0, len(pos_words) - 1, rev_as_list[i]) != -1) :
            positive = positive + 1
        if (binarySearch(neg_words, 0, len(neg_words) - 1, rev_as_list[i]) != -1) :
            negative = negative + 1
    return (negative, positive)
    
def binarySearch (arr, l, r, x): 
  
    # Check base case 
    if r >= l: 
  
        mid = l + (r - l) // 2
  
        # If element is present at the middle itself 
        if arr[mid] == x: 
            return mid 
          
        # If element is smaller than mid, then it  
        # can only be present in left subarray 
        elif arr[mid] > x: 
            return binarySearch(arr, l, mid-1, x) 
  
        # Else the element can only be present  
        # in right subarray 
        else: 
            return binarySearch(arr, mid + 1, r, x) 
  
    else: 
        # Element is not present in the array 
        return -1

#def vader_analyse(review) :
#    rev_as_list = list(review.split(" "))
#    analyser = SentimentIntensityAnalyzer()
#    for rev_line in review :
#        snt = analyser.polarity_scores(rev_line)
#        print(snt)
