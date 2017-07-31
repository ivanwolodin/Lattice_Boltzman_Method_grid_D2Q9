#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}


/*
 * вычислительная часть происходит в классе lbm
 * визуализационная часть в классе widget
 * построение графиков с помощью QtCharts
 * визуализационный класс написан более-менее оптимально
 *
 * !!!!!
 * за исключением части принскринов
 * используется два слота для этого (для икс- и игрек- компонент)
 * должен быть один слот, с помощью сигналов нужно передавать
 * какой из графиков нужно отсканировать, примерно так:
 *  connect(prcr_Vy,SIGNAL(clicked(bool)),SIGNAL(QChart for Vy);
 *  connect(prcr_Vy,SIGNAL(QChart for Vy)),SLOT printscreen(QChart for Vy);
*/
