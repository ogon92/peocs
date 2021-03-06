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
		cMessage* openAck = new cMessage("OPEN_ACK");
		send(openAck, "out");
	}
	else if(strcmp(msg->getName(), "PACKAGE") == 0){
		cMessage* packageAck = new cMessage("PACKAGE_ACK");
		send(packageAck, "out");
	}
	else if(strcmp(msg->getName(), "RELEASE") == 0){
		cMessage* releaseAck = new cMessage("RELEASE_ACK");
		send(releaseAck, "out");
	}
	
}

void K2::finish()
{
	
}
