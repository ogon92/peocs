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
simple Server
{
    parameters:
        volatile double service_time0;  
		volatile double service_time1;	
		
		volatile int out_1_chance;      
		volatile int out_2_chance; 
		
		volatile double miA;
		volatile double miB;
		
		volatile double lambdaA;
		volatile double lambdaB;
		
		volatile int queue_max_size;
		
		volatile string name; 
		volatile string file_name; 
		
		
    gates:
        input in;
		input in2;
        output out;
		output out2;
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
        server1: Server {
            parameters:
                @display("i=block/queue");
        }
		server2: Server {
            parameters:
                @display("i=block/queue");
        }
		server3: Server {
            parameters:
                @display("i=block/queue");
        }
        sink: Sink {
            parameters:
                @display("i=block/sink");
        }
    connections allowunconnected:
        source1.out --> server1.in;
		source2.out --> server1.in2;
		server1.out  --> server2.in;
		server1.out2  --> server3.in;
		
        server2.out  --> sink.in;
		server3.out  --> sink.in2;
}

//////////////////////////////////////////////////
network gg1 extends GG1
{
    parameters:
}
