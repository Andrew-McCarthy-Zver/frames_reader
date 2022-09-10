#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "frame.h"
#include "func.h"
#include "graph.h"
#include "vertex.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "alglib\\stdafx.h"
#include "alglib\\dataanalysis.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void ReadLog(QList <Frame> &frames);
    void Framesstat (QList <Frame> &frames, Graph &graph);
    void Restart ();
    void Framesnet(Graph &graph);
    void Framesdata(Graph &graph,QList<QList<Frame>> &data);
    void Framessamples(QList<QList<Frame>> &data, QList<QList<func>> &samples_func);
    void Standartfunction(double med[],int count,func &f,int k);
    void Savefunc(QList<QList<func>> &samples_func);
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
