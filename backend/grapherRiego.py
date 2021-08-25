import plotly.graph_objects as go
import plotly.express as px
import pymongo
import constants
import requests
import pandas as pd

def crearGrafica():
    try:
        print('Conectando a la DB...')
        cliente = pymongo.MongoClient(constants.DB_URL)
        eventsCol = cliente.jardin.events
        #dataLuz = eventsCol.find({},{'fecha':1, 'luz':1, '_id':0})
        data = eventsCol.find()
        df = pd.DataFrame(list(data))
        print(df)
        fechas = df[['fecha']].squeeze()
        temp = df[["temperatura"]].squeeze()
        luz = df[["fecha","luz"]].squeeze()
        humedad = df[["fecha","humedad"]].squeeze()
        specTemp = {
            "type":"scatter"
        }
        specLuz = {
            "type":"scatter"
        }
        specHumedad = {
            "type":"scatter",
            "secondary_y":True
        }
        
        #https://plotly.com/python/reference/scatter/
        fig = go.Figure()
        fig.add_trace(go.Scatter(x=fechas, y=temp,mode='lines+markers',text='Temperatura', name='Temperatura'))
        fig.add_trace(go.Scatter(x=fechas, y=luz,mode="lines+markers",text='Luz', name='Luz')) 
        fig.add_trace(go.Scatter(x=fechas, y=humedad,mode="lines+markers",text='Humedad', name='Humedad')) 
        #fig = px.line(df,x="fecha", y="luz", markers=True)
        #fig = px.line(df,x="fecha", y="temperatura", markers=True)
        #no chuta, solo se queda la última
        fig.show()

    except pymongo.errors.ServerSelectionTimeoutError as e:
        extIP = requests.get('http://ip.42.pl/raw').text
        #informarViaTelegram('Timeout en la conexión a la BD:\n' + e.args[0])
        #informarViaTelegram("Checking in from IP#: %s " % extIP)
        print(extIP)
    #añadir datos
    #dataY = [1,2,3,4]    
    #dataX = [1,2,3,4]
    #fig = go.Figure()

    #create Styles

if(__name__ == '__main__'):
    crearGrafica()