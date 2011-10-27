#ifndef INCLUDE___
#define INCLUDE___

#include <cmath>
#include <ctime>

#define PI 3.14

using namespace std;

class Asserv {

    //variable asserv
    double x;
    double y;
    double angle;
    double speed;
    double speedX;
    double speedY;
    double speedA;
    double goalx;
    double goaly;
    double goala;
    clock_t start;

public :

    void init() {

        //initPos = vector3df(-1.5+0.2,0.2,-1+0.2);
        //robot->setPosition(initPos);
        x=300;
        y=300;
        angle=0;
        speed=0;
        speedX=0;
        speedY=0;
        speedA=0;
        goalx=x;
        goaly=y;
        goala=angle;
        start = clock();
    }

    void run() {
        runCalc();
    }


    int getX() {
        return x;
    }
    int getY() {
        return y;
    }
    double getAngle() {
        return angle;
    }

    void runCalc() {
        //robot->setPosition()
        //robot->setRotation(vector3df(0,45,0));

        //cout<<(double)(clock () - start)/ CLOCKS_PER_SEC<<endl;
        if(((double)(clock() - start) / CLOCKS_PER_SEC) >=0.1) {
            start = clock();


            double deltaA = fmod(goala - angle,2.0*PI);
            double deltaX = fabs(x - goalx);
            double deltaY = fabs(y - goaly);

            if(fabs(deltaA)>=0.01) {
                if (deltaA <= PI) {
                    speedA = deltaA/4.0;
                } else {
                    speedA = -deltaA/4.0;
                }
            } else {
                speedA = 0;
            }


            speedX = cos(angle)*speed;
            speedY = sin(angle)*speed;
            if(x!=goalx && y!=goaly) {
                goala = atan2(goaly - y,goalx - x);
                if(goala < 0) {
                    goala = goala + (2.0*PI);
                }
            }

            //test de destination
            if(deltaX>=100.0 || deltaY>=100.0) {
                speed=deltaX/4.0;
                if(speed > 50.0) speed = 50.0;
            } else {
                speed=0;
                //System.out.println(x+" / "+y);
            }

            //iterateurs
            x = x + speedX;
            y = y + speedY;
            if(angle + speedA < 0) {
                angle = angle + (2.0*PI);
            }


            angle = fmod(angle + speedA,2.0*PI);

            //cout<<x<<" "<<y<<" "<<angle<<" "<<goala<<endl;

            //move(x,y,angle);
        }
    }

    void goTo(int x,int y) {
        goalx=x;
        goaly=y;
    }

    void turn(double a) {
        goala=a;
    }

};

#endif
