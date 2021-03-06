#include <omnetpp.h>
#include "histogram.h"
#include "FileUtil.h"
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
	long jobLostCount;
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
	jobLostCount = 0;
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
			// kolejka pusta
			// zadanie jest natychmiast obsługiwane
			insertToQ(msgin); // jeden element w kolejce –
			// zadanie jest obsługiwane
			departure_time = simTime() + par("service_time");
			scheduleAt(departure_time, departure);
			return;
		}
		else if (queue.length() < queueMaxSize)
		{
			insertToQ(msgin); //put job at the end of the queue
			
		}
		else
		{
			jobLostCount++;
			delete msgin;
			// pakiet dropped
		}
		
	}
}

void Server::finish()
{
	std::ostream &stream = FileUtil::getInstance().getFile(par("file_name"));

	//std::ostream &stream = cout;
	stream << "Mi: " << (double)par("mi") << endl;
	stream << "Lambda: " << (double)par("lambda") << endl;
	stream << "średnia dlugość kolejki: " << histogram.createPv() << endl;

	stream << "średni czas pobytu zadania w systemie: " << jobsTime / jobInSystemCount << endl;

	stream << "Histogram: " << endl;
	histogram.printPv(stream);

	stream << "Prawdopodobienstwo liczby zadan w systemie: " << endl;
	histogram.printProbability(stream);

	stream << "Prawdopodobienstwo strat pakietow = " << ((double)jobLostCount / jobInSystemCount) << "\n";
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
