#https://realpython.com/api-integration-in-python/#rest-and-python-tools-of-the-trade
#https://realpython.com/flask-connexion-rest-api/

from flask import Flask, request, jsonify
from datetime import datetime
import pymongo
import requests
import constants

app = Flask(__name__)

configuration =""
@app.get("/configurations")
def get_configurations():
    return jsonify(configuration)

@app.get("/events")
def get_events():
    aux = {}
    coleccion = conectarBD()
    resultado = coleccion.find()
    for each in resultado:
        for key in each.keys():
            if(key!='_id'):
                aux[key]=each.get(key)
    return jsonify(aux)

# @app.get("/events/id")
# def get_events(id):
#     #TODO: comprobar que id cumple formato
#     coleccion = conectarBD()
#     resultado = coleccion.find_one({'id':id})
#     return jsonify(resultado)

@app.post("/events")
def add_event():
    if request.is_json:
        event= request.get_json()
        coleccion = conectarBD()
        momento = datetime.now()
        if(event['event'] == 'lectura'):
            evento = {
                'event' : event['event'],
                'fecha': momento,
                'humedad': event['humedad'],
                'temperatura': event['temp'],
                'luz': event['luz']
            }
        elif (event.event == 'riego'):
            evento = {
                'event': event['event'],
                'fecha': momento,
            }
        resultado = coleccion.insert_one(evento)
        if(resultado.inserted_id !=''):
            return evento, 201
        else:
            return "error al insertar en BD", 401
    else:
        return {"error": "Request must be JSON"}, 415

def conectarBD():

    try:
        print('Conectando a la DB...')
        cliente = pymongo.MongoClient(constants.DB_URL)
        db = cliente.jardin
        eventsCol = db.events
        return eventsCol
    
    except pymongo.errors.ServerSelectionTimeoutError as e:
        extIP = requests.get('http://ip.42.pl/raw').text
        #informarViaTelegram('Timeout en la conexión a la BD:\n' + e.args[0])
        #informarViaTelegram("Checking in from IP#: %s " % extIP)
    