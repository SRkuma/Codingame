#include <iostream>
#include <stdio.h>       // printf,scanf
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>  //sort,binarySearch
#include <functional> 
#include <iomanip>    // setprecision
#include <utility>    // c+11 Array
#include <set>
#include <sstream>
#include <map>
#include <bits/stdc++.h>
#define MOD (1e+9)+7
#define PI 3.141592653589793238462643383279
#define FOR(i,a,b) for(int i=(a);i<(b);++i)
#define REP(i,n)  FOR(i,0,n)
#define pb(x) push_back(x)
#define debug(x) cerr << #x << " = " << x << endl;
#define debret(x) debug(x) ; return 0;

using namespace std;

/**
 * Grab Snaffles and try to throw them through the opponent's goal!
 * Move towards a Snaffle and use your team id to determine where you need to throw it.
 **/

int mp;

class Wizard{
public:
    int x, y,state;
    int id;
    int vx,vy;
    //throw
    void throw2point(int x,int y,int power);
    void throw2goal(int myTeamid);
    // move
    void move2point(int x,int y);
    // spell
    void Obliviate(int eId);
    void Petrificus(int eId);
    void Accio(int eId);
    void Flipendo(int eId);
    // status
    void incrMp();
    void decrMp(int cost);
    void setStatusW(int eId,int ex,int ey,int eVx, int eVy, int eState);
};

class SnaffleState{
public:
    int x, y ,vx,vy,state,id;
    double dist;
    void setStatusS(int eId,int ex,int ey,int eVx, int eVy, int eState){
        id = eId;
        x = ex;
        y = ey;
        vx = eVx;
        vy = eVy;
        state = eState;
    };
    void calcDistS(Wizard w){
        
        double X = x-w.x;
        double Y = y-w.y;
        
        dist = sqrt(X*X+Y*Y);
    };

};
class BludgerState{
public:
    int x, y , vx, vy,state,id;
    double dist;
    void setStatusB(int eId,int ex,int ey,int eVx, int eVy, int eState){
        id = eId;
        x = ex;
        y = ey;
        vx = eVx;
        vy = eVy;
        state = eState;
    };
    void calcDistB(Wizard w){

    double X = x-w.x;
    double Y = y-w.y;

    dist = sqrt(X*X+Y*Y);
    };

};

void Wizard::throw2point(int x,int y,int power){

    cout << "THROW " << x << " " << y << " " << power << endl;
}

void Wizard::throw2goal(int myTeamid){

    if(myTeamid){
        throw2point(0, 3000, 500);
    }else{
        throw2point(16000, 3000, 500);
    }
}

void Wizard::move2point(int x,int y){

    cout << "MOVE " << x << " " << y << " " << 150 << endl;
}


void Wizard::Obliviate(int eId){
    mp -= 5;
    cout << "OBLIVIATE " << eId << endl;
}
void Wizard::Petrificus(int eId){
    mp -= 10;
    cout << "PETRIFICUS " << eId << endl; 
}
void Wizard::Accio(int eId){
    mp -= 20;
    cout << "ACCIO " << eId << endl;
}
void Wizard::Flipendo(int eId){
    mp -= 20;
    cout << "FLIPENDO " << eId << endl;
}


void Wizard::setStatusW(int eId,int ex,int ey,int eVx, int eVy, int eState){
    id = eId;
    x = ex;
    y = ey;
    vx = eVx;
    vy = eVy;
    state = eState;
}

