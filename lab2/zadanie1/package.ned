simple Source
{
    parameters:
		int kind;
        volatile double interarrival_time; 
        volatile int initial_queue;        
									 					  
    gates:
        output out;
}


//////////////////////////////////////////////////
simple ServerFifo
{
    parameters:
        volatile double service_time;      
		volatile int out_1_chance;      
		volatile int out_2_chance;    	
		
    gates:
        input in;
		input in2;
        output out1;
		output out2;
}

simple Server
{
    parameters:
        volatile double service_time;      
		volatile int out_1_chance;      
		volatile int out_2_chance;   
		
    gates:
        input in;
        output out;
}

//////////////////////////////////////////////////
simple Sink
{
    gates:
        input in;
		input in2;
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
        server1: ServerFifo {
            parameters:
                @display("i=block/queue");
        }
		server2: Server {
            parameters:
                @display("i=block/cogwheel");
        }
		server3: Server {
			parameters:
                @display("i=block/cogwheel");
		}
        sink: Sink {
            parameters:
                @display("i=block/sink");
        }
		
    connections allowunconnected:
        source1.out --> server1.in;
		source2.out --> server1.in2;
		
		server1.out1  --> server2.in;
		server1.out2  --> server3.in;
		
        server2.out  --> sink.in;
		server3.out  --> sink.in2;
}

//////////////////////////////////////////////////
network gg1 extends GG1
{
    parameters:
}
