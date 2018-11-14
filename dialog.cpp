/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include<iostream>
#include<cstdio>
#include<vector>
#include<cmath>
#include<algorithm>
#include<fstream>
#include<string>
#include<QTime>

#include <QtWidgets>
#include "mywidget.h"
#include "dialog.h"
#include <QFile>
#include<QString>
#include<style.h>
#include<QLabel>
#include "wdg.h"

using namespace std;

//! [0]
Dialog::Dialog()
{
    myTimer = new QTime;
    setStyleSheet(styleg);
    createMenu();
    createHorizontalGroupBox();

    bigEditor = new MyWidget();
    bigEditor->color = 1;
    bigEditor->tsplen = 0;
    bigEditor->flag = 1;
    connect(bigEditor, SIGNAL(accepted()), this, SLOT(accept()));
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                     | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    //menuBar->setStyleSheet("padding: 0 0 0 0; color: cyan;");

    mainLayout->setMenuBar(menuBar);
    //horizontalGroupBox->setContentsMargins(0, -1000, 0, 0);
    horizontalGroupBox->setStyleSheet("max-height: 70;");
    mainLayout->addWidget(horizontalGroupBox);
    mainLayout->addWidget(bigEditor);
    len = new QLabel("length: ");
    cnt = new QLabel("vertex_count: ");
    time = new QLabel("time_work: ");
    inform = new QHBoxLayout;
    inform->addWidget(len);
    inform->addWidget(cnt);
    inform->addWidget(time);
    inform1 = new QGroupBox;
    inform1->setLayout(inform);
    inform1->setStyleSheet("max-height: 50;");
    mainLayout->addWidget(inform1);
//! [4] //! [5]
    setLayout(mainLayout);

    setWindowTitle(tr("TspSolver"));
}
//! [5]

//! [6]
void Dialog::createMenu()
{
    menuBar = new QMenuBar;
    fileMenu = new QMenu(tr("&File"), this);
    exitAction = fileMenu->addAction(tr("E&xit"));
    menuBar->addMenu(fileMenu);

    connect(exitAction, SIGNAL(triggered()), this, SLOT(accept()));
}
//! [6]

//! [7]
void Dialog::createHorizontalGroupBox()
{
    horizontalGroupBox = new QGroupBox();
    QHBoxLayout *layout = new QHBoxLayout;
    buttons[0] = new QPushButton(tr("open"));
    buttons[1] = new QPushButton(tr("save graph"));
    buttons[2] = new QPushButton(tr("new cluster"));
    buttons[3] = new QPushButton(tr("new vertex"));
    buttons[4] = new QPushButton(tr("run algoritm"));
    buttons[5] = new QPushButton(tr("clear"));
    buttons[6] = new QPushButton(tr("save solve"));
    for (int i = 0; i < NumButtons; ++i) {

        layout->addWidget(buttons[i]);
    }
    connect(buttons[0], SIGNAL (clicked()), this, SLOT (openfile()));
    connect(buttons[1], SIGNAL (clicked()), this, SLOT (savefile()));
    connect(buttons[5], SIGNAL (clicked()), this, SLOT (clear()));
    connect(buttons[2], SIGNAL (clicked()), this, SLOT (newcluster()));
    connect(buttons[3], SIGNAL (clicked()), this, SLOT (newvertex()));
    connect(buttons[4], SIGNAL (clicked()), this, SLOT (solve()));
    connect(buttons[6], SIGNAL (clicked()), this, SLOT (savetext()));
    buttons[0]->setObjectName("openbutton");
    buttons[1]->setObjectName("savebutton");
    buttons[2]->setObjectName("newcluster");
    buttons[3]->setObjectName("newvertex");
    buttons[4]->setObjectName("runalgorim");
    buttons[5]->setObjectName("clear");
    buttons[6]->setObjectName("savetext");
    horizontalGroupBox->setLayout(layout);
}

void Dialog::savetext(){
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save ans file"), "", tr("TSP Files (*.txt)"));
    QFile file( fileName );
    QString dat;
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);
        out << "length: " + QString("%3").arg(bigEditor->tsplen) << endl;
        for (int i = 0; i < bigEditor->resx.size(); i++){
            out << QString("%0").arg(bigEditor->resx[i]) + " " + QString("%0").arg(bigEditor->resy[i]) << endl;
        }
        file.close();
}



