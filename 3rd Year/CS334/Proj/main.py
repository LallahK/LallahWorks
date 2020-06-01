from flask import Flask, render_template, request
import top_restaurant
import json
import nextRestaurants

app = Flask(__name__)
# Load the file into memory
with open("./jsons/restaurant_categories.json", "r") as f:
    data = json.load(f)

@app.route('/', methods = ['POST', 'GET'])
def index() :
    if request.method == 'POST' :
        #city = 'Las Vegas'
        #category = 'Seafood'
        #day = 'Tuesday'
        #time = '14:00'

        city = request.form['city_input']
        day = request.form['day_input']
        category = request.form['category_input']
        time = request.form['time_input']

        top = {
            "Resturant_name" : "",
            "Address" : "",
            "Stars" : 0,
            "Review_Count" : 0,
            "Most Useful Review Author" : 0,
            "Most Useful Review" : "",
            "Photo Urls" : [],
            "usID": "",
            "busID": ""
        }
        top = top_restaurant.top_resturant_using_neo4j(city, category, day, time)
        #top_restaurant.print_dict(top)
        top['Stars'] = int(top['Stars'])
        
        userID = top['usID']
        businessID = top['busID']
        #other = nextRestaurants.getNext(userID, businessID, city, category)

        return render_template('results.html', title = 'Results', content=top, categories=data)
    else :
        return render_template('front_page.html', title = 'Home', categories=data)

if __name__ == "__main__" :
    app.run(debug=True)
