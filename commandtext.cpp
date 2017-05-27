#include "commandtext.h"

CommandText::CommandText(QString command, QString description) : commandWindows(command), commandMac(command), description(description){}
CommandText::CommandText(QString commandWindows, QString commandMac, QString description) : commandWindows(commandWindows), commandMac(commandMac), description(description){}

CommandText::~CommandText(){
    if (label != nullptr){
        delete label;
        label = nullptr;
    }
}

QLabel* CommandText::getLabel(){
    if (label == nullptr) {
        label = new QLabel("Pouet");
        /*QLabel commandLabel(command);
        QLabel descriptionLabel(description);
        QHBoxLayout layout;

        commandLabel.setStyleSheet("font: italic");

        layout.addWidget(&commandLabel);
        layout.addWidget(&descriptionLabel);

        label->setLayout(&layout);*/
    }
    label->adjustSize();
    return label;
}

QString CommandText::getCommand() {
    // On teste l'OS pour savoir quelle description renvoyer
    if (QSysInfo::windowsVersion() == QSysInfo::WinVersion::WV_None) return commandMac;
    return commandWindows;
}
