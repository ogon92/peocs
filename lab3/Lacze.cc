#include <omnetpp.h>

using namespace omnetpp;
using namespace std;
class Lacze : public cSimpleModule
{

	double controlMsgDelay = 20.0 / 16384.0;
	double pakietMsgDelay = 1500.0 / 16384.0;

  protected:
	virtual void initialize();
	virtual void handleMessage(cMessage *msgin);
	virtual void finish();

	double getDelay(cMessage *msg);
	simtime_t currentDelay = 0;
};

Define_Module(Lacze);

void Lacze::initialize()
{
	cout << "lacze init" << endl;
}

void Lacze::finish()
{
	cout << "lacze finish" << endl;
}

const char *toK2Gate = "out2";
const char *toK1Gate = "out";
void Lacze::handleMessage(cMessage *msg)
{
	/**
	 * Tutaj zamiast tego troche dziwnego rozwiazania mozna by jednak zastosowac kolejke, do ktorej wrzucane bylyby wiadomosci jakie przychodza i uruchamiane 
	 * scheduleAt z opoznieniem odpowiednim do typu pakiety z jakims sterujacym cMessage by w reakcji na jego przyjscie zdjac z kolejki element i wyslac.
	 * 
	 * */
	cout << "Lacze::handleMessage: " << msg->getName() << endl;
	if (!msg->hasObject("REDY_FOR_SEND"))
	{
		cMsgPar *par = new cMsgPar("REDY_FOR_SEND");
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
		else if (strcmp(msg->getName(), "PAKIET") == 0)
		{
			// 1500
			send(msg, toK2Gate);
		}
		else if (strcmp(msg->getName(), "PAKIET_ACK") == 0)
		{
			// 20
			send(msg, toK1Gate);
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
	if (strcmp(msg->getName(), "OPEN") == 0)
	{
		return controlMsgDelay;
	}
	else if (strcmp(msg->getName(), "OPEN_ACK") == 0)
	{
		return controlMsgDelay;
	}
	else if (strcmp(msg->getName(), "PAKIET") == 0)
	{
		return pakietMsgDelay;
	}
	else if (strcmp(msg->getName(), "PAKIET_ACK") == 0)
	{
		return controlMsgDelay;
	}
	else if (strcmp(msg->getName(), "RELEASE") == 0)
	{
		return controlMsgDelay;
	}
	else if (strcmp(msg->getName(), "RELEASE_ACK") == 0)
	{
		return controlMsgDelay;
	}
	return 0.0;
}