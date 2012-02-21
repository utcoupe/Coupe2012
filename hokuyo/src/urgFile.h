/**
 * 
 * */
#include "UrgCtrl.h"
using namespace qrk;


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
/*
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
	//int scan_msec = urg.scanMsec();

	// Défintion de l'angle de traitement
#if 0
	const double rad90 = 90.0 * M_PI / 180.0;
	urg.setCaptureRange(urg.rad2index(-rad90), urg.rad2index(rad90));
#endif









*/
	return NULL;
}
