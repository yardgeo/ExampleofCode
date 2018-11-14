

#ifndef DIALOG_H
#define DIALOG_H
#include "mywidget.h"
#include <QDialog>
#include "wdg.h"
#include <QLayout>


QT_BEGIN_NAMESPACE
class QAction;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QMenu;
class QMenuBar;
class QPushButton;
class QTextEdit;
QT_END_NAMESPACE

//! [0]
class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog();
public slots:
    void openfile();
    void savefile();
    void clear();
    void newcluster();
    void newvertex();
    void out();
    void solve();
    void savetext();
public:
    void createMenu();
    void createHorizontalGroupBox();
    void createGridGroupBox();
    void createFormGroupBox();
    void improve_info();

    enum { NumGridRows = 3, NumButtons = 7 };

    QMenuBar *menuBar;
    QGroupBox *horizontalGroupBox, *inform1;
    QGroupBox *gridGroupBox;
    QGroupBox *formGroupBox;
    QTextEdit *smallEditor;
    MyWidget *bigEditor;
    QLabel *labels[NumGridRows];
    QLineEdit *lineEdits[NumGridRows];
    QPushButton *buttons[NumButtons];
    QDialogButtonBox *buttonBox;
    QMenu *fileMenu;
    QAction *exitAction;
    QLineEdit *linex;
    QLineEdit *liney;
    QDialog *q1;
    QWidget *excp;
    QHBoxLayout *inform;
    QLabel *len, *cnt, *time;
    QTime * myTimer;
    double time1;
};
//! [0]

#endif // DIALOG_H
