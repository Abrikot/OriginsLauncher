import os
import re
import datetime
import time

A_UPLOAD = "origins"
REPERTOIRE = os.getcwd() + "/" + A_UPLOAD # Répertoire qui devra être mis en ligne
                                        # Par défaut, c'est le répertoire origins présent dans le dossier actuel

def listing(stamp, liste, path = REPERTOIRE):
    filesList = os.listdir(path)
    for file in filesList:
        if os.path.isdir(path + "/" + file):
            result = listing(stamp, liste, path + "/" + file)
        else:
            fileStamp = os.path.getmtime(path + "/" + file)
            if (fileStamp > stamp):
                liste.append(path + "/" + file)
    return liste

def printList(liste, stamp, profondeur = 0):
    for file in liste:
        if isinstance(file, list):
            printList(file, stamp, profondeur+1)
        else:
            fileStamp = os.path.getmtime(file)
            if (fileStamp > stamp):
                print("\t"*profondeur + file[len(os.getcwd())+1:] + " : " + datetime.datetime.fromtimestamp(fileStamp).strftime('%Y-%m-%d %H:%M:%S'))

def inputTime():
    print("Entrez la date de dernière modification au format dd/mm/yyyy hh:mm:ss")
    print("Si l'heure n'est pas rentrée, minuit sera utilisé.")
    print("Si la date n'est pas rentrée, la date d'aujourd'hui sera utilisée.")
    date = input()
    timeRE = re.match('\d\d:\d\d:\d\d', date)
    dateRE = re.match('\d\d/\d\d/\d\d\d\d', date)

    if (dateRE != None):
        realDate = dateRE.group()
    else:
        today = datetime.date.today()
        realDate = str(today.day).zfill(2) + "/" + str(today.month).zfill(2) + "/" + str(today.year)
    if (timeRE != None):
        realTime = timeRE.group()
    else:
        realTime = "00:00:00"
    real = realDate + " " + realTime
    return real


def main():
    realTime = inputTime()
    print(realTime)
    stamp = time.mktime(datetime.datetime.strptime(realTime, "%d/%m/%Y %H:%M:%S").timetuple())

    liste = listing(stamp, [], REPERTOIRE)
    return(liste)