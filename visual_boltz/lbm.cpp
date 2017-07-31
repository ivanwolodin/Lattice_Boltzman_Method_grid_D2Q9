#include "lbm.h"
//#include <QDebug>
LBM::LBM()
{
 qDebug()<<"Constructor LBM!"<<endl;


    allocate();
}
/*
vector<vector<vector<double>>> n(102, vector<vector<double>>(52, vector<double>(9)));
vector<vector<vector<double>>> u(102, vector<vector<double>>(52, vector<double>(2)));
vector<vector<vector<double>>> neq(102 , vector<vector<double>>(52, vector<double>(9)));
*/
LBM::~LBM()
{

    qDebug()<<"Destructor LBM!"<<endl;
    deallocate();
}

 void LBM::getInput()
{
     //

     qDebug()<<"tau= "<<tau;
     //qDebug()<<"rho= "<<rho;
     qDebug()<<"Vx0= "<<u0[0];
     qDebug()<<"Vxf= "<<uf[0];
     qDebug()<<"Vy0= "<<u0[1];
     qDebug()<<"Vyf= "<<uf[1];

   /*
    *  //   cout << " Enter initial fluid velocity u0_x: ";
    //   cin >> u0[0];  u0[1] = 0;   //двумерный массив. u0[0] - х компонента скорости, u0[1] = 0 - у компонента скорости
       u0[1] = 0; //y-компонента = 0
       u0[0] = 0 ;//x-компонента = 1
      // cout << " Enter fluid density rho: ";
     //  cin >> rho;                           //макроскопическая плотность
       rho=1;//плотность = 1
     //  cout << " Enter final fluid velocity uf_x: ";
     //  cin >> uf[0];  uf[1] = 0;   // uf[0] - конечная скорость иксовой компоненты скорости,  uf[1] = 0 - у-компонента
       uf[1] = 0; // конечная скорость игрековой компоненты
       uf[0] = 1; // конечная скорость иксовой компоненты. То значение, до которого будет посчитана скорость
      // cout << " Enter relaxation time tau: ";
      // cin >> tau;
       tau = 1; // время релаксации системы
       //препятствия - нет!
     // cout << " Enter extent of obstacle in y direction: ";  // высота препятствия, если меньше 1 - то препятствия нет
     //  cin >> nObstacle;*/

}

void LBM::  allocate() { //освободить память массива и создать новый
  /*
   static int oldNx = 0, oldNy = 0;
    if (Nx != oldNx) {
        if (n != 0) {
            for (int x = 0; x < oldNx+2; x++) {
                for (int y = 0; y < oldNy+2; y++) {
                    delete [] n[x][y];  delete [] neq[x][y]; delete [] u[x][y];
                }
                delete [] n[x];  delete [] neq[x]; delete [] u[x];
            }
        }
        delete [] n;  delete [] neq;  delete [] u;
*/
/*  */
        n = new double** [Nx+2];
        neq = new double** [Nx+2];
        u = new double** [Nx+2];

        for (int x = 0; x < Nx+2; x++) {

            n[x] = new double* [Ny+2];
            neq[x] = new double* [Ny+2];
            u[x] = new double* [Ny+2];
            for (int y = 0; y < Ny+2; y++) {
                n[x][y] = new double [Nc];
                neq[x][y] = new double [Nc];
                u[x][y] = new double [2];
            }
        }

}

void LBM::initialize_u()
{
    for (int x = 0; x < Nx+2; x++)
    for (int y = 0; y < Ny+2; y++) {
        for (int d = 0; d < 2; d++)
            u[x][y][d] = u0[d] ;   // u[x][y][0] – x-ая компонента скорости в точке (x,y),
                                   // u[x][y][1] – y-ая компонента скорости в точке (x,y), d=0,1
                                   // y-компонента = 0
    }
}

void LBM::density()
{/*
    for (int x = 0; x < Nx+2; x++){
       for (int y = 0; y < Ny+2; y++){
           double summ=0;
            for (int i = 0; i < Nc; i++) {
                summ+=n[x][y][i];
            }
         //   qDebug()<<"summ = "<<summ;
            rho[x][y]=summ;
       }
    }*/
  //  qDebug()<<"dfdf";
}

void LBM::compute_neq()
{
    for (int x = 0; x < Nx+2; x++){
       for (int y = 0; y < Ny+2; y++){
          double uSqd = u[x][y][0] * u[x][y][0] + u[x][y][1] * u[x][y][1];
           uSqd /= 2 * csSqd;
            for (int i = 0; i < Nc; i++) {
               double uci = u[x][y][0] * c[i][0] + u[x][y][1] * c[i][1];
               uci /= csSqd;
               neq[x][y][i] = rho * w[i] * (1 + uci * (1 + uci / 2) - uSqd);}
      }
    }
}

