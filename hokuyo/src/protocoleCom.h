#define SEP "."
#define TYPE_IDCMD int

/**
 * Fonction retournant la postion des robots en coordonnées cartésiennes  
 * */ 
 #define QH_GETDATA 1 

/**
 * Signal pour stoper l'application hokuyo
 * */ 
 #define QH_KILL 9


//! Fonction d'envoie des données
//! Format (idCMD_SEP_(x,y)_SEP_(x,y)) 
void send(TYPE_IDCMD idCmd)
{
	std::ostringstream mess;
	mess.str(""); 
	mess << ("[");
	mess << idCmd;
	
	std::list<coord>::iterator it;
	for ( it=robot.begin() ; it!=robot.end(); it++ )
	{
		mess << SEP << "(" << (*it).x << "," << (*it).y << ")";
	}
	
	mess << "]";

	std::cout << mess.str() << std::endl; 
}

//! Fonction de gestion de la communication
//! Format reception id_SEP_cmd
void* comLoop(void* arg)
{
	TYPE_IDCMD id=0;
	int command=0;
	size_t pos;
	std::string request;	
	
	while(!stop)
	{
		std::cin >> request;
		pos = request.find(SEP); 
		id = atoi(request.substr(0,pos).c_str());
		command = atoi(request.substr(pos+1).c_str()); 
			
		// Send data
		if(command==QH_GETDATA)
		{
			pthread_mutex_lock(&mutex);
			send(id);
			pthread_mutex_unlock(&mutex);
		}
	
		// KILL
		if(command==QH_KILL)
		{
			stop = true;
		}
	}
	
	return NULL;
}