int main()
{
    int myTeamId; // if 0 you need to score on the right of the map, if 1 you need to score on the left
    cin >> myTeamId; cin.ignore();
    Wizard wiz[2];
    Wizard opWiz[2];
    vector <SnaffleState> sPos;
    vector <BludgerState> bPos;
    int coolT = 0;
    // game loop
    while (1) {
        sPos.erase(sPos.begin(),sPos.end());
        bPos.erase(bPos.begin(),bPos.end());
        int entities; // number of entities still in game
        cin >> entities; cin.ignore();
        int wizCnt = 0;
        int opWizCnt = 0;
        int toward = -1;
        for (int i = 0; i < entities; i++) {
            int entityId; 
            string entityType; 
            int x; // position            
            int y; // position
            int vx; // velocity
            int vy; // velocity
            int state; // 1 if the wizard is holding a Snaffle, 0 otherwise
            cin >> entityId >> entityType >> x >> y >> vx >> vy >> state; cin.ignore();
            if(entityType == "WIZARD"){
                wiz[wizCnt].setStatusW(entityId,x,y,vx,vy,state);
                wizCnt++;
            }else if(entityType == "OPPONENT_WIZARD"){
                opWiz[opWizCnt].setStatusW(entityId,x,y,vx,vy,state);
                opWizCnt++;
            }else if(entityType == "SNAFFLE"){
                SnaffleState tmpS;
                tmpS.setStatusS(entityId,x,y,vx,vy,state);
                sPos.pb(tmpS);
            }else if(entityType == "BLUDGER"){
                BludgerState tmpB;
                tmpB.setStatusB(entityId,x,y,vx,vy,state);
                bPos.pb(tmpB);
            }
        }
        toward = -1;
        wizCnt = 0;
        bool isPt = false;
        int posIdSmin;
        int posIdB;
        while(wizCnt < 2){
            double mindistS = 1e+9;
            double mindistB = 1e+9;
            if(wiz[wizCnt].state == 1){
                bool exFront1= (opWiz[0].x > wiz[wizCnt].x ||
                                 (opWiz[0].x < wiz[wizCnt].x &&
                                  abs(opWiz[0].y - 3500) > 1000)) &&
                                (opWiz[1].x > wiz[wizCnt].x ||
                                 (opWiz[1].x < wiz[wizCnt].x && 
                                  abs(opWiz[1].y - 3500) > 1000)); 
                bool exFront2= (opWiz[0].x < wiz[wizCnt].x ||
                                 (opWiz[0].x > wiz[wizCnt].x &&
                                  abs(opWiz[0].y - 3500) > 1000)) &&
                                (opWiz[1].x < wiz[wizCnt].x ||
                                 (opWiz[1].x > wiz[wizCnt].x && 
                                  abs(opWiz[1].y - 3500) > 1000)); 
                if(((myTeamId && exFront1)|| (myTeamId && wiz[wizCnt].x < 2500))||
                   ((!myTeamId && exFront2) || (!myTeamId && wiz[wizCnt].x > 13500))){
                    wiz[wizCnt].throw2goal(myTeamId);
                    
                }else if(myTeamId){
                    wiz[wizCnt].throw2point(0,abs(wiz[wizCnt].y-2000),500);
                }else if(!myTeamId){
                   wiz[wizCnt].throw2point(16000,abs(wiz[wizCnt].y-2000),500);
                }
                wizCnt++;
            }else{
                

                if(mp >= 10 && coolT == 0){
                    int  maxX;
                    int  maxY;
                    int posIdSmax = -1;
                    double maxdistS = 0;
                    REP(j,sPos.size()){
                        if((myTeamId && 14000 > sPos[j].x && sPos[j].vx > 1300)
                           ||(!myTeamId && 2000 < sPos[j].x && sPos[j].vx < -1300)){
                            sPos[j].calcDistS(wiz[wizCnt]);
                                if(maxdistS < sPos[j].dist){
                                    maxdistS = sPos[j].dist;
                                    maxX = sPos[j].x;
                                    maxY = sPos[j].y;
                                    posIdSmax = sPos[j].id;
                                }
                        }
                    }
                    //Nothing to do
                    if(posIdSmax == -1){
                        
                    }else{
                        
                        wiz[wizCnt].Petrificus(posIdSmax);
                        wizCnt++;
                        coolT = 2;
                        continue;
                    }

                }
                
                if(mp >= 22){
                    int  maxX;
                    int  maxVx;
                    int  maxY;
                    int posIdSmax = -1;
                    double maxdistS = 0;
                    bool isFl = false;
                    REP(j,sPos.size()){
                        sPos[j].calcDistS(wiz[wizCnt]);
                        if(myTeamId && sPos[j].x < 2000 &&
                           abs(sPos[j].y - wiz[wizCnt].y) > 1000 && 
                           2500< sPos[j].y && sPos[j].y < 5500 && sPos[j].dist < 4000){
                            wiz[wizCnt].Flipendo(sPos[j].id);
                            wizCnt++;
                            isFl = true;
                            break;
                        }else if(!myTeamId && sPos[j].x > 14000 &&
                                 abs(sPos[j].y - wiz[wizCnt].y) > 1000 &&
                                 2500< sPos[j].y && sPos[j].y < 5500 && sPos[j].dist < 4000){
                            wiz[wizCnt].Flipendo(sPos[j].id);
                            wizCnt++;
                            isFl = true;
                            break;
                            
                        }
                        double difX = (wiz[wizCnt].x -sPos[j].x); 
                        if((sPos[j].x+sPos[j].vx < 16000
                            || sPos[j].x+sPos[j].vx > 0) && sPos[j].dist <= 3000
                           && sPos[j].dist < sqrt(10) * difX ){
                            if(maxdistS < sPos[j].dist){
                                maxdistS = sPos[j].dist;
                                maxX = sPos[j].x;
                                maxVx = sPos[j].vx;
                                maxY = sPos[j].y;
                                posIdSmax = sPos[j].id;
                            }
                        }
                    }
                    if(isFl) continue;
                    // Nothing to do
                    if(posIdSmax == -1){
                        
                    }else if(myTeamId
                             && wiz[wizCnt].x > maxX+maxVx
                             && abs(wiz[wizCnt].y - maxY)){
                        wiz[wizCnt].Flipendo(posIdSmax);
                        wizCnt++;
                        continue;
                    }else if(!myTeamId && wiz[wizCnt].x < maxX+maxVx){
                        wiz[wizCnt].Flipendo(posIdSmax);
                        wizCnt++;
                        continue;
                    }
                }
                
                if(mp >= 22){
                    int  maxX;
                    int  maxY;
                    int posIdSmax = -1;
                    double maxdistS = 0;
                    
                    REP(j,sPos.size()){
                        sPos[j].calcDistS(wiz[wizCnt]);
                        if((sPos[j].x+sPos[j].vx < 16000
                            || sPos[j].x+sPos[j].vx > 0) &&
                           1000 <= sPos[j].dist && sPos[j].dist <= 4000){
                            if(maxdistS < sPos[j].dist){
                                maxdistS = sPos[j].dist;
                                maxX = sPos[j].x;
                                maxY = sPos[j].y;
                                posIdSmax = sPos[j].id;
                            }
                        }
                    }
                    // Nothing to do
                    if(posIdSmax == -1){
                        
                    }else if((myTeamId && maxX > 12000 && maxX > wiz[wizCnt].x)
                             || (!myTeamId && maxX < 6000 && maxX < wiz[wizCnt].x)){
                        wiz[wizCnt].Accio(posIdSmax);
                        wizCnt++;
                        continue;
                    }
                }



                if(wizCnt == 2) break;

                if(mp >= 30){
                    if(myTeamId){
                        (opWiz[0].x > opWiz[1].x) ?
                            wiz[wizCnt].Flipendo(opWiz[0].id) :
                            wiz[wizCnt].Flipendo(opWiz[1].id) ;
                    }else{
                        (opWiz[0].x > opWiz[1].x) ?
                            wiz[wizCnt].Flipendo(opWiz[1].id) :
                            wiz[wizCnt].Flipendo(opWiz[0].id) ;
                    }
                    wizCnt++;
                    continue;
                    // REP(j,bPos.size()){
                    //     bPos[j].calcDistB(wiz[wizCnt]);
                    //     if(mindistB > bPos[j].dist){
                    //         mindistB = bPos[j].dist;
                    //         posIdB = bPos[j].id;
                    //     }
                    // }
                    // wiz[wizCnt].Obliviate(posIdB);
                    // wizCnt++;
                    // continue;
                    
                }

                

                REP(j,sPos.size()){

                    if(toward == sPos[j].id) continue;
                    if(sPos[j].state == 0){
                        sPos[j].calcDistS(wiz[wizCnt]);
                        if(mindistS > sPos[j].dist){
                            mindistS = sPos[j].dist;
                            posIdSmin = sPos[j].id;
                        }
                    }
                }
                REP(j,sPos.size()){
                    if(posIdSmin == sPos[j].id){
                        toward = posIdSmin;
                        wiz[wizCnt].move2point(sPos[j].x,sPos[j].y);
                        wizCnt++;
                        break;
                    }
                }
            }
        }
        if(coolT > 0) coolT--;
        if(mp < 100)
            mp++;
        cerr << mp << endl;
    }
}
