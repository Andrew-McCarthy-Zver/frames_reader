#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include "QMessageBox"
#include <QRegularExpression>

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
    path = QFileDialog::getOpenFileName(this,"Выбор файла","C:/","All Files (*.*) ;; Log Files (*.log))");
    ui->lineEdit->setText(path);
}

void MainWindow::on_pushButton_3_clicked()
{
    QString path;
    path = QFileDialog::getOpenFileName(this,"Выбор файла","C:/","All Files (*.*) ;; Log Files (*.log))");
    ui->lineEdit_2->setText(path);
}
void MainWindow::on_pushButton_4_clicked()
{
    QString path;
    path = QFileDialog::getOpenFileName(this,"Выбор файла","C:/","All Files (*.*) ;; Log Files (*.log))");
    ui->lineEdit_3->setText(path);
}
void MainWindow::on_pushButton_5_clicked()
{
    QString filename_1 = ui->lineEdit_2->text();
    QString filename_2 = ui->lineEdit_3->text();
    QString filename_3 = filename_2;
    QRegularExpression f("/fram.+.log");
    filename_3.remove(f);
    QFile file_1(filename_1);
    QFile file_2(filename_2);
    QFile file_3(filename_3+"/frames.log");
    if ((file_1.exists())&&(file_1.open(QIODevice::ReadOnly)) && (file_2.exists())&&(file_2.open(QIODevice::ReadOnly))&& (file_3.open(QIODevice::WriteOnly)) )
        {
            QString str_1="";
            QString str_2="";
            QTextStream out(&file_3);
            while(!file_1.atEnd())
            {
                str_1=file_1.readLine();
                str_2=file_2.readLine();
                out << str_2 << "\n"<< str_1 << "\n";
            }

        }


}

void framenull (Frame &frame)
{
    frame.fragnum = NULL;
    frame.more_fragments = NULL;
    frame.number = NULL;
    frame.offset = NULL;
    frame.ra = "NULL";
    frame.seqnum = NULL;
    frame.size = NULL;
    frame.ssid = "NULL";
    frame.ta = "NULL";
    frame.type = "NULL";
}

void MainWindow::ReadLog(QList <Frame> &frames)
{
     QFile file(ui->lineEdit->text());

     if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
     {

         int framescount = 0;
         Frame currentframe;
         QRegularExpression r_offset("Offset=");
         QRegularExpression r_size("Size=");
         QRegularExpression r_type("Type=");
         QRegularExpression r_more_fragments("More Fragments=");
         QRegularExpression r_ta("TA=");
         QRegularExpression r_ra("RA=");
         QRegularExpression r_ra_1("RA/BSSID=");
         QRegularExpression r_ta_2("TA/BSSID=");
         QRegularExpression r_seqnum("Seqnum");
         QRegularExpression r_fragnum("Fragnum=");
         QRegularExpression r_fcs("FCS=Fail");
         QRegularExpression r_ssid("SSID='");

         while(!file.atEnd())
         {
              QString line = file.readLine();
              if (r_offset.match(line).hasMatch())
              {
                  framenull(currentframe);
                  framescount++;
                  currentframe.number = framescount;
                  currentframe.offset =  (line.mid(line.indexOf("Offset=")+7,line.indexOf(",",line.indexOf("Offset=")+7) - (line.indexOf("Offset=")+7))).toDouble();
              }
              if (r_size.match(line).hasMatch())
              {
                  currentframe.size = (line.mid(line.indexOf("Size=")+5,line.indexOf(",",line.indexOf("Size=")+5) - (line.indexOf("Size=")+5))).toInt();
              }
              if (r_type.match(line).hasMatch())
              {
                  currentframe.type = line.mid(line.indexOf("Type=")+5,line.indexOf(",",line.indexOf("Type=")+5) - (line.indexOf("Type=")+5));
              }
              if (r_more_fragments.match(line).hasMatch())
              {
                  currentframe.more_fragments = (line.mid(line.indexOf("More Fragments=")+15,line.indexOf(",",line.indexOf("More Fragments=")+15) - (line.indexOf("More Fragments=")+15))).toInt();
              }
              if (r_ta.match(line).hasMatch())
              {
                  currentframe.ta = line.mid(line.indexOf("TA=")+3,17);
              }
              if (r_ra.match(line).hasMatch())
              {
                  currentframe.ra = line.mid(line.indexOf("RA=")+3,17);
              }
              if (r_ra_1.match(line).hasMatch())
              {
                  currentframe.ra= line.mid(line.indexOf("RA/BSSID=")+9,17);
              }
              if (r_ta_2.match(line).hasMatch())
              {
                  currentframe.ta= line.mid(line.indexOf("TA/BSSID=")+9,17);
              }
              if (r_seqnum.match(line).hasMatch())
              {
                  currentframe.seqnum= (line.mid(line.indexOf("Seqnum=")+7,line.indexOf(",",line.indexOf("Seqnum=")+7) - (line.indexOf("Seqnum=")+7))).toInt();
              }
              if (r_fragnum.match(line).hasMatch())
              {
                  currentframe.fragnum= (line.mid(line.indexOf("Fragnum=")+8,line.indexOf(",",line.indexOf("Fragnum=")+8) - (line.indexOf("Fragnum=")+8))).toInt();
              }
              if (r_fcs.match(line).hasMatch())
              {
                  currentframe.type = "Fail";
              }
              if (r_ssid.match(line).hasMatch())
              {
                  currentframe.ssid= line.mid(line.indexOf("SSID='")+6,line.indexOf(",",line.indexOf("SSID='")+6) - (line.indexOf("SSID='")+6)-1);
              }
              if (currentframe.offset != NULL && currentframe.type != "NULL")
              {
                  frames.append(currentframe);
                  framenull(currentframe);
              }


         }
         file.close();
     }


}

