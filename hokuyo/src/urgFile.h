/**
 * \file 	urgFile.h
 * \author 	Xavier RODRIGUEZ
 * \date	19/02/2012
 * 
 * */



const long min_length=20;
const long max_length=3000;
 
//! initalisation des valeurs de références
void initRef(std::vector<long> data, int n, UrgCtrl* urg)
{	
	indexMax = urg->deg2index(0);
	indexMin = urg->deg2index(-90);
	
	#if DEBUG
	std::cout << "InitRef" << std::endl;
	#endif
	
	distanceMax = new long[data.size()];
	for(int ind=indexMin ; ind<indexMax ; ind++)
	{
		double radian = urg->index2rad(ind);
		radian = ABS(radian);
		if(radian<TETA_DIAG){
			distanceMax[ind]=LX*cos(radian);
		}
		else{
			distanceMax[ind]=LY*cos(RAD90-radian);	
		}
		
		#if DEBUG
			std::cout << urg->index2deg(ind) << ":" << distanceMax[ind] << "||";
		#endif
	}
	
	#if DEBUG
	std::cout << std::endl;
	std::cout << std::endl;
	#endif
}

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

	int distance = sqrt( cox*cox + coy*coy );
	if(distance>TOLERANCE){
		return false;
	}
	
	return true;
} 

//! Traitement des données venant de l'hokuyo
void interpretData(std::vector<long> data, int n, UrgCtrl* urg)
{	
	robot.clear();
	std::list<coord> bot;
	
	for(int j = indexMin; j < indexMax; ++j) {
		long l = data[j];
		
		if(l>min_length && l<distanceMax[j])
		{
			coord c;
			double radian = urg->index2rad(j);
			radian = ABS(radian);
			c.x = l*cos(radian); 
			c.y = l*sin(radian);
			
			#if DEBUG
			double degr = urg->index2deg(j);
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

#if 1
		// Recupération des données
		MainParameters* data=(MainParameters*)arg;

		// Check connection
		
		if (! urg.connect(data->comPort.c_str())) {
			std::cout << "UrgCtrl::connect: " << urg.what() << std::endl;
			exit(1);
		}
		
		// Capture Mode
	#if URG_AUTO_CAPTURE
		urg.setCaptureMode(AutoCapture);
	#else
	   urg.setCaptureMode(IntensityCapture);
	   urg.setCaptureSkipLines(2);
	#endif

		// Temps d'une acquisition
		int scan_msec = urg.scanMsec();

		// Défintion de l'angle de traitement
		//! TODO changer les angles en fonction du coté jouer
	#if 1
		double radMin = -(90.0 * M_PI / 180.0);
		double radMax = 00.0 * M_PI / 180.0;
		urg.setCaptureRange(urg.rad2index(radMin), urg.rad2index(radMax));
	#endif

		/*
		// Logiquement il est demandé de définir la fréquence de lecture des données
		// mais il n'y a aucun détails sur l'unité utilisé, a tester.
		enum { CaptureTimes = 10 };
		urg.setCaptureTimes(CaptureTimes);
		*/
		
		// Boucle d'acuqisitions, traitements
		bool initRefe = false;
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
			int n = urg.capture(data, &timestamp);
			if(n <= 0){
				delay(scan_msec);
				continue;
			}
			
			//
			if(!initRefe && n>0){
				initRef(data,n,&urg);
				initRefe = true;
			}
			
			if(n>0){
				// C'est ici que l'on traite les données
				interpretData(data,n,&urg);	
			}	
		}

#endif
	return NULL;
}






//! Fonction pour retrouver le port ttyAcm de l'hokuyo automatiquement
#if REGLAGE_AUTO
string getTtyAcm()
{
	ostringstream device;
	
	// Test des 10 ports ACM ^^
	for(int port=0 ; port<10 ; port++)
	{
		device.str(""); 
		device << "/dev/ttyACM";
		device << port; 
		
		#if DEBUG
			cout << device.str() << " : ";
		#endif
		if( urg.connect(device.str().c_str()) ) {
			#if DEBUG
			cout << " OK " << endl;
			#endif
			return device.str();
		}
		#if DEBUG
			cout << " Fail " << endl;
		#endif
	}
	
	return NULL;
}
#endif


