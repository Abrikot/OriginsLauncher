#ifndef DOWNLOADWINDOW_H
#define DOWNLOADWINDOW_H

#include <QtWidgets/QLabel>
#include <filedownloader.h>
#include <QVector>
#include <QPair>
#include <QMutex>
#include <QProgressBar>
#include <QDateTime>
#include <QFile>
#include <filedownloader.h>
#include <iostream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDir>
#include <QNetworkConfigurationManager>

class DownloadWindow : public QLabel
{
    Q_OBJECT

// Attributs
private:
    static DownloadWindow* window;                          // Singleton
    static bool downloadNeeded;                             // Indique si on est à jour ou non
    int nbEnded = 0;                                        // Nombre de téléchargements finis (dans l'absolu, osef è_é)
    QProgressBar* bar;                                      // Barre indiquant les téléchargements effectués
    QMutex mutex;                                           // On protège les sections critiques /o
    FileDownloader* listDownloader;                         // Downloader de la liste des fichiers à télécharger plus tard
    QVector <FileDownloader*> downloaders;                  // Liste des downloaders pour tous les fichiers
    QVector <QPair<QString, QString>> fichiers;             // Liste des fichiers avec nom/adresse
    QString versionNumber;                                  // Numéro de version
    bool ended = false;                                     // Le téléchargement est-il terminé ?
    QNetworkConfigurationManager manager;
    bool alreadyCleened = false;
    QLabel* text;


// Méthodes
public:
    static QString getOldVersionNumber();
    static DownloadWindow* getDownloadWindow();          // Singleton
    bool isDownloadNeeded();                                // Renvoie si le téléchargement est nécessaire
    void startUpdate();                                     // Lance la mise à jour
    const QString getVersionNumber() {return versionNumber;}
    void cleaning(bool);                                    // Nettoie les fichiers temporaires

private:
    DownloadWindow();
    void launchDownload(QString, int);                                          // Lance le téléchargement du fichier dont l'id est le premier paramètre, et donne un numéro au downloader
    void makeTree(QJsonObject& object, QJsonObject* old, QString path = "");    // Crée l'arborescence des fichiers à télécharger
    QDateTime* getModifiedTime(QString idItem, QJsonObject& file);

public slots:
    void loadFilesList();   // Charge la liste des fichiers à télécharger et lance les téléchargements
    void loadFiles(int);    // Sauvegarde les fichiers téléchargés

private slots:
    void onlineChanged(bool);
    void closeEvent(QCloseEvent* event);

signals:
    void downlodIsNeeded();
    void endedDownload(bool);   // Téléchargement terminé (true = OK, false = erreur)
};

#endif // DOWNLOADWINDOW_H
