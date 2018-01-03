
#include "Aircraft.hpp"
double newtan(double angle){
    if (tan(angle)>MaxValue)
        return MaxValue;
    else if (tan(angle) < -MaxValue)
        return -MaxValue;
    else if(tan(angle)<=1/MaxValue && tan(angle)>=0)
        return 1/MaxValue;
    else if(tan(angle)>=-1/MaxValue && tan(angle)<=0)
        return -1/MaxValue;
    else return tan(angle);
}

double newatan(double value){
    if(atan(value)<=1/MaxValue &&atan(value)>=0)
        return 1/MaxValue;
    else if(atan(value)>=-1/MaxValue && atan(value)<=0)
        return -1/MaxValue;
    else if (atan(value)>MaxValue)
        return MaxValue;
    else if (atan(value) < -MaxValue)
        return -MaxValue;
    else return atan(value);
}

void Copy(double arr0[4],double arr1[4]){
    for (int i = 0; i<4;i++)
        arr1[i] = arr0[i];
}


void VectorsComposition(double a1_norm, double a1_angle, double a2_norm, double a2_angle,double a3[]){
    a3[2] = a1_norm*cos(a1_angle)+a2_norm*cos(a2_angle);
    a3[3] = a1_norm*sin(a1_angle)+a2_norm*sin(a2_angle);
    a3[0] = sqrt(a3[2]*a3[2]+a3[3]*a3[3]);
    a3[1] = newatan(a3[3]/a3[2]);
    if (a3[2] < 0)
        if(a3[3] > 0) a3[1] += π;
        else a3[1] -= π;
        else;
}

void Aircraft::VectorsComposit(double sc_norm, double sc_angle){
    double rel_x = speed*cos(angle)+sc_norm*cos(sc_angle);
    double rel_y = speed*sin(angle)+sc_norm*sin(sc_angle);
    speed = sqrt(rel_x*rel_x+rel_y*rel_y);
    angle = newatan(rel_y/rel_x);
    if (rel_x < 0)
        if(rel_y > 0) angle += π;
        else angle  -=π;
        else;
}

void Aircraft::Move(double time){
    x += speed*cos(angle)*time;
    y += speed*sin(angle)*time;
}
void Aircraft::virtualmove(double time, double *state){
    state[0] =x + speed*cos(angle)*time;
    state[1] =y + speed*sin(angle)*time;
}
void Aircraft::selfmirror(double selfmirror[][2]){
    selfmirror[0][0] = x;
    selfmirror[0][1] = y;
    if (tan(angle)>500)
        selfmirror[1][0] = 500;
    else if (tan(angle) < -500)
        selfmirror[1][0] = -500;
    else selfmirror[1][0] = tan(angle);
    selfmirror[1][1] = y-selfmirror[1][0]*x;
}

int judge(double *point0, double*point1, double *line){
    if((point0[1]-line[0]*point0[0]-line[1]) * (point1[1]-line[0]*point1[0]-line[1])<=0)
        return 1;
    else return 0;
}

int Aircraft::Crash(double distance, double time,Aircraft &B,int i,double line[][4]){
    double V_relat_x = speed*cos(angle)-B.speed*cos(B.angle);
    double V_relat_y = speed*sin(angle)-B.speed*sin(B.angle);
    double dis_AB = sqrt((x-B.x)*(x-B.x)+(y-B.y)*(y-B.y));
    double stan_angle = asin(distance/dis_AB);
    double V_norm =sqrt(V_relat_x*V_relat_x+V_relat_y*V_relat_y);
    double V_relat_angle = newatan(V_relat_y/V_relat_x);//==NAN)?0.00001:atan(V_relat_y/V_relat_x);

    double AB_angle = newatan((y-B.y)/(x-B.x));
    if (x>=B.x)
        if(y <= B.y)
            AB_angle = π + AB_angle;
        else
            AB_angle = -π + AB_angle;
        else;
    if (V_relat_x < 0)
        if(V_relat_y > 0)
            V_relat_angle = π + V_relat_angle;
        else
            V_relat_angle = -π + V_relat_angle;
        else;
    
    double radius =distance/time;
    
    double front_x =(1-1/time) *x + 1/time* B.x;
    double front_y =(1-1/time) *y + 1/time* B.y;
    
    double Re_angle,sc_angle;
    if (AB_angle-V_relat_angle>=0){
        Re_angle = stan_angle - AB_angle +V_relat_angle;
        sc_angle = AB_angle - stan_angle-π/2;
    }else{
        Re_angle = stan_angle + AB_angle -V_relat_angle;
        sc_angle = AB_angle + stan_angle+π/2;
    }
    //    speed = speed*cos(angle)+0.5*sc_norm*cos(sc_angle);
    //    VectorsComposition(speed, angle, 0.5*sc_norm, sc_angle, speed, angle);
    
    //    VectorsComposition(V_norm, V_relat_angle, sc_norm, sc_angle, V_norm, V_relat_angle);

    if (stan_angle+AB_angle <V_relat_angle || AB_angle - stan_angle>V_relat_angle)
        return 5;
    else if(((front_x-V_relat_x-x)*(front_x-V_relat_x-x)+(front_y-V_relat_y-y)*(front_y-V_relat_y-y) >= radius*radius) && (front_x-x)*(front_x-x)+(front_y-y)*(front_y-y) > V_norm*V_norm)
        return 6;
    else {
        double sc_norm = sqrt(V_norm*V_norm*sin(Re_angle)*sin(Re_angle));
        //        speed = speed*cos(angle)+0.5*sc_norm*cos(sc_angle);
        if (dis_AB < distance){
            sc_angle = AB_angle;
            if (sc_angle<1/MaxValue && sc_angle >=0)
                sc_angle+= 1/MaxValue;
            if (sc_angle>-1/MaxValue && sc_angle <=0)
                sc_angle-= 1/MaxValue;
            sc_norm = distance-dis_AB;
        }
        double state[4];
        VectorsComposition(speed, angle, 0.50*sc_norm, sc_angle, state);
//        VectorsComposition(B.speed, B.angle, -0.55*sc_norm, sc_angle, state[1]);
        
        double slope = -1/newtan(sc_angle);
//        cout <<slope<<endl;
        double intercept =(y+state[3])-slope*(x+state[2]);
        double orientation = sin(sc_angle);//>0 or <0
        
        line[i][0] =slope;
        line[i][1] =intercept;
        line[i][2] =orientation;
        line[i][3] =i;
//        cout << slope<<endl<<intercept<<endl<<orientation<<endl;
        // though the double feature, the result is random if the crack will happen or not.
        return Crack;
    }
}