void Dialog::openfile(){
    //buttons[0]->setText("Example");

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open TSP file"), "", tr("TSP Files (*.tsp)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           return;

    QTextStream in(&file);
    vector < string > foroutput;
    bool flag = false;
    bool flag1 = false;
    QStringList sl;
    int number, k, tek = -1;
    int x1, y1;
    clear();
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QString tp;
        string s, s1;
        s = line.toStdString();

        if (s == "")
                return;
         if (flag) {
                try {
                 sl = line.split(" ");
                 if (sl.size() != 4){
                     QMessageBox *qm = new QMessageBox;
                     qm->setText("INCORRECT DATA, VERTEX NOT ADDED");
                     qm->setWindowTitle("error");
                     qm->show();
                     clear();
                     return;
                 }
                 tp = sl[0];
                 number = tp.toInt();
                 tp = sl[1];
                 k = tp.toInt();
                 tp = sl[2];
                 x1 = tp.toInt();
                 tp = sl[3];
                 y1 = tp.toInt();
                 if (number <= 0 || k <= 0 || x1 <= 0 || y1 <= 0){
                                      QMessageBox *qm = new QMessageBox;
                                      qm->setText("INCORRECT DATA, VERTEX NOT ADDED");
                                      qm->setWindowTitle("error");
                                      qm->show();
                                      clear();
                                      return;
                                  }
                 if (k == bigEditor->color){
                     bigEditor->xk.push_back(x1);
                     bigEditor->yk.push_back(y1);
                     bigEditor->temp = (100 + (31 * bigEditor->color) % 156) * 256 * 256 + ((83 * bigEditor->color) % 256) * 256 + (157 * bigEditor->color) % 256;
                     bigEditor->colors.push_back(bigEditor->temp);
                     bigEditor->update();
                 }
                 else if (k == bigEditor->color + 1){
                     bigEditor->color++;
                     bigEditor->xk.push_back(x1);
                     bigEditor->yk.push_back(y1);
                     bigEditor->temp = (100 + (31 * bigEditor->color) % 156) * 256 * 256 + ((83 * bigEditor->color) % 256) * 256 + (157 * bigEditor->color) % 256;
                     bigEditor->colors.push_back(bigEditor->temp);
                     bigEditor->update();
                 }
                 else{
                     //cout << "INCORRECT!";
                 }
                }
                catch (exception) {
                    //cout << "INCORRECT!";
                }
            }
         if (flag1){
             try {
              sl = line.split(" ");
              if (sl.size() != 4){
                  //cout << "INCORRECT!";
              }
              tp = sl[0];
              number = tp.toInt();
              tp = sl[1];
              k = tp.toInt();
              tp = sl[2];
              x1 = tp.toInt();
              tp = sl[3];
              y1 = tp.toInt();
              if (k == bigEditor->color){
                  bigEditor->xk.push_back(x1);
                  bigEditor->yk.push_back(y1);
                  bigEditor->resx.push_back(x1);
                  bigEditor->resy.push_back(y1);
                  bigEditor->temp = (100 + (31 * bigEditor->color) % 156) * 256 * 256 + ((83 * bigEditor->color) % 256) * 256 + (157 * bigEditor->color) % 256;
                  bigEditor->colors.push_back(bigEditor->temp);
                  bigEditor->update();
              }
              else if (k == bigEditor->color + 1){
                  bigEditor->xk.push_back(x1);
                  bigEditor->yk.push_back(y1);
                  bigEditor->color++;
                  bigEditor->resx.push_back(x1);
                  bigEditor->resy.push_back(y1);
                  bigEditor->temp = (100 + (31 * bigEditor->color) % 156) * 256 * 256 + ((83 * bigEditor->color) % 256) * 256 + (157 * bigEditor->color) % 256;
                  bigEditor->colors.push_back(bigEditor->temp);
                  bigEditor->update();
              }
              else{
                  //cout << "INCORRECT!";
              }
             }
             catch (exception) {
                 //cout << "INCORRECT!";
             }
         }
            if (s == "NODE_COORD_SECTION")
                flag = true;
            if (s == "TOUR_SECTION")
                flag1 = true;
            foroutput.push_back(s);
        }
    if(flag1){
        bigEditor->resx.push_back(x1);
        bigEditor->resy.push_back(y1);
        bigEditor->tsplen = 0;
        for (int i = 1; i < bigEditor->resx.size(); i++){
            bigEditor->tsplen += sqrt((bigEditor->resx[i] - bigEditor->resx[i - 1]) * (bigEditor->resx[i] - bigEditor->resx[i - 1]) + (bigEditor->resy[i] - bigEditor->resy[i - 1]) * (bigEditor->resy[i] - bigEditor->resy[i - 1]));
        }
        improve_info();
        bigEditor->update();
    }
    bigEditor->flag = 0;
}

void Dialog::savefile(){
    //buttons[0]->setText("Example");
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save TSP file"), "", tr("TSP Files (*.tsp)"));
    QFile file( fileName );
    QString dat;
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);
        int nm = bigEditor->resx.size();
        out << "<name>" << endl;
        out << "TOUR_SECTION" << endl;
        //out <<  QString("%0").arg() + " " + QString("%0").arg(1) << " ";
        //out <<  QString("%0").arg(bigEditor->resx[0]) + " " + QString("%0").arg(bigEditor->resy[0]) << endl;
        for (int i = 0; i < max((int)bigEditor->resx.size() -1 , 0); i++){
            out <<  QString("%0").arg(i + 1) + " " + QString("%0").arg(i + 1) << " ";
            out <<  QString("%0").arg(bigEditor->resx[i]) + " " + QString("%0").arg(bigEditor->resy[i]) << endl;
        }
        out <<  QString("%0").arg(1) + " " + QString("%0").arg(1) << " ";
        out <<  QString("%0").arg(bigEditor->resx[0]) + " " + QString("%0").arg(bigEditor->resy[0]) << endl;
        file.close();
}

void Dialog::clear(){
    bigEditor->xk.clear();
    bigEditor->yk.clear();
    bigEditor->resx.clear();
    bigEditor->resy.clear();
    bigEditor->color = 0;
    bigEditor->colors.clear();
    bigEditor->tsplen = 0;
    improve_info();
    bigEditor->update();
    bigEditor->flag = 0;
    newcluster();
}

void Dialog::newcluster(){
    if (bigEditor->flag == 0){
    bigEditor->color++;
    bigEditor->update();
    }
    else{
        //exception
    }
    bigEditor->flag = 1;
}


void Dialog::newvertex(){
    q1 = new QDialog();
    q1->setWindowTitle(tr("AddVertex"));
    linex = new QLineEdit;
    liney = new QLineEdit;
    //connect(linex, SIGNAL (clicked()), this, SLOT (out()));
    //connect(liney, SIGNAL (clicked()), this, SLOT (out()));
    QHBoxLayout *verlay = new QHBoxLayout(this);
    verlay->addWidget(linex);
    verlay->addWidget(liney);
    q1->setLayout(verlay);
    QHBoxLayout *verlay1 = new QHBoxLayout(this);
    QLabel *inf1 = new QLabel("x(positive int)");
    QLabel *inf2 = new QLabel("y(positive int)");
    verlay1->addWidget(inf1);
    verlay1->addWidget(inf2);
    QVBoxLayout *verlay2 = new QVBoxLayout(this);
    QGroupBox *up = new QGroupBox;
    up->setLayout(verlay1);
    QGroupBox *down = new QGroupBox;
    QPushButton *enter = new QPushButton(tr("enter"));
    connect(enter, SIGNAL (clicked()), this, SLOT (out()));
    down->setLayout(verlay);
    verlay2->addWidget(up);
    verlay2->addWidget(down);
    verlay2->addWidget(enter);
    q1->setLayout(verlay2);
    q1->setStyleSheet(styleg);
    q1->setFixedSize(350, 160);
    q1->show();
    q1->setModal(true);
    q1->setWindowModality(Qt::ApplicationModal);
}

void Dialog::out(){
    q1->close();
    //int x1, y1;
    int x1 = linex->text().toInt();
    int y1 = liney->text().toInt();
        if (x1  > 0 && y1 > 0) {
         bigEditor->xk.push_back(x1);
         bigEditor->yk.push_back(y1);
         bigEditor->temp = (100 + (31 * bigEditor->color) % 156) * 256 * 256 + ((83 * bigEditor->color) % 256) * 256 + (157 * bigEditor->color) % 256;
         bigEditor->colors.push_back(bigEditor->temp);
         bigEditor->flag = 0;
         bigEditor->update();
    }
        else {

            QMessageBox *qm = new QMessageBox;
            qm->setText("INCORRECT DATA, VERTEX NOT ADDED");
            qm->setWindowTitle("error");
            qm->show();
            return;
    }
}












