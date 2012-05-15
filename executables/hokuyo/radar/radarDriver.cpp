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
#include <cmath>
#include "protocole.h"
using namespace std; 
using namespace qrk;

const char* device = "/dev/ttyACM0";


int main() {
	
	UrgCtrl urg;
	if (! urg.connect(device)) {
		printf("UrgCtrl::connect: %s\n", urg.what());
	}

	urg.setCaptureMode(AutoCapture);

	int scan_msec = urg.scanMsec();

	const double rad90 = 90.0 * M_PI / 180.0;
	const unsigned int begin = urg.rad2index(-rad90);
	const unsigned int end = urg.rad2index(rad90);
	urg.setCaptureRange(begin, end);

	int command=55;
	int id=42;
	string request;
	size_t pos;	
	while(command!=9) {
		cin >> request;
		fflush(stdin);
		pos = request.find(SEP);
		try {
			id = atoi(request.substr(0, pos).c_str());
			command = atoi(request.substr(pos+1).c_str());
		}
		catch(...) {
			continue;
		}
	
		long timestamp = 0;
		vector<long> data;
	
		// Get data
		int n = urg.capture(data, &timestamp);
		if (n <= 0) {
			delay(scan_msec);
			cout << id << SEP << "[]" << endl;
		}
		else {
			cout << id << SEP << "[";
			for (unsigned int j = begin; j < end; ++j) {
				double teta = urg.index2rad(j);
				double delta = data[j];
				int x = delta * cos(teta);
				int y = delta * sin(teta);
				cout << "(" << x << "," << y << "),";
			}
			cout << "]";
			cout << endl;
		}
	}
	
	
	
	return 0;
	
}