void LBM::initialize_n()
{
    for (int x = 0; x < Nx; x++)
        for (int y = 0; y < Ny; y++)
            for (int i = 0; i < Nc; i++)
                n[x][y][i] = neq[x][y][i]; // f = feq
}

void LBM:: initialize_u_on_the_surface()
{
    for (int x = 1; x <= Nx; x++){
         u[x][Ny+1][0] = uf[0];     //икс-компонента на верхней стенке = 0.2
         u[x][Ny+1][1] = uf[1];     //игрек компонента на верхней стенке = 0
    }
}

void LBM::initialize()
{
    qDebug()<<"begin of initialize" ;
    StepTime=0;
 //   qDebug()<<Velocity_X<<endl ;
     deallocate();  //освобождаем память
    getInput();    //считываем данные
    TempVector_for_X.clear();
    Velocity_X.clear();
    TempVector_for_Y.clear();
    Velocity_Y.clear();
    allocate();    //инициал.массив
    fillZero();    //заполняем его нулями

    initialize_u();
    initialize_u_on_the_surface();

    compute_neq();
    initialize_n();
   // density();
    qDebug()<<"end of initialize" ;
}

void LBM::boundaryConditions()
{
    // на левой и правой стенке – периодические гранусловия, на верхней и нижней – прилипание.
    // periodic boundary conditions for east moving particles at east/west ends
    //верхняя no-slip Couette variant 1
  /**/ for (int x = 1; x <= Nx; x++){
        n[x][Ny + 1][South]     = n[x][Ny][North]      ;
        n[x][Ny + 1][SouthEast] = n[x][Ny][NorthWest] +0.5*(n[x][Ny][West]-n[x][Ny][East])+rho*(uf[0]/2.0)   ;
        n[x][Ny + 1][SouthWest] = n[x][Ny][NorthEast] -0.5*(n[x][Ny][West]-n[x][Ny][East])-rho*(uf[0]/2.0)   ;
    }


    //верхняя no-slip Couette variant 2
  /*  for (int x = 1; x <= Nx; x++){
        n[x][Ny + 1][South]     = n[x][Ny][North]      ;
        n[x][Ny + 1][SouthEast] = n[x+1][Ny][NorthWest]+0.0*(n[x+1][Ny][West]-n[x+1][Ny][East])+rho*(uf[0]/2.0)   ;
     //   n[x][Ny + 1][SouthWest] = n[x-1][Ny][NorthEast]-0.5*(n[x][Ny][West]-n[x][Ny][East])-rho*(uf[0]/2.0)   ;
    }
    for (int x = Nx; x <= 1; x--){
        //    n[x][Ny + 1][South]     = n[x][Ny][North]      ;
        //    n[x][Ny + 1][SouthEast] = n[x][Ny][NorthWest]+0.5*(n[x][Ny][West]-n[x][Ny][East])+rho*(uf[0]/2.0)   ;
            n[x][Ny + 1][SouthWest] = n[x-1][Ny][NorthEast]-0.0*(n[x-1][Ny][West]-n[x-1][Ny][East])-rho*(uf[0]/2.0)   ;
        }
      n[Nx][Ny + 1][SouthEast] = rho*(uf[0]/2.0)   ;
      n[1][Ny + 1][SouthWest] = -rho*(uf[0]/2.0)   ;*/
/**/
     //periodic Левая стенка
   for (int y = 1; y <= Ny; y++) {
       n[Nx+1][y][NorthWest] =   n[1][y][NorthWest];
       n[Nx+1][y][West] =        n[1][y][West] ;
       n[Nx+1][y][SouthWest] =   n[1][y][SouthWest] ;
     }

  //periodic. Правая стенка
   for (int y = 1; y <= Ny; y++) {
         n[0][y][NorthEast] =   n[Nx][y][NorthEast];
         n[0][y][East] =        n[Nx][y][East] ;
         n[0][y][SouthEast] =   n[Nx][y][SouthEast] ;
     }

    //левая и правая стенки bounce-back - невязкая жидкость
    //левая
  /*  for (int y = 0; y <= Ny+1; y++) {
            n[0][y][East]         =  n[1][y][West] ;
            n[0][y][SouthEast]    =   n[1][y][SouthWest] ;
            n[0][y][NorthEast]    =   n[1][y][NorthWest] ;

      }
  */
    //no-slip
   /* for (int y = 0; y <= Ny+1; y++) {
            n[0][y][East]         =  n[1][y][West] ;
            n[0][y][SouthEast]    =   n[1][y][NorthWest] ;
            n[0][y][NorthEast]    =   n[1][y][SouthWest] ;

      }
    //no - slip
    for (int y = 0; y <= Ny+1; y++) {
            n[Nx+1][y][NorthWest]=  n[Nx][y][SouthEast]  ;
            n[Nx+1][y][West]     =    n[Nx][y][East]        ;
            n[Nx+1][y][SouthWest]    = n[Nx][y][NorthEast]   ;
        }
*/
    /*
     *     NorthWest   North       NorthEast
     *     West                    East
     *     SouthWest   South       SouthEast
     *
     * 6  2  5
     * 3     1
     * 7  4  8
     *            |
     *  unknown   |    known nodes
     *   nodes    |
     *            |
     *
     *   for left
     */

    //правая bounce-back
 /*   for (int y = 0; y <= Ny+1; y++) {
        n[Nx+1][y][NorthWest]=    n[Nx][y][NorthEast];
        n[Nx+1][y][West]     =    n[Nx][y][East]        ;
        n[Nx+1][y][SouthWest]    =   n[Nx][y][SouthEast] ;
    }
*/

/*
    for (int y = 1; y <= Ny; y++) {
           n[0][y][East] =      n[Nx][y][East];
           n[0][y][NorthEast] = n[Nx][y][NorthEast];
           n[0][y][SouthEast] = n[Nx][y][SouthEast];
       }

       // periodic boundary conditions for west moving particles at east/west ends
       for (int y = 1; y <= Ny; y++){
           n[Nx + 1][y][West]      = n[1][y][West];
           n[Nx + 1][y][NorthWest] = n[1][y][NorthWest];
           n[Nx + 1][y][SouthWest] = n[1][y][SouthWest];
       }
       */
     // no-slip boundary conditions for south moving particles at the bottom end
   //variant 1
  /* */  for (int x = 0; x <= Nx+1; x++) {
         n[x][0][North]     = n[x][1][South];
         n[x][0][NorthWest] = n[x][1][SouthEast];
         n[x][0][NorthEast] = n[x][1][SouthWest];
     }

   // no-slip variant 2
 /*  for (int x = 1; x <= Nx; x++) {
            n[x][0][North]     = n[x][1][South];
         //   n[x][0][NorthWest] = n[x][1][SouthEast];
            n[x][0][NorthEast] = n[x+1][1][SouthWest];
        }
    for (int x = Nx; x <= 1; x--) {
        n[x][0][NorthWest] = n[x-1][1][SouthEast];
    }
    n[Nx][0][NorthEast] = 0;
     n[1][0][NorthWest] = 0;*/

}

