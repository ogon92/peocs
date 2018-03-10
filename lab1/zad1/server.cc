#include <omnetpp.h>
#include "histogram.h"
using namespace omnetpp;
class Server : public cSimpleModule
{
  private:
	cQueue queue;			  //the queue; first job in the queue is being serviced
	cMessage *departure;	  //message-reminder of the end of service (job departure)
	simtime_t departure_time; //time of the next departure
	int queueMaxSize;
	QueueHist histogram;

	long jobInSystemCount;
	SimTime jobsTime;

	cMessage *popFromQ();
	void insertToQ(cMessage *msg);

  protected:
	virtual void initialize();
	virtual void handleMessage(cMessage *msgin);
	virtual void finish();
};

Define_Module(Server);

void Server::initialize()
{
	departure = new cMessage("Departure");
	queueMaxSize = par("queue_max_size");
	histogram = QueueHist(queueMaxSize); // + 1 ??
	jobsTime = 0;
	jobInSystemCount = 0;
}



void Server::handleMessage(cMessage *msgin)
{
	if (msgin == departure) //job departure
	{
		cMessage *msg = popFromQ(); //remove job from the head of the queue
		jobsTime += (simTime() - msg->getTimestamp());
		send(msg, "out");
		if (!queue.isEmpty()) //schedule next departure event
		{
			departure_time = simTime() + par("service_time");
			scheduleAt(departure_time, departure);
		}
	}
	else //job arrival
	{
		jobInSystemCount++;
		if (queue.isEmpty())
		{
			departure_time = simTime() + par("service_time");
			scheduleAt(departure_time, departure);
		}

		if (queue.length() < queueMaxSize)
		{
			insertToQ(msgin); //put job at the end of the queue
		}
		else
		{
			// pakiet dropped
		}
	}
}

void Server::finish()
{
	cout << "srednia dlugosc kolejki: " << histogram.createPv() << endl;
	histogram.printPv();
	cout << endl
		 << "średni czas pobytu zadania w systemie: " << jobsTime/jobInSystemCount << endl;
}

void Server::insertToQ(cMessage *msg)
{
	histogram.put();
	queue.insert(msg);
}

cMessage *Server::popFromQ()
{
	histogram.get();
	return (cMessage *)queue.pop();
}
