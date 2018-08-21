from flask import Response, Flask, current_app as app
import os
app = Flask(__name__)

@app.route('/')
def index():
    return "Hello"

@app.route('/<cardID>/<locationID>',methods=['GET','POST'])
def acceptSwipeRequest(cardID, locationID):
    try:
        cardOwnerName=os.environ.get(cardID)+"Hello"
        return cardOwnerName
    except:
        return "Not available"
    #return "a"
 

if __name__ == "__main__":
    port = int(os.environ.get("PORT", 5000))
    app.run(host='0.0.0.0', port=port,debug=True)
