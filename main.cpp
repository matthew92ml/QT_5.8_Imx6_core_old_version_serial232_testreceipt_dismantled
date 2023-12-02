#include <QCoreApplication>
#include <qextserialport.h>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

QextSerialPort* portPrinter;
void apriPorta();
void printBarcodeSerialTest(QextSerialPort *port, QString peso);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    apriPorta();
    printBarcodeSerialTest(portPrinter,"0.0");
    return a.exec();
}

void apriPorta(){
    QString printerPort = "/dev/ttymxc3";
    portPrinter = new QextSerialPort(printerPort);
    portPrinter->setDataBits(DATA_8);
    portPrinter->setStopBits(STOP_1);
    portPrinter->setParity(PAR_NONE);
    portPrinter->setFlowControl(FLOW_OFF);
    portPrinter->setTimeout(100);
    bool out = portPrinter->open(QIODevice::ReadWrite | QIODevice::Unbuffered);
    qDebug()<<"OPEN: "<<out;

}

void printBarcodeSerialTest(QextSerialPort *port, QString peso){

    char sep = 'X';

    QString lastDays = "0";
    QString idComune = "0";
    QString idStazione = "X";
    QString nomeComune ="TEST";
    QString uid = "DUMMY";

    QString rdPersonale = "99.9%";
    rdPersonale.append(' ').append(lastDays);

    QString tipoRifiuto = "99";
    QString descRifiuto  = "RIFIUTO TEST";

    QString barcodeHumanRead = uid;
    barcodeHumanRead.append(' ');
    barcodeHumanRead.append(peso).append(' ');
   // barcodeHumanRead.append(tipoRifiuto).append(' ');
    barcodeHumanRead.append(idComune).append('/').append(nomeComune).append(' ').append(idStazione);


    QString pesocoded = peso;
    if(pesocoded.contains('.')){
        pesocoded.replace(".","X");
    }
    else pesocoded.append("X0");

    QString etichetteRimaste = "100";

    QString dateAndLabel = QDateTime::currentDateTime().toString("yyMMddHHmm");
    dateAndLabel.append(" ").append(etichetteRimaste);


    QString barcodeRaw = uid;
    QString barcodeRaw2 = pesocoded;
    barcodeRaw2.append(sep);
    barcodeRaw2.append(tipoRifiuto);
    barcodeRaw2.append(sep);
    barcodeRaw2.append(idComune);
    barcodeRaw2.append(sep);
    barcodeRaw2.append(idStazione);

    QString inName = "/home/user/ECOFILL/GRD/data/barcode.txt";
    QFile fileIn(inName);

    if(fileIn.open(QIODevice::ReadOnly)){
        QTextStream inFile(&fileIn);
        QString inString = inFile.readAll();

        inString.replace("XXXXXXXXXX",barcodeHumanRead);
        inString.replace("YYYYYYYYYY",descRifiuto);
        inString.replace("DDDDDDDDDD",dateAndLabel);
        inString.replace("ZZZZZZZZZZ",barcodeRaw);
        inString.replace("WWWWWWWWWW",barcodeRaw2);
        inString.replace("**********",rdPersonale);

        fileIn.close();
        port->write(inString.toStdString().c_str());
   }
    else qDebug()<<"ERR FILEOPEN";
}
