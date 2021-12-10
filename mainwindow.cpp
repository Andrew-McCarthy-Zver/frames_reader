#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include <QRegularExpression>
#include <QFile>
#include "QFileInfo"
#include <winsock2.h>
#include <pcap.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString path;
    path = QFileDialog::getOpenFileName(this,"Выбор файла","C:/","All Files (*.*) ;; Log Files (*.log) ;; Pcap Files (*.pcap)");
    ui->lineEdit->setText(path);
}

float LogReader (QString filename,float &packetCount, float &failpacketCount)
{

    QRegularExpression re("Offset");
    QRegularExpression rs("FCS=Fail");
    QFile file(filename);
    if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
        {
            QString str="";
            while(!file.atEnd())
            {

                str=file.readLine();
                if (re.match(str).hasMatch())
                {
                   packetCount=packetCount+1;
                }
                if (rs.match(str).hasMatch())
                {
                   failpacketCount+=1;
                }
            }
            file.close();
        }
    return packetCount,failpacketCount;
}



float PcapReader (QString filename,u_int &packetCount)
{
    char errbuff[PCAP_ERRBUF_SIZE];
    struct pcap_pkthdr *header;
    const u_char *data;
    pcap_t * pcap = pcap_open_offline(qPrintable(filename), errbuff);

    while (int returnValue = pcap_next_ex(pcap, &header, &data) >= 0)
        {
             packetCount +=1;
        }
    pcap_close(pcap);
    return packetCount;
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->label_5->setText("");
    ui->label_6->setText("");
    ui->label_7->setText("");
    QString filename = ui->lineEdit->text();
    float packetCount = 0;
    float failpacketCount = 0;
    QFileInfo fi(filename);
    if (filename == "") {
        QMessageBox::critical(this,"Ошибка","Не выбран путь к файлу.");
    }
    else if (fi.suffix() == "log") {
        packetCount,failpacketCount = LogReader (filename,packetCount,failpacketCount);
        ui->label_5->setText(QString::number(packetCount));
        if (packetCount>0) {
             ui->label_6->setText(QString::number(packetCount-failpacketCount)+" ("+QString::number(round(((packetCount-failpacketCount)/packetCount)*100))+"%)");
             ui->label_7->setText(QString::number(failpacketCount)+" ("+QString::number(round(((failpacketCount)/packetCount)*100))+"%)");
         }

    }
    else if (fi.suffix() == "pcap")  {
        u_int packetCount = 0;
        packetCount = PcapReader(filename,packetCount);
        ui->label_5->setText(QString::number(packetCount));

    }
    else {
        QMessageBox::critical(this,"Ошибка","Неподходящее расширение файла.");
    }
}

