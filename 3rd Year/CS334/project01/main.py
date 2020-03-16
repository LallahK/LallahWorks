import sys
import debug
import senti
import json
import datetime
from operator import itemgetter

def main(obj) :
    if (len(obj) <= 1) :
        print("ERROR: Not enough arguments")
        exit()

    params = {}
    for i in range(1, int(len(obj)/2)) :
        params.update({obj[i*2] : obj[i*2 + 1]})

    reviews = debug.status_decode(obj[1], params)
    senti.senti_tree_constr()
    
    filtered_reviews = []
    for rev in reviews :
        url = rev['link']['url']
        summary = debug.html_extract_review(url)
        details = filter(rev, summary)
        details.update({"Review" : summary})
        filtered_reviews.append(details)
        rating = senti.analyse(details['Review'])
        details.update({"Rating" : rating})

    filtered_reviews = sorted(filtered_reviews, key=lambda x: (x['Opening Date'] is None, x['Opening Date'], x['Last Modified'] is None, x['Last Modified']), reverse = True)
    lprint(filtered_reviews)

    news_article = debug.html_extract_source("https://www.news24.com/TopStories")
    if (news_article == 0) :
        print("ERROR: No news article found")
    else :
        print(news_article[0])
        print('-' * 75)
        print(news_article[1])
        print('-' * 75)


def jprint(obj) :
    text = json.dumps(obj, sort_keys=True, indent=4)
    print(text)

def filter(review, summary) :
    details = {}
    details.update({"Title" : review['display_title']})
    details.update({"Opening Date" : review['opening_date']})
    details.update({"Last Modified" : review['date_updated']})
    details.update({"Author" : review['byline']})

    return details

def lprint(reviews) :
    for i in range(0, 15) :
        rev = reviews[i]
        print('-' * 75)
        print()
        print("Title : " + rev['Title'])
        if rev['Opening Date'] :
            print("Opening Date : " + rev['Opening Date'])
        else :
            print("Opening Date : Not available")
        if rev['Last Modified'] :
            print("Last Modified : " + rev['Last Modified'])
        else :
            print("Last Modified: Not available")
        print("Author : " + rev['Author'])
        print("Review : " + rev['Review'])
        pos = rev['Rating'][1]
        neg = rev['Rating'][0]
        if (pos > neg) :
            if neg == 0 :
                rat = (1.0, 0.0) 
            else : 
                rat = (pos/neg, 1)
            print("Sentiment Analysis :  pos:neg = %.2f:%.2f" % rat)
        else :
            if (pos == 0) :
                rat = (1.0, 0.0)
            else :
                rat = (neg/pos, 1)
            print("Sentiment Analysis :  neg:pos = %.2f:%.2f" % rat)

        print()      
    
if __name__ == "__main__" :
    main(sys.argv)
