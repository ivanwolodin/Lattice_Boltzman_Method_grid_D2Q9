#include "widget.h"
#include "ui_widget.h"
#include "lbm.h"
//#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
   // set_frame();

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(set_frame_units_control(), 0, 0,1,2);
    grid->addWidget(set_splinechart_Vx(),1,0);
    grid->addWidget(set_splinechart_Vy(),1,1);
    setLayout(grid);
    set_menu();

    this->setMinimumWidth(800);
    this->setMinimumHeight(600);
    this->setWindowTitle("Modelling of Fluid Dynamics. Lattice Boltzman Method");
    connect(calculateButton,SIGNAL(clicked(bool)),SLOT(calculation_velocity()));
    connect(this,SIGNAL(indicator_of_progressBar_in_progress_window(int)),SLOT(change_status_of_progressBar(int)));
    connect(this,SIGNAL(elapsed_time(int)),SLOT(elapsed_time_window(int)));

    connect(number_of_step_for_Vy,SIGNAL(textChanged(QString)),SLOT(plot_Velocity_Y()));
    connect(number_of_step_for_Vx,SIGNAL(textChanged(QString)),SLOT(plot_Velocity_X()));

    connect(slider_for_Vx,SIGNAL(sliderMoved(int)),SLOT(slider_changed_for_Vx()));
    connect(slider_for_Vy,SIGNAL(sliderMoved(int)),SLOT(slider_changed_for_Vy()));

    connect(prcr_Vy,SIGNAL(clicked(bool)),SLOT(printscreen_Vy()));
    connect(prcr_Vx,SIGNAL(clicked(bool)),SLOT(printscreen_Vx()));
}

Widget::~Widget()
{

    delete ui;
}
LBM object_lbm; //creat object of class, which calculates
QGroupBox *Widget::set_frame_units_control()
{
    Vx0_=new QLineEdit(QString::number(0));Vxf_=new QLineEdit(QString::number(0.2));
    Vx0=new QLabel("Vx in the beginning");Vxf=new QLabel("Vx in the end");
    Vy0_=new QLineEdit(QString::number(0));Vyf_=new QLineEdit(QString::number(0));
    Vy0=new QLabel("Vy in the beginning");Vyf=new QLabel("Vy in the end");
    Tau = new QLabel("relax.time");Tau_=new QLineEdit(QString::number(1));
    Viscocity = new QLabel("rho");Viscocity_=new QLineEdit(QString::number(1));
    Viscocity_->setEnabled(false);

    calculateButton = new QPushButton("Calculate!");

    //QProgressBar*prgs=new QProgressBar;
    prg_for_progress_window->setRange(0,Number_of_steps-1);


    QGridLayout *layout = new QGridLayout();
    layout->addWidget(Vx0,0,0);layout->addWidget(Vx0_,0,1);layout->addWidget(Vxf,0,2);layout->addWidget(Vxf_,0,3);
    layout->addWidget(Vy0,1,0);layout->addWidget(Vy0_,1,1);layout->addWidget(Vyf,1,2);layout->addWidget(Vyf_,1,3);
    layout->addWidget(Tau,2,0);layout->addWidget(Tau_,2,1);layout->addWidget(Viscocity,2,2);layout->addWidget(Viscocity_,2,3);
    layout->addWidget(prg_for_progress_window,3,0,1,4,Qt::AlignTop);
    layout->addWidget(calculateButton,4,0,1,4);
    QGroupBox* groupbox = new QGroupBox("UNITS CONTROL");
    groupbox->setLayout(layout);

    return groupbox;

}

QGroupBox *Widget::set_splinechart_Vx()
{


    chart_for_Vx->legend()->hide();
    chart_for_Vx->addSeries(series_for_Vx);
    chart_for_Vx->setTitle("Couette Flow");
    chart_for_Vx->createDefaultAxes();
    chart_for_Vx->axisX()->setRange(0, 0.2);
    chart_for_Vx->axisY()->setRange(0,100);
    chart_for_Vx->axisY()->setTitleText("Number of Node, N");
    chart_for_Vx->axisX()->setTitleText("Velocity, Vx");



    slider_for_Vx = new QSlider(Qt::Orientation::Horizontal);
    slider_for_Vx->setRange(1,Number_of_steps-1);
    slider_for_Vx->setEnabled(false);
    number_of_step_for_Vx = new QLineEdit("Enter number of step");
    number_of_step_for_Vx->setValidator(new QIntValidator(1,Number_of_steps-1));
    number_of_step_for_Vx->setEnabled(false);

    QVBoxLayout *vert = new QVBoxLayout;
    QHBoxLayout *horiz = new QHBoxLayout;
    vert->addWidget(chartView_Vx);
   // vert->addWidget(slider);
    horiz->addWidget(slider_for_Vx,Qt::AlignLeft);horiz->addWidget(number_of_step_for_Vx);
    vert->addLayout(horiz);
    vert->addWidget(prcr_Vx);
    QGroupBox* groupbox = new QGroupBox("X-component of velocity");
    groupbox->setLayout(vert);
    return groupbox;
}

