#include <omnetpp.h>

using namespace omnetpp;
using namespace std;
class Lacze : public cSimpleModule
{	
	double controlMessageDelay = 20.0 / 16384.0;
	double packageMessageDelay = 1500.0 / 16384.0;
	int param;
	
  protected:
	virtual void initialize();
	virtual void handleMessage(cMessage *msgin);
	virtual void finish();

	double getDelay(cMessage *msg);
	simtime_t currentDelay = 0;
	simtime_t timeOut = 0;
};

Define_Module(Lacze);

void Lacze::initialize()
{
	param = par("losePackage");
	cout << "lacze init" << endl;
}

void Lacze::finish()
{
	cout << "lacze Finish" << endl;
}

const char *toK2Gate = "out2";
const char *toK1Gate = "out";
void Lacze::handleMessage(cMessage *msg)
{
	cout << "Lacze::handleMessage: " << msg->getName() << endl;
	if (!msg->hasObject("READY_FOR_SEND"))
	{
		cMsgPar *par = new cMsgPar("READY_FOR_SEND");
		msg->addPar(par);
		currentDelay = currentDelay + getDelay(msg);
		scheduleAt(simTime() + currentDelay, msg);
	}
	else
	{
		cout << "currentDelay = 0;" << endl;
		currentDelay = 0;
		if (strcmp(msg->getName(), "OPEN") == 0)
		{
			// 20
			send(msg, toK2Gate);
		}
		else if (strcmp(msg->getName(), "OPEN_ACK") == 0)
		{
			// 20
			send(msg, toK1Gate);
		}
		else if (strcmp(msg->getName(), "PACKAGE") == 0)
		{
			// 1500
			if (uniform(1, 10) < param)
			{
				delete msg;
				cout << "Message " << msg->getName() << "deleted." << endl;
			}
			else
			{
				send(msg, toK2Gate);
			}
		}
		else if (strcmp(msg->getName(), "PACKAGE_ACK") == 0)
		{
			// 20
			if (uniform(1, 10) < param)
			{
				delete msg;
				cout << "Message " << msg->getName() << "deleted." << endl;
			}
			else
			{
				send(msg, toK1Gate);
			}
		}
		else if (strcmp(msg->getName(), "RELEASE") == 0)
		{
			// 20
			send(msg, toK2Gate);
		}
		else if (strcmp(msg->getName(), "RELEASE_ACK") == 0)
		{
			// 20
			send(msg, toK1Gate);
		}
	}
}

double Lacze::getDelay(cMessage *msg)
{
	if (strcmp(msg->getName(), "PACKAGE") == 0)
	{
		return packageMessageDelay;
	}
	else
	{
		return controlMessageDelay;
	}
}