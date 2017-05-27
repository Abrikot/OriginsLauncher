#include "filedownloader.h"
#include <iostream>

QNetworkAccessManager* FileDownloader::m_WebCtrl = nullptr;

FileDownloader::FileDownloader(QUrl url, QObject *parent, int number) :
    QObject(parent), number(number)
{
    if (m_WebCtrl == nullptr) {
        m_WebCtrl = new QNetworkAccessManager();
    }
    //connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)),this, SLOT (fileDownloaded(QNetworkReply*)));

    launchNewDownload(url);
}
FileDownloader::~FileDownloader() { }
void FileDownloader::launchNewDownload(QUrl url) {
    QNetworkRequest request(url);
    this->reply = m_WebCtrl->get(request);
    connect(reply, &QNetworkReply::finished, this, &FileDownloader::fileDownloaded);
}
void FileDownloader::fileDownloaded() {
    m_DownloadedData = reply->readAll();
    /*QByteArray length = pReply->rawHeader("Content-Length");
    std::cout << number << " : " << length.toStdString() << std::endl;*/ // Récupère la taille du fichier

    //emit a signal
    reply->deleteLater();
    emit downloaded(this->number);
}
QByteArray FileDownloader::downloadedData() const {
    return m_DownloadedData;
}
void FileDownloader::stopDownloads() {
    if (m_WebCtrl != nullptr) {
        delete m_WebCtrl;
    }
    m_WebCtrl = new QNetworkAccessManager();
}
