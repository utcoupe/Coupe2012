/**
 * \file 	urgDriverHelp.cpp
 * \author 	Xavier RODRIGUEZ
 * \date	10/03/2012
 *
 * <p>Fontions de gestion de l'hokuyo</p> 
 * */

#include <sstream>
#include "urgDriver.h"
#include "urgException.h"

using namespace std;
	
/***********************************************************************
 * <p>Fonction pour connecter l'hokuyo sur le port défini avec
 * setComPort(), attention sans ça cela ne fonctionnera pas</p>
 **********************************************************************/
void UrgDriver::connectHokuyo()
{
	try
	{
		if(!urg.isConnected()) {
			if(!urg.connect(comPort.c_str())) {
				throw new urgException(this,urgException::Err_connectHokuyo_urgNoConnect);
			}
			else {
				scanMsec = urg.scanMsec();
				urg.setCaptureMode(AutoCapture);
			}
		} 
	}
	catch(urgException* e) {
		e->react();
	}
}
	
/***********************************************************************
 * <p>Fonction pour deconnecter l'hokuyo</p>
 **********************************************************************/
void UrgDriver::deconnectHokuyo()
{
	this->stop();
	if(urg.isConnected()){
		urg.disconnect();
	} 
}

/***********************************************************************
 * \param data - 
 * \param timestamp -
 * \return long - 
 * 
 * <p>Récupération d'une donnée valide à coup sur</p>
 **********************************************************************/
long UrgDriver::getData(vector<long>& data, long* timestamp)
{
	int h = 10;
	while(h > 0)
	{
		h--;	

		int n = urg.capture(data,timestamp);
		if(n <= 0){ delay(scanMsec); continue; }
			
		if(n>0){
			return n;
		}
	}
	return 0;
}


//!
string UrgDriver::hokuyoFindPortCom(int nbOfPortToTest, int firstPortToTest)
{
	ostringstream device;
	deconnectHokuyo();
	
	for(int port=firstPortToTest ; port<nbOfPortToTest ; port++)
	{
		device.str(""); 
		device << "/dev/ttyACM";
		device << port; 
			
		if( urg.connect(device.str().c_str()) ) {
			return device.str();
		}
		
	}
		
	return string("");
}


//! ---
void UrgDriver::defineRange(double minDeg,double maxDeg)
{
	radMin = (minDeg * M_PI / 180.0);
	radMax = (maxDeg * M_PI / 180.0);
}

//! ---
void UrgDriver::defineRangeIndex(double minDeg,double maxDeg)
{
	indexMin = urg.deg2index(minDeg);
	indexMax = urg.deg2index(maxDeg);
}

//! ---
void UrgDriver::UpdateRange()
{
	urg.setCaptureRange(urg.rad2index(radMin), urg.rad2index(radMax));
}


/***********************************************************************
 * 
 * \param autoSearch - si false est passé dX et dY devra être donnée.
 * 						  true recherche auto // non implémenté
 * \param dX - Décalage en X
 * \param dY - Décalage en Y
 **********************************************************************/
void UrgDriver::setDelta(bool autoSearch, int dX, int dY)
{
	if(!autoSearch) {
		this->deltaX = dX;
		this->deltaY = dY;
	}
}

/***********************************************************************
 * 
 * \param color - 	La couleur du robot pour le match 
 * 			 		utiliser de préférence : UrgDriver::ROUGE / VIOLET
 **********************************************************************/
void UrgDriver::updateParamWithColor(short color)
{
	this->color = color;
	if(this->color == UrgDriver::VIOLET) {
		defineRange(-90.0,0.0);
		defineRangeIndex(-90.0,0.0);
	}
	else {
		defineRange(0.0,90.0);
		defineRangeIndex(0.0,90.0);
	}

	urg.setCaptureRange(urg.rad2index(radMin), urg.rad2index(radMax));	
}


//!
void UrgDriver::refInit()
{
	long timestamp = 0;
	std::vector<long> data;
	getData(data,&timestamp);

	distanceMax = new long[data.size()];
	for(int ind=indexMin ; ind<indexMax ; ind++)
	{
		double radian = urg.index2rad(ind);
		radian = ABS(radian);
		if(radian<TETA_DIAG){
			distanceMax[ind]=LX*cos(radian);
		}
		else{
			distanceMax[ind]=LY*cos(RAD90-radian);	
		}
	}
}


//! --- 
short UrgDriver::hokuyoFindColor()
{
	short colorDeduction = -10;
	//
	defineRange(-90.0,90.0);
	UpdateRange();
	//
	long timestamp = 0;
	std::vector<long> data;
	
	for(int test = 0 ; test<10 ; test++)
	{
		getData(data,&timestamp);
		
		// Coté Rouge
		defineRangeIndex(-90,-70);
		for(int j = indexMin; j < indexMax; ++j) 
		{
			long l = data[j];
			if( l>(LY-100) && l>(LY+100) ){
				if(colorDeduction == UrgDriver::ROUGE){
					return UrgDriver::ROUGE;
				}else{
					colorDeduction = UrgDriver::ROUGE;
				}					
			}
		}
					
		// Coté Violet
		defineRangeIndex(70,90);
		for(int j = indexMin; j < indexMax; ++j) {
			long l = data[j];
			if( l>(LY-100) && l>(LY+100) ){
				if(colorDeduction == UrgDriver::VIOLET){
					return UrgDriver::VIOLET;
				}else{
					colorDeduction = UrgDriver::VIOLET;
				}
			}
		}
	}	
	
	
	cerr << "Couleur non trouvé automatiquement" << endl;
	return UrgDriver::ROUGE;
}



