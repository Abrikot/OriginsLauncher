from __future__ import print_function
from ftplib import FTP
import httplib2
import os
import json
import time

import modifiedFiles

from apiclient import discovery
from googleapiclient.http import MediaFileUpload
from oauth2client import client
from oauth2client import tools
from oauth2client.file import Storage

try:
    import argparse
    flags = argparse.ArgumentParser(parents=[tools.argparser]).parse_args()
except ImportError:
    flags = None

# Insérier ici les identifiants pour se connecter au site en ftp (comme avec FileZilla /o)
SITE = "files.000webhost.com"
USERNAME = "abrikot"
PASSWORD = "Naruto1996"

# Insérer ici l'URL de partage du fichier
URL = "https://drive.google.com/drive/folders/0BzI0caP7diyfdlpwck44cC1WYVk?usp=sharing"

# Numéro de la version du jeu
VERSION = "4.2.7"

# If modifying these scopes, delete your previously saved credentials
# at ~/.credentials/drive-python-quickstart.json
SCOPES = 'https://www.googleapis.com/auth/drive'
CLIENT_SECRET_FILE = 'client_secret.json'
APPLICATION_NAME = 'linkCreator'

# Fonction très l'importance, pas toucher
def get_credentials():
    """Gets valid user credentials from storage.

    If nothing has been stored, or if the stored credentials are invalid,
    the OAuth2 flow is completed to obtain the new credentials.

    Returns:
        Credentials, the obtained credential.
    """
    home_dir = os.path.expanduser('~')
    credential_dir = os.path.join(home_dir, '.credentials')
    if not os.path.exists(credential_dir):
        os.makedirs(credential_dir)
    credential_path = os.path.join(credential_dir, 'linkCreator.json')

    store = Storage(credential_path)
    credentials = store.get()
    if not credentials or credentials.invalid:
        flow = client.flow_from_clientsecrets(CLIENT_SECRET_FILE, SCOPES)
        flow.user_agent = APPLICATION_NAME
        if flags:
            credentials = tools.run_flow(flow, store, flags)
        print('Storing credentials to ' + credential_path)
    return credentials

# Fonction d'upload de fichier
def upload(filePath, service, parent = ""):
    filePath = str(filePath).split("/" + modifiedFiles.A_UPLOAD)[1][1:]
    folders = filePath.split('/')
    for i in range(0,len(folders)-1):
        parent = createFolder(folders[i], service, parent)

    # Existe-t-il déjà une ancienne version du fichier ? Si oui, on le met à jour. Sinon, on l'upload.
    query = "trashed=false and name = '" + folders[-1] + "'"
    if parent != "":
        query += " and '" + parent + "'" + " in parents"
    results = service.files().list(q=query, fields="files(id)").execute()
    for result in results.get('files', []):
        id = result.get('id')
        if (id != ""): # Le fichier existe, on le met à jour.
            media = MediaFileUpload(modifiedFiles.A_UPLOAD + '/' + filePath)
            service.files().update(fileId=id, media_body=media).execute()
            return

    fileMetaData = {'name': folders[-1], 'parents': [parent]}
    media = MediaFileUpload(modifiedFiles.A_UPLOAD + '/' + filePath)
    service.files().create(body=fileMetaData, media_body=media).execute()


# Fonction de création de dossier
def createFolder(name, service, parent = ""):
    # Le dossier existe-t-il déjà ?
    query = "trashed=false and name = '" + name + "'"
    if parent != "":
        query += " and '" + parent + "'" + " in parents"
    results = service.files().list(q=query, fields="files(id)").execute()
    for result in results.get('files', []):
        id = result.get('id')
        if (id != ""): # Le dossier existe, on renvoie son ID
            return id

    # Non => On le crée
    folderMetadata = {'name' : name, 'mimeType' : 'application/vnd.google-apps.folder'}
    if (parent != ""):
        folderMetadata['parents'] = [parent]
    folder = service.files().create(body=folderMetadata, fields='id').execute()
    return folder.get('id')

def main():
    tab = URL.split('/')
    tab = tab[5].split('?')
    id = tab[0]

    # Connexion au Drive
    credentials = get_credentials()
    http = credentials.authorize(httplib2.Http())
    service = discovery.build('drive', 'v3', http=http)
    """
        # On récupère les fichiers modifiés
    listeFichiers = modifiedFiles.main()

    # On upload chaque fichier modifié
    print("Upload des fichiers en cours, veuillez patienter. Cette opération peut durer plusieurs minutes.")
    i = 0
    for fichier in listeFichiers:
        upload(fichier, service, id)
        i+=1
        print("  Fichiers uploadés : " + str(i) + " sur " + str(len(listeFichiers)))
    """
    # On récupère le dossier principal
    folder = service.files().get(fileId=id).execute()
    dict = createJson(folder, service)

    print(str(NB) + " fichiers")

    jsonFile = json.dumps({"Folder=>origins":dict[1], "version":VERSION})

    file = open('file.json', 'w')
    file.write(jsonFile)
    file.close()

    print("\nAttendez quelques secondes avant d'envoyer le fichier...")
    time.sleep(1)

    ftp = FTP(SITE, USERNAME, PASSWORD)
    f_name = 'file.json'
    file = open(f_name, 'rb')
    ftp.storbinary('STOR public_html/' +  f_name, file)
    file.close()
    ftp.quit()
    os.remove('file.json')

    print("Mise à jour effectuée avec succès, les joueurs ont désormais accès à cette dernière.")


def createJson(object, service):
    global NB
    name = object['name']
    id = object['id']
    try :
        modifiedTime = object['modifiedTime']
    except:
        modifiedTime = ""
    if (object['mimeType'] == "application/vnd.google-apps.folder"):
        isFolder = True
        dict = {}
        query = "'" + id + "'" + " in parents and trashed=false"
        results = service.files().list(q=query, pageSize=100, fields="nextPageToken, files(id, name, mimeType, modifiedTime)").execute()
        children = results.get('files', [])
        token = results.get('nextPageToken')
        while token != None:
            results = service.files().list(q=query, pageSize=100, pageToken=token,
                                           fields="nextPageToken, files(id, name, mimeType, modifiedTime)").execute()
            children += (results.get('files', []))
            token = results.get('nextPageToken')
        for child in children:
            result = createJson(child, service)
            childIsFolder = result[3]
            folder = ""
            if (childIsFolder):
                folder = "Folder=>"
                dict[folder + result[4]] = result[1]
            else:
                dict[result[0]] = {'name':result[1], 'modifiedTime':result[2]}
        return [id, dict, modifiedTime, isFolder, name]
    else:
        NB +=1
        isFolder = False
        return [id, name, modifiedTime, isFolder]

NB = 0

if __name__ == '__main__':
    main()
