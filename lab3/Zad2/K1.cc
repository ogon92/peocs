#include <omnetpp.h>
using namespace omnetpp;
using namespace std;
class K1 : public cSimpleModule
{
  private:
	int totalPackageCount = 0;
	int sentPackageCount = 0;
	int packageAcknowledgeCount = 0;

	cMessage *sendPackage;
	
	double timeout = 1.0;

  protected:
	virtual void initialize();
	virtual void handleMessage(cMessage *msgin);
	virtual void finish();
};

Define_Module(K1);

void K1::initialize()
{
	sendPackage = new cMessage("SEND");
	cMessage *msg = new cMessage("OPEN");
	send(msg, "out");
}

void K1::handleMessage(cMessage *msg)
{
	cout << "K1::handleMessage " << msg->getName() << endl;
	if (strcmp(msg->getName(), "OPEN_ACK") == 0)
	{
		totalPackageCount = exponential(10);
		scheduleAt(simTime(), sendPackage);
	}
	else if (strcmp(msg->getName(), "PACKAGE_ACK") == 0)
	{
		packageAcknowledgeCount++;
		cancelEvent(sendPackage);
		cMessage *msg = new cMessage("PACKAGE");
		send(msg, "out");
		
		scheduleAt(simTime() + timeout, sendPackage);
		
		cout << "Pakiet: " << msg->getName() << "potwierdzony" << endl;
	}
	else if (strcmp(msg->getName(), "RELEASE_ACK") == 0)
	{
		cout << "Koniec transmisji" << endl;
		endSimulation();
	}
	else if (msg == sendPackage)
	{
		cMessage *msg = new cMessage("PACKAGE");
		send(msg, "out");
		
		scheduleAt(simTime() + timeout, sendPackage);
	}

	if (packageAcknowledgeCount == totalPackageCount)
	{
		cMessage *msg = new cMessage("RELEASE");
		send(msg, "out");
	}
}

void K1::finish()
{
}
