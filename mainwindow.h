#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef _WIN32
#include <windows.h>
#endif

// Éléments Qt
#include <QObject>
#include <QLabel>
#include <QPushButton>
#include <QFontDatabase>
#include <QDesktopServices>
#include <QMouseEvent>
#include <QPoint>

// Random
#include <random>
#include <chrono>

// Fichiers locaux
#include "downloadwindow.h"
#include "commandwindow.h"

class DownloadWindow;

class MainWindow : public QLabel
{
// Attributs
private:
    // Boutons
    QPushButton *playButton;
    QPushButton *updateButton;
    QPushButton *forumButton;
    QPushButton *exitButton;
    QPushButton *aboutButton;
    QPushButton *fbButton;
    QPushButton *twitterButton;
    QPushButton *commandsButton;
    QPushButton *optionsButton;
    QPushButton *CSPButton;

    // Polices
    QFont menuFont;
    QFont pkmnFont;

    // Styles
    QString enableStylesheet  = "font-size: 12pt; font-weight: bold; color: black;";
    QString disableStylesheet = "font-size: 12pt; font-weight: bold; color: grey;";
    QString majStylesheet = "font-size: 12pt; font-weight: bold; color: red;";

    // Liste des citations
    std::vector<QString> quotes;

    // Fenêtres
    DownloadWindow* downloadWindow;
    CommandWindow* commandWindow;

    // Déplacement de la fenêtre
    bool isMoving = false;
    QPoint beginPos;

// Méthodes
public:
    MainWindow();

private:
    void createContent();
    QPushButton* createButton(const QString &iconPath, const QString &text, const int idx );
    void fillQuotes();
    void printQuote();
    void printVersion();
    QFont createFont(const QString fontPath, const int fontSize);
    void installFonts();

private slots:
    void launchGame();

    // Download
    void unlockDownloadButton();
    void endedDownload(bool);
    void update();

    // Liens
    void openForum();
    void openFacebook();
    void openTwitter();
    void openCSP();

    // Ouverture des fenêtres
    void displayCommands();

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

};

#endif // MAINWINDOW_H
