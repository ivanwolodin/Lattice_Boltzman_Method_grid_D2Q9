#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//#include <QtWidgets>
#include <QtCharts>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QGroupBox>
#include <QLineSeries>
#include <QPixmap>
#include <QChartView>
#include <QProgressBar>
#include <QDebug>
using namespace QtCharts;
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    int Number_of_steps=40000;
    QLabel* Vx0;
    QLabel *Vxf;
    QLabel *Vy0;
    QLabel *Vyf;
    QLabel *Viscocity;
    QLabel *Tau;

    QLineEdit*Tau_;
    QLineEdit*Viscocity_;
    QLineEdit *Vx0_;
    QLineEdit *Vxf_;
    QLineEdit *Vy0_;
    QLineEdit *Vyf_;



    QGroupBox* set_frame_units_control();
    QGroupBox* set_splinechart_Vx();     // responsible for x-component of Velocity in Chart
    QGroupBox* set_splinechart_Vy();     // responsible for y-component of Velocity in Chart
    QPushButton* calculateButton;


    /***************************************/
    QLineEdit* number_of_step_for_Vx;
    QSlider* slider_for_Vx;
    QLineEdit* number_of_step_for_Vy;
    QSlider* slider_for_Vy;
    QLineSeries* series_for_Vy= new QLineSeries();
    QLineSeries* series_for_Vx = new QLineSeries();
    QChart* chart_for_Vy = new QChart();
    QChart* chart_for_Vx = new QChart();
    /***************************************/

    QProgressBar*prg_for_progress_window = new QProgressBar;

    //************************************************************
     //для принскрина
    QPushButton*prcr_Vx=new QPushButton("Print Screen");
    QPushButton*prcr_Vy=new QPushButton("Print Screen");
    QPixmap originalPixmap;
    QChartView *chartView_Vy = new QChartView(chart_for_Vy);
    QChartView *chartView_Vx = new QChartView(chart_for_Vx);


    //меню
    void set_menu();

public slots:
    void calculation_velocity();
    void plot_Velocity_Y();
    void plot_Velocity_X();

    void slider_changed_for_Vx();
    void slider_changed_for_Vy();


    void change_status_of_progressBar(int indicator);
    void elapsed_time_window(int elaps_time);

    void printscreen_Vy();
    void printscreen_Vx();

    void f1_aboutprogram();

signals:
    void indicator_of_progressBar_in_progress_window(int indicator);
    void elapsed_time(int elaps_time);


    //void sdsdsd();

private:
        Ui::Widget *ui;
};

#endif // WIDGET_H