const double PSI = 0.75;
const double INF = 10000000000;
const int L = 500;
int NP = 5;
const double EPS = 0.0001;


struct ver {
    double x, y;
    int cluster;
    bool inT = false;
    int cnt_of_T = 0;
    double dist_to_center = 0;
    bool tabu = false;
    int tabu_cnt = 0;
    int num;
    vector < int > np;
    ver(double x = 0, double y = 0) {
        this->x = x;
        this->y = y;
    }
    double len(ver a) {
        return (a.x - x) * (a.x - x) + (a.y - y) * (a.y - y);
    }
    double len2(ver a) { return sqrt(len(a)); }
    bool operator ==(ver a) {
        if (x == a.x && y == a.y && cluster == a.cluster)
            return true;
        return false;
    }
    bool operator !=(ver a) {
        return !(this->operator==(a));
    }
};


struct Neigbohour {
    bool fashionable;
    double length;
    vector < int > used;
    vector < ver > vr;
};


struct subgroup {
    vector < ver > vr;
    ver represent;
    int rep_ind;
};


struct cluster {
    vector < subgroup > sb;
    vector < ver > vr;
};


double dispertion(subgroup s) {
    int n = s.vr.size();
    if (n == 1) return 0;
    double ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            ans += s.vr[i].len2(s.vr[j]);
        }
    }
    return ans / (double)(n * (n - 1));
}


double deltasub(subgroup a, subgroup b) {
    int n = a.vr.size();
    int m = b.vr.size();
    double ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            ans += a.vr[i].len2(b.vr[j]);
        }
    }
    return 2.0 * ans / (double)(n * m) - dispertion(a) - dispertion(b);
}



vector <subgroup> initilization_subgroup(cluster v) {
    subgroup Vh;
    Vh.vr = v.vr;
    double dispVh = dispertion(Vh);
    double dispWh, minim;
    int n = v.vr.size();
    if (n == 1) {
        vector < subgroup > ans(1);
        ans[0].represent = v.vr[0];
        ans[0].vr.push_back(v.vr[0]);
        ans[0].rep_ind = 0;
        return ans;
    }
    vector < subgroup > w(n);
    vector < bool > used(n, false);
    for (int i = 0; i < n; i++)
        w[i].vr.push_back(v.vr[i]);
    int ind1, ind2;
    do {
        minim = INF;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (!used[i] && !used[j] && minim > deltasub(w[i], w[j])) {
                    minim = deltasub(w[i], w[j]);
                    ind1 = i;
                    ind2 = j;
                }
            }
        }
        subgroup g;
        g = w[ind1];
        for (int j = 0; j < w[ind2].vr.size(); j++){
            ver p = w[ind2].vr[j];
            g.vr.push_back(p);
        }
        dispWh = dispertion(g);
        if (dispWh / dispVh > PSI)
            break;
        used[ind2] = true;
        for (int j = 0; j < w[ind2].vr.size(); j++){
            ver p = w[ind2].vr[j];
            g.vr.push_back(p);
        }
    } while (true);
    vector < subgroup > ans;
    for (int i = 0; i < n; i++) {
        if (!used[i])
            ans.push_back(w[i]);
    }
    return ans;
}


bool comp(ver a, ver b) {
    return a.dist_to_center < b.dist_to_center;
}


ver initilization_represent(subgroup v) {
    int n = v.vr.size();
    ver center;
    for (int j = 0; j< v.vr.size(); j++) {
        ver p = v.vr[j];
        center.x += p.x;
        center.y += p.y;
    }
    center.x /= n;
    center.y /= n;
    for (int i = 0; i < n; i++) {
        v.vr[i].dist_to_center = center.len2(v.vr[i]);
    }
    sort(v.vr.begin(), v.vr.end(), comp);
    v.represent = v.vr[0];
    v.rep_ind = 0;
    return v.vr[0];
}


Neigbohour refresh_neigbohour(Neigbohour T) {
    int len1 = T.vr.size();
    int minim1, minind1;
    int P = min(len1 - 1, NP);
    vector < bool > used1;
    for (int i = 0; i < len1; i++) {
        used1.clear();
        used1.resize(len1, false);
        T.vr[i].np.clear();
        T.vr[i].np.resize(P);
        for (int j = 0; j < P; j++) {
            minim1 = INF;
            for (int e = 0; e < len1; e++) {
                if (i == e || used1[e])
                    continue;
                if (minim1 > T.vr[i].len2(T.vr[e])) {
                    minim1 = T.vr[i].len2(T.vr[e]);
                    minind1 = e;
                }
            }
            T.vr[i].np[j] = minind1;
            used1[minind1] = true;
        }
    }
    return T;
}



