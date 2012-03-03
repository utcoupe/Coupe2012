/**
 * \file 	urgFile.h
 * \author 	Xavier RODRIGUEZ
 * \date	19/02/2012
 * 
 * */



//! calcul la position d'un robot en fonction d'un groupe de points
coord computeBotLocation(std::list<coord> bot)
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
bool checkPointBot(coord p1, coord p2)
{
	int cox=p1.x-p2.x;
	int coy=p1.y-p2.y;

	int g_distance = sqrt( cox*cox + coy*coy );
	if(g_distance>TOLERANCE){
		return false;
	}
	
	return true;
} 

//! Traitement des données venant de l'hokuyo
void interpretData(std::vector<long> data, int n, UrgCtrl* urg)
{	
	robot.clear();
	std::list<coord> bot;
	
	for(int j = g_indexMin; j < g_indexMax; ++j) {
		long l = data[j];
		
		if(l>min_length && l<g_distanceMax[j])
		{
			coord c;
			double radian = g_urg.index2rad(j);
			radian = ABS(radian);
			c.x = l*cos(radian); 
			c.y = l*sin(radian);
			
			#if DEBUG
			double degr = g_urg.index2deg(j);
			std::cout << "(" << c.x << "," << c.y << ")" << degr << ":" << l << "|||";
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


//! Fonction de traitement des données de l'hokuyo
void* urgAnalyse(void* arg)
{
		/*
		// Logiquement il est demandé de définir la fréquence de lecture des données
		// mais il n'y a aucun détails sur l'unité utilisé, a tester.
		enum { CaptureTimes = 10 };
		urg.setCaptureTimes(CaptureTimes);
		*/
		
		// Boucle d'acuqisitions, traitements
#if DEBUG
		int h=10;
		while(h>0)
#else
		while(!stop)
#endif
		{
			#if DEBUG
			h--;
			std::cout << std::endl;
			#endif
			
			long timestamp = 0;
			std::vector<long> data;

			// Get data
			int n = g_urg.capture(data, &timestamp);
			if(n <= 0){
				delay(g_scan_msec);
				continue;
			}
			
			if(n>0){
				// C'est ici que l'on traite les données
				interpretData(data,n,&g_urg);	
			}	
		}


	return NULL;
}









