//
//  main.cpp
//  Path Planning
//
//  Created by 孙浩 on 2017/12/23.
//  Copyright © 2017年 孙浩. All rights reserved.
//

#include <iostream>
#include "Aircraft.hpp"

double crackline[max][max][4];//k,b,orient,j
double square[max][4][4];
double point_line[max][max*2][4];
double significantpoint_line[max][max/10][4]={0};



void PointExist(double squ[][4], double cra[][4], double poi[][4],int Pos, int countPE[]){
    for (int i = 0; i < max; i++)
        for (int k = 4; k < 8; k++)
            if ((cra[i][3] != 0)) {
                if((squ[k%4][3]-cra[i][0]*squ[k%4][2]-cra[i][1])*(squ[(k-1)%4][3]-cra[i][0]*squ[(k-1)%4][2]-cra[i][1])<=0){
                    poi[countPE[Pos]][0] = -(squ[k%4][1]-cra[i][1])/(squ[k%4][0]-cra[i][0]);
                    poi[countPE[Pos]][1] = -(squ[k%4][1]*cra[i][0]-cra[i][1]*squ[k%4][0])/(squ[k%4][0]-cra[i][0]);
                    poi[countPE[Pos]][2] = cra[i][0];
                    poi[countPE[Pos]][3] = cra[i][1];
                    countPE[Pos] ++;
                }
            }
}

void FindLineGroup(Aircraft ai[], double line[][max][4], double distance, double timegap,int i){
        for(int j = 0; j < max; j++){
            if (j != i)
                ai[i].Crash(distance, timegap, ai[j], j, line[i]);
        }
}

void KB2line(double line0[4], double line1[4],double state[4]){
    Copy(line1, state);
    state[0]=(line0[3]-line1[3])/(line0[2]-line1[2]);
    state[1]=(line0[3]*line1[2]-line1[3]-line0[2])/(line1[2]-line0[2]);
}

int Points_Line(double point0[4], double point1[4], double line[4]){
    if((point1[1]-line[2]*point1[0]-line[3])*(point0[1]-line[2]*point0[0]-line[3])<0)
       return 1;
    else return 0;
}

void StateInsert(double state[4], double point[][4],int i,int SPG){
    for (int j = SPG; j>i; j--)
        Copy(point[j],point[j+1]);
    Copy(state, point[i+1]);
}

void DeleteSPG(int PointPlace[2],double PointLine[][4],int countSPG){
    int i = PointPlace[0];
    for (int j = PointPlace[1]; j<=countSPG; j++)
        Copy(PointLine[j],PointLine[++i]);
}

int SignificantPointGroup(double poi_li[][4],int Pos, double sig_poi_line[][4],Aircraft A){
    double state[4];
    int insertPointPlace[2]={0};
    int countS_=2,countSPG = 2,mark =0;
    KB2line(sig_poi_line[0], sig_poi_line[1], state);
    Copy(sig_poi_line[1], sig_poi_line[2]);
    Copy(state, sig_poi_line[1]);
    for(int i = 0; i<countSPG; i++){
        for(int j = 0; j<Pos; j+=2){
            if(sig_poi_line[i][3]!=poi_li[j][3]){
                if(Points_Line(sig_poi_line[i], sig_poi_line[i+1], poi_li[j])){
                    KB2line(sig_poi_line[i], poi_li[j], state);
                    StateInsert(state, sig_poi_line, i, countSPG);
                    insertPointPlace[0] = i+1;
                    for (int k = i;k < countSPG+1; k++){
                        if(sig_poi_line[i][3]!=poi_li[j][3]){
                            if (Points_Line(sig_poi_line[k], sig_poi_line[k+1], poi_li[j])) {
                                KB2line(sig_poi_line[i], poi_li[j], state);
                                StateInsert(state, sig_poi_line, k, countSPG+1);
                                insertPointPlace[1] = k+1;
                                break;
                            }
                        }
                    }
                    i++;
                    countSPG+=2;
                    mark = 1;
                    break;
                }
            }
        }
        if (countS_ !=countSPG) break;
    }
    DeleteSPG(insertPointPlace, sig_poi_line, countSPG);
    return mark;
}

void ChangeSpeed(Aircraft ai[],double lon, double lat, int distance,int timegap,int countPE[]){
    for (int i = 0; i<max; i++) {
        FindLineGroup(ai,crackline,distance,timegap,i);
        ai[i].Square(lon, lat, square[i]);
        PointExist(square[i], crackline[i], point_line[i],i,countPE);
        //        if(!air[i].JudgeGoodPoint(point_line[i], countPE[i], significantpoint_line[i]))
        //            break;
//        ai[i].JudgeGoodPoint(point_line[i],countPE[i],significantpoint_line[i]);
//        if (point_line[i][2][3]!=0) {
//            while(SignificantPointGroup(point_line[i],countPE[i],significantpoint_line[i],ai[i]));
//        }
        double sc_norm = 0, sc_angle = 0;
        ai[i].FindSC(point_line[i],sc_norm,sc_angle);
//        ai[i].FindSC(significantpoint_line[i],sc_norm,sc_angle);
        ai[i].VectorsComposit(sc_norm,sc_angle);
    }
}

int main(int argc, const char * argv[]) {
    double distance = 10, timegap = 3,totaltime=0;
    double lat =5,lon =5;
    double CrashArray[100*max][4]={0};
    Aircraft air[max];
    int crashcounttime=0;
    InitiateGroup(air);
    while(Speed*totaltime<500){
        int countPE[max] = {0};
        ChangeSpeed(air, lon, lat, distance,timegap,countPE);
        SpeedArrange(air);
        CountCrash(air, CrashArray, totaltime/timegap+1, crashcounttime,timegap);
        Move_par(air,timegap);
        totaltime += timegap;
    }
    return 0;
}
