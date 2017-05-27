#include "commandwindow.h"

CommandWindow* CommandWindow::window = nullptr;

CommandWindow::CommandWindow(){
    layout = new QVBoxLayout();

    std::map<QString, std::vector<CommandText>*> map;
    std::vector<CommandText> vect = {CommandText("— Alt+Entrée :", "Alt+cmd(pomme)+Entrée :", " Mettre en plein écran."),
                                     CommandText("— Alt + F4 :", " Fermer le jeu."),
                                     CommandText("— F1 :", " Modifier les commandes, options diverses."),
                                     CommandText("— F12 :", " Redémarrer le jeu.")};
    map.insert(std::make_pair(QString("N'importe quand en jeu : "), &vect));

    std::vector<CommandText> vect;

    for(auto&& it : map){
        QLabel* lab = new QLabel(it.first, this);
        lab->setStyleSheet("font-weight: bold;");
        layout->addWidget(lab);
        for(auto&& commandText : *(it.second)){
            QLabel* label = new QLabel();

            QLabel* commandLabel = new QLabel(commandText.getCommand());
            QLabel* descriptionLabel = new QLabel(commandText.getDescription());
            QHBoxLayout* commandLayout = new QHBoxLayout();

            commandLabel->setStyleSheet("font: italic;");
            commandLabel->adjustSize();
            descriptionLabel->adjustSize();

            commandLayout->addWidget(commandLabel);
            commandLayout->addWidget(descriptionLabel);

            label->setLayout(commandLayout);
            label->adjustSize();
            label->show();
            layout->addWidget(label);
        }
    }

    this->setLayout(layout);
    this->adjustSize();
}
CommandWindow* CommandWindow::getCommandWindow(){
    if (window == nullptr) {
        window = new CommandWindow();
    }
    return window;
}