QGroupBox *Widget::set_splinechart_Vy()
{
    number_of_step_for_Vy = new QLineEdit("Enter number of step");
    number_of_step_for_Vy->setEnabled(false);
    number_of_step_for_Vy->setValidator(new QIntValidator(1,Number_of_steps-1));
    chart_for_Vy->addSeries( series_for_Vy);
    chart_for_Vy->legend()->hide();
    chart_for_Vy->setTitle("Vy (N)");
    chart_for_Vy->createDefaultAxes();
    chart_for_Vy->axisY()->setRange(-0.1,0.1);
    chart_for_Vy->axisX()->setRange(0,100);
    chart_for_Vy->axisX()->setTitleText("Number of Node, N");
    chart_for_Vy->axisY()->setTitleText("Velocity, Vy ");


   // emit printscree_Vx( *chartView);

    slider_for_Vy = new QSlider(Qt::Orientation::Horizontal);
    slider_for_Vy->setRange(1,Number_of_steps-1);
    slider_for_Vy->setEnabled(false);


    QVBoxLayout *vert = new QVBoxLayout;
    QHBoxLayout *horiz = new QHBoxLayout;
    vert->addWidget(chartView_Vy);
   // vert->addWidget(slider);

    horiz->addWidget(slider_for_Vy,Qt::AlignLeft);horiz->addWidget(number_of_step_for_Vy);

    vert->addLayout(horiz);
    vert->addWidget(prcr_Vy);
    QGroupBox* groupbox = new QGroupBox("Y-component of velocity");
    groupbox->setLayout(vert);
    //connect(number_of_step_for_Vy,SIGNAL(textEdited(QString)),SLOT(update()));
    return groupbox;
}

void Widget::set_menu()
{
    QMenu*   pmnu   = new QMenu("&Menu");

    pmnu->addSeparator();
    pmnu->addAction("Close",this,SLOT(close()),Qt::CTRL+Qt::Key_Q);
    QMenu*   fil    = new QMenu("&Help");fil->addAction("About program",this,SLOT(f1_aboutprogram()),Qt::CTRL+Qt::Key_F1);
    QMenuBar *mb = new QMenuBar;
    mb->addMenu(pmnu);
    mb->addMenu(fil);
    layout()->setMenuBar(mb);
}



void Widget::calculation_velocity()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTime measuretime;
    measuretime.start();
    slider_for_Vx->setEnabled(false);
    number_of_step_for_Vx->setEnabled(false);
    slider_for_Vy->setEnabled(false);
    number_of_step_for_Vy->setEnabled(false);

    // **********************************************
    // считывает данные
    //object_lbm.rho=Viscocity_->text().toFloat();
    object_lbm.tau=Tau_->text().toFloat();
    object_lbm.u0[0]=Vx0_->text().toFloat();
    object_lbm.u0[1]=Vy0_->text().toFloat();
    object_lbm.uf[0]=Vxf_->text().toFloat();
    object_lbm.uf[1]=Vyf_->text().toFloat();
    // ***********************************************
    ofstream  filestr;
    filestr.open ("Vx.txt", ios_base::out | ios_base::trunc);
    if(filestr.is_open()==false)
    {
        cout << "error! didn't open the file!"<<endl;
        //return -1;
    }
    else cout<<"The file has been opened!"<<endl;
    object_lbm.initialize();


    ofstream  filestr_f_eq;
    filestr_f_eq.open ("f_feq.txt", ios_base::out | ios_base::trunc);
    if(filestr_f_eq.is_open()==false)
    {
        cout << "error! didn't open the file!"<<endl;
        //return -1;
    }
    else cout<<"The file f_feq has been opened!"<<endl;

    ofstream  filestr_velocity;
    filestr_velocity.open ("velocity.txt", ios_base::out | ios_base::trunc);
    if(filestr_velocity.is_open()==false)
    {
        cout << "error! didn't open the file!"<<endl;
        //return -1;
    }
    else cout<<"The file f_feq has been opened!"<<endl;


    for(int i=0;i<Number_of_steps;i++){
        object_lbm.takeStep();
        object_lbm.output_the_result_to_file(filestr);
     //   object_lbm.output_f_and_feq(filestr_f_eq);
     //   object_lbm.output_v(filestr_velocity);
        emit indicator_of_progressBar_in_progress_window(i);
        if(i==10||i==100||i==1000||i==10000||i==39990){
            filestr_velocity<<"stepTime = "<<i<<endl;
            object_lbm.outVx_50(filestr_velocity);
        }

    }
    //  object_lbm.output_the_result_to_file(filestr);
    //object_lbm.output_f_and_feq();
    object_lbm.create_file_for_matlab();
    slider_for_Vx->setEnabled(true);
    number_of_step_for_Vx->setEnabled(true);
    slider_for_Vy->setEnabled(true);
    number_of_step_for_Vy->setEnabled(true);
   // measuretime.restart();
    int elapss_time = measuretime.elapsed();
    QApplication::restoreOverrideCursor();
    emit elapsed_time(elapss_time);
}