void Aircraft::Square(int lon, int lat, double (*Square)[4])
{
    double new_x = x + speed* cos(angle);
    double new_y = y + speed* sin(angle);
    double k = newtan(angle);
    double k_ve = -1/k;
    
    for(int i = 0; i<4;i++){
        if (i%2) Square[i][0] = k_ve;
        else Square[i][0] = k;
    }
    Square[1][1] = new_x+ lon* cos(angle);
    Square[1][1] = new_y+ lon* sin(angle) - Square[0][1]*Square[0][0];
    Square[3][1] = new_x- lon* cos(angle);
    Square[3][1] = new_y- lon* sin(angle) - Square[1][1]*Square[1][0];
    
    Square[0][1] = new_x+ lat* sin(angle);
    Square[0][1] = new_y+ lat* cos(angle) - Square[2][1]*Square[2][0];
    Square[2][1] = new_x- lat* sin(angle);
    Square[2][1] = new_y- lat* cos(angle) - Square[3][1]*Square[3][0];
    
    Square[0][2] = new_x+ lon* cos(angle) + lat* sin(angle);
    Square[1][2] = new_x+ lon* cos(angle) - lat* sin(angle);
    Square[2][2] = new_x- lon* cos(angle) - lat* sin(angle);
    Square[3][2] = new_x- lon* cos(angle) + lat* sin(angle);
    Square[0][3] = new_y+ lon* sin(angle) + lat* cos(angle);
    Square[1][3] = new_y+ lon* sin(angle) - lat* cos(angle);
    Square[2][3] = new_y- lon* sin(angle) - lat* cos(angle);
    Square[3][3] = new_y- lon* sin(angle) + lat* cos(angle);
}

int Aircraft::JudgeGoodPoint(double (*point)[4], int total, double (*sigpoint)[4]){
    if(!total) return 0;
    Copy(point[0],sigpoint[0]);
    Copy(point[1],sigpoint[1]);
    int tag = 1, countJGP = 0;
    for(int i = 2;i<total;i++){
        for(int j = 0; j<total; j+=2){
            if(i/2 != j/2)
                if((point[i][1]-point[j][2]*point[i][0]-point[j][3])*(y-point[j][2]*x-point[j][3])>0)
                    tag = 0;
        }
        if(tag) Copy(point[i],sigpoint[countJGP++]);
    }
    return countJGP;
}
double Norm( double x, double y){
    return sqrt(x*x+y*y);
}
void Aircraft::FindSC(double sig_po_line[][4],double &sc_no,double &sc_ang){
    double sc_angle, sc_norm;
    double disA[3],disB[3],xc,k,b;
    int sgn = 1;
    double new_x = x + speed* cos(angle);
    double new_y = y + speed* sin(angle);
    for(int i = 0; sig_po_line[i][3] != 0; i++){
        k = sig_po_line[i][2];
        b = sig_po_line[i][3];
        sc_angle = newatan(-1/sig_po_line[i][2]);
        sc_norm = (k*new_x-new_y+b)/sqrt(k*k+1);
        if (sc_norm<0) {
            sc_norm = -sc_norm;
            sgn =-1;
        }else sc_angle -=π;
        sc_ang = sc_angle;
        sc_no = sc_norm;
        disA[0] = sig_po_line[i][0];
        disA[1] = sig_po_line[i][1];
        disB[0] = sig_po_line[i+1][0];
        disB[1] = sig_po_line[i+1][1];
        disA[2] =Norm(disA[0]-new_x, disA[1]-new_y);
        disB[2] =Norm(disB[0]-new_x, disB[1]-new_y);
        xc = (k*new_y+new_x-k*b)/(k*k+1);
        if ((disA[0]-xc)/(disA[0]-disB[0])>1||(disA[0]-xc)/(disA[0]-disB[0])<0){
            if (disB[2]<disA[2]) {
                sc_norm = disB[2];
                sc_angle = newatan((disB[1]-new_y)/(disB[0]-new_x));
            }
            else{
                sc_norm = disA[2];
                sc_angle = newatan((disA[1]-new_y)/(disA[0]-new_x));
            }
            if(sgn==-1){
                if (sc_angle>0) sc_angle -=π;
                else sc_angle +=π;
            }
        }
        if (sc_norm) {
            sc_no = (sc_no <sc_norm)?sc_no:sc_norm;
            sc_ang = (sc_no <sc_norm)?sc_ang:sc_angle;
        }
    }
}
