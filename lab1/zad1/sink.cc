#include <omnetpp.h>
#include "FileUtil.h"

using namespace omnetpp;
class Sink : public cSimpleModule
{
  protected:
  	virtual void initialize();
    virtual void handleMessage(cMessage *msgin);
    virtual void finish();
};


Define_Module(Sink);


	long jobInSystemCount;
	double jobsTime;

void Sink::initialize()
{
	jobsTime = 0;
	jobInSystemCount = 0;
}

void Sink::finish()
{
	double mean = jobsTime/jobInSystemCount;

    std::ostream &stream = FileUtil::getInstance().getFile(par("file_name"));
    stream << "Sredni czas zadania w systemie = " << mean << "\n";
    stream << "Zakonczenie symulacji" << "\n\n";

    stream << std::flush;
}


void Sink::handleMessage(cMessage *msgin)
{
	jobInSystemCount++;
	jobsTime += (simTime().dbl() - msgin->getTimestamp().dbl());
	delete msgin;
}
