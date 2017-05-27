#ifndef COMMANDTEXT_H
#define COMMANDTEXT_H

#include <QString>
#include <QLabel>
#include <QHBoxLayout>
#include <QSysInfo>
#include <memory>

class CommandText
{
private:
    QString commandWindows;
    QString commandMac;
    QString description;
    QLabel* label = nullptr;
public:
    CommandText(QString command, QString description);
    CommandText(QString commandWindows, QString commandMac, QString description);
    ~CommandText();
    QString getCommand();
    QString getDescription(){return description;}
    QLabel* getLabel();
};

#endif // COMMANDTEXT_H
