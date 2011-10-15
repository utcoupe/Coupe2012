#ifndef ROBOT_H
#define ROBOT_H

#include "main.h"
#include "Window.h"
#include "Irc.h"
#include IRRLICHTPATH
#include <math.h>
#include <ctime>

#include <iostream>
#include <sstream>
#include <string>

using namespace irr;
using namespace scene;
using namespace video;
using namespace std;
using namespace core;
using namespace io;

class Robot : public IRCConnection
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
	bool ackGoTo;
	bool ackTurn;
	vector3df initPos;
	clock_t start;

    public:
        Robot();
        virtual ~Robot();

        void goTo(int x , int y){
            goalx = x;
            goaly = y;
            ackGoTo = true;
        }

        void goTo2(int gx , int gy){
            double goala2 = atan2(gy - y,gx - x);
            if(goala2 < 0){
                goala2 = goala2 + (2*PI);
            }
            if(fabs(fmod((goala2 - angle),(2*PI))>(PI/8))) {
                turnRad(goala2);
                while(ackTurn==true){}
            }
            goTo(gx , gy);
        }

        void turn(double a){//a en degree
            turnRad((a*180.0)/3.14);
        }

        void turnRad(double a){//a en radian
            goala=a;
            ackTurn = true;
        }

        void stop(){
            goalx=x;
            goaly=y;
            goala=angle;
        }

        void doCmd(std::string time,std::string chan,std::string user,std::string msg){
            std::istringstream iss(msg);
            std::vector<std::string> args;
            do
            {
                std::string sub;
                iss >> sub;
                args.push_back(sub);
            } while (iss);

            if(args[0]=="move"){
                goTo(atoi(args[1].c_str()),atoi(args[2].c_str()));
            }
        }

        bool load(std::string filename) {
            robot = Window::getSmgr()->addOctreeSceneNode(Window::getSmgr()->getMesh(filename.c_str()),0,-1,1);
            initPos = vector3df(-1.5,0.2,-1);
            robot->setPosition(initPos);
            x=200;
            y=200;
            angle=0;
            speed=0;
            speedX=0;
            speedY=0;
            speedA=0;
            goalx=x;
            goaly=y;
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
                        if(ackTurn==true){
                            //send ack msg
                            ackTurn=false;
                        }
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
                        if(ackGoTo==true){
                            //send ack
                            ackGoTo=false;
                        }
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

                    //cout<<x<<" "<<y<<" "<<angle<<" "<<goala<<endl;

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
