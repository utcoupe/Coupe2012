/**
 * \file 	cFile.cpp
 * \author 	Xavier RODRIGUEZ
 * \date	00/00/2012
 * 
 * */
 


#include <iostream>
#include <string>
#include "UrgCtrl.h"
#include <cstdlib>
#include "delay.h"
#include "stdio.h"
#include "UrgCtrl.h"
using namespace std; 
using namespace qrk;

#define device "/dev/ttyACM0"


int main()
{
	
	UrgCtrl urg;
	if (! urg.connect(device)) {
		printf("UrgCtrl::connect: %s\n", urg.what());
	}

	urg.setCaptureMode(AutoCapture);

	int scan_msec = urg.scanMsec();

	const double rad90 = 90.0 * M_PI / 180.0;
	urg.setCaptureRange(urg.rad2index(-rad90), urg.rad2index(rad90));

	int command=55;
	string request;
	size_t pos;	
	while(command!=9)
	{
		cin >> request;
		fflush(stdin);
		pos = request.find('.'); 
		command = atoi(request.substr(pos+1).c_str()); 
	
		long timestamp = 0;
		vector<long> data;
	
		// Get data
		int n = urg.capture(data, &timestamp);
		if (n <= 0) {
			delay(scan_msec);
		}

		bool pass = false;
		cout << "[";
		for (int j = 0; j < n; ++j) {
			if(pass){
				cout << ",";
			}
			cout << "(" <<urg.index2rad(j)<< "," <<data[j]<< ")";
			pass = true;
		}
		cout << "]";
		cout << endl;
	}
	
	
	
	return 0;
	
}
