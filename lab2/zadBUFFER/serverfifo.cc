#include <omnetpp.h>
#include "histogram.h"
#include "FileUtil.h"

using namespace omnetpp;

class ServerFifo : public cSimpleModule
{		
  private:
    cQueue queue;	 			//the queue; first job in the queue is being serviced
	cMessage *departure; 		//message-reminder of the end of service (job departure)
	simtime_t departure_time;  	//time of the next departure
	int queueMaxSize;
	QueueHist *histogramA;
	QueueHist *histogramB;
	cMessage *popFromQ();
	void insertToQ(cMessage *msg);
	
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msgin);
	virtual void finish();
};

Define_Module(ServerFifo);

void ServerFifo::initialize()
{	
	departure = new cMessage("Departure");
	queueMaxSize = par("queue_max_size");
	
	histogramA = new QueueHist();
	histogramB = new QueueHist();
}

void ServerFifo::handleMessage(cMessage *msgin)
{		
    if (msgin==departure) //job departure
	{
		cMessage *msg = popFromQ(); //remove job from the head of the queue
		
		if(msg->getKind() == 1)
		{
			send(msg,"out1");
		}
		else
		{
			send(msg,"out2");
		}

		if (!queue.isEmpty()) //schedule next departure event
		{
			
			if(msg->getKind() == 1)
            {
                departure_time=simTime()+par("service_time0");
            }
            else
            {
                departure_time=simTime()+par("service_time1");
            }

	        scheduleAt(departure_time,departure);
		}
	}
	else //job arrival
	{			
		if (queue.isEmpty())
		{
			if(msgin->getKind() == 1)
            {
                departure_time=simTime()+par("service_time0");
            }
            else
            {
                departure_time=simTime()+par("service_time1");
            }
            scheduleAt(departure_time,departure);
		}
		
		if (queue.getLength() < queueMaxSize)
		{
			insertToQ(msgin);
		}
		else
		{
			delete msgin;
		}
	}
}

void ServerFifo::finish()
{
	std::ostream &stream = FileUtil::getInstance().getFile(par("file_name"));
	
	stream << par("name").str() << endl;
	stream << endl;
	stream << "średnia dlugość kolejki A: " << histogramA->createPv() << endl;
	
	stream << "HISTOGRAM A" << endl;
	histogramA->printPv(stream);

	stream << "Prawdopodobienstwo liczby zadan A w systemie: " << endl;
	histogramA->printProbability(stream, (double)par("lambdaA"), (double)par("miA"), par("queue_max_size"));

	stream << "średnia dlugość kolejki B: " << histogramB->createPv() << endl;
	
	stream << "HISTOGRAM B" << endl;
	histogramB->printPv(stream);

	
	stream << "Prawdopodobienstwo liczby zadan B w systemie: " << endl;
	histogramB->printProbability(stream, (double)par("lambdaB"), (double)par("miB"), par("queue_max_size"));
}

void ServerFifo::insertToQ(cMessage *msg)
{
	if(msg->getKind() == 1)
		histogramA->put();
	else
		histogramB->put();
	
	queue.insert(msg);
}

cMessage *ServerFifo::popFromQ()
{
	cMessage *msg =  (cMessage *)queue.pop(); 
	
	if(msg->getKind() == 1)
		histogramA->get();
	else
		histogramB->get();
	
	return msg;
}



