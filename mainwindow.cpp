#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include <QRegularExpression>
#include <QFile>
#include "QFileInfo"
#include <QList>
#include "Frame.h"
#include "graph.h"
#include "vertex.h"
#include "edge.h"
#include <iostream>

QDateTime createdtime;

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
    path = QFileDialog::getOpenFileName(this,"Выбор файла","C:/Users/Asus/Desktop/doc/СТЦ/drones","All Files (*.*) ;; Log Files (*.log))");
    ui->lineEdit->setText(path);
}




void LogReader (QString filename,float &packetCount, float &failpacketCount, QList <Frame> &frames)
{
    QRegularExpression re("Offset");
    QRegularExpression rs("FCS=Fail");
    QRegularExpression rt("Type=");
    QRegularExpression rtm_b("Type=Management/Beacon");
    QRegularExpression rtd("Type=Data");
    QRegularExpression ta("TA=");
    QRegularExpression ra("RA=");
    QRegularExpression ra2("RA/BSSID=");
    QRegularExpression ta2("TA/BSSID=");
    QRegularExpression sz("Size=");
    QRegularExpression tm("Offset=");
    QRegularExpression m_f("More Fragments=");
    QRegularExpression sn("Seqnum");
    QRegularExpression frag("Fragnum=");

    QFile file(filename);
    QFileInfo file1(filename);
    if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
        {      QString size = "";
                QString time = "";
            QString str="";
            createdtime = file1.birthTime();

            while(!file.atEnd())
            {
                str=file.readLine();

                if (sz.match(str).hasMatch()) {
                    size = str.mid(str.indexOf("Size=")+5,5);
                }
                if (tm.match(str).hasMatch()) {
                    int time_size = str.indexOf(",",str.indexOf("Offset=")+7) - (str.indexOf("Offset=")+7);
                    time = str.mid(str.indexOf("Offset=")+7,time_size);
                }


                if (re.match(str).hasMatch())
                {
                   packetCount=packetCount+1;
                }
                if (rs.match(str).hasMatch() || rt.match(str).hasMatch()) {
                    if (rs.match(str).hasMatch()){
                       failpacketCount+=1;
                       frames.append(Frame((int)packetCount,"Не определен","Не определен"));
                    }
                    else if ((ta.match(str).hasMatch()||ta2.match(str).hasMatch()) & (ra.match(str).hasMatch()||ra2.match(str).hasMatch())) {
                       if (ra.match(str).hasMatch()&ta.match(str).hasMatch()) {frames.append(Frame((int)packetCount,str.mid(str.indexOf("TA=")+3,17),str.mid(str.indexOf("RA=")+3,17)));}
                       if (ra2.match(str).hasMatch()&ta.match(str).hasMatch()){frames.append(Frame((int)packetCount,str.mid(str.indexOf("TA=")+3,17),str.mid(str.indexOf("RA/BSSID=")+9,17)));}
                       if (ra.match(str).hasMatch()&ta2.match(str).hasMatch()){frames.append(Frame((int)packetCount,str.mid(str.indexOf("TA/BSSID=")+9,17),str.mid(str.indexOf("RA=")+3,17)));}
                       if (ra2.match(str).hasMatch()&ta2.match(str).hasMatch()){frames.append(Frame((int)packetCount,str.mid(str.indexOf("TA/BSSID=")+9,17),str.mid(str.indexOf("RA/BSSID=")+9,17)));}
                    } else if (ta.match(str).hasMatch() ||ta2.match(str).hasMatch() ){
                        if (ta.match(str).hasMatch()) {frames.append(Frame((int)packetCount,str.mid(str.indexOf("TA=")+3,17),"Не определен"));}
                        else frames.append(Frame((int)packetCount,str.mid(str.indexOf("TA/BSSID=")+9,17),"Не определен"));
                    } else if (ra.match(str).hasMatch()||ra2.match(str).hasMatch()){
                        if (ra.match(str).hasMatch()) { frames.append(Frame((int)packetCount,"Не определен",str.mid(str.indexOf("RA=")+3,17)));}
                        else frames.append(Frame((int)packetCount,"Не определен",str.mid(str.indexOf("RA/BSSID=")+9,17)));
                    } else frames.append(Frame((int)packetCount,"Не определен","Не определен"));

                    if (rtm_b.match(str).hasMatch()) {
                        int ssid_size = str.indexOf("'",str.indexOf("SSID='")+6) - (str.indexOf("SSID='")+6);
                        frames[frames.size()-1].setSSID(str.mid(str.indexOf("SSID='")+6,ssid_size));

                    }
                    if (rtd.match(str).hasMatch()) {
                        frames[frames.size()-1].setDATA("data");
                        int sn_size = str.indexOf(",",str.indexOf("Seqnum=")+7) - (str.indexOf("Seqnum=")+7);
                        frames[frames.size()-1].setSeqnum(str.mid(str.indexOf("Seqnum=")+7,sn_size));
                        int m_f_size = str.indexOf(",",str.indexOf("More Fragments=")+15) - (str.indexOf("More Fragments=")+15);
                        frames[frames.size()-1].setMore_Fragments(str.mid(str.indexOf("More Fragments=")+15,m_f_size));
                        frames[frames.size()-1].setsize(size.toInt());
                        frames[frames.size()-1].setTime(time.toDouble());
                        int frag_size = str.indexOf(",",str.indexOf("Fragnum=")+8) - (str.indexOf("Fragnum=")+8);
                        frames[frames.size()-1].setFragnum((str.mid(str.indexOf("Fragnum=")+8,frag_size)).toInt());
                    }
                }
            }
            file.close();
        }
}


