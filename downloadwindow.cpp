#include "downloadwindow.h"

bool DownloadWindow::downloadNeeded = false;
DownloadWindow* DownloadWindow::window = nullptr;

DownloadWindow* DownloadWindow::getDownloadWindow(){
    if (window == nullptr){
        window = new DownloadWindow();
    }
    return window;
}
DownloadWindow::DownloadWindow(){
    this->setWindowTitle("Téléchargement en cours...");
    text = new QLabel("Préparation de la mise à jour", this);
    this->adjustSize();
    if (!manager.isOnline()) {
        std::cout << "No internet" << std::endl;
        emit endedDownload(false);
        return;
    }

    QString url = "http://abrikot.000webhostapp.com/file.json"; // Lien de téléchargement du fichier contenant les dernières MàJ
    QUrl listUrl(url);
    listDownloader = new FileDownloader(listUrl, this, -1);
    connect(listDownloader, &FileDownloader::downloaded, this, &DownloadWindow::loadFilesList);
    connect(&manager, &QNetworkConfigurationManager::onlineStateChanged, this, &DownloadWindow::onlineChanged);
}
bool DownloadWindow::isDownloadNeeded(){
    return downloadNeeded;
}
void DownloadWindow::loadFilesList(){
    std::cout << "Loaded !" << std::endl;
    QFile listFile("listFiles.new");
    listFile.open(QIODevice::WriteOnly);
    listFile.write(listDownloader->downloadedData());
    listFile.close();
    QJsonDocument list = QJsonDocument::fromJson(listDownloader->downloadedData());
    QJsonObject object = list.object();
    QFile old("listFiles");
    if (old.exists())
    {
        old.open(QIODevice::ReadOnly);
        QJsonDocument oldDocument = QJsonDocument::fromJson(old.readAll());
        QJsonObject oldObject = oldDocument.object();
        makeTree(object, &oldObject);
        old.close();
    }
    else
    {
        makeTree(object, NULL);
    }
    std::cout << "Nombre de fichiers a telecharger : " << fichiers.size() << std::endl;
    if (fichiers.size() == 0) { // Si on n'a aucun fichier à télécharger, le téléchargement n'est pas nécessaire
        downloadNeeded = false;
        listFile.remove();
    } else {
        downloadNeeded = true;
        emit downlodIsNeeded();
    }
}
void DownloadWindow::startUpdate() {
    // On commence par sauvegarder tous les fichiers déjà existants
    for (int i(0); i < fichiers.size(); ++i) {
        QString fileName = fichiers[i].first;
        QFile file(fileName);
        if (file.exists()) {
            file.rename(fileName + ".old");
        }
    }

    // Puis on lance les téléchargments
    bar = new QProgressBar(this);
    bar->setMinimum(0);
    bar->setMaximum(fichiers.size());
    bar->setVisible(true);
    bar->setTextVisible(true);
    for (int i(0); i < fichiers.size(); ++i) {
        launchDownload(fichiers[i].second, i);
    }
}
void DownloadWindow::launchDownload(QString id, int number)
{
    QString url = "https://www.googleapis.com/drive/v3/files/" + id + "?alt=media&key=AIzaSyC9ONZ2v8oKb-P7ZY_wIx7eM-lg9Kl7g9U";
    QUrl imageUrl(url);
    FileDownloader *d = new FileDownloader(imageUrl, this, number);
    connect(d, &FileDownloader::downloaded, this, &DownloadWindow::loadFiles);
    if (downloaders.size() < number + 1)
        downloaders.push_back(d);
    else
    {
        delete downloaders[number];
        downloaders[number] = d;
    }
}
void DownloadWindow::loadFiles(int number) {
    mutex.lock();
    if (!manager.isOnline() || downloaders[number]->downloadedData().length() == 0) {
        FileDownloader::stopDownloads();
        emit endedDownload(false);
        return;
    }
    QString string = fichiers[number].first;
    QFile file(string);
    file.open(QIODevice::WriteOnly);
    file.write(downloaders[number]->downloadedData());
    file.close();
    QJsonDocument document = QJsonDocument::fromJson(downloaders[number]->downloadedData());
    QStringList errorUnauthorized;
    errorUnauthorized << "appNotAuthorizedToFile" << "insufficientFilePermissions" << "domainPolicy" << "fileNotDownloadable";
    if (!document.isNull())
    {
        QJsonObject errorFile = document.object();
        QJsonObject error = errorFile["error"].toObject();
        QJsonArray errors = error["errors"].toArray();
        if (error["code"].toInt() == 404)
        {
            // T'es pas au bon endroit mon gars !
            std::cout << "404 dans ta gueule maggle !" << std::endl;
            QFile file(string);
            file.remove();
            nbEnded++;
            mutex.unlock();
            return;
        }
        if (error["code"].toInt() == 403)
        {
            if (errorUnauthorized.contains(errors.at(0).toObject()["reason"].toString()))
            {
                // Fichier non téléchargeable è_é
                std::cout << "403, parce que je le vaux bien !" << std::endl;
                QFile file(string);
                file.remove();
                mutex.unlock();
                nbEnded++;
                return;
            }
            else
            {
                // Le téléchargement a foiré ? Pas de problème, on le relance !
                launchDownload(fichiers[number].second, number);
                mutex.unlock();
                return;
            }
        }

        // Erreur inconnue
        std::cout << "Erreur inconnue :o" << std::endl;
        QFile file(string);
        file.remove();
        mutex.unlock();
        nbEnded++;
        return;
    }
    bar->setValue(++nbEnded);
    if (bar->maximum() == nbEnded) {
        this->downloadNeeded = false;
        emit endedDownload(true);
    }
    mutex.unlock();
}
void DownloadWindow::makeTree(QJsonObject& object, QJsonObject* old, QString path)
{
    QStringList keys = object.keys();
    for (int i(0); i < keys.size(); ++i)
    {
        QString key = keys.at(i);
        if (key == "version") { // On récupère le numéro de version
            this->versionNumber = object[key].toString();
            std::cout << versionNumber.toStdString() << std::endl;
            return;
        }
        if (key.startsWith("Folder=>"))
        {
            QString newKey = key.right(key.length()-8);
            QString newPath = path+newKey;
            QDir dir(QDir::currentPath());
            dir.mkpath(newPath);
            QJsonObject fils = object[key].toObject();
            makeTree(fils, old, newPath + "/");
            continue;
        }
        else
        {
            QString adresse = key;
            QString name = object[key].toObject()["name"].toString();
            QString stringDate = object[key].toObject()["modifiedTime"].toString();
            QFile file(path+name);

            if (old != NULL && file.exists())
            {
                QDateTime newDate = QDateTime::fromString(stringDate, "yyyy-MM-ddThh:mm:ss.zzzZ");
                QDateTime* poldDate = getModifiedTime(adresse, *old);
                if (poldDate != NULL)
                {
                    QDateTime oldDate = *poldDate;
                    QString newDateString = newDate.toString("yyyy-MM-dd hh-mm-ss");
                    QString oldDateString = oldDate.toString("yyyy-MM-dd hh-mm-ss");
                    if (oldDate < newDate)
                    {
                        QPair<QString, QString> pair(path + name, adresse);
                        fichiers.push_back(pair);
                    }
                    continue; // Fichier traité, on passe à la suite
                }
            }
            QPair<QString, QString> pair(path + name, adresse);
            fichiers.push_back(pair);
            continue;
        }
    }
}
QDateTime* DownloadWindow::getModifiedTime(QString idItem, QJsonObject& object)
{
    // Si aucun objet n'a été passé... Eh bah il n'y a aucune date ! Namého !
    if (object.isEmpty()) {
        return nullptr;
    }

    // On récupère la liste des clés
    QStringList keys = object.keys();
    for (int i(0); i < keys.size(); ++i)    // On parcourt cette liste
    {
        QString key = keys.at(i);
        if (key.startsWith("Folder=>")) // Si c'est un dossier, on regarde les fichiers contenus à l'intérieur
        {
            QJsonObject fils = object[key].toObject();
            QDateTime* result = getModifiedTime(idItem, fils);
            if (result != nullptr) {    // S'il n'y a aucun fils... Bah on laisse tranquille è_é
                return result;
            }
            continue;
        }
        else
        {
            if (key == idItem)  // Si c'est un fichier, on renvoie sa date de modification
            {
                QString stringDate = object[key].toObject()["modifiedTime"].toString();
                QDateTime* date = new QDateTime(QDateTime::fromString(stringDate, "yyyy-MM-ddThh:mm:ss.zzzZ"));
                return date;
            }
        }
    }
    return nullptr;
}
QString DownloadWindow::getOldVersionNumber() {
    QFile file("listFiles");
    file.open(QIODevice::ReadOnly);
    QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    QJsonObject object = document.object();
    file.close();
    return object["version"].toString();
}
void DownloadWindow::cleaning(bool success) {
    if (!alreadyCleened) {
        if (success) {
            for (int i(0); i < fichiers.size(); ++i) {
                QString fileName = fichiers[i].first;
                QFile file(fileName + ".old");
                if (file.exists()) {
                    file.remove();
                }
            }
            QFile oldList("listFiles");
            if (oldList.exists()) {
                oldList.remove();
            }
            QFile newList("listFiles.new");
            if (newList.exists()) {
                newList.rename("listFiles");
            }
        } else {
            for (int i(0); i < fichiers.size(); ++i) {
                QString fileName = fichiers[i].first;
                QFile file(fileName);
                if (file.exists()) {
                    file.remove();
                }
                QFile oldFile(fileName + ".old");
                if (oldFile.exists()){
                    oldFile.rename(fileName);
                }
            }
            QFile newList("listFiles.new");
            if (newList.exists()) {
                newList.remove();
            }
            QFile oldList("listFiles");
            if (oldList.exists()) {
                oldList.remove();
            }
        }
        alreadyCleened = true;
    }
}
void DownloadWindow::onlineChanged(bool isOnline){
    if (!isOnline) {
        FileDownloader::stopDownloads();
        emit endedDownload(false);
    }
}
void DownloadWindow::closeEvent(QCloseEvent *) {
    if (downloadNeeded){
        FileDownloader::stopDownloads();
        emit endedDownload(false);
    }
}
