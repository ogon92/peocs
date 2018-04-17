#include <omnetpp.h>
#include "histogram.h"
using namespace omnetpp;
class K2 : public cSimpleModule
{
  private:
  protected:
	virtual void initialize();
	virtual void handleMessage(cMessage *msgin);
	virtual void finish();
};

Define_Module(K2);

void K2::initialize()
{
}

void K2::handleMessage(cMessage *msg)
{
	if(strcmp(msg->getName(), "OPEN") == 0){
		cMessage* apenAck = new cMessage("OPEN_ACK");
		send(apenAck, "out");
	}
	else if(strcmp(msg->getName(), "PAKIET") == 0){
		cMessage* pakietAck = new cMessage("PAKIET_ACK");
		send(pakietAck, "out");
	}
	else if(strcmp(msg->getName(), "RELEASE") == 0){
		cMessage* releaseAck = new cMessage("RELEASE_ACK");
		send(releaseAck, "out");
	}
	
}

void K2::finish()
{
	
}
