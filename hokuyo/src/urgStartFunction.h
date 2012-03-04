/**
 * \file 	global.h
 * \author 	Xavier RODRIGUEZ
 * \date	29/02/2012
 * 
 * */
#ifndef URGSTARTFUNCTION_H
#define URGSTARTFUNCTION_H



//! initalisation des valeurs de références
void initRef(std::vector<long> data, int n, UrgCtrl* urg)
{	
	#if DEBUG
	std::cout << "InitRef" << std::endl;
	#endif
	
	g_distanceMax = new long[data.size()];
	for(int ind=g_indexMin ; ind<g_indexMax ; ind++)
	{
		double radian = g_urg.index2rad(ind);
		radian = ABS(radian);
		if(radian<TETA_DIAG){
			g_distanceMax[ind]=LX*cos(radian);
		}
		else{
			g_distanceMax[ind]=LY*cos(RAD90-radian);	
		}
		
		#if DEBUG
			std::cout << g_urg.index2deg(ind) << ":" << g_distanceMax[ind] << "||";
		#endif
	}
	
	#if DEBUG
	std::cout << std::endl;
	std::cout << std::endl;
	#endif
}


//! Fonction pour trouver le port com et connecter l'hokuyo
//! Pour une définition manuel faire (false,"/dev/...")
bool activeHokuyo(bool autoSearch=true, string comPort="/dev/ttyACM0")
{
	if(autoSearch)
	{
		ostringstream device;
		
		//
		if(g_urg.isConnected()){
			g_urg.disconnect();
		} 
	
		// Test des 10 ports ACM ^^
		#if DEBUG
			cout << device.str() << " Trouver le Port Com " << endl;
		#endif
		for(int port=0 ; port<10 ; port++)
		{
			device.str(""); 
			device << "/dev/ttyACM";
			device << port; 
			
			#if DEBUG
				cout << device.str() << " : ";
			#endif
			if( g_urg.connect(device.str().c_str()) ) {
				#if DEBUG
				cout << " OK " << endl;
				#endif
				g_comPort = device.str();
				return true;
			}
			#if DEBUG
				cout << " Fail " << endl;
			#endif
		}
		
		#if DEBUG
			cout << " Aucun urg connecté... " << endl;
		#endif
		return false;
	}
	else
	{
		g_comPort = comPort;
		#if DEBUG
			cout << "PortCom : " << comPort << endl;
		#endif 
	}

	return false;
}

//!
bool setParam()
{
	// Temps d'une acquisition
	g_scan_msec = g_urg.scanMsec();
	
	// Capture Mode
	#if URG_AUTO_CAPTURE
		g_urg.setCaptureMode(AutoCapture);
	#else
	   g_urg.setCaptureMode(IntensityCapture);
	   g_urg.setCaptureSkipLines(2);
	#endif
	
	return true;
}

//! Trouve de quelle coté est posé l'hokuyo
bool findLocParameters(bool autoSearch=true, short color=ROUGE)
{
	if(autoSearch)
	{
		short colorDeduction = color;
		
		//
		g_radMin = -(90.0 * M_PI / 180.0);
		g_radMax =  (90.0 * M_PI / 180.0);
		g_urg.setCaptureRange(g_urg.rad2index(g_radMin), g_urg.rad2index(g_radMax));
		
		int h=10;
		while(h>0)
		{
			h--;
			long timestamp = 0;
			std::vector<long> data;

			// Get data
			int n = g_urg.capture(data, &timestamp);
			if(n <= 0){ delay(g_scan_msec); continue; }
				
			if(n>0){
			
				// Coté Rouge
				long iMin = g_urg.deg2index(-90);
				long iMax = g_urg.deg2index(-70);
			
				for(int j = iMin; j < iMax; ++j) {
					long l = data[j];
					if( l>(LY-100) && l>(LY+100) ){
						if(colorDeduction == ROUGE){
							g_color = ROUGE;
							return true;
						}else{
							colorDeduction = ROUGE;
						}
						
					}
				}
				
				// Coté Violet
				iMin = g_urg.deg2index(90);
				iMax = g_urg.deg2index(70);
				
				for(int j = iMin; j < iMax; ++j) {
					long l = data[j];
					if( l>(LY-100) && l>(LY+100) ){
						if(colorDeduction == VIOLET){
							g_color = VIOLET;
							return true;
						}else{
							colorDeduction = VIOLET;
						}
					}
				}
				
			}	
		}
	}
	else
	{
		g_color = color;
	}
	
	if(g_color == ROUGE){
		g_radMin = -(90.0 * M_PI / 180.0);
		g_radMax =   0.0; //(00.0 * M_PI / 180.0);
		g_indexMin = g_urg.deg2index(-90);
		g_indexMax = g_urg.deg2index(0);
	}
	else{
		g_radMin =   0.0; //(00.0 * M_PI / 180.0);
		g_radMax =  (90.0 * M_PI / 180.0);
		g_indexMin = g_urg.deg2index(0);
		g_indexMax = g_urg.deg2index(90);
	}
	g_urg.setCaptureRange(g_urg.rad2index(g_radMin), g_urg.rad2index(g_radMax));

	return false;
}


//! Fonction qui fait toute l'initialisation
bool initHokuyo()
{

	if(!activeHokuyo()){
		cout << "Erreur lors de l'activation de l'hokuyo !!!" << endl;
		return false;
	}
	
	if(!setParam()){
		cout << "Erreur lors de l'initialisation des paramétres !!!" << endl;
		return false;
	}
	
	if(!findLocParameters()){
		cout << "Erreur lors de l'initialisation des paramétres !!!" << endl;
		return false;
	}
	
	return true;
}


#endif // URGSTARTFUNCTION_H