Neigbohour stringing1(Neigbohour T, ver x, int n) {
    Neigbohour Best = T;
    double len = INF, temp;
    int a = -1, b = -1, c = -1;
    int P = min(n - 1, NP);
    vector < bool > used(n, false);
    double minim = INF;
    x.np.clear();
    x.np.resize(P);
    int ind;
    for (int i = 0; i < P; i++) {
        minim = INF;
        for (int j = 0; j < n; j++) {
            if (!used[j] && minim > x.len2(T.vr[j])) {
                minim = x.len2(T.vr[j]);
                ind = j;
            }
        }
        used[ind] = true;
    }
    for (int i1 = 0; i1 < P; i1++) {
        for (int j1 = 0; j1 < P; j1++) {
            for (int k1 = 0; k1 < P; k1++) {
                int i = x.np[i1];
                int j = x.np[j1];
                int k = T.vr[(i + 1) % n].np[k1];
                if (i == k || k == j || i == j)
                    continue;
                if (((i + 1) % n >= j) || ((j + 1) % n >= k) || ((k + 1) % n >= i))
                    continue;
                temp = T.length - T.vr[i].len2(T.vr[(i + 1 + n) % n]) - T.vr[j].len2(T.vr[(j + 1 + n) % n]);
                temp = temp - T.vr[k].len2(T.vr[(k + 1 + n) % n]);
                temp = temp + x.len2(T.vr[i]) + x.len2(T.vr[j]);
                temp = temp + T.vr[(i + 1 + n) % n].len2(T.vr[(k + n) % n]) + T.vr[(j + 1 + n) % n].len2(T.vr[(k + 1 + n) % n]);
                if (temp < len) {
                    a = i;
                    b = j;
                    c = k;
                    len = temp;
                }
            }
        }
    }
    //for (int i = 0; i < n; i++) {
    //	for (int j = i + 1; j < n; j++) {
    //		for (int k = j + 1; k < n; k++) {
    //			temp = T.length - T.vr[i].len2(T.vr[(i + 1 + n) % n]) - T.vr[j].len2(T.vr[(j + 1 + n) % n]);
    //			temp = temp - T.vr[k].len2(T.vr[(k + 1 + n) % n]);
    //			temp = temp + x.len2(T.vr[i]) + x.len2(T.vr[j]);
    //			temp = temp + T.vr[(i + 1 + n) % n].len2(T.vr[(k + n) % n]) + T.vr[(j + 1 + n) % n].len2(T.vr[(k + 1 + n) % n]);
    //			if (temp < len) {
    //				a = i;
    //				b = j;
    //				c = k;
    //				len = temp;
    //			}
    //		}
    //	}
    //}
    Best.length = len;
    if (a == -1 || b == -1 || c == -1)
        return Best;
    if (!((a < b) && (b < c)))
        return Best;
    Best.vr.clear();
    for (int i = 0; i <= a; i++) {
        Best.vr.push_back(T.vr[i]);
    }
    Best.vr.push_back(x);
    for (int i = b; i > a; i--)
        Best.vr.push_back(T.vr[i]);
    for (int i = c; i > b; i--)
        Best.vr.push_back(T.vr[i]);
    for (int i = c + 1; i < n; i++)
        Best.vr.push_back(T.vr[i]);
    n++;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < P; j++) {
            if (Best.vr[i].np[j] > a && Best.vr[i].np[j] <= b) {
                Best.vr[i].np[j] = b - (Best.vr[i].np[j] - a) + 1;
            }
            if (Best.vr[i].np[j] > b && Best.vr[i].np[j] <= c) {
                Best.vr[i].np[j] = c - (Best.vr[i].np[j] - b) + 1;
            }
            if (Best.vr[i].np[j] > a) {
                Best.vr[i].np[j] = (Best.vr[i].np[j] + 1) % n;
            }
        }
    }
    double maxim;
    for (int i = 0; i < n; i++) {
        maxim = -INF;
        if (Best.vr[i] == x)
            continue;
        for (int j = 0; j < P; j++) {
            if (Best.vr[i].np[j] > maxim){
                maxim = Best.vr[i].np[j];
                ind = j;
            }
        }
        if (Best.vr[i].len2(x) < Best.vr[i].len2(Best.vr[i].np[ind]))
            Best.vr[i].np[ind] = a + 1;
    }
    return Best;
}


Neigbohour stringing2(Neigbohour T, ver x, int n) {
    Neigbohour Best = T;
    double len = INF, temp;
    int P = min(n - 1, NP);
    int a = -1, b = -1, c = -1, d = -1;
    vector < bool > used(n, false);
    double minim = INF;
    x.np.clear();
    x.np.resize(P);
    int ind;
    for (int i = 0; i < P; i++) {
        minim = INF;
        for (int j = 0; j < n; j++) {
            if (!used[j] && minim > x.len2(T.vr[j])) {
                minim = x.len2(T.vr[j]);
                ind = j;
            }
        }
        used[ind] = true;
    }
    for (int i1 = 0; i1 < P; i1++) {
        for (int l1 = 0; l1 < P; l1++) {
            for (int j1 = 0; j1 < P; j1++) {
                for (int k1 = 0; k1 < P; k1++) {
                    int i = x.np[i1];
                    int j = x.np[j1];
                    int k = T.vr[(i + 1) % n].np[k1];
                    int l = T.vr[(j + 1) % n].np[l1];
                    if (i == k || k == j || i == j || l == i || l == j || l == k)
                        continue;
                    if (((i + 2) % n >= l) || (l >= j) || ((j + 2) % n >= k) || (k >= i))
                        continue;
                    temp = T.length - T.vr[i].len2(T.vr[(i + 1 + n) % n]) - T.vr[l].len2(T.vr[(l - 1 + n) % n]);
                    temp = temp - T.vr[j].len2(T.vr[(j + 1 + n) % n]) - T.vr[k].len2(T.vr[(k - 1 + n) % n]);
                    temp = temp + x.len2(T.vr[i]) + x.len2(T.vr[j]) + T.vr[l].len2(T.vr[(j + 1 + n) % n]);
                    temp = temp + T.vr[(l - 1 + n) % n].len2(T.vr[(k - 1 + n) % n]) + T.vr[(i + 1 + n) % n].len2(T.vr[k]);
                    if (temp < len) {
                        a = i;
                        b = l;
                        c = j;
                        d = k;
                        len = temp;
                    }
                }
            }
        }
    }

    /*for (int i = 0; i < n; i++) {
        for (int l = i + 1; l < n; l++) {
            for (int j = l + 2; j < n; j++) {
                for (int k = j + 2; k < n; k++) {
                    temp = T.length - T.vr[i].len2(T.vr[(i + 1 + n) % n]) - T.vr[l].len2(T.vr[(l - 1 + n) % n]);
                    temp = temp - T.vr[j].len2(T.vr[(j + 1 + n) % n]) - T.vr[k].len2(T.vr[(k - 1 + n) % n]);
                    temp = temp + x.len2(T.vr[i]) + x.len2(T.vr[j]) + T.vr[l].len2(T.vr[(j + 1 + n) % n]);
                    temp = temp + T.vr[(l - 1 + n) % n].len2(T.vr[(k - 1 + n) % n]) + T.vr[(i + 1 + n) % n].len2(T.vr[k]);
                    if (temp < len) {
                        a = i;
                        b = l;
                        c = j;
                        d = k;
                        len = temp;
                    }
                }
            }
        }
    }*/
    Best.length = len;
    if (a == -1 || b == -1 || c == -1 || d == -1)
        return Best;
    if (!((a < b) && (b < c) && (c < d)))
        return Best;
    Best.vr.clear();
    for (int i = 0; i <= a; i++) {
        Best.vr.push_back(T.vr[i]);
    }
    Best.vr.push_back(x);
    for (int i = c; i >= b; i--)
        Best.vr.push_back(T.vr[i]);
    for (int i = c + 1; i <= d - 1; i++)
        Best.vr.push_back(T.vr[i]);
    for (int i = b - 1; i >= a + 1; i--)
        Best.vr.push_back(T.vr[i]);
    for (int i = d; i < n; i++)
        Best.vr.push_back(T.vr[i]);
    n++;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < P; j++) {
            if (Best.vr[i].np[j] > a && Best.vr[i].np[j] < b) {
                Best.vr[i].np[j] = (b - 1) - (Best.vr[i].np[j] - (a + 1)) - (a + 1) + a + (d - b) + 1;
            }
            if (Best.vr[i].np[j] >= b && Best.vr[i].np[j] <= c) {
                Best.vr[i].np[j] = c - (Best.vr[i].np[j] - b) - b + a + 1;
            }
            if (Best.vr[i].np[j] > a) {
                Best.vr[i].np[j] = (Best.vr[i].np[j] + 1) % n;
            }
        }
    }
    double maxim;
    for (int i = 0; i < n; i++) {
        maxim = -INF;
        if (Best.vr[i] == x)
            continue;
        for (int j = 0; j < P; j++) {
            if (Best.vr[i].np[j] > maxim) {
                maxim = Best.vr[i].np[j];
                ind = j;
            }
        }
        if (Best.vr[i].len2(x) < Best.vr[i].len2(Best.vr[i].np[ind]))
            Best.vr[i].np[ind] = a + 1;
    }
    return Best;
}



