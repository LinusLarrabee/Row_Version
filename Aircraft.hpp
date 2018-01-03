#ifndef Aircraft_hpp
#define Aircraft_hpp

#include <iostream>
#include <math.h>
#define Crack 1
#define π 3.14159265
#define max 10
#define MaxValue 50000.0
#define Speed 1.0
using namespace std;
class Aircraft{
private:
    double x, y;
    double speed,angle;
public:
    Aircraft(double x_=rand()%500-250, double y_=rand()%500-250,double s_=rand()%10, double a_=random()):x(x_),y(y_),speed(s_),angle(a_){
        srand(int (time(NULL)));
    }
    int Crash(double distance, double time,Aircraft &B,int i,double line[][4]);
    void Move(double time);
    void ReturnSpeed(){
        if(speed>2*Speed ||speed < Speed/100){
            speed = Speed;
        }
        if(x>250) {x=250;angle=-π;}
        if(x<-250) {x=-250;angle=0;}
        if(y>250) {y=250;angle=-π/2;}
        if(y<-250) {y=-250;angle=π/2;}
    }
    void selfmirror(double selfmirror[][2]);
    void virtualmove(double time,double state[2]);
    void Square(int lon,int lat, double Square[][4]);
    int JudgeGoodPoint(double point[][4],int total, double sigpoint[][4]);
    void VectorsComposit(double sc_norm, double sc_angle);
    void FindSC(double sig_po_line[][4],double &sc_no,double &sc_ang);
};

void InitiateGroup(Aircraft air[]);
int judge(double *point0,double*point1,double *line);
void CountCrash(Aircraft air[],double CrashArray[][4],int times,int &time, double timegap);
void Move_par(Aircraft air[],double time);
void VectorsComposition(double a1_norm, double a1_angle, double a2_norm, double a2_angle,double &a3_norm,double &a3_angle);
void Copy(double arr0[4],double arr1[4]);
void SpeedArrange(Aircraft air[]);
#endif /* Aircraft_hpp */