void  MainWindow::Framesstat (QList <Frame> &frames, Graph &graph)
{
    double failframes = 0;
    double framescount = frames.size();
    double adrframes = 0;
    ui->textBrowser->insertPlainText("Количество фреймов: " + QString::number(framescount)+"\n");
    QList <QString> vx;
    int k = 1;
    foreach(Frame fr, frames)
    {
       if ( fr.type == "Fail") failframes++;
       if (fr.ra != "NULL" && fr.ta != "NULL")
       {
           adrframes++;
           graph.edges.append(fr);
           if (!vx.contains(fr.ta)) {vx.append(fr.ta); graph.vertexes.append(Vertex(k,fr.ta,fr.ssid,fr.type,fr.type == "Management/Beacon"?true:false));k++;} else  if (fr.type == "Management/Beacon") {graph.vertexes[vx.indexOf(fr.ta)].t_d = true; graph.vertexes[vx.indexOf(fr.ta)].ssid = fr.ssid; }
           if (!vx.contains(fr.ra)) {vx.append(fr.ra); graph.vertexes.append(Vertex(k,fr.ra,fr.ssid,fr.type,false));k++;}
       }
       ui->textBrowser_2->insertPlainText(QString::number(fr.number)+"  TA: "+(fr.ta!="NULL"?fr.ta+" RA: ":"Не определен   RA: ")+(fr.ra!="NULL"?fr.ra+"\n":"Не определен\n"));
    }
   graph.Form();
    ui->textBrowser->insertPlainText("Количество корректных фреймов: " + QString::number(framescount - failframes) + "("+ QString::number(round((framescount-failframes)*100/framescount))+"%)\n");
    ui->textBrowser->insertPlainText("Количество некорректных фреймов: " + QString::number(failframes) + "(" + QString::number(round(failframes*100/framescount)) + "%)\n");
    ui->textBrowser->insertPlainText("Количество фреймов с TA и RA: " + QString::number(adrframes) + "(" + QString::number(round(adrframes*100/framescount)) + "%)\n");
    ui->textBrowser->insertPlainText("Количество фреймов без адресной информации: " + QString::number(framescount - adrframes) + "(" + QString::number(round((framescount-adrframes)*100/framescount)) + "%)\n");
}

void  MainWindow::Restart () {
    ui->textBrowser->setText("");
    ui->textBrowser_2->setText("");
    ui->textBrowser_3->setText("");
    ui->textBrowser_4->setText("");
    ui->textBrowser_5->setText("");
    ui->textBrowser_6->setText("");
}

