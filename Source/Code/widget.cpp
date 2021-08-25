#include "./Headers/widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    FC = QColor("red");

    sc = new Scene;

    QObject::connect(sc, SIGNAL(moveCam(vector<int>)), this, SLOT(moveCam(vector<int>)));
    QObject::connect(sc, SIGNAL(rotateCam(vector<double>)), this, SLOT(rotateCam(vector<double>)));
    QObject::connect(sc, SIGNAL(scaleCam(vector<double>)), this, SLOT(scaleCam(vector<double>)));
    QObject::connect(sc, SIGNAL(checkInside(pair<int,int>)), this, SLOT(checkInside(pair<int,int>)));
    QObject::connect(sc, SIGNAL(checkGroupInside(pair<int,int>)), this, SLOT(checkGroupInside(pair<int,int>)));

    ui->label->setStyleSheet("background-color:" + FC.name() + ";");
    facade.setColor(FC);

    for (int i = 0 ;i < 12; ++i)
    {
        pn[i] = new Panel;
        pn[i]->setNumber(i);
    }

    ui->scene->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->scene->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_1->setMouseTracking(true);
    ui->panel_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_2->setMouseTracking(true);
    ui->panel_3->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_3->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_3->setMouseTracking(true);
    ui->panel_4->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_4->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_4->setMouseTracking(true);
    ui->panel_5->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_5->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_5->setMouseTracking(true);
    ui->panel_6->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_6->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_6->setMouseTracking(true);
    ui->panel_7->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_7->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_7->setMouseTracking(true);
    ui->panel_8->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_8->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_8->setMouseTracking(true);
    ui->panel_9->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_9->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_9->setMouseTracking(true);
    ui->panel_10->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_10->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_10->setMouseTracking(true);
    ui->panel_11->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_11->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_11->setMouseTracking(true);
    ui->panel_12->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_12->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->panel_12->setMouseTracking(true);

    ui->panel_1->setScene(pn[0]);
    ui->panel_2->setScene(pn[1]);
    ui->panel_3->setScene(pn[2]);
    ui->panel_4->setScene(pn[3]);
    ui->panel_5->setScene(pn[4]);
    ui->panel_6->setScene(pn[5]);
    ui->panel_7->setScene(pn[6]);
    ui->panel_8->setScene(pn[7]);
    ui->panel_9->setScene(pn[8]);
    ui->panel_10->setScene(pn[9]);
    ui->panel_11->setScene(pn[10]);
    ui->panel_12->setScene(pn[11]);
    ui->scene->setScene(sc);

    facade.setScene(sc);
    facade.setPanel(pn);

    facade.paintPanel();
    facade.paintScene();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::moveCam(vector<int> m)
{
    facade.moveCam(m);
}

void Widget::rotateCam(vector<double> r)
{
    facade.rotateCam(r);
}

void Widget::scaleCam(vector<double> s)
{
    facade.scaleCam(s);
}

void Widget::checkInside(pair<int, int> p)
{
    Sorter s;
    Transformator tr;
    sc->findSelected(s, facade.getCam(), tr, p);
}

void Widget::checkGroupInside(pair<int, int> p)
{
    Sorter s;
    Transformator tr;
    sc->findGroupSelected(s, facade.getCam(), tr, p);
}

void Widget::on_initButton_clicked()
{
    facade.initCam();
}

void Widget::on_chooseColorButton_clicked()
{
    FC = QColorDialog::getColor();

    ui->label->setStyleSheet("background-color:" + FC.name() + ";");

    facade.setColor(FC);
    facade.paintPanel();
}


void Widget::on_delButton_clicked()
{
    sc->removeObject();
    facade.paintScene();
}

void Widget::on_addButton_clicked()
{
    sc->addObject(FC);
    facade.paintScene();
}

void Widget::on_helpButton_clicked()
{
    QMessageBox::information(this, "Подсказка", "← ↑ → ↓ - Смещение камеры\n"
                                                "Колесико мыши - Масштабирование\n"
                                                "Перемещение курсора с зажатой ПКМ - вращение\n"
                                                "A Q - Перемещение объекта по ОХ\n"
                                                "W S - Перемещение объекта по ОY\n"
                                                "E D - Перемещение объекта по ОZ\n");
}