void LBM::freeStream()
{
    // start at NW corner and stream north and north-west moving particles
    for (int x = 1; x <= Nx; x++)
    for (int y = Ny; y >= 1; y--) {
        n[x][y][North    ] = n[x][y - 1][North];
        n[x][y][NorthWest] = n[x + 1][y - 1][NorthWest];
    }

    // start at NE corner and stream east and north-east moving particles
    for (int x = Nx; x >= 1; x--)
    for (int y = Ny; y >= 1; y--) {
        n[x][y][East] = n[x - 1][y][East];
        n[x][y][NorthEast] = n[x - 1][y - 1][NorthEast];
    }

    // start at SE corner and stream south and south-east moving particles
    for (int x = Nx; x >= 1; x--)
    for (int y = 1; y <= Ny; y++) {
        n[x][y][South] = n[x][y + 1][South];
        n[x][y][SouthEast] = n[x - 1][y + 1][SouthEast];
    }

    // start at SW corner and stream west and south-west moving particles
    for (int x = 1; x <= Nx; x++){
    for (int y = 1; y <= Ny; y++) {
        n[x][y][West] = n[x + 1][y][West];
        n[x][y][SouthWest] = n[x + 1][y + 1][SouthWest];
         }
    }

    /*
    if(StepTime%100==0)
    {
    qDebug()<<" ********************************* "<<endl;
    qDebug()<<" function n Ny "<<endl;
    for (int x = 1; x <= Nx; x++){
        qDebug()<<n[x][Ny][2];
    }
    qDebug()<<" function n  Ny+1"<<endl;
    for (int x = 1; x <= Nx; x++){
        qDebug()<<n[x][Ny+1][2];
    }
    qDebug()<<" ********************************* "<<endl;

     }*/
}

