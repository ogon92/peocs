simple Source
{
    parameters:
        volatile double interarrival_time; 
									 					  
    gates:
        output out;
}


//////////////////////////////////////////////////
simple Server
{
    parameters:
        volatile double service_time;           
        volatile int queue_max_size;     
    gates:
        input in1;
        input in2;
        output out;
}


//////////////////////////////////////////////////
simple Sink
{
    gates:
        input in;
}


//////////////////////////////////////////////////
module GG1
{
	parameters:
	
    submodules:
        source1: Source {
            parameters:
                @display("i=block/source");
        }
        source2: Source {
            parameters:
                @display("i=block/source");
        }
        server: Server {
            parameters:
                @display("i=block/queue");
        }
        sink: Sink {
            parameters:
                @display("i=block/sink");
        }
    connections:
        source1.out --> {  } --> server.in1;
        source2.out --> {  } --> server.in2;
        server.out --> {  } --> sink.in;
}

//////////////////////////////////////////////////
network gg1 extends GG1
{
    parameters:
}