#include <iostream>

void  MainWindow::Restart () {
    ui->label_5->setText("");
    ui->label_6->setText("");
    ui->label_6->setText("");
    ui->label_7->setText("");
    ui->textBrowser->setText("");
    ui->textBrowser_2->setText("");
    ui->textBrowser_3->setText("");
}

void MainWindow::TakeInfo(QList <Frame> &frames, Graph &graph, QList <Frame> &td, QList <Frame> &data)
{
    bool checkV = true;
    bool checkE = true;
    bool checkfirsttd = true;
    foreach (Frame fr, frames)
    {
        if (fr.getDATA() !="NULL" && fr.getRA() != "Не определен" && fr.getTA() != "Не определен"  ){data.append(fr);}
        bool checktd = true;
         if (fr.getSSID() != "NULL" && checkfirsttd) { td.append(fr); checkfirsttd = false;}
         if (!checkfirsttd) {
             foreach(Frame fr2, td)
             {
                 if (fr.getTA() == fr2.getTA()) checktd = false;
             }
             if (checktd && fr.getSSID() != "NULL" )  td.append(fr);
         }
         bool status = true;
         bool status2 = true;
         ui->textBrowser->insertPlainText(QString::number(fr.getNum())+":   TA: "+fr.getTA()+"    RA: "+fr.getRA()+"\n");
         if (checkV) {
            if ( fr.getRA() != "Не определен" ) {
                graph.addVertex(Vertex(graph.countVertex()+1,fr.getRA())); checkV = false;  }
            if ( fr.getTA() != "Не определен" ) {
                graph.addVertex(Vertex(graph.countVertex()+1,fr.getTA())); checkV = false; }


        }
        else {
            foreach (Vertex v, graph.getVertexes())
            {
                if (v.Address == fr.getRA() || fr.getRA() == "Не определен") {status = false;}
                if (v.Address == fr.getTA() || fr.getTA() == "Не определен") {status2 = false;}
            }

            if (status) { graph.addVertex(Vertex(graph.countVertex()+1,fr.getRA()));  }
            if (status2) { graph.addVertex(Vertex(graph.countVertex()+1,fr.getTA())); }
        }

        if ( fr.getRA() != "Не определен" && fr.getTA() != "Не определен" )
        {
            bool str =  false ;
            Edge ok = Edge(Vertex(0,""),Vertex(0,""),0);
            if (!checkE) {
            foreach (Edge m, graph.getEdges())
             {
                 if (m.getFrom().Address == fr.getTA() && m.getTo().Address == fr.getRA())
                 {
                     str = true;
                     ok = m;
                 }
             }

             if (str) {graph.countweightedge(ok);}
             else
                 {
                 Vertex p = Vertex (0,"Ошибка");
                 Vertex k  = Vertex (0,"Ошибка");
                     foreach (Vertex v, graph.getVertexes())
                     {
                         if (v.Address == fr.getRA()) { k = v;}
                         if (v.Address == fr.getTA()) { p = v;}
                     }
                  graph.addEdge(Edge(p,k,1));
                 }
            }
            else
            {
            Vertex p = Vertex (0,"Ошибка");
            Vertex k  = Vertex (0,"Ошибка");
                foreach (Vertex v, graph.getVertexes())
                {
                    if (v.Address == fr.getRA()) { k = v;}
                    if (v.Address == fr.getTA()) { p = v;}
                }
             graph.addEdge(Edge(p,k,1));
             checkE =  false ;
            }
           }
        }

}

