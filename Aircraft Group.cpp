#include "Aircraft.hpp"
void InitiateGroup(Aircraft air[]){
    double Array[max];
    srand(int (time(NULL)));
    for (int i=0; i <= max; i++){
        Array[i] =rand()%5000;
        Array[i] =Array[i]/10.0-250;
//        int n, m, temp;
//        for (m = 1; m <= max-1; m++){
//            for (n = m-1; n >= 0 ; n--){
//                if( Array[n] > Array[n+1]){
//                    temp = Array[n+1];
//                    Array[n+1] = Array[n];
//                    Array[n] = temp;
//                }
//            }
//        }
    }
//    for (int i = 0; i<max; i++){
//        Array[i] = (i + rand()%100/200)*20-250;
//    }
    int titime=0;
    while (titime<2){
        air[4*titime]= Aircraft (Array[titime],-250,Speed,π/2);
//        titime++;
//        do {titime++;} while (Array[titime]!=Array[titime-1]);
        air[4*titime+1]= Aircraft (250,Array[titime],Speed,-π);
//        titime++;
//        do {titime++;} while (Array[titime]!=Array[titime-1]);
        air[4*titime+2]= Aircraft (Array[titime]+10,250,Speed,-π/2);
//        titime++;
//        do {titime++;} while (Array[titime]!=Array[titime-1]);
        air[4*titime+3]= Aircraft (-250,Array[titime]+10,Speed,0);
        titime++;
//        do {titime++;} while (Array[titime]!=Array[titime-1]);
    }
//    while (titime<80){
//        air[2*titime]= Aircraft (Array[titime],-250,Speed,π/2);
//        //        titime++;
////        //        do {titime++;} while (Array[titime]!=Array[titime-1]);
////        air[4*titime+1]= Aircraft (250,Array[titime],Speed,-π);
////        //        titime++;
//        //        do {titime++;} while (Array[titime]!=Array[titime-1]);
//        air[2*titime+1]= Aircraft (Array[titime],250,Speed,-π/2);
////        //        titime++;
////        //        do {titime++;} while (Array[titime]!=Array[titime-1]);
////        air[4*titime+3]= Aircraft (-250,Array[titime],Speed,0);
//        titime++;
//        //        do {titime++;} while (Array[titime]!=Array[titime-1]);
//    }
    air[8] =Aircraft (-40,-250,Speed,π/2);
    air[9] =Aircraft (-250,-200,Speed,0);
//    air[2] =Aircraft (-245,-250,Speed,π/2);
//    air[3] =Aircraft (-250,-245,Speed,0);
}

void CountCrash(Aircraft air[],double CrashArray[][4],int times,int &time, double timegap){
    double self[max][2][2];
    for(int i = 0; i<max; i++)
        air[i].selfmirror(self[i]);
    for(int i = 0; i<max; i++){
        for (int j = i+1; j<max;j++){
            double state[2][2];
            air[i].virtualmove(timegap, state[0]);
            air[j].virtualmove(timegap, state[1]);
            if(judge(self[i][0],state[0],self[j][1]) && judge(self[j][0], state[1], self[i][1])){
                CrashArray[time][0] = i;
                CrashArray[time][1] = j;
                CrashArray[time][2] = times;
                CrashArray[time][3] = 0;
                time ++;
                //Record the crash information: the two point & which round and more...
            }
//            else CrashArray[time][3] = 1;
        }
    }
}

void Move_par(Aircraft air[],double time){
    for (int i =0; i<max; i++){
        air[i].Move(time);
    }
}
void SpeedArrange(Aircraft air[]){
    for (int i = 0; i<max; i++) {
        air[i].ReturnSpeed();
    }
}
