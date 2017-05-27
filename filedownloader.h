#ifndef FILEDOWNLOADER_H
#define FILEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class FileDownloader : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloader(QUrl url, QObject *parent = 0, int number = 0);
    virtual ~FileDownloader();
    QByteArray downloadedData() const;
    static QNetworkAccessManager *m_WebCtrl;
    QByteArray m_DownloadedData;
    int number;
    QNetworkReply* reply;
    void launchNewDownload(QUrl url);
    static void stopDownloads();

signals:
    void downloaded(int);

private slots:
    void fileDownloaded();
};

#endif // FILEDOWNLOADER_H
