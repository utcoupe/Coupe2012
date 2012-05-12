/**
 * \file 	urgDriverCompute.cpp
 * \author 	Xavier RODRIGUEZ
 * \date	10/03/2012
 *
 * Fontions de gestion des calculs de postions et interprétation des données
 * */
 
#include "global.h"
#include "urgDriver.h"
	
using namespace std;


//! calcul la position d'un robot en fonction d'un groupe de points
coord UrgDriver::computeBotLocation(const list<coord> & bot)
{
	coord rob;
	rob.x=0;
	rob.y=0;
	int nb=0;
	
	std::list<coord>::const_iterator it;
	for ( it=bot.begin() ; it!=bot.end(); it++ )
	{
		rob.x+=(*it).x;
		rob.y+=(*it).y;
		
		if(nb>0){
		rob.x/=2;
		rob.y/=2;
		}
		
		nb++;
	}
		
	// Changement de repére
	rob.y = LY - rob.y;
	
	// Erreur si le robot est trop prét du bord c'est surement une main
	// qui traine ^^ on est plus à une apporximation de se genre 
	if( (rob.y < TOL_BORDS) || (rob.y >(LY-TOL_BORDS)) || (rob.x <TOL_BORDS) || (rob.x >(LX-TOL_BORDS)) ) {
		coord a; a.x = -100 ; return a;
	}
	
	return rob;
}

//! vérifie si deux points sont suffisement prés pour appartenir au même robot
bool UrgDriver::checkPointBot(coord p1, coord p2)
{
	int cox=p1.x-p2.x;
	int coy=p1.y-p2.y;

	int distance = sqrt( cox*cox + coy*coy );
	if(distance>TOLERANCE){
		return false;
	}
	
	return true;
} 




long distanceAuBord(coord c) {
	
	int dx;
	if( c.x > LX/2 ) {
		dx=LX-c.x;
	}else {
		dx=c.x;
	}

	int dy;
	if( c.y > LY/2 ) {
		dy=LX-c.y;
	}else {
		dy=c.y;
	}
	
	if(dx<dy) {
		return dx;
	} else {
		return dy;
	}
	
}





//! Traitement des données venant de l'hokuyo
void UrgDriver::interpretData(const std::vector<long> & data, int n)
{	
	#if DEBUG
		cout << endl << endl << endl;
	#endif
	
	robot.clear();
	list<coord> bot;			// Un robot peut étre composé de plusieurs points
	
	for(int j = indexMin; j < indexMax; ++j) {
		long l = data[j];
		
		#if DEBUG 
			cout << " : Angle = " << urg.index2deg(j) << " | " << l << "/" << distanceMax[j] << endl;
		#endif
		
		if(l>minLength && l<distanceMax[j])
		{
			coord c;
			double radian = urg.index2rad(j);
			radian = ABS(radian);
			c.x = l*cos(radian)  + deltaX; 
			c.y = l*sin(radian)  + deltaY;
		
			#if DEBUG
				cerr << "X = " << c.x << " Y = " << c.y << endl;
			#endif 
				
			if(bot.empty())
			{
				bot.push_front(c);
			}
			else
			{
				if(checkPointBot(bot.front(), c))
				{
					bot.push_front(c);
				}
				else
				{
					coord res = computeBotLocation(bot);
					if(res.x != -100) {
						robot.push_front(res);
					}
					bot.clear();
					bot.push_front(c);
				}
			}
		}
		else
		{
			if(!bot.empty())
			{
				coord res = computeBotLocation(bot);
				if(res.x != -100) {
					robot.push_front(res);
				}
				bot.clear();
			}
		}
	}


	
	// Erreur si plus de quatres robots 
	while(robot.size()>NB_MAX_ROBOT) 
	{
		
		list<coord>::iterator it;
		list<coord>::iterator it2 = robot.begin();
		int tmp = distanceAuBord(*(robot.begin()));
		for ( it=robot.begin() ; it != robot.end() ; it++ ) {
			
			coord value = (*it);
			int lololol = distanceAuBord(value);
			if(lololol<tmp) {
				tmp = lololol;
				it2 = it;
			}
			
		}
		
		robot.erase(it2);
	
	}
 	


	#if DEBUG 
		sleep(2);
	#endif
		
}
