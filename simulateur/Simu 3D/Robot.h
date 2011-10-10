#ifndef ROBOT_H
#define ROBOT_H

#include "main.h"
#include "Window.h"
#include IRRLICHTPATH
#include <math.h>
#include <ctime>

using namespace irr;
using namespace scene;
using namespace video;
using namespace std;
using namespace core;
using namespace io;

class Robot
{
    IMeshSceneNode * robot;

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
	vector3df initPos;
	clock_t start;

    public:
        Robot();
        virtual ~Robot();
        bool load(std::string filename) {
            robot = Window::getSmgr()->addOctreeSceneNode(Window::getSmgr()->getMesh(filename.c_str()),0,-1,1);
            initPos = vector3df(-1.5+0.2,0.2,-1+0.2);
            robot->setPosition(initPos);
            x=0;
            y=0;
            angle=0;
            speed=0;
            speedX=0;
            speedY=0;
            speedA=0;
            goalx=3000;
            goaly=2000;
            goala=0;
            start = clock();

            return true;
        }
        void run(){
                //robot->setPosition()
                //robot->setRotation(vector3df(0,45,0));

                //cout<<(double)(clock () - start)/ CLOCKS_PER_SEC<<endl;
                if(((double)(clock() - start) / CLOCKS_PER_SEC) >=0.1){
                    start = clock();


                    double deltaA = fmod(goala - angle,2.0*PI);
                    double deltaX = fabs(x - goalx);
                    double deltaY = fabs(y - goaly);

                    if(fabs(deltaA)>=0.01){
                        if (deltaA <= PI) {
                            speedA = deltaA/4.0;
                        } else{
                            speedA = -deltaA/4.0;
                        }
                    }else{
                        speedA = 0;
                    }


                    speedX = cos(angle)*speed;
                    speedY = sin(angle)*speed;
                    if(x!=goalx && y!=goaly){
                        goala = atan2(goaly - y,goalx - x);
                        if(goala < 0){
                            goala = goala + (2.0*PI);
                        }
                    }

                    //test de destination
                    if(deltaX>=100.0 || deltaY>=100.0){
                        speed=deltaX/4.0;
                        if(speed > 50.0) speed = 50.0;
                    }else {
                        speed=0;
                        //System.out.println(x+" / "+y);
                    }

                    //iterateurs
                    x = x + speedX;
                    y = y + speedY;
                    if(angle + speedA < 0){
                        angle = angle + (2.0*PI);
                    }


                    angle = fmod(angle + speedA,2.0*PI);

                    cout<<x<<" "<<y<<" "<<angle<<" "<<goala<<endl;

                    move(x,y,angle);
                }
        }
        void move(int x,int y,double angle){
            //x,y en mm angle en radian
            robot->setPosition(initPos+vector3df(x/1000.0,0,y/1000.0));
            robot->setRotation(vector3df(0,-angle*(180.0/3.14),0));
            //robot->setRotation(vector3df(0,angle,0));

        }
    protected:
    private:
};

#endif // ROBOT_H