Neigbohour unstringing1(Neigbohour T, int ind, int n) {
    Neigbohour Best = T;
    double len = INF, temp;
    int P = min(n - 1, NP);
    int a = -1, b = -1, c = -1, d = -1;
    for (int k1 = 0; k1 < P; k1++) {
        for (int j1 = 0; j1 < P; j1++) {
            int j = T.vr[(ind + 1) % n].np[j1];
            int k = T.vr[(ind - 1 + n) % n].np[k1];
            if (ind == k || k == j || ind == j)
                continue;
            if (((ind + 1) % n >= k) || ((k + 1) % n >= j) || ((j + 2) % n >= ind))
                continue;
            temp = T.length - T.vr[ind].len2(T.vr[(ind - 1 + n) % n]) - T.vr[(ind + 1) % n].len2(T.vr[ind]);
            temp = temp - T.vr[k].len2(T.vr[(k + 1 + n) % n]) - T.vr[j].len2(T.vr[(j + 1 + n) % n]);
            temp = temp + T.vr[(ind - 1 + n) % n].len2(T.vr[k]);
            temp = temp + T.vr[(ind + 1 + n) % n].len2(T.vr[j]) + T.vr[(k + 1 + n) % n].len2(T.vr[(j + 1 + n) % n]);
            if (temp < len) {
                a = k;
                b = j;
                len = temp;
            }
        }
    }
    /*for (int k1 = 0; k1 < n - 2; k1++) {
        for (int j1 = k1 + 2; j1 < n - 4; j1++) {
            int k = (ind + 2 + k1) % n;
            int j = (ind + 2 + j1) % n;
            if ((ind - 1 + n) % n == (j + 1 + n) % n)
                break;
            int prov = j;
            if (prov < ind)
                prov += n;
            if (!(ind < k && k < j))
                continue;
            temp = T.length - T.vr[ind].len2(T.vr[(ind - 1 + n) % n]) - T.vr[(ind + 1) % n].len2(T.vr[ind]);
            temp = temp - T.vr[k].len2(T.vr[(k + 1 + n) % n]) - T.vr[j].len2(T.vr[(j + 1 + n) % n]);
            temp = temp + T.vr[(ind - 1 + n) % n].len2(T.vr[k]);
            temp = temp + T.vr[(ind + 1 + n) % n].len2(T.vr[j]) + T.vr[(k + 1 + n) % n].len2(T.vr[(j + 1 + n) % n]);
            if (temp < len) {
                a = k;
                b = j;
                len = temp;
            }
        }
    }*/
    Best.length = len;
    if (a == -1 || b == -1)
        return Best;
    Best.vr.clear();
    for (int i = 0; i <= (b - (a + 1) + n) % n; i++) {
        Best.vr.push_back(T.vr[(b - i + n) % n]);
    }
    for (int i = 0; i <= ((ind - 1) - (b + 1) + 2 * n) % n; i++) {
        Best.vr.push_back(T.vr[(b + 1 + i) % n]);
    }
    for (int i = 0; i <= (a - (ind + 1) + n) % n; i++) {
        Best.vr.push_back(T.vr[(a - i + n) % n]);
    }
    //Best = refresh_neigbohour(Best);

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < P; j++) {
            if (Best.vr[i].np[j] == 0)
                Best.vr[i].np[j] = 1;
        }
    }

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < P; j++) {
            if (Best.vr[i].np[j] > a && Best.vr[i].np[j] <= b) {
                Best.vr[i].np[j] = b - (Best.vr[i].np[j] - (a + 1)) - (a + 1);
            }
            else if (Best.vr[i].np[j] <= a) {
                Best.vr[i].np[j] = a - (Best.vr[i].np[j] - 0) + n - a - 1;
            }
            else {
                Best.vr[i].np[j] = (Best.vr[i].np[j] - a - 1);
            }
            Best.vr[i].np[j] = (Best.vr[i].np[j] - 1 + n) % n;
        }
    }
    return Best;
}


