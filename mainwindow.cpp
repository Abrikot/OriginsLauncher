#include "mainwindow.h"
#include <iostream>

MainWindow::MainWindow()
{
    fillQuotes();
    createContent();

    this->setVisible(true);

    downloadWindow = DownloadWindow::getDownloadWindow();
    commandWindow = CommandWindow::getCommandWindow();
    connect(downloadWindow, &DownloadWindow::downlodIsNeeded,this, &MainWindow::unlockDownloadButton);
}

// Interface
void MainWindow::createContent() {
    setFixedSize (700, 600);
    setWindowIcon(QIcon(":/assert/pokeball.png"));
    setWindowTitle("Pokémon Origins");
    setWindowFlags(Qt::FramelessWindowHint);

    pkmnFont = createFont(QString(":/assert/pkmnemn.ttf"), 14);
    menuFont = createFont(QString(""), 14);
    QLabel* label = new QLabel(this);
    label->setPixmap(QPixmap(":/assert/launcher.png"));

    playButton = createButton(":/assert/pokeball.png", "Jouer", 0);
    //updatePlayButton();   // Refaire le lien

    updateButton = createButton(":/assert/pierrefeu.png", "Mise à jour", 1);
    updateButton->setStyleSheet(disableStylesheet);
    //disableButton(updateButton);  // A réimplémenter

    aboutButton = createButton(":/assert/icoCredits.png", "Crédits", 2);
    commandsButton = createButton(":/assert/velocross.png", "Commandes", 4);
    optionsButton = createButton(":/assert/cs.png", "Options", 5);

    forumButton = createButton(":/assert/carte.png", "Forum", 3);
    fbButton = createButton(":/assert/fb.png", "", 0);
    fbButton->move(10, this->height() - 35);
    twitterButton = createButton(":/assert/twitter.png", "", 0);
    twitterButton->move(40, this->height() - 35);

    CSPButton = createButton(":/assert/csp.png", "", 0);
    CSPButton->move(70, this->height() - 45);
    CSPButton->setIconSize(QSize(90, 40));

    exitButton = createButton(":/assert/close.png", "", 7);
    exitButton->move(this->width() - 35, 7 );//caca

    // Jeu et téléchargement
    QObject::connect(playButton, &QPushButton::clicked, this, &MainWindow::launchGame);
    QObject::connect(updateButton, &QPushButton::clicked, this, &MainWindow::update);

    // Divers liens
    QObject::connect(forumButton, &QPushButton::clicked, this, &MainWindow::openForum);
    QObject::connect(fbButton, &QPushButton::clicked, this, &MainWindow::openFacebook);
    QObject::connect(twitterButton, &QPushButton::clicked, this, &MainWindow::openTwitter);
    QObject::connect(CSPButton, &QPushButton::clicked, this, &MainWindow::openCSP);

    // Quitter
    QObject::connect(exitButton, SIGNAL(clicked()), this, SLOT(close()));

    // Autres boutons
    QObject::connect(aboutButton, SIGNAL(clicked()), this, SLOT(about()));
    QObject::connect(commandsButton, &QPushButton::clicked, this, &MainWindow::displayCommands);
    QObject::connect(optionsButton, SIGNAL(clicked()), this, SLOT(openOptions()));

    printVersion();
    printQuote();

    installFonts();
}
QPushButton* MainWindow::createButton(const QString &iconPath, const QString &text, const int idx ) {

    int marge = 25;
    int marginTop = 160;
    int marginEsp = 50;

    QPushButton* button = new QPushButton(text, this);
    button->setFlat(true);
    button->setIcon(QIcon(iconPath));
    button->move(marge, marginTop + marginEsp*idx);
    button->setStyleSheet(enableStylesheet);
    button->setFont(menuFont);

    return button;
}
void MainWindow::fillQuotes() {
    quotes = {"Il y a Serpang sous roche.",
              "Il y a Wailord sous gravier.",
              "Miaouss échaudé craint l'eau froide.",
              "Ne réveillez pas le Skitty qui dort.",
              "A Galopa donné on ne regarde pas la bride.",
              "Les Ponchiens aboient, la caravane passe.",
              "Petit à petit, le Roucool fait son nid.",
              "Les Medhyènas ne se mangent pas entre eux.",
              "Il ne faut pas vendre la peau de l'Ursaring avant de l'avoir capturé.",
              "Ponchiot qui aboie ne mord pas.",
              "Petit Barloche deviendra grand.",
              "La nuit, tous les Chaglams sont gris.",
              "Avoir un Chaglam dans la gorge.",
              "Quand on parle du Feunard, on en voit la queue.",
              "Quand Persian n’est pas là, les Pichus dansent.",
              "On n’apprend pas à un vieux Férosinge à faire la grimace.",
              "Un vent à décorner les Écrémeuhs.",
              "Muet comme un Magicarpe.",
              "Tôt ou tard, petit Aspicot, tu tomberas sur un bec.",
              "Un Nirondelle ne fait pas le printemps.",
              "Avoir un Mimigal au plafond.",
              "Être accroché comme un Kokiyas à son rocher.",
              "Gravalanch qui roule n'amasse pas mousse.",
              "Mygavol du soir, espoir.",
              "Statitik du matin, chagrin.",
              "Il fait un froid de Couaneton.",
              "Un caractère de Grotichon.",
              "Mettre les pieds dans l'Archéomire.",
              "Être connu comme le Zoroark chromatique.",
              "Être myope comme un Rototaupe.",
              "Être rusé comme un Feunard.",
              "Avaler des Abos.",
              "Il ne faut pas courir deux Laporeilles à la fois.",
              "Il ne faut pas mettre tous ses Noeuneufs dans la même Pokéball.",
              "Prendre le Tauros par les cornes.",
              "Avoir un oeil de Lixy.",
              "Bailler aux Cornèbres.",
              "Avoir des larmes de Crocorible.",
              "Être doux comme un Wattouat.",
              "Ça ne casse pas trois pattes à un Couaneton.",
              "Être plat comme un Limonde.",
              "Un Sharpedo de la finance.",
              "Un panier de Krabboss.",
              "Celui qui ne voit rien d'étrange n'a jamais regardé un Colhomard en face.",
              "Donner de la confiture aux Grorets.",
              "On n'attrape pas les Dardargnans avec du vinaigre.",
              "Une langue de Séviper.",
              "Avoir des Charmillons dans le ventre.",
              "Avoir des Staris dans les yeux.",
              "Ça ne se trouve pas sous le sabot d'un Ponyta.",
              "Être fait comme un Rattata.",
              "Pousser comme des Trompignons.",
              "Il court, il court, le Farfuret.",
              "Quand les Poussifeus auront des dents.",
              "Avoir une mémoire de Donphan.",
              "Avoir des Chovsourirs dans le beffroi.",
              "Les Mystherbes sont toujours plus verts chez le voisin.",
              "La bave du Crapustule n'atteint pas le blanc Lakmécygne.",
              "C'est le Simularbre qui cache la forêt.",
              "Mygavolt du soir, espoir.",
              "Ça ne casse pas trois pattes à un Canarticho.",
              "Il court, il court, le Farfuret !",
              "C'est en charpentant que l'on devient Charpenti.",
              "Être gai comme un Tylton.",
              "Quand le Noarfang chante, la nuit est silence.",
              "Il n'y a pas de Salamèche qui ne finisse par se brûler.",
              "La pensée est Chrysacier, la parole est Papilord.",
              "L'Aspicot file un mauvais Coconfort.",
              "Les mots sont des Rapasdepics qui tournent hallucinés.",
              "J'entends l'Absol, le Feunard, le Sabelette ! J'entends l",
              "A Goupix endormi, rien ne lui tombe en gueule.",
              "Lorsque le Bekipan se gratte le séant, c'est qu'il va faire mauvais temps.",
              "L’expérience est un Lanturn que l’on porte sur le dos et qui n’éclaire jamais que le chemin parcouru.",
              "L’évolution est au Barpau ce que le Pharamp est aux bateaux.",
              "Pour les Roigadas, le monde est très simplifié. Tous les Ramoloss sont des sujets.",
              "Gare en fuyant le Scorplane à éviter la morsure d'Arbok.",
              "Un Roucool dans la main vaut mieux que deux dans les hautes herbes.",
              "Tant va le Nénupiot à l'eau qu'à la fin il se lasse.",
              "Quand les Goélises ont pied, il est temps de virer.",
              "Ronflex affamé n'a point d'oreilles."
              };
}
void MainWindow::printQuote() {
    std::default_random_engine gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> dis(1, quotes.size());
    int idx = dis(gen) - 1;
    QLabel* quoteLabel = new QLabel(this);
    quoteLabel->setFixedWidth(500);
    quoteLabel->setText(quotes[idx]);
    quoteLabel->move(200 , this->height() - quoteLabel->height());
    quoteLabel->setStyleSheet("font-size: 10pt; font-weight: bold; ");
    quoteLabel->show();
    quoteLabel->setFont(pkmnFont);
}
void MainWindow::printVersion() {
    QFile file("listFiles");
    if (file.exists()) {
        QLabel* version = new QLabel(this);
        version->setText("Version " + DownloadWindow::getOldVersionNumber());
        version->move(20,20);
        version->show();
    }
}
QFont MainWindow::createFont(const QString fontPath, const int fontSize) {

    QFont font;
    QFontDatabase fontDatabase;

    int fontID = fontDatabase.addApplicationFont(fontPath);

    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(fontID);
    if(!loadedFontFamilies.empty()) {
        font = loadedFontFamilies.at(loadedFontFamilies.size()-1);
        font.setPixelSize(fontSize);
    }
    return font;
}
void MainWindow::installFonts() {
    std::vector<QString> fontsToInstall = {
        "pkmndp.ttf", "pkmnemn.ttf", "pkmnems.ttf"
    };

    // Récupération de l'installateur de polices
    QFile::copy(":assert/installFont.bat", "installFont.bat");

    // Répertoire actuel
    QString currentPath = QDir::currentPath();
    currentPath.replace('/', '\\');

    for (QString& font : fontsToInstall) {
        QFile installedFont("C:\\Windows\\Fonts\\" + font);
        if (!installedFont.exists()) {
            QFile::copy(":assert/" + font, font);
            QString cmd = "installFont.bat " + currentPath + "\\" + font;
            std::cout << cmd.toStdString() << std::endl;
            const char* cmdChar = cmd.toStdString().c_str();
            std::system(cmdChar);
            QFile file(font);
            file.setPermissions(QFileDevice::WriteOther | QFileDevice::WriteGroup | QFileDevice::WriteOwner);
            file.remove();
        }
    }
    QFile installer("installFont.bat");
    installer.setPermissions(QFileDevice::WriteOther | QFileDevice::WriteGroup | QFileDevice::WriteOwner);
    installer.remove();
}

