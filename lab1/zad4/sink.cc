#include <omnetpp.h>
#include "FileUtil.h"
using namespace omnetpp;
class Sink : public cSimpleModule
{
  double jobsTime = 0.0;
  long jobCount = 0;

protected:
  virtual void handleMessage(cMessage *msgin);
  virtual void finish();
};

Define_Module(Sink);

void Sink::handleMessage(cMessage *msgin)
{

  jobCount++;
  jobsTime += (simTime() - msgin->getTimestamp()).dbl();
  delete msgin;
}

void Sink::finish()
{
  std::ostream &stream = FileUtil::getInstance().getFile(par("file_name"));
  stream << "średni czas pobytu zadania w kolejce 2: " << jobsTime / jobCount << endl;
}