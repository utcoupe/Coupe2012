/**
 * \file 	urgManagementFunction.h
 * \author 	Xavier RODRIGUEZ
 * \date	05/03/2012
 * 
 * */
#ifndef URGMANAGEMENTFUNCTION_H
#define URGMANAGEMENTFUNCTION_H

//! ---
inline bool startHokuyo();
inline void stopHokuyo();
inline string findPortCom();
inline bool hokuyoReference();
//! ---


//! initalisation des valeurs de références
void initRef(std::vector<long> data, int n)
{	
	#if DEBUG
	std::cout << "Initialisation des Références" << std::endl;
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

//!
#define HTEST 10
inline
bool hokuyoReference()
{
	int h = HTEST;
	while(h>0)
	{
		h--;	
		long timestamp = 0;
		std::vector<long> data;

		// Get data
		int n = g_urg.capture(data, &timestamp);
		if(n <= 0){ delay(g_scanMsec); continue; }
			
		if(n>0){
			initRef(data,n);
			return true;
		}
	}
	
	return false;
}
	

//! Trouver la couleur automatiquement 
inline
short hokuyoFindColor()
{
	short colorDeduction = ROUGE;
	g_scanMsec = g_urg.scanMsec();
	g_urg.setCaptureMode(AutoCapture);
		
	//
	g_radMin = -(90.0 * M_PI / 180.0);
	g_radMax =  (90.0 * M_PI / 180.0);
	g_urg.setCaptureRange(g_urg.rad2index(g_radMin), g_urg.rad2index(g_radMax));
		
	int h=10;
	bool colorFind=false;
	while(h>0 && !colorFind)
	{
		h--;
		long timestamp = 0;
		std::vector<long> data;

		// Get data
		int n = g_urg.capture(data, &timestamp);
		if(n <= 0){ delay(g_scanMsec); continue; }
				
		if(n>0){
			
			// Coté Rouge
			long iMin = g_urg.deg2index(-90);
			long iMax = g_urg.deg2index(-70);
			
			for(int j = iMin; j < iMax; ++j) {
				long l = data[j];
				if( l>(LY-100) && l>(LY+100) ){
					if(colorDeduction == ROUGE){
						return ROUGE;
						colorFind = true;
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
						return VIOLET;
						colorFind = true;
					}else{
						colorDeduction = VIOLET;
					}
				}
			}
			// ---
		}	
	}
	
	return ROUGE;
}

//! Récupération et activation des différents paramétres de l'hokuyo
inline 
void hokuyoSetGetParam()
{
	if(g_color == ROUGE){
		#if DEBUG
			cout << "Couleur ROUGE" << endl;
		#endif
		g_radMin = -(90.0 * M_PI / 180.0);
		g_radMax =   0.0;
		g_indexMin = g_urg.deg2index(-90);
		g_indexMax = g_urg.deg2index(0);
	}
	else{
		#if DEBUG
			cout << "Couleur VIOLET" << endl;
		#endif
		g_radMin =  0.0;
		g_radMax =  (90.0 * M_PI / 180.0);
		g_indexMin = g_urg.deg2index(0);
		g_indexMax = g_urg.deg2index(90);
	}
	
	g_scanMsec = g_urg.scanMsec();
	
	g_urg.setCaptureMode(AutoCapture);
	
	g_urg.setCaptureRange(g_urg.rad2index(g_radMin), g_urg.rad2index(g_radMax));
}


//! Trouver le port com automatiquement
#define MAXPORTTEST 10 
inline 
string hokuyoFindPortCom()
{
	ostringstream device;
	stopHokuyo();
	
	// Test des 10 ports ACM ^^
	#if DEBUG
	cout << device.str() << "Trouver le Port Com " << endl;
	#endif
	
	for(int port=0 ; port<MAXPORTTEST ; port++)
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
			//stopHokuyo();
			return device.str();
		}
		
		#if DEBUG
		cout << " Fail " << endl;
		#endif
	}
		
	#if DEBUG
	cout << " Aucun urg connecté... " << endl;
	#endif
	return string("");
}

//! Connection de l'hokuyo
inline
bool startHokuyo()
{
	if(!g_urg.isConnected()){
		if(g_urg.connect(g_comPort.c_str())) {
			return true;
		}else{
			return false;
		}
	} 
	return true;
}

//! Deconnection de l'hokuyo
inline
void stopHokuyo()
{
	if(g_urg.isConnected()){
		g_urg.disconnect();
	} 
}


#endif // URGMANAGEMENTFUNCTION_H
