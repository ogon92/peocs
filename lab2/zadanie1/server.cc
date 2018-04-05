#include <omnetpp.h>

class Server : public cSimpleModule
{		
  private:
    cQueue queue;	 			//the queue; first job in the queue is being serviced
	//Kolejkę priorytetową można zaimplementować za pomocą dwóch buforów (A i B)–
	cQueue bufferClassA;	 	//the queue with A-class objects
	cQueue bufferClassB;		//the queue; first job in the queue is being serviced
	cMessage *departureA; 		//message-reminder of the end of service (job departure)
	cMessage *departureB; 		//message-reminder of the end of service (job departure)
	simtime_t departure_time;  	//time of the next departure
 public:
	static bool	is1stQueueEmpty;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msgin);

};

bool Server::is1stQueueEmpty = true;

Define_Module(Server);

void Server::initialize()
{	
	departureA = new cMessage("DepartureA");
	departureB = new cMessage("DepartureB");
}

void Server::handleMessage(cMessage *msgin)
{		
    if ((msgin==departureA) && !(msgin==departureB)) //job departure
	{
				cMessage *msg = (cMessage *)bufferClassA.pop(); //remove job from the head of the queue
				send(msg,"out");
				if (!bufferClassA.empty()) //schedule next departure event
				{
					departure_time=simTime()+par("service_time"); //czas A
					scheduleAt(departure_time,departureA);
				}
				else
				{
					Server::is1stQueueEmpty = true;
					if ((!bufferClassB.empty()) && (!departureB->isScheduled()) && (Server::is1stQueueEmpty==true)){
						departure_time=simTime()+par("service_time"); //czas B
						scheduleAt(departure_time,departureB);			
					}
				}				
	}
	if (!(msgin==departureA) && (msgin==departureB) && (Server::is1stQueueEmpty==true))
	{
		if (!bufferClassA.empty())
		{
				departure_time=simTime()+par("service_time"); //czas B
				scheduleAt(departure_time,departureB);
				if (!departureA->isScheduled())
				{
					departure_time=simTime()+par("service_time"); //czas A
					scheduleAt(departure_time,departureA);
				}
				
		}
		else
		{
				cMessage *msg = (cMessage *)bufferClassB.pop(); //remove job from the head of the queue
				send(msg,"out");
				if (!bufferClassA.empty()) //schedule next departure event
				{
					departure_time=simTime()+par("service_time"); //czas A
					scheduleAt(departure_time,departureA);
				}else{
					Server::is1stQueueEmpty = true;
					if(!bufferClassB.empty()){
						departure_time=simTime()+par("service_time"); //czas B
						scheduleAt(departure_time,departureB);			
					}
				}				
		}
	}
	if (!(msgin==departureA) && !(msgin==departureB))
	{			
		if ((bufferClassA.empty()) && (bufferClassB.empty()))
		{
			if(msgin->getKind()==1)
			{
				departure_time=simTime()+par("service_time"); //czas A
				scheduleAt(departure_time,departureA);
			}else
			{
				departure_time=simTime()+par("service_time"); //czas B
				scheduleAt(departure_time,departureB);
			}
		}	
		
		if(msgin->getKind()==1){
			bufferClassA.insert(msgin);
			Server::is1stQueueEmpty = false;
		}else{
			bufferClassB.insert(msgin);
		}
	}
}
/*
void Server::handleMessage(cMessage *msgin)
{		
    if (msgin==departure) //job departure
	{
		cMessage *msg = (cMessage *)queue.pop(); //remove job from the head of the queue
		srand(time(NULL));
        
		if(rand()%100 <= par("out_1_chance").longValue()){
			send(msg,"out");
		}else{
			send(msg,"out2");
		}
		
		if (!queue.empty()) //schedule next departure event
		{
			departure_time=simTime()+par("service_time");
	        scheduleAt(departure_time,departure);
		}
	}
	else //job arrival
	{				
		if (queue.empty())
		{
			departure_time=simTime()+par("service_time");
            scheduleAt(departure_time,departure);
		}
		queue.insert(msgin); //put job at the end of the queue
	}
}*/








