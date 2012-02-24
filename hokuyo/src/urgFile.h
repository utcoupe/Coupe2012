/**
 * 
 * */
#include "UrgCtrl.h"
using namespace qrk;

// A calculer une seule fois 
long indexMax;
long indexMin;
long *distanceMax;
 
//! initalisation des valeurs de références
void initRef(std::vector<long> data, int n, UrgCtrl* urg)
{
	indexMax = urg->index2deg(0);
	indexMin = urg->index2deg(-90);
	
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
		std::cout << distanceMax[ind] << std::endl;
	}
	
} 

//! Traitement des données venant de l'hokuyo
void interpretData(std::vector<long> data, int n, UrgCtrl* urg)
{	
	for (int j = 0; j < n; ++j) {
		
		//distanceMax
		
	}
}


//! intialisation des valeurs
void initRobotLocation()
{
	for(int i=0 ; i<NB_MAX_ROBOT ; i++)
	{
		robot[i].x=-1;
		robot[i].y=-1;
	}
}

//! Fonction de traitement des données de l'hokuyo
void* urgAnalyse(void* arg)
{
	initRobotLocation();
	
	
#if 0
		// Recupération des données
		MainParameters* data=(MainParameters*)arg;

		// Check connection
		UrgCtrl urg;
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
	#if 1
		const double rad90 = 90.0 * M_PI / 180.0;
		urg.setCaptureRange(urg.rad2index(-rad90), urg.rad2index(rad90));
	#endif

		/*
		// Logiquement il est demandé de définir la fréquence de lecture des données
		// mais il n'y a aucun détails sur l'unité utilisé, a tester.
		enum { CaptureTimes = 10 };
		urg.setCaptureTimes(CaptureTimes);
		*/
		

		// Boucle d'acuqisitions, traitements
		bool initRefe = false;
		while(!stop)
		{
			long timestamp = 0;
			std::vector<long> data;

			// Get data
			int n = urg.capture(data, &timestamp);
			if(n <= 0){
				delay(scan_msec);
				continue;
			}
			
			//
			if(!initRefe){
				initRef(n, &urg);
				initRefe = true;
			}
			
			// C'est ici que l'on traite les données
			interpretData(data,n,&urg);	
		}

#endif
	return NULL;
}
