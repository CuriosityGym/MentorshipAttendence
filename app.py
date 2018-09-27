from flask import Response, Flask, current_app as app
from datetime import datetime
import os
import requests
import sys 
app = Flask(__name__)
IFTTT_KEY=os.environ.get("IFTTT_KEY")
IFTTT_MENTORSHIP_EVENT_NAME=os.environ.get("IFTTT_EVENT_NAME")
IFTTT_UNKNOWN_EVENT_NAME=os.environ.get("IFTTT_UNKNOWN_NAME")
#IFTTT_EVENT_NAME="track_mentorship"
#IFTTT_KEY="c1BVwMuv-fI8ryLpLIihJe"
IFTTT_MENTORSHIP_URL="https://maker.ifttt.com/trigger/"+IFTTT_MENTORSHIP_EVENT_NAME+"/with/key/"+IFTTT_KEY
IFTTT_UNKNOWN_CARD_URL="https://maker.ifttt.com/trigger/"+IFTTT_UNKNOWN_EVENT_NAME+"/with/key/"+IFTTT_KEY



@app.route('/')
def index():
    return "Hello"

@app.route('/<cardID>/<locationID>',methods=['GET','POST'])
def acceptSwipeRequest(cardID, locationID):
    try:
        cardOwnerName=os.environ.get(cardID)
        if(cardOwnerName is not None):
            
            data={}
            data["value1"]=cardOwnerName
            data["value2"]=locationID
            data["value3"]=datetime.now()        
            #sys.stdout.write(IFTTT_URL)
            response=requests.post(IFTTT_MENTORSHIP_URL, data=data)
            return response.text
        else:
            data={}
            data["value1"]=cardID
            data["value2"]=locationID
            data["value3"]=datetime.now()        
            #sys.stdout.write(IFTTT_URL)
            response=requests.post(IFTTT_UNKNOWN_CARD_URL, data=data)
            return response.text
        
    except:
        return "Error"
 

if __name__ == "__main__":
    port = int(os.environ.get("PORT", 5000))
    app.run(host='0.0.0.0', port=port,debug=True)
