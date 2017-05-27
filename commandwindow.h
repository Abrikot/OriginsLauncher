#ifndef COMMANDWINDOW_H
#define COMMANDWINDOW_H

#include <QVBoxLayout>
#include <QLabel>
#include <QStringList>
#include <map>
#include <vector>
#include <iostream>

#include <commandtext.h>

class CommandWindow : public QWidget
{
public:
    static CommandWindow* getCommandWindow();
private:
    static CommandWindow* window;
    CommandWindow();

    // Layout
    QVBoxLayout* layout;
};

#endif // COMMANDWINDOW_H