void MainWindow::on_pushButton_2_clicked()
{

    QRegularExpression mac1("90:3a:e6");
    QRegularExpression mac2("38:1d:14");
    QRegularExpression ssid1("Parrot");
    QRegularExpression ssid2("Skydio");
    Graph graph;
    Restart();
    QList <Frame> frames;
    QList <Frame> td;
    QList <Frame> data;
    QList <Frame> da;
    QString filename = ui->lineEdit->text();
    float packetCount = 0;
    float failpacketCount = 0;
    QFileInfo fi(filename);
    if (filename == "") {
        QMessageBox::critical(this,"Ошибка","Не выбран путь к файлу.");
    }
    else if (fi.suffix() == "log") {
         LogReader (filename,packetCount,failpacketCount,frames);
        ui->label_5->setText(QString::number(packetCount));
        if (packetCount>0) {
             ui->label_6->setText(QString::number(packetCount-failpacketCount)+" ("+QString::number(round(((packetCount-failpacketCount)/packetCount)*100))+"%)");
             ui->label_7->setText(QString::number(failpacketCount)+" ("+QString::number(round(((failpacketCount)/packetCount)*100))+"%)");
             TakeInfo(frames,graph,td,data);
             bool checkfirstda = true;
             foreach(Frame fr,data) {

                if (checkfirstda) {da.append(fr); checkfirstda = false;}
                bool checkda = true;
                if (!checkfirstda) {
                foreach (Frame f,da) {
                    if (f.getTA() == fr.getTA()) {checkda = false;}
                }
                if (checkda) {da.append(fr);}
                }
             }
             bool MoreFragment = true;
             int numframe;
             int Fragnum;
             int number = 0;
             int koef = 0;
             int counter = 1;
             int count = 0;
             foreach (Frame fr,data)
             {

             if (fr.getMore_Fragments()!="0") { if(MoreFragment) {numframe =  number-koef; MoreFragment = false; Fragnum = fr.getFragnum(); } else if (Fragnum < fr.getFragnum() ) { data[numframe].setsize(data[numframe].getsize()+fr.getsize()); data.removeAt( number-koef); koef ++;} else if (Fragnum == fr.getFragnum()) {numframe = number-koef;} }
            number ++;
             }
             foreach (Frame fr,data)
             {


             if (count > 0)  {for (int vr = 1; vr<=counter; vr++) {if (fr.getSeqnum() == data[count-vr].getSeqnum() && fr.getRA() == data[count-vr].getRA() && fr.getTA() == data[count-vr].getTA()) {data.removeAt(count); count = count -1; counter--;  }}
             if (counter < 8) counter ++; }
               count ++;
             }
             foreach (Frame k, da) {
                 ui->textBrowser_3->insertPlainText("Устройство: " + k.getTA() + ": \n");
                 int mtu = 0;
             foreach(Frame fr,data) {
                 if (k.getTA() == fr.getTA() && fr.getRA() !="ff:ff:ff:ff:ff:ff") {
                  if (fr.getsize() > mtu) mtu = fr.getsize();
                  ui->textBrowser_3->insertPlainText("  Получатель: " + fr.getRA() + " Рзамер: " + QString::number(fr.getsize()) + " Время: " + createdtime.addSecs(fr.getTime()).time().toString() + " MTU: " + QString::number(mtu) + "\n");
              }
                 }

              }

        }
             int** matrix;
             matrix = new int*[graph.countVertex()];
             for(int k = 0; k < graph.countVertex(); k++)
                     matrix[k] = new int[graph.countVertex()];

             int addframes = 0;
             int tdk = 0;
             foreach (Frame fr, td) {
                 tdk++;
                 int kk = 0;
             if (mac1.match(fr.getTA()).hasMatch() || mac2.match(fr.getTA()).hasMatch())
                  ui->textBrowser_2->insertPlainText("Сеть №" + QString::number(tdk) + " (" + fr.getSSID()  + ") BSSID: " + fr.getTA() + "(дрон - совпадение mac)" +   "\n");
             else
                  if (ssid1.match(fr.getTA()).hasMatch() || ssid2.match(fr.getTA()).hasMatch())
                      ui->textBrowser_2->insertPlainText("Сеть №" + QString::number(tdk) + " (" + fr.getSSID()  + ") BSSID: " + fr.getTA() + "(дрон - совпадение ssid)" +   "\n");
                  else
                ui->textBrowser_2->insertPlainText("Сеть №" + QString::number(tdk) + " (" + fr.getSSID()  + ") BSSID: " + fr.getTA() +   "\n");
             for(int k = 0; k < graph.countVertex(); k++)
                 for(int p = 0; p < graph.countVertex(); p++)
                 {
                     QString name1 = graph.getNameVertex(k+1);
                     QString name2 = graph.getNameVertex(p+1);

                     int weight1 = graph.get(matrix)[k][p];
                     int weight2 = graph.get(matrix)[p][k];

                     if (name1 == fr.getTA() && name1 != name2  && !ui->checkBox->isChecked())
                     {
                         kk++;
                         if (mac1.match(name2).hasMatch() || mac2.match(name2).hasMatch())
                             ui->textBrowser_2->insertPlainText("    Клиент №" + QString::number(kk) + " (" + name2  + ")[дрон - совпадение mac] " + "  Принято: " + QString::number(weight1) + "   Передано: " + QString::number(weight2) +   "\n");
                         else if (ssid1.match(name2).hasMatch() || ssid2.match(name2).hasMatch())
                             ui->textBrowser_2->insertPlainText("    Клиент №" + QString::number(kk) + " (" + name2  + ")[дрон - совпадение ssid] " + "  Принято: " + QString::number(weight1) + "   Передано: " + QString::number(weight2) +   "\n");
                         else
                            ui->textBrowser_2->insertPlainText("    Клиент №" + QString::number(kk) + " (" + name2  + ") " + "  Принято: " + QString::number(weight1) + "   Передано: " + QString::number(weight2) +   "\n");
                     }
                     else if (name1 == fr.getTA() && (weight1 != 0 || weight2 !=0) )
                     {
                         kk++;
                         if (mac1.match(name2).hasMatch() || mac2.match(name2).hasMatch())
                             ui->textBrowser_2->insertPlainText("    Клиент №" + QString::number(kk) + " (" + name2  + ")[дрон - совпадение mac] " + "  Принято: " + QString::number(weight1) + "   Передано: " + QString::number(weight2) +   "\n");
                         else if (ssid1.match(name2).hasMatch() || ssid2.match(name2).hasMatch())
                             ui->textBrowser_2->insertPlainText("    Клиент №" + QString::number(kk) + " (" + name2  + ")[дрон - совпадение ssid] " + "  Принято: " + QString::number(weight1) + "   Передано: " + QString::number(weight2) +   "\n");
                         else
                            ui->textBrowser_2->insertPlainText("    Клиент №" + QString::number(kk) + " (" + name2  + ") " + "  Принято: " + QString::number(weight1) + "   Передано: " + QString::number(weight2) +   "\n");
                     }

                 }
             }
             ui->textBrowser_2->insertPlainText("Не удалось установить сеть: \n");

             for(int k = 0; k < graph.countVertex(); k++)
                 for(int p = 0; p < graph.countVertex(); p++)
                 {
                     int find = true;
                     QString name1 = graph.getNameVertex(k+1);
                     QString name2 = graph.getNameVertex(p+1);
                     int weight1 = graph.get(matrix)[k][p];
                     addframes += weight1;
                     foreach (Frame fr, td) {
                         if (name1 == fr.getTA() ||  name2 == fr.getTA() )
                             find = false;
                     }
                     if (find && !ui->checkBox->isChecked() && name1 != name2 )
                     {ui->textBrowser_2->insertPlainText("    "+ name1 +" -> " +name2 +" : " + QString::number(weight1) +"\n");}
                     else if (find && weight1 != 0) ui->textBrowser_2->insertPlainText("    "+ name1 +" -> " +name2 +" : " + QString::number(weight1) +"\n");
                 }

              ui->label_12->setText(QString::number(addframes)+" ("+QString::number(round(((addframes)/packetCount)*100))+"%)");
               ui->label_13->setText(QString::number(packetCount - addframes)+" ("+QString::number(round(((packetCount - addframes)/packetCount)*100))+"%)");

    }

    else {
        QMessageBox::critical(this,"Ошибка","Неподходящее расширение файла.");
    }
}

