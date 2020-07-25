#include "word_counter.h"
#include "ui_wordcounter.h"


WordCounter::WordCounter(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WordCounter)
{
    ui->setupUi(this);
}

WordCounter::~WordCounter()
{
    delete ui;
    delete manager;
}

//download @https://www.cs.cmu.edu/~spok/grimmtmp/016.txt
void WordCounter::on_download_clicked()
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl(url_016)));
}

//download file @https://www.cs.cmu.edu/~spok/grimmtmp/012.txt
void WordCounter::on_download_2_clicked()
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl(url_012)));
}


void WordCounter::replyFinished(QNetworkReply *reply)
{
    if (reply->error())
    {
        qDebug() << "Errore!";
        qDebug() << reply->errorString();
    }
    else
    {
        QFile *file = new QFile("plaintext.txt");
        if (file->open(QFile::WriteOnly | QFile::Text))
        {
            file->write(reply->readAll());
            file->flush();
            file->close();
        }

        delete file;
        read();
    }

    reply->deleteLater();
}

void WordCounter::read()
{
    QFile file("plaintext.txt");
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "File chiuso");
    }

    QTextStream in(&file);

    QString text = in.readAll();

    ui->text->setPlainText(text);
    file.close();
}


void WordCounter::on_statistics_clicked()
{

    read();
    QFile file("plaintext.txt");
    file.open(QIODevice::ReadWrite | QIODevice::Text);

    int frase = 0;
    int carattere = 0;
    int parola = 0;
    int carattere_senza_spazio = 0;
    int parag = 0;

    bool isSpace;

    QString line;
    QTextStream in(&file);

    do
    {
        isSpace = true;
        line = in.readLine();

        for (int i = 0; i < line.size(); i++)
        {
            carattere++;

            //contralla il punto al termine di una frase, quindi incrementa frase
            if ((line.at(i) == '.'))
                frase++;

            //esclude gli spazi e conta solo i caratteri
            if ((line.at(i) != ' '))
                carattere_senza_spazio++;

            //controlla la presenza di uno spazio o trattino e incrementa parola
            if ((line.at(i) == ' ') || (line.at(i) == '-'))
                parola++;

            //conta gli spazi
            if (line.at(i) != ' ')
                isSpace = false;
        }

        if (!isSpace)
            parag++;

    } while (!line.isNull());

    file.close();
    file.remove();

    QString totale = "Numero di parole: ";
    totale.append(QString::number(parola));
    totale += "\nNumero di caratteri senza spazi: ";
    totale.append(QString::number(carattere_senza_spazio));
    totale += "\nNumero di caratteri: ";
    totale.append(QString::number(carattere));
    totale += "\nNumero di paragrafi: ";
    totale.append(QString::number(parag));
    totale += "\nNumero di frasi: ";
    totale.append(QString::number(frase));

    ui->label->setText(totale);
}