void LBM::compute_u()
{
    for (int y = 1; y <= Ny; y++){
       for (int x = 1; x <= Nx; x++){
            for (int d = 0; d < 2; d++){
                 u[x][y][d] = 0;
                 for (int i = 1; i < Nc; i++){
                  u[x][y][d] += n[x][y][i] * c[i][d];
                  u[x][y][d] /= rho;

                 }
             }
         }
    }

    /*
    qDebug()<<"Ny "<<endl;
   for (int x = 1; x <= Nx; x++){
       qDebug()<<u[x][Ny][0];
   }

   qDebug()<<"Ny+1 "<<endl;
  for (int x = 1; x <= Nx; x++){
      qDebug()<<u[x][Ny+1][0];
  }
  qDebug()<<"_______________________________"<<endl;*/
}

void LBM::collide()
{
    for (int x = 0; x <= Nx+1; x++)
    for (int y = 0; y <= Ny+1; y++)
    for (int i = 0; i < Nc; i++)
        n[x][y][i] -= (n[x][y][i] - neq[x][y][i]) / tau;

}

void LBM::force()
{
    /*
     *            D2Q9
     *
         NorthWest  North    NorthEast
         West                East
         SouthWest  South    SouthEast
    */

    double nu = csSqd * (tau - 0.5);
     double f = 8 * nu * uf[0] * rho / (6.0 * Ny * Ny);

    /*цикл для движения жидкости. Задан градиент силы. "Бежит" с запада на восток */
    //Пуазейлевское течение. Градиент для каждого узла.
    //Для Куэттовского течения, нужно задать градиент только для верхнего слоя

    // By Default:
 /* */  for (int x = 1; x <= Nx; x++){
        for (int y = 1; y <= Ny; y++){
             n[x][y][East] += f;
             n[x][y][NorthEast] += f;
             n[x][y][SouthEast] += f;

             n[x][y][West] -= f;
             n[x][y][NorthWest] -= f;
             n[x][y][SouthWest] -= f;
          }
      }
    /*  //Для течения Куэтта
    // "прокачка" только для верхней стенки
       for (int x = 1; x <= Nx; x++)
        {
           n[x][Ny][East] += f;
           n[x][Ny][NorthEast] += f;
           n[x][Ny][SouthEast] += f;

           n[x][Ny][West] -= f;
           n[x][Ny][NorthWest] -= f;
           n[x][Ny][SouthWest] -= f;
       }
*/
}

void LBM::deallocate()
{
    /* */
 static int oldNx = 0, oldNy = 0;
    if (Nx != oldNx) {
        if (n != 0) {
            for (int x = 0; x < oldNx+2; x++) {
                for (int y = 0; y < oldNy+2; y++) {
                    delete [] n[x][y];  delete [] neq[x][y]; delete [] u[x][y];
                }
                delete [] n[x];  delete [] neq[x]; delete [] u[x];
            }
        }
        delete [] n;  delete [] neq;  delete [] u;
   }
}


void LBM::fillZero()
{
    for (int x = 0; x <  Nx+2; x++) {
        for (int y = 0; y <  Ny+2; y++) {
            for(int i=0;i<Nc;i++){
                n[x][y][i]=0;
                neq[x][y][i]=0;
            }
            u[x][y][1]=0;
            u[x][y][0]=0;
          //  rho[x][y]=1;
        }
    }
}

void LBM::output_the_result()
{
    for (int y = 1; y <= Ny; y++){
        for (int x = 1; x <= Nx; x++){}
        TempVector_for_X.push_back(u[90][y][0]); // заполнили иксовым значением на определенном шаге, в узле 50
        TempVector_for_Y.push_back(u[90][y][1]); // игрек компонента
    }

    //   qDebug()<<"X COMPONENT"<<endl;
    //  qDebug()<<TempVector_for_X<<endl;
    //   qDebug()<<"_________________________________________________________________________"<<endl<<endl<<endl;
    Velocity_X.push_back(TempVector_for_X); // таким образом, на каждом шаге пополняем нужный вектор и освобождаем временной
    TempVector_for_X.clear();

    //*************************************************************************************
    Velocity_Y.push_back(TempVector_for_Y);
    //   qDebug()<<"Y COMPONENT"<<endl;
    //   qDebug()<<TempVector_for_Y<<endl;
    //  qDebug()<<"_________________________________________________________________________"<<endl<<endl<<endl;
    TempVector_for_Y.clear();
}

