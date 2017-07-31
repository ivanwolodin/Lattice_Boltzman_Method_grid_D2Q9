#ifndef LBM_H
#define LBM_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <QVector>
//#include <vector>
#include <QDebug>
//using namespace std;
using std::ofstream;
using std::ifstream;
using std::cout;
using std::ios_base;
using std::setw;
using std::setprecision;
using std::endl;
class LBM
{
public:
    LBM();
    ~LBM();
//static int oldNx,oldNy;

    int StepTime; //для записи номера шага в файл
    QVector<double>  Nx50_velocityy;  //для построения графика
    QVector<double> Nx90_velocityy;

    QVector<double> TempVector_for_X;  //вспомогательный вектор, куда будут записываться значения скорости на текущем шаге
    QVector<QVector<double> >Velocity_X ;   //вектор, в котором будут хранится все значения скорости на каждом шаге. Velocity_X[steptime][value]
    QVector<double> TempVector_for_Y;  //вспомогательный вектор, куда будут записываться значения скорости на текущем шаге
    QVector<QVector<double> >Velocity_Y ;

     double rho=1;                   // fluid density

    double u0[2];                 // initial fluid velocity
    double uf[2];                 // final fluid velocity
    double tau;                   // relaxation time


    int Nx = 100;                  // number of cells in x direction
    int Ny = 100;                  // number of cells in y direction
 //     double rho[102][102];

private:
//для моделирования течений нужно менять только force() и boundaryConditions()




int Nc = 9;                   // number of discrete velocities
enum direction {
    East = 1, North, West, South,
    NorthEast, NorthWest, SouthWest, SouthEast
};
/**/
double ***n;                  // number densities
double ***neq;                // equilibrium number densities
double ***u;                  // fluid velocity




int nObstacle;                // extent of obstacle in y direction ?

const double w0 = 16 / 36.0,   //коэфициенты для решетки D2Q9
             w1 = 4 / 36.0,
             w2 = 1 / 36.0;
const double w[9] = {         // directional weights
    w0,                       // zero
    w1, w1, w1, w1,           // east, north, west, south
    w2, w2, w2, w2            // north-east, north-west, south-west, south-east
};
const double c[9][2] = {      // particle velocities
    {0, 0},                   // zero
    {1, 0}, {0, 1},           // east, north
    {-1, 0}, {0, -1},         // west, south
    {1, 1}, {-1, 1},          // north-east, north-west
    {-1, -1}, {1, -1}         // south-west, south-east
};
const double csSqd = 1 / 3.0; // speed of sound squared

//функции для вычисления
void getInput() ;
void allocate();
void initialize_u();
void compute_neq();
void initialize_n();

void initialize_u_on_the_surface();

void boundaryConditions();
void freeStream();
void compute_u();
void collide();
void force();

void deallocate();
void fillZero();

void output_the_result();


void density();

void is_valid(double***arr_);


public:
void outVx_50(ofstream&filestr);
void output_f_and_feq(ofstream &filestr);
void output_v(ofstream &filestr);
void initialize() ;
void takeStep();
void output_the_result_to_file(ofstream &filestr);
void create_file_for_matlab();
};

#endif // LBM_H
