#ifndef STYLE_H
#define STYLE_H
#include<QString>
#define MULTI_LINE_STRING(a) #a;
const QString styleg =  MULTI_LINE_STRING(

            QPushButton:hover{
                color: #fff;
                background-color: #0069d9;
                border-color: #0062cc;
              }
QPushButton{
                color: #fff;
                    background-color: #007bff;
                    border-color: #007bff;
                font-weight: 400;
                    text-align: center;
                    white-space: nowrap;
                    vertical-align: middle;
                    -webkit-user-select: none;
                    -moz-user-select: none;
                    -ms-user-select: none;
                    user-select: none;
                    border: 1px solid transparent;
                    padding: .375rem .75rem;
                    font-size: 1rem;
                    line-height: 1.5;
                    font-family: Arial;
                  color: #ffffff;
                  font-size: 20px;
              }
 QLabel{
                //margin-left:30;
                text-align: center;
                font-size: 1rem;
                line-height: 1.5;
                font-family: Arial;
              color: black;
              font-size: 20px;
            }

QGroupBox{border:0;}
        );
#endif // STYLE_H