void Widget::plot_Velocity_Y( )
{

   int n = number_of_step_for_Vy->text().toInt(); //номер шага по времени
   slider_for_Vy->setValue(n);
    series_for_Vy->clear();
    for(int i=0;i<object_lbm.Nx;i++){
      series_for_Vy->append(i,object_lbm.Velocity_Y[n][i]  );
    }
}

void Widget::plot_Velocity_X()
{
     //number_of_step_for_Vx->setText(QString::number(slider_for_Vx->value()));
    int n = number_of_step_for_Vx->text().toInt(); //номер шага по времени
     slider_for_Vx->setValue(n);

     series_for_Vx->clear();
     for(int i=0;i<object_lbm.Nx;i++){
       series_for_Vx->append(object_lbm.Velocity_X[n][i],i);
     }
}

// ****************************************************************************
//для синхронизованной работы слайдера и LineEdit
void Widget::slider_changed_for_Vx()
{
    number_of_step_for_Vx->setText(QString::number(slider_for_Vx->value()))  ;
}

void Widget::slider_changed_for_Vy()
{
    number_of_step_for_Vy->setText(QString::number(slider_for_Vy->value()))  ;
}
//******************************************************************************


void Widget::elapsed_time_window(int elaps_time)
{
    QMessageBox a;
    a.setText("Done in ");
    a.setInformativeText(QString::number(elaps_time));
    a.setWindowTitle("Info");
    a.exec();
}

void Widget::printscreen_Vy()
{
    qDebug()<<"plotScr Vy"<<endl;

//    chartView->setChart(chart_for_Vx);
//    QPixmap okno = new QPixmap::grabWindow(QApplication::desktop()->winId());
    QScreen *screen = QGuiApplication::primaryScreen();

  //  originalPixmap = screen->grabWindow(ui->widget->winId(),0,0,-10,-10);
  //     originalPixmap = screen->grabWindow(set_splinechart_Vy()->winId());
     originalPixmap = screen->grabWindow(chartView_Vy->winId());

     const QString format = "jpg";
     QString initialPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
     if (initialPath.isEmpty())
         initialPath = QDir::currentPath();
     initialPath += tr("/untitled.") + format;

     QFileDialog fileDialog(this, tr("Save As"), initialPath);
     fileDialog.setAcceptMode(QFileDialog::AcceptSave);
     fileDialog.setFileMode(QFileDialog::AnyFile);
     fileDialog.setDirectory(initialPath);
     QStringList mimeTypes;
  //   foreach (const QByteArray &bf, QImageWriter::supportedMimeTypes())
   //      mimeTypes.append(QLatin1String(bf));
     fileDialog.setMimeTypeFilters(mimeTypes);
     fileDialog.selectMimeTypeFilter("image/" + format);
     fileDialog.setDefaultSuffix(format);
     if (fileDialog.exec() != QDialog::Accepted)
         return;
     const QString fileName = fileDialog.selectedFiles().first();
     if (!originalPixmap.save(fileName)) {
         QMessageBox::warning(this, tr("Save Error"), tr("The image could not be saved to \"%1\".")
                              .arg(QDir::toNativeSeparators(fileName)));
     }
}

void Widget::printscreen_Vx()
{
    qDebug()<<"plotScr Vx"<<endl;
    QScreen *screen = QGuiApplication::primaryScreen();
  //  originalPixmap = screen->grabWindow(ui->widget->winId(),0,0,-10,-10);
  //     originalPixmap = screen->grabWindow(set_splinechart_Vy()->winId());
     originalPixmap = screen->grabWindow(chartView_Vx->winId());

     const QString format = "jpg";
     QString initialPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
     if (initialPath.isEmpty())
         initialPath = QDir::currentPath();
     initialPath += tr("/untitled.") + format;

     QFileDialog fileDialog(this, tr("Save As"), initialPath);
     fileDialog.setAcceptMode(QFileDialog::AcceptSave);
     fileDialog.setFileMode(QFileDialog::AnyFile);
     fileDialog.setDirectory(initialPath);
     QStringList mimeTypes;
     foreach (const QByteArray &bf, QImageWriter::supportedMimeTypes())
         mimeTypes.append(QLatin1String(bf));
     fileDialog.setMimeTypeFilters(mimeTypes);
     fileDialog.selectMimeTypeFilter("image/" + format);
     fileDialog.setDefaultSuffix(format);
     if (fileDialog.exec() != QDialog::Accepted)
         return;
     const QString fileName = fileDialog.selectedFiles().first();
     if (!originalPixmap.save(fileName)) {
         QMessageBox::warning(this, tr("Save Error"), tr("The image could not be saved to \"%1\".")
                              .arg(QDir::toNativeSeparators(fileName)));
     }
}

void Widget::f1_aboutprogram()
{
    QMessageBox::information(0,"About program","Program: LBM-Qt Visualization\n"
                                                   "Mathematical modelling of Fluid Dynamics\n"
                                                   "Version: 3.0\n"
                                                   "Authors: I.V. & A.A. & S.S.\n"
                                                   "");
}

void Widget::change_status_of_progressBar(int indicator)
{
    prg_for_progress_window->setValue(indicator);
}