Neigbohour unstringing2(Neigbohour T, int ind, int n) {
    Neigbohour Best = T;
    double len = INF, temp;
    int a = -1, b = -1, c = -1, d = -1;
    int P = min(n - 1, NP);
    for (int l1 = 0; l1 < P; l1++) {
        for (int j1 = 0; j1 < P; j1++) {
            for (int k1 = 0; k1 < P; k1++) {
                int j = T.vr[(ind + 1) % n].np[j1];
                int k = T.vr[(ind - 1 + n) % n].np[k1];
                int l = T.vr[(k + 1) % n].np[l1];
                if (ind == k || k == j || ind == j || l == ind || l == j || l == k)
                    continue;
                if (((ind + 2) % n >= j) || (j >= l) || ((l + 1) % n >= k) || ((k + 2) % n >= ind))
                    continue;
                temp = T.length - T.vr[ind].len2(T.vr[(ind - 1 + n) % n]) - T.vr[(ind + 1) % n].len2(T.vr[ind]) - T.vr[k].len2(T.vr[(k + 1 + n) % n]);
                temp = temp - T.vr[j].len2(T.vr[(j - 1 + n) % n]) - T.vr[l].len2(T.vr[(l + 1 + n) % n]);
                temp = temp + T.vr[(ind + 1 + n) % n].len2(T.vr[j]) + T.vr[(k + 1 + n) % n].len2(T.vr[l]);
                temp = temp + T.vr[(ind - 1 + n) % n].len2(T.vr[k]) + T.vr[(l + 1 + n) % n].len2(T.vr[(j - 1 + n) % n]);
                if (temp < len) {
                    a = j;
                    b = l;
                    c = k;
                    len = temp;
                }
            }
        }
    }



    /*for (int j1 = 0; j1 < n - 3; j1++) {
        for (int l1 = j1 + 1; l1 < n - 4; l1++) {
            for (int k1 = l1 + 2; k1 < n - 6; k1++) {
                int j = (ind + 3 + j1) % n;
                int l = (ind + 3 + l1) % n;
                int k = (ind + 3 + k1) % n;
                if (ind == j || ind == l || ind == k)
                    continue;
                if ((ind - 1 + n) % n == (k + 1 + n) % n)
                    break;
                int prov1 = j + 1, prov2 = ind - 2;
                if (prov2 < prov1)
                    prov2 += n;
                if (!(prov1 < k && k < prov2))
                    continue;
                prov1 = j, prov2 = k - 1;
                if (prov2 < prov1)
                    prov2 += n;
                if (!(prov1 < l && l < prov2))
                    continue;
                temp = T.length - T.vr[ind].len2(T.vr[(ind - 1 + n) % n]) - T.vr[(ind + 1) % n].len2(T.vr[ind]) - T.vr[k].len2(T.vr[(k + 1 + n) % n]);
                temp = temp - T.vr[j].len2(T.vr[(j - 1 + n) % n]) - T.vr[l].len2(T.vr[(l + 1 + n) % n]);
                temp = temp + T.vr[(ind + 1 + n) % n].len2(T.vr[j]) + T.vr[(k + 1 + n) % n].len2(T.vr[l]);
                temp = temp + T.vr[(ind - 1 + n) % n].len2(T.vr[k]) + T.vr[(l + 1 + n) % n].len2(T.vr[(j - 1 + n) % n]);
                if (temp < len) {
                    a = j;
                    b = l;
                    c = k;
                    len = temp;
                }
            }
        }
    }*/
    Best.length = len;
    if (a == -1 || b == -1 || c == -1)
        return Best;
    Best.vr.clear();
    for (int i = 0; i <= (b - a + 2 * n) % n; i++) {
        Best.vr.push_back(T.vr[(a + i) % n]);
    }
    for (int i = 0; i <= ((ind - 1) - (c + 1) + 2 * n) % n; i++) {
        Best.vr.push_back(T.vr[(c + 1 + i) % n]);
    }
    for (int i = 0; i <= (c - (b + 1) + n) % n; i++) {
        Best.vr.push_back(T.vr[(c - i + n) % n]);
    }

    for (int i = 0; i <= ((a - 1) - (ind + 1) + n) % n; i++) {
        Best.vr.push_back(T.vr[((a - 1) - i + n) % n]);
    }
    //Best = refresh_neigbohour(Best);
    for (int i = 1; i < n; i++) {
        for (int j = 0; j < P; j++) {
            if (Best.vr[i].np[j] == 0)
                Best.vr[i].np[j] = 1;
        }
    }

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < P; j++) {
            if (Best.vr[i].np[j] >= a && Best.vr[i].np[j] <= b) {
                Best.vr[i].np[j] = Best.vr[i].np[j] - a;
            }
            else if (Best.vr[i].np[j] > c) {
                Best.vr[i].np[j] = Best.vr[i].np[j] - (c + 1) + (b - a + 1);
            }
            else if (Best.vr[i].np[j] > b && Best.vr[i].np[j] <= c) {
                Best.vr[i].np[j] = c - (Best.vr[i].np[j] - (b + 1)) - (b + 1) + (n - (c - b + a));
            }
            else {
                Best.vr[i].np[j] = (a - 1) - (Best.vr[i].np[j] - 0) + (n - a);
            }
            Best.vr[i].np[j] = (Best.vr[i].np[j] - 1 + n) % n;
        }
    }
    return Best;
}




Neigbohour GENUS(Neigbohour T, int m) {
    double z = T.length, minim1, temp;
    int t = 1;
    int n = T.vr.size();
    Neigbohour T1, T2, T0 = T, T3;
    T0.length = -1;
    if (T.vr.size() < 4) {
        T.length = 0;
        for (int i = 0; i < T.vr.size(); i++) {
            T.length += T.vr[i].len2(T.vr[(i + 1) % T.vr.size()]);
        }
        return T;
    }
    else {
        T = refresh_neigbohour(T);
        while (t <= NP) {
            ver v;
            T0.length = INF;
            for (int i = 0; i < n; i++) {
                v = T.vr[i];
                T1 = unstringing1(T, 0, n);
                T2 = unstringing2(T, 0, n);
                if (T2.length < T1.length)
                    T1 = T2;
                T2 = stringing1(T1, v, n - 1);
                T3 = stringing2(T1, v, n - 1);
                if (T2.length < T3.length)
                    T3 = T2;
                if (T0.length > T3.length)
                    T0 = T3;
                T2 = T;
                for (int j = 0; j < n; j++) {
                    T2.vr[j] = T.vr[(j + 1) % n];
                    for (int e = 0; e < NP; e++) {
                        T2.vr[j].np[e] = (T.vr[j].np[e] + 1) % n;
                    }
                }
                T = T2;
            }
            if (T.length > T0.length && T0.vr.size() == T.vr.size()) {
                T = T0;
                t = 1;
            }
            else
                t++;
        }
        return T;
    }
}



//удаление
pair <double, double> f(Neigbohour T0, int ind, double &a) {
    double length = 0, minim;
    int n = T0.vr.size();
    ver x = T0.vr[ind];
    if (n == 1) {
        if (T0.used[x.cluster] > 1) {
            return make_pair(0 - T0.length - a, 0);
        }
        else {
            return make_pair(0 - T0.length + a, 0);
        }
    }
    length = T0.length + T0.vr[(ind - 1 + n) % n].len2(T0.vr[(ind + 1 + n) % n]) - T0.vr[ind].len2(T0.vr[(ind + 1 + n) % n]) - T0.vr[ind].len2(T0.vr[(ind - 1 + n) % n]);
    if (T0.used[x.cluster] > 1) {
        return make_pair(length - T0.length - a, length);
    }
    else {
        return make_pair(length - T0.length + a, length);
    }
}



