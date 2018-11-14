#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QtGui>
#include <QWidget>
#include <QMouseEvent>
#include <vector>

using namespace std;

class MyWidget : public QWidget
{
    Q_OBJECT
public:
 //  MyWidget();
    void mousePressEvent( QMouseEvent * event );
    void mouseReleaseEvent( QMouseEvent * event );
    vector < int > xk;
    vector < int > yk;
    vector < int > resx;
    vector < int > resy;
    int color;
    vector < int > colors;
    int temp;
    double tsplen;
    bool flag;
    QTimer * timer;
    int x1, y1;
protected:
    void paintEvent(QPaintEvent *event);
signals:

public slots:
    void onTick();
};

#endif // MYWIDGET_H
