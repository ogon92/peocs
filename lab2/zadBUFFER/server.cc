#include <omnetpp.h>
#include "FileUtil.h"
#include "histogram.h"

using namespace omnetpp;

class Server : public cSimpleModule
{		
  private:
	cQueue bufferClassA;	 	//the queue with A-class objects
	cQueue bufferClassB;		//the queue with B-class objects
	cMessage *departureA; 		//message-reminder of the end of service (job departure)
	cMessage *departureB; 		//message-reminder of the end of service (job departure)
	simtime_t departure_time;  	//time of the next departure
	int queueMaxSize;
	QueueHist histogram;
	
	QueueHist *histogramA;
	QueueHist *histogramB;
	
	cMessage *popFromQA();
	void insertToQA(cMessage *msg);
	cMessage *popFromQB();
	void insertToQB(cMessage *msg);
	
 public:
	static bool	is1stQueueEmpty;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msgin);
	virtual void finish();
};

bool Server::is1stQueueEmpty = true;

Define_Module(Server);

void Server::initialize()
{	
	departureA = new cMessage("DepartureA");
	departureB = new cMessage("DepartureB");
	
	queueMaxSize = par("queue_max_size");
	histogram = QueueHist(queueMaxSize);
	
	histogramA = new QueueHist(queueMaxSize);
	histogramB = new QueueHist(queueMaxSize);
}

void Server::handleMessage(cMessage *msgin)
{		
    if ((msgin==departureA) && !(msgin==departureB)) //job departure
	{
		cMessage *msg = popFromQA(); //remove job from the head of the queue
		send(msg,"out");
		if (!bufferClassA.isEmpty()) //schedule next departure event
		{
			departure_time=simTime()+par("service_time0"); //czas A
			scheduleAt(departure_time,departureA);
		}
		else
		{
			Server::is1stQueueEmpty = true;
			if ((!bufferClassB.isEmpty()) && (!departureB->isScheduled()) && (Server::is1stQueueEmpty==true))
			{	
				departure_time=simTime()+par("service_time1"); //czas B
				scheduleAt(departure_time,departureB);			
			}
		}				
	}
	if (!(msgin==departureA) && (msgin==departureB) && (Server::is1stQueueEmpty==true))
	{
		if (!bufferClassA.isEmpty())
		{
			departure_time=simTime()+par("service_time1"); //czas B
			scheduleAt(departure_time,departureB);
			if (!departureA->isScheduled())
			{
				departure_time=simTime()+par("service_time0"); //czas A
				scheduleAt(departure_time,departureA);
			}
		}
		else
		{
			cMessage *msg = popFromQB(); //remove job from the head of the queue
			send(msg,"out");
			if (!bufferClassA.isEmpty()) //schedule next departure event
			{
				departure_time=simTime()+par("service_time0"); //czas A
				scheduleAt(departure_time,departureA);
			}else{
				Server::is1stQueueEmpty = true;
				if(!bufferClassB.isEmpty()){
					departure_time=simTime()+par("service_time1"); //czas B
					scheduleAt(departure_time,departureB);			
				}
			}				
		}
	}
	if (!(msgin==departureA) && !(msgin==departureB))
	{			
		
		if ((bufferClassA.isEmpty()) && (bufferClassB.isEmpty()))
		{
			if(msgin->getKind()==1)
			{
				departure_time=simTime()+par("service_time0"); //czas A
				scheduleAt(departure_time,departureA);
			}else
			{
				departure_time=simTime()+par("service_time1"); //czas B
				scheduleAt(departure_time,departureB);
			}
		}	
		
		if(msgin->getKind()==1){
			if(bufferClassA.getLength() >= (int)par("queue_max_size"))
			{
				delete msgin;
				return;
			}
			insertToQA(msgin);
			Server::is1stQueueEmpty = false;
		}else{
			if(bufferClassB.getLength() >= (int)par("queue_max_size"))
			{
				delete msgin;
				return;
			}
			insertToQB(msgin);
		}
	}
}

void Server::finish()
{
	std::ostream &stream = FileUtil::getInstance().getFile(par("file_name"));
	
	if(std::strcmp(par("name"), "A")==0)
	{
		stream << "A: " << endl;
		stream << endl;
		stream << "średnia dlugość kolejki: " << histogramA->createPv() << endl;
		
		stream << "HISTOGRAM " << endl;
		histogramA->printPv(stream);

		stream << "Prawdopodobienstwo liczby zadan w systemie: " << endl;
		histogramA->printProbability(stream, (double)par("lambda"), (double)par("mi"), queueMaxSize);
	}
	else
	{
		stream << "B: " << endl;
		stream << endl;
		
		stream << "średnia dlugość kolejki: " << histogramB->createPv() << endl;
		stream << " HISTOGRAM " << endl;
		histogramB->printPv(stream);

		stream << "Prawdopodobienstwo liczby zadan w systemie: " << endl;
		histogramB->printProbability(stream, (double)par("lambda"), (double)par("mi"), queueMaxSize);
	}
}

void Server::insertToQA(cMessage *msg)
{
	histogramA->put();
	histogram.put();
	bufferClassA.insert(msg);
}

cMessage *Server::popFromQA()
{
	histogramA->get();
	histogram.get();
	return (cMessage *)bufferClassA.pop();
}

void Server::insertToQB(cMessage *msg)
{
	histogramB->put();
	histogram.put();
	bufferClassB.insert(msg);
}

cMessage *Server::popFromQB()
{
	histogramB->get();
	histogram.get();
	return (cMessage *)bufferClassB.pop();
}