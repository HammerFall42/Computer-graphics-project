#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QColorDialog>
#include <QMessageBox>

#include "facade.h"
#include "sorter.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void moveCam(vector <int> m);
    void rotateCam(vector <double> r);
    void scaleCam(vector <double> s);

    void checkInside(pair <int, int> p);
    void checkGroupInside(pair <int, int> p);

    void on_initButton_clicked();
    void on_chooseColorButton_clicked();

    void on_delButton_clicked();

    void on_addButton_clicked();

    void on_helpButton_clicked();

private:
    Ui::Widget *ui;
    QColor FC;
    Facade facade;
    Scene *sc;
    Panel *pn[12];
};

#endif // WIDGET_H
