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
        volatile double mi; 
        volatile double lambda;   
        volatile string file_name;   
        volatile string name;  
    gates:
        input in[];
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
        server1: Server {
            parameters:
                @display("i=block/queue");
        }
        server2: Server {
            parameters:
                @display("i=block/queue");
        }
    connections:
        server1.out --> {  } --> server2.in++;
        server2.out --> {  } --> server1.in++;

}

//////////////////////////////////////////////////
network gg1 extends GG1
{
    parameters:
}
