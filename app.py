from flask import Response, Flask, current_app as app
import os
import requests
app = Flask(__name__)
IFTTT_KEY=os.environ.get("IFTTT_KEY")
IFTTT_EVENT_NAME=os.environ.get("IFTTT_EVENT_NAME")
IFTTT_URL="https://maker.ifttt.com/trigger/"+IFTTT_KEY+"/with/key/"+IFTTT_KEY



@app.route('/')
def index():
    return "Hello"

@app.route('/<cardID>/<locationID>',methods=['GET','POST'])
def acceptSwipeRequest(cardID, locationID):
    try:
        cardOwnerName=os.environ.get(cardID)
        dataURL=IFTTT_URL+ "?value1="+cardOwnerName+"&value2="+locationID+"&value3="
        response=requests.get(IFTTT_URL)
        return response.text
    except:
        return "Not available"
    #return "a"
 

if __name__ == "__main__":
    port = int(os.environ.get("PORT", 5000))
    app.run(host='0.0.0.0', port=port,debug=True)