void  MainWindow::Framesnet(Graph &graph)
{
    int k = 0;
    bool w=true;
    foreach(Vertex v,graph.vertexes)
    {
        if (v.t_d)
        {
            k++;
            ui->textBrowser_3->insertPlainText("Сеть №"+ QString::number(k) +" ("+v.ssid+")"+" BSSID: "+v.address+"\n");
            int m = 0;
            for (int i=0; i<graph.vertexes.size();i++)
            {
                if ((graph.matrix[v.num-1][i]!=0 || graph.matrix[i][v.num-1]!=0))
                {

                    if (graph.vertexes[i].address!="ff:ff:ff:ff:ff:ff")
                    {
                    m++;
                    ui->textBrowser_3->insertPlainText("    Клиент №"+ QString::number(m) +" ("+graph.vertexes[i].address+")"+" Принято: "+QString::number(graph.matrix[v.num-1][i])+" Передано:" +  QString::number(graph.matrix[i][v.num-1])+"\n");
                    }

                    }
            }
        }
    }
    foreach(Vertex v,graph.vertexes)
    {
        if (!v.t_d)
        {
            for (int i=0; i<graph.vertexes.size();i++)
            {
                if (graph.matrix[v.num-1][i]!=0  && !graph.vertexes[i].t_d && (v.num-1)!=i)
                {
                    if (w && graph.vertexes[i].address!="ff:ff:ff:ff:ff:ff") {ui->textBrowser_3->insertPlainText("Не удалось установить сеть:\n"); w=false;}
                    if (graph.vertexes[i].address!="ff:ff:ff:ff:ff:ff")
                    ui->textBrowser_3->insertPlainText("    "+v.address+" -> " + graph.vertexes[i].address + ": " + QString::number(graph.matrix[v.num-1][i])+"\n");
                }
            }
        }


    }
}

