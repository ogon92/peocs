#include <omnetpp.h>
using namespace omnetpp;
using namespace std;
class K1 : public cSimpleModule
{
  private:
	cMessage *departure;
	int totalPackedCount = 50000;
	int sentPackedCount = 0;
	int packedAckCount = 0;

	cMessage *sendPacket;

  protected:
	virtual void initialize();
	virtual void handleMessage(cMessage *msgin);
	virtual void finish();
};

Define_Module(K1);

void K1::initialize()
{
	sendPacket = new cMessage("SEND");
	cMessage *msg = new cMessage("OPEN");
	send(msg, "out");
}

void K1::handleMessage(cMessage *msg)
{
	cout << "K1::handleMessage " << msg->getName() << endl;
	if (strcmp(msg->getName(), "OPEN_ACK") == 0)
	{
		sentPackedCount++;
		scheduleAt(simTime(), sendPacket);
	}
	else if (strcmp(msg->getName(), "PAKIET_ACK") == 0)
	{
		packedAckCount++;
		cout << "Pakiet: " << msg->getName() << "potwierdzony" << endl;
	}
	else if (strcmp(msg->getName(), "RELEASE_ACK") == 0)
	{
		cout << "Koniec transmisji" << endl;
		endSimulation();
	}
	else if (msg == sendPacket)
	{
		if (sentPackedCount <= totalPackedCount)
		{
			cMessage *msg = new cMessage("PAKIET");
			send(msg, "out");

			scheduleAt(simTime(), sendPacket);
			sentPackedCount++;
		}
	}

	if (packedAckCount == totalPackedCount)
	{
		cMessage *msg = new cMessage("RELEASE");
		send(msg, "out");
	}
}

void K1::finish()
{
}
