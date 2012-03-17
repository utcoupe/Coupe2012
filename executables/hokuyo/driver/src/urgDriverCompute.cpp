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
coord UrgDriver::computeBotLocation(list<coord> bot)
{
	coord rob;
	rob.x=0;
	rob.y=0;
	int nb=0;
	
	std::list<coord>::iterator it;
	for ( it=bot.begin() ; it!=bot.end(); it++ )
	{
		rob.x+=(*it).x;
		rob.y+=(*it).y;
		
		if(nb>0){
		rob.x/=2;
		rob.y/=2;
		}
		
		nb++;
		
		#if DEBUG
		std::cout << (*it).x << ":" << (*it).y << std::endl;
		std::cout << "__" << rob.x << ":" << rob.y << std::endl;
		#endif
	}
	
	// Changement de repére
	rob.y = LY - rob.y;
	
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

//! Traitement des données venant de l'hokuyo
void UrgDriver::interpretData(std::vector<long> data, int n)
{	
	
	// TODO
	// Erreur si plus de quatres robots
	// A gérer
	
	robot.clear();
	list<coord> bot;
	
	for(int j = indexMin; j < indexMax; ++j) {
		long l = data[j];
		
		if(l>minLength && l<distanceMax[j])
		{
			coord c;
			double radian = urg.index2rad(j);
			radian = ABS(radian);
			c.x = l*cos(radian); 
			c.y = l*sin(radian);
		
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
					robot.push_front(computeBotLocation(bot));
					bot.clear();
					bot.push_front(c);
				}
			}
		}
		else
		{
			if(!bot.empty())
			{
				robot.push_front(computeBotLocation(bot));
				bot.clear();
			}
		}
	}
	
}
