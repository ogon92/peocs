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
	double jobsTime;
	long jobCount;

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
	jobCount = 0;
	jobsTime = 0;
}

void Server::handleMessage(cMessage *msgin)
{
	jobCount++;
	if (msgin == departure) //job departure
	{
		cMessage *msg = popFromQ(); //remove job from the head of the queue
		jobsTime += (simTime() - msg->getTimestamp()).dbl();
		if(strncmp(par("name").stringValue(), "serwer1", 7) == 0){
			msg->setTimestamp(simTime());
		}
		send(msg, "out");
		if (!queue.isEmpty()) //schedule next departure event
		{
			departure_time = simTime() + par("service_time");
			scheduleAt(departure_time, departure);
		}
	}
	else //job arrival
	{
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
			delete msgin;
			// pakiet dropped
		}
	}
}

void Server::finish()
{
	std::ostream &stream = FileUtil::getInstance().getFile(par("file_name"));

	//std::ostream &stream = cout;
	stream << par("name").stringValue() << endl;
	stream << "Mi: " << (double)par("mi") << endl;
	stream << "Lambda: " << (double)par("lambda") << endl;
	if(strncmp(par("name").stringValue(), "serwer1", 7) == 0){
		stream << "średni czas pobytu zadania w kolejce 1: " << jobsTime / jobCount << endl;
	}
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