//добавление
pair <double, double> g(Neigbohour T0, ver x, double &a, int&t) {
    double length = 0, minim = INF;
    int n = T0.vr.size();
    vector < bool > used(n, false);
    ver pr, p;
    int ind;
    pr = x;
    if (n == 1) {
        length = 2 * T0.vr[0].len2(x);
        if (T0.used[x.cluster] >= 1) {
            return make_pair(length - T0.length + a, length);
        }
        else {
            return make_pair(length - T0.length - a, length);
        }
    }
    if (t % 5 == 6) {
        if (t <= x.tabu_cnt)
            return make_pair(INF, INF);
        Neigbohour T1 = T0;
        T1.vr.clear();
        for (int i = 0; i < n; i++) {
            if (T0.vr[i].tabu_cnt < t)
                T1.vr.push_back(T0.vr[i]);
        }
        T1.vr.push_back(x);
        T1.length = 0;
        for (int i = 0; i < T1.vr.size(); i++) {
            T1.length += T1.vr[i].len2(T1.vr[(i + 1) % T1.vr.size()]);
        }
        T1 = GENUS(T1, (int)T1.vr.size());
        length = T1.length;
        if (T0.used[x.cluster] >= 1) {
            return make_pair(length - T0.length + a, length);
        }
        else {
            return make_pair(length - T0.length - a, length);
        }
    }
    else {
        for (int i = 0; i < n; i++) {
            minim = min(minim, T0.vr[i].len2(x) + T0.vr[(i + 1) % n].len2(x) - T0.vr[(i + 1) % n].len2(T0.vr[i]));
        }
        if (minim == INF)
            minim = 0;
        length = T0.length + minim;
        if (T0.used[x.cluster] >= 1) {
            return make_pair(length - T0.length + a, length);
        }
        else {
            return make_pair(length - T0.length - a, length);
        }
    }
}



bool check_fesiable(Neigbohour T, int m) {
    for (int j = 0; j < T.used.size(); j++) {
        int p = T.used[j];
        if (p != 1)
            return false;
    }
    if (m != T.vr.size())
        return false;
    return true;
}


double frand(double fmin, double fmax) {
    double f = (double)rand() / RAND_MAX;
    return fmin + f * (fmax - fmin);
}