void LBM::output_f_and_feq(ofstream &filestr)
{    
    // ofstream  f_feq;
    // f_feq.open ("f_feq.txt", ios_base::out | ios_base::trunc);
    filestr<<"Step Time = "<<StepTime<<endl;
    for (int x = 0; x <= Nx+1; x++){
        for (int y = 0; y <= Ny+1; y++){
            double summ_rho=0;
            for (int i = 0; i < Nc; i++) {
                summ_rho+=n[x][y][i];}
            for (int i = 0; i < Nc; i++) {
                filestr<<x<<"    "<<y<<"       "<<i<<"      ";
                filestr<<n[x][y][i]<<"            "<<neq[x][y][i];
                filestr<<"      "<<summ_rho <<endl;
            }
        }
    }
}

void LBM::output_v(std::ofstream &filestr)
{
    filestr<<"Step Time = "<<StepTime<<endl;
    for (int x = 0; x <= Nx+1; x++){
        for (int y = 0; y <= Ny+1; y++){
         //   for (int i = 0; i < 2; i++) {
                filestr<<x<<"     "<<y<<"        "<<"      ";
                filestr<<"Vx=  "<<u[x][y][0]<<"      ";
                filestr<<"Vy=  "<<u[x][y][1]<<"      "<<endl;
          //  }
        }
    }
}

void LBM::create_file_for_matlab( )
{
    ofstream  filestr_Vx;
    filestr_Vx.open ("Vx.txt", ios_base::out | ios_base::trunc);
    if(filestr_Vx.is_open()==false)
    {
        cout << "error! didn't open the file!"<<endl;
        //return -1;
    }
    else cout<<"The file has been opened!"<<endl;

    ofstream  filestr_Vy;
    filestr_Vy.open ("Vy.txt", ios_base::out | ios_base::trunc);
    if(filestr_Vy.is_open()==false)
    {
        cout << "error! didn't open the file!"<<endl;
        //return -1;
    }
    else cout<<"The file has been opened!"<<endl;

    for (int y = 0; y <= Ny+1; y++){
        for (int x = 0; x <= Nx+1; x++){
         //  filestr_Vx<<setprecision(5)<<setw(30)<<u[x][y][0] ;
            filestr_Vx<<u[x][y][0]<<endl;
            filestr_Vy<< u[x][y][1]<<endl;
        }
        //filestr_Vx<<endl;
    }
    filestr_Vx.close();
    filestr_Vy.close();
}

void LBM::is_valid(double ***arr_)
{
     for (int x = 0; x <= Nx+1; x++){
         float summNy=0;
         float summNy_1=0;
         for (int i = 0; i < Nc; i++)
         {
             summNy+=arr_[x][Ny][i];
             summNy_1+=arr_[x][Ny+1][i];
         }
         qDebug()<<"********************";
         qDebug()<<"Ny=       "<<summNy ;
         qDebug()<<"Ny+1=     "<<summNy_1;
         qDebug()<<"x = "<<x<<endl;
     }
}

void LBM::outVx_50(std::ofstream &filestr)
{
     for (int y = 0; y <= Ny; y++){
         filestr<<u[50][y][0]<<endl;
     }
}

void LBM::output_the_result_to_file(ofstream &filestr) //ссылка на объект
{
    filestr<<"Step Time = "<<StepTime<<endl;
    filestr<<"Node x Number "<<10<<setw(49)<<"Node x Number "<<50<<setw(47)<<"Node x Number "<<90<<endl<<endl;
    for (int y = 0; y <= Ny+1; y++){
        filestr<<setprecision(5)<<setw(15)<<u[2][y][0]<< setw(17)<< setprecision(5)<<u[2][y][1];       // 10 узел
        filestr<<setprecision(5)<<setw(15)<<u[50][y][0]<< setw(17)<< setprecision(5)<<u[50][y][1];       // 50 узел
        filestr<<setprecision(5)<<setw(15)<<u[98][y][0]<< setw(17)<< setprecision(5)<<u[98][y][1]<<endl; // 90 узел
    }
        StepTime++;
}




void LBM::takeStep( )
{
    boundaryConditions();
    freeStream();
    compute_u( );
 //  initialize_u_on_the_surface();
    output_the_result();                 //заполняются векторы для построения графиков
   // density();
    compute_neq();
    collide();
 //   output_f_and_feq();
 //   is_valid(n);
  //     force();
  double  nu = csSqd * (tau - 0.5);
    qDebug()<<"nu = "<<nu;

    double Re = 0.2 * 100 / nu;
    qDebug()<<"Re = "<<Re;
}
