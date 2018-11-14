#include "mywidget.h"
#include<style.h>
#include <QMouseEvent>
#include <vector>
#include<QTimer>

void MyWidget::mousePressEvent ( QMouseEvent * event )
{
    int x3 = event->pos().x();
    int y3 = event->pos().y();
    double temp;
    flag = false;
    for (int i = 0; i < xk.size(); i++){
        temp = sqrt((x3 - xk[i]) * (x3 - xk[i]) + (y3 - yk[i]) * (y3 - yk[i]));
        if (temp <= 25){
            flag = true;
            x1 = y1 = i;
        }
    }
    if (flag){
        timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(onTick()));
        timer->start(30);
    }
    else{
        QPoint point;
        point = event->pos();
        xk.push_back(event->pos().x());
        yk.push_back(event->pos().y());
        flag = 0;
        temp = (100 + (31 * color) % 156) * 256 * 256 + ((83 * color) % 256) * 256 + (157 * color) % 256;
        colors.push_back(temp);
        update();
    }
}

void MyWidget::onTick(){
    QPoint p = mapFromGlobal(QCursor::pos());
    int x2 = p.x() - xk[x1];
    int y2 = p.y() - yk[y1];
    for (int i = 0; i < xk.size(); i++){
        xk[i] += x2;
        yk[i] += y2;
    }
    for (int i = 0; i < resx.size(); i++){
        resx[i] += x2;
        resy[i] += y2;
    }
    update();
}

void MyWidget::mouseReleaseEvent ( QMouseEvent * event )
{
    if (flag)
    timer->stop();
}



void MyWidget::paintEvent(QPaintEvent *event)
{

     QPainter painter (this) ;
     painter.setRenderHint(QPainter::Antialiasing, true);
     painter.setPen(QPen(Qt::black, 3, Qt::SolidLine));
     for (int i = 0; i < max((int)resx.size() - 1, 0); i++){
         painter.drawLine(resx[i] + 5, resy[i] + 5,resx[i + 1] + 5, resy[i + 1] + 5);
     }
     for (int i = 0; i < xk.size(); i++){
         painter.setBrush(QBrush(QColor(colors[i] / (256 * 256), (colors[i] / 265) % 256, colors[i] % 256), Qt::SolidPattern));
        painter.drawEllipse(xk[i] - 10, yk[i] - 10, 25, 25);
     }

}