void Dialog::solve(){
    myTimer->start();
    int n = bigEditor->color;
    if (n < 3){
        QMessageBox *qm = new QMessageBox;
        qm->setText("INCORRECT NUMNER OF CLUSTER MUST BE MORE THAN 3");
        qm->setWindowTitle("error");
        qm->show();
        return;
    }
    vector < cluster > cl(n);
    bigEditor->resx.clear();
    bigEditor->resy.clear();
    int per = -1, bign = bigEditor->xk.size(), tek = -1;
    for (int i = 0; i < bign; i++){
        if (per != bigEditor->colors[i])
            tek++;
        cl[tek].vr.push_back(ver(bigEditor->xk[i], bigEditor->yk[i]));
        per = bigEditor->colors[i];
    }

    int N = bigEditor->xk.size();
    int x, y;
    if (n == 3)
        NP = 2;
    for (int k = 0; k < n; k++) {
        cl[k].sb = initilization_subgroup(cl[k]); //разделение на subgroup
        for (int i = 0; i < cl[k].sb.size(); i++) {
            cl[k].sb[i].represent = initilization_represent(cl[k].sb[i]);
            for (int j = 0; j < cl[k].sb[i].vr.size(); j++){
                cl[k].sb[i].vr[j].cluster = k;
                cl[k].sb[i].represent.cluster = k;
            }
        }
    }

    //стартовое решение
    Neigbohour T0;
    T0.used.resize(n, 0);
    T0.used[0] = 1;
    T0.length = 0;
    ver pr = cl[0].sb[0].represent;
    cl[0].sb[0].represent.inT = true;
    cl[0].sb[0].represent.cnt_of_T = 1;
    vector < int > st(n, 0);
    double minim;
    T0.vr.push_back(pr);
    T0.vr[0].cnt_of_T = 1;
    T0.vr[0].inT = true;
    while (true) {
        minim = INF;
        for (int i = 0; i < n; i++) {
            if (!T0.used[i]) {
                for (int j = 0; j < cl[i].sb.size(); j++) {
                    if (minim > cl[i].sb[j].represent.len2(pr)) {
                        minim = cl[i].sb[j].represent.len2(pr);
                        x = i;
                        y = j;
                    }
                }
            }
        }
        if (minim == INF)
            break;
        st[x] = y;
        T0.used[x] = true;
        T0.length += minim;
        pr = cl[x].sb[y].represent;
        T0.vr.push_back(pr);
        cl[x].sb[y].represent.inT = T0.vr[T0.vr.size() - 1].inT = true;
        cl[x].sb[y].represent.cnt_of_T = T0.vr[T0.vr.size() - 1].cnt_of_T = 1;
    }
    T0.length += cl[x].sb[y].represent.len2(cl[0].sb[0].represent);
    T0.fashionable = true;
    //T0 = refresh_neigbohour(T0);
    T0 = GENUS(T0, N);
    for (int e = 0; e < 2000; e++){
        int u = T0.vr.size();
        int ab = rand() % u;
        int ba = rand() % u;
        if (ab == ba)
            continue;
        if (ba < ab)
            swap(ab, ba);
        Neigbohour T21 = T0;
        for (int i = 0; i <= max(0, (ba - ab)) / 2;i++){
            swap(T21.vr[i+ab], T21.vr[ba - i]);
        }
        double length = T21.vr[0].len2(T21.vr[u - 1]);
        for (int i = 1; i < u; i++){
            length = length + T21.vr[i].len2(T21.vr[i - 1]);
        }
        T21.length = length;
        if (T21.length < T0.length)
            T0 = T21;
    }


    //алгоритм
    int t0 = 0;
    bool intence = true;
    double a = 0.09 * T0.length;
    int t = 1, t1 = 0, t2 = 1, cnt = 0;
    int na = n;
    double tabulen = sqrt(N);
    double minf, ming, lenf, leng, phi;
    ver temp1, temp2;
    Neigbohour T = T0;
    while (t1 + 1000 > t) {
        if (t1 + 31 == t)
            intence = false;


        //double lol = 0;
        //if (t == 1550)
        //	cout << t;
        //for (int i = 0; i < T.vr.size(); i++) {
        //	lol += T.vr[i].len2(T.vr[(i + 1) % T.vr.size()]);
        //}
        //if (abs(lol - T.length) > EPS) {
        //	cout << t;
        //}



        //Best move determination
        /*if (t % 5 == 0) {
            T = refresh_neigbohour(T);
        }*/
        minf = ming = INF;
        bool flag;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < cl[i].sb.size(); j++) {
                flag = true;
                for (int k = 0; k < T.vr.size(); k++) {
                    if (cl[i].sb[j].represent == T.vr[k]) {
                        cl[i].sb[j].represent.inT = true;
                        flag = false;
                    }
                }
                if (flag)
                    cl[i].sb[j].represent.inT = false;
            }
        }
        for (int i = 0; i < T.vr.size(); i++) {
            if (minf > f(T, i, a).first) {
                minf = f(T, i, a).first;
                lenf = f(T, i, a).second;
                temp1 = T.vr[i];
            }
        }
        int ind = -1;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < cl[i].sb.size(); j++) {
                subgroup p1 = cl[i].sb[j];
                ver p = p1.represent;
                if (!p.inT) {
                    phi = 1 + p.cnt_of_T / t;
                    pair < double, double > pr = g(T, p, a, t);
                    double pr1 = pr.first;
                    double pr2 = pr.second;
                    if (ming > pr1 * phi) {
                        ming = pr1 * phi;
                        leng = pr2;
                        temp2 = p;
                        ind = i;
                    }
                }
            }
        }



        //выборка
        if (ming < minf) {
            //добавление
            if (t % 5 == 0) {
                Neigbohour T1 = T;
                T1.vr.clear();
                for (int i = 0; i < T.vr.size(); i++) {
                    if (T.vr[i].tabu_cnt < t)
                        T1.vr.push_back(T.vr[i]);
                }
                T1.vr.push_back(temp2);
                T1.length = 0;
                for (int i = 0; i < T1.vr.size(); i++) {
                    T1.length += T1.vr[i].len2(T1.vr[(i + 1) % T1.vr.size()]);
                }
                T = GENUS(T1, (int)T1.vr.size());
                leng = T.length;
            }
            else {
                double le;
                int ind1 = -1;
                minim = INF;
                Neigbohour T1 = T;
                T1.vr.clear();
                for (int i = 0; i < (int)(T.vr.size()); i++) {
                    le = T.vr[i].len2(temp2) + T.vr[(i + 1) % (int)(T.vr.size())].len2(temp2) - T.vr[(i + 1) % (int)(T.vr.size())].len2(T.vr[i]);
                    if (minim > le) {
                        minim = le;
                        ind1 = i;
                    }
                }
                if (T.vr.size()) {
                    for (int i = 0; i <= ind1; i++)
                        T1.vr.push_back(T.vr[i]);
                }
                T1.vr.push_back(temp2);
                T1.vr[T1.vr.size() - 1].cnt_of_T++;
                T1.vr[T1.vr.size() - 1].inT = true;
                for (int i = ind1 + 1; i < (int)(T.vr.size()); i++)
                    T1.vr.push_back(T.vr[i]);
                T = T1;
            }
            T.length = leng;
            T.used[temp2.cluster]++;
            T.fashionable = check_fesiable(T, n);
            for (int j = 0; j < cl[ind].sb.size(); j++) {
                if (cl[ind].sb[j].represent == temp2) {
                    cl[ind].sb[j].represent = temp2;
                    cl[ind].sb[j].represent.cnt_of_T++;
                    cl[ind].sb[j].represent.inT = true;
                }
            }
        }
        else {
            Neigbohour Temp;
            int n1 = T.vr.size();
            T.length = lenf;
            if (T.vr.size() == 2)
                T.length = 0;
            for (int i = 0; i < n1; i++)
            {
                if (T.vr[i] != temp1)
                    Temp.vr.push_back(T.vr[i]);
            }//удаление
            T.vr = Temp.vr;
            T.used[temp1.cluster]--;
            T.fashionable = check_fesiable(T, n);
            for (int j = 0; j < cl[temp1.cluster].sb.size(); j++) {
                if (cl[temp1.cluster].sb[j].represent == temp1) {
                    cl[temp1.cluster].sb[j].represent = temp1;
                    cl[temp1.cluster].sb[j].represent.tabu = true;
                    if (intence)
                        cl[temp1.cluster].sb[j].represent.tabu_cnt = t + 2 + rand() % 3;
                    else
                        cl[temp1.cluster].sb[j].represent.tabu_cnt = t + frand(sqrt(N), 2 * sqrt(N));

                    cl[temp1.cluster].sb[j].represent.inT = false;
                }
            }
        }
        if (intence)
            T = GENUS(T, N);




        //Best solution update
        if (T.length < T0.length && T.fashionable) {
            T0 = T;
            t1 = t;
            intence = true;
            //intensification phase.
        }



        //Diversification phase
        if (t1 + L < t) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < cl[i].sb.size(); j++) {
                    cl[i].sb[j].rep_ind = (cl[i].sb[j].rep_ind + 1) % int(cl[i].sb[j].vr.size());
                    if (cl[i].sb[j].represent != cl[i].sb[j].vr[cl[i].sb[j].rep_ind])
                        cl[i].sb[j].represent = cl[i].sb[j].vr[cl[i].sb[j].rep_ind];
                }
            }
        }



        //Penalty update
        if (t % 2) {
            if (T.fashionable) {
                cnt++;
            }
            else
                cnt = 0;
            if (cnt == na || a > 1007)
                a = a / frand(1.5, 2.0);
            else
                a = a * frand(1.5, 2.0);
        }
        t++;
    }
    time1 = myTimer->elapsed()/ 1000.0;
    for (int i = 0; i < T0.vr.size();i++){
       bigEditor->resx.push_back(T0.vr[i].x);
       bigEditor->resy.push_back(T0.vr[i].y);
    }
    bigEditor->resx.push_back(T0.vr[0].x);
    bigEditor->resy.push_back(T0.vr[0].y);
    bigEditor->tsplen = T0.length;
    bigEditor->update();
    improve_info();
}


void Dialog::improve_info(){
    len->setText( "length: " + QString("%3").arg(bigEditor->tsplen));
    cnt->setText( "vertex_count: " + QString("%0").arg((int)bigEditor->xk.size()));
    time->setText( "time_work: " + QString("%3").arg(time1));
}
