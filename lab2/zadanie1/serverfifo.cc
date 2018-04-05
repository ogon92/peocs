#include <omnetpp.h>

class ServerFifo : public cSimpleModule
{		
  private:
    cQueue queue;	 			//the queue; first job in the queue is being serviced
	//Kolejkę priorytetową można zaimplementować za pomocą dwóch buforów (A i B)–
	cQueue bufferClassA;	 	//the queue with A-class objects
	cQueue bufferClassB;		//the queue; first job in the queue is being serviced
	cMessage *departure; 		//message-reminder of the end of service (job departure)
	simtime_t departure_time;  	//time of the next departure

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msgin);

};

Define_Module(ServerFifo);

void ServerFifo::initialize()
{	
	departure = new cMessage("Departure");
}
/*
void Server::handleMessage(cMessage *msgin)
{		
    if (msgin==departure) //job departure
	{
		if (!bufferClassA.empty())
			if(msgin->getKind()==1){
				cMessage *msg = (cMessage *)bufferClassA.pop(); //remove job from the head of the queue
				send(msg,"out");
			}
			else
			{
				departure_time=simTime()+par("service_time"); //czas B
				scheduleAt(departure_time,departure);
			}
		else
			if ((!bufferClassB.empty())&&(msgin->getKind()==2))
			{
				cMessage *msg = (cMessage *)bufferClassB.pop(); //remove job from the head of the queue
				send(msg,"out2");			
			}
		
		if (!bufferClassA.empty()) //schedule next departure event
		{
			departure_time=simTime()+par("service_time"); //czas A
	        scheduleAt(departure_time,departure);
		}else{
			if(!bufferClassB.empty()){
			departure_time=simTime()+par("service_time"); //czas B
	        scheduleAt(departure_time,departure);			
			}
		}
	}
	else //job arrival
	{			
		if ((bufferClassA.empty()) && bufferClassB.empty())
		{
			if(msgin->getKind()==1){
				departure_time=simTime()+par("service_time"); //czas A
			}else{
				departure_time=simTime()+par("service_time"); //czas B
			}
            scheduleAt(departure_time,departure);
		}	
		
		if(msgin->getKind()==1){
			bufferClassA.insert(msgin);
		}else{
			bufferClassB.insert(msgin);
		}
	}
}
*/
void ServerFifo::handleMessage(cMessage *msgin)
{		
    if (msgin==departure) //job departure
	{
		cMessage *msg = (cMessage *)queue.pop(); //remove job from the head of the queue
		
		if(msg->getKind() == 1)
		{
			send(msg,"out1");
		}
		else
		{
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
}








