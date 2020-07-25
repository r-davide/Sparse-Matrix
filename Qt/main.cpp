#include "word_counter.h"
#include <QtCore>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WordCounter w;
    w.show();
    return a.exec();
}