void  MainWindow::Framesdata(Graph &graph, QList<QList<Frame>> &data)
{
    QRegularExpression r_dat("Data");
    QFileInfo file(ui->lineEdit->text());
    QDateTime createdtime = file.birthTime();
    QList<Frame> dat = graph.edges;
    int count = 1;
    bool mf = false;
    int seqnum;
    int count_2 =0;
    int k;
    int fragnum;
    foreach(Frame fr,dat)
    {
            for (int i=1;i<count&&i<8;i++)
            {
                if (fr.ra == dat[count-i-1].ra && fr.ta == dat[count-i-1].ta && fr.seqnum == dat[count-i-1].seqnum && fr.more_fragments == dat[count-i-1].more_fragments && fr.fragnum == dat[count-i-1].fragnum ) {dat.removeAt(count-i-1); count--;}
            }
            count++;
    }
    foreach(Frame fr,dat)
    {
           if (!mf && fr.more_fragments!=0 && fr.fragnum==0) {mf = true; seqnum = fr.seqnum; k = count_2; fragnum=0;}
           if (mf&&fr.seqnum==seqnum&&fr.fragnum>fragnum)
           {
               fragnum = fr.fragnum;
               if(fr.more_fragments==0) mf = false;
               dat[k].size += fr.size;
               dat.remove(count_2);
               count_2--;
           }
           count_2++;

    }
    int d = 0;
    foreach(Frame fr,dat)
    {
        if (!r_dat.match(fr.type).hasMatch() || fr.ta == "ff:ff:ff:ff:ff:ff" || fr.ra == "ff:ff:ff:ff:ff:ff")
        {
        dat.remove(d);
        d--;
        }
        d++;
    }
    int num = 0;
    foreach(Vertex v,graph.vertexes)
    {
        bool m = true;
        foreach(Frame fr,dat)
        {
            if  (fr.ta == v.address )
            {
                if (m) { ui->textBrowser_4->insertPlainText("Устройство: " + fr.ta + ":\n"); data.append(QList<Frame>{fr}); num++;  m = false;}
                else
                data[num-1].append(fr);
                ui->textBrowser_4->insertPlainText("Получатель: " + fr.ra + " Размер: "+QString::number(fr.size)+" Время:"+createdtime.addSecs(fr.offset).time().toString()+"\n");
            }
        }
    }
}
int compare(const void *a, const void *b)
{
    const double *ad, *bd;

    ad = (const double*)a;
    bd = (const double*)b;

    if (*ad < *bd)
    {
        return -1;
    }
    else if (*ad > *bd)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void  MainWindow::Standartfunction (double med[],int count)
{
    qsort(med, count, sizeof(double), compare);
    double mean = 0;
    double median = 0;
    for (int i = 0; i < count; i++)
    {
        mean += med[i];
    }
    mean /= count;
    floor((count+1)/2) == (count+1)/2?median = med[int((count+1)/2)]:median = (med[int((count+1)/2)-1] + med[int((count+1)/2)])/2;
    double medad[count];
    for (int i = 0; i < count; i++)
    {
        medad[i]=fabs(med[i]-median);
    }
    qsort(medad, count, sizeof(double), compare);
    double medianad = 0;
    floor((count+1)/2) == (count+1)/2?medianad = medad[int((count+1)/2)]:medianad = (medad[int((count+1)/2)-1] + medad[int((count+1)/2)])/2;
    double sd = 0;
    double variance = 0;
    for (int i = 0; i < count; i++)
    {
        sd += pow(med[i]-mean,2);
    }
    variance = sd/(count -1);
    sd = sqrt(sd/(count - 1));
    double m_sq = 0;
    for (int i = 0; i < count; i++)
    {
        m_sq += pow(med[i],2);
    }
    m_sq = m_sq/count;
    double rms = sqrt(m_sq);
    double p_skewness = 3*(mean-median)/variance;
    double kurtosys = 0;
    double skewness = 0;
    for (int i = 0; i < count; i++)
    {
        kurtosys += pow((med[i]-mean)/variance,4);
        skewness += pow((med[i]-mean)/variance,3);
    }
    kurtosys =  kurtosys/count;
    skewness =  skewness/count;

    ui->textBrowser_6->insertPlainText("        standart deviation =  "+QString::number(sd)+"\n");
    ui->textBrowser_6->insertPlainText("        variance =  "+QString::number(variance)+"\n");
    ui->textBrowser_6->insertPlainText("        root mean square =  "+QString::number(rms)+"\n");
    ui->textBrowser_6->insertPlainText("        m_square =  "+QString::number(m_sq)+"\n");
    ui->textBrowser_6->insertPlainText("        p_skewness =  "+QString::number(p_skewness)+"\n");
    ui->textBrowser_6->insertPlainText("        kurtosys =  "+QString::number(kurtosys)+"\n");
    ui->textBrowser_6->insertPlainText("        skewness=  "+QString::number(skewness)+"\n");
    ui->textBrowser_6->insertPlainText("        min=  "+QString::number(med[0])+"\n");
    ui->textBrowser_6->insertPlainText("        max=  "+QString::number(med[count-1])+"\n");
    ui->textBrowser_6->insertPlainText("        mean =  "+QString::number(mean)+"\n");
    ui->textBrowser_6->insertPlainText("        median =  "+QString::number(median)+"\n");
    ui->textBrowser_6->insertPlainText("        medianAD =  "+QString::number(medianad)+"\n");
}

void MainWindow::Framessamples(QList<QList<Frame>> &data)
{
    QList<QList<QList<Frame>>> samples;
    QFileInfo file(ui->lineEdit->text());
    QDateTime createdtime = file.birthTime();

    foreach (QList<Frame> frames,data)
    {
        int num = 0;
        int size = frames.size();
        int count = 1;
        QList<QList<Frame>> sampl;
        sampl.append(QList<Frame>{frames[0]});
        while (count < size)
        {
            if (count+20<size)
            {
                for (int i =1; i<20;i++)
                {
                    sampl[num].append(frames[count]);
                    count++;
                }
                count -= 13;
                sampl.append(QList<Frame>{frames[count]});
                count ++;
                num++;
            }
            else
            {
                for(int i=0;i<size-count;i++)
                {
                    sampl[num].append(frames[count]);
                    count++;
                }
            }
        }
        samples.append(sampl);
    }

   foreach(QList<QList<Frame>> devices,samples)
    {
        ui->textBrowser_5->insertPlainText("Устройство: " +devices[0][0].ta + ":\n");
        int k = 1;
        double mtu = 0;
        foreach(QList<Frame> sample,devices)
        {
            ui->textBrowser_5->insertPlainText("    Выборка № "+QString::number(k)+":\n");
            ui->textBrowser_6->insertPlainText("\n Выборка № "+QString::number(k)+":\n");
            k++;
            int count = 0;
            double med[sample.size()];
            double offsets[sample.size()];
            int allsize = 0;
            double pivotsize = 0;
            bool notpivotsize = false;
            foreach(Frame fr, sample)
            {
                allsize += fr.size;
                med[count] = fr.size;
                offsets[count] = fr.offset;
                if (fr.size>mtu) mtu = fr.size;
                if (count==1 && sample.size()>count+1)
                {
                    if (fr.size < mtu && sample[count+1].size == mtu && sample[count-1].size==mtu )
                    {
                        ui->textBrowser_5->insertPlainText("            Получатель: " + fr.ra + " Размер: "+QString::number(fr.size)+" Время:"+createdtime.addSecs(fr.offset).time().toString()+" [*]\n");
                        if (pivotsize==0||fr.size == pivotsize) { pivotsize = fr.size; }
                        else {notpivotsize = true;}
                    }
                    else
                    ui->textBrowser_5->insertPlainText("            Получатель: " + fr.ra + " Размер: "+QString::number(fr.size)+" Время:"+createdtime.addSecs(fr.offset).time().toString()+"\n");
                }
                else if (count ==2 && sample.size()>count+1)
                {
                    if (fr.size < mtu && sample[count+1].size == mtu && (sample[count-1].size==mtu||sample[count-2].size==mtu) )
                    {
                        ui->textBrowser_5->insertPlainText("            Получатель: " + fr.ra + " Размер: "+QString::number(fr.size)+" Время:"+createdtime.addSecs(fr.offset).time().toString()+" [*]\n");
                        if (pivotsize==0||fr.size == pivotsize) { pivotsize = fr.size; }
                        else {notpivotsize = true;}
                    }
                    else
                    ui->textBrowser_5->insertPlainText("            Получатель: " + fr.ra + " Размер: "+QString::number(fr.size)+" Время:"+createdtime.addSecs(fr.offset).time().toString()+"\n");
                }
                else if (count>2 && count <19 && sample.size()>count+1)
                {
                    if (fr.size < mtu && sample[count+1].size == mtu && (sample[count-1].size==mtu||sample[count-2].size==mtu|| sample[count-3].size==mtu) )
                    {
                        ui->textBrowser_5->insertPlainText("            Получатель: " + fr.ra + " Размер: "+QString::number(fr.size)+" Время:"+createdtime.addSecs(fr.offset).time().toString()+" [*]\n");
                        if (pivotsize==0||fr.size == pivotsize) { pivotsize = fr.size; }
                        else {notpivotsize = true;}
                    }
                    else
                    ui->textBrowser_5->insertPlainText("            Получатель: " + fr.ra + " Размер: "+QString::number(fr.size)+" Время:"+createdtime.addSecs(fr.offset).time().toString()+"\n");
                }
                else
                ui->textBrowser_5->insertPlainText("            Получатель: " + fr.ra + " Размер: "+QString::number(fr.size)+" Время:"+createdtime.addSecs(fr.offset).time().toString()+"\n");
                count++;

            }
            if (notpivotsize) pivotsize = 0;
            ui->textBrowser_6->insertPlainText("  Уникальные функции: \n");
            ui->textBrowser_6->insertPlainText("    pivot size: " + QString::number(pivotsize) + " \n");
            double pm = pivotsize/mtu;
            ui->textBrowser_6->insertPlainText("    PM: " + QString::number(pm) + " \n");
            double pt = pivotsize/allsize;
            ui->textBrowser_6->insertPlainText("    PT: " + QString::number(pt) + " \n");
            ui->textBrowser_6->insertPlainText("\n  Стандартные функции по размерам пакетов: \n");
            Standartfunction(med,count);
            ui->textBrowser_6->insertPlainText("\n  Стандартные функции по времени прибытия: \n");
            Standartfunction(offsets,count);
        }
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    Restart();
    QList <Frame> frames;
    Graph graph;
    QList<QList<Frame>> data;
    ReadLog(frames);
    Framesstat(frames,graph);
    Framesnet(graph);
    Framesdata(graph,data);
    Framessamples(data);
    //    qDebug() <<  ;

}