void MainWindow::unlockDownloadButton() {
    updateButton->setStyleSheet(majStylesheet);
}
void MainWindow::endedDownload(bool success) {
    if (success) {
        updateButton->setStyleSheet(disableStylesheet);
        printVersion();
    }
    downloadWindow->setVisible(false);
    downloadWindow->cleaning(success);  // On nettoie les fichiers
    delete downloadWindow;
    downloadWindow = DownloadWindow::getDownloadWindow();
}

// Actions des boutons
void MainWindow::launchGame() {
    // A compléter
}
void MainWindow::update() {
    if (downloadWindow->isDownloadNeeded()) {
        connect(downloadWindow, &DownloadWindow::endedDownload, this, &MainWindow::endedDownload);
        downloadWindow->show();
        downloadWindow->startUpdate();
    }
}

// Ouverture des fenêtres
void MainWindow::displayCommands() {
    commandWindow->show();
}

// Liens
void MainWindow::openForum(){
    QDesktopServices::openUrl(QUrl("http://pokemonorigins.xooit.org"));
}
void MainWindow::openFacebook(){
    QDesktopServices::openUrl(QUrl("https://www.facebook.com/pages/Pok%C3%A9mon-Origins/223348097822997"));
}
void MainWindow::openTwitter(){
    QDesktopServices::openUrl(QUrl("https://twitter.com/PkmnOrigins"));
}
void MainWindow::openCSP(){
    QDesktopServices::openUrl(QUrl("http://communityscriptproject.com/"));
}

// Mouse events (déplacement de la fenêtre)
void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        this->isMoving = true;
        this->beginPos = event->pos();
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if( event->buttons().testFlag(Qt::LeftButton) && isMoving)
    {
        //if (d->isWindowEnabled){
            QPoint diff = event->pos() - this->beginPos;
            this->move(this->pos() + diff);
        //}
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        this->isMoving = false;
    }
}
