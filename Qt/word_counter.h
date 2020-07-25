#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H

#include <QMessageBox>
#include <QMainWindow>
#include <QTextStream>
#include <QtNetwork>
#include <QFile>
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class WordCounter; }
QT_END_NAMESPACE

class WordCounter : public QMainWindow
{
    Q_OBJECT

public:
    WordCounter(QWidget *parent = nullptr);
    ~WordCounter();

private slots:
    void replyFinished(QNetworkReply*);
    void on_download_clicked();
    void on_download_2_clicked();
    void on_statistics_clicked();
    void read();

private:
    Ui::WordCounter *ui;
    QNetworkAccessManager *manager;
    QString url_016 = "https://www.cs.cmu.edu/~spok/grimmtmp/016.txt";
    QString url_012 = "https://www.cs.cmu.edu/~spok/grimmtmp/012.txt";
};
#endif // WORDCOUNTER_H
