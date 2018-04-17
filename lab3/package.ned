simple K1
{
    parameters:
        //volatile double interarrival_time;       
									 					  
    gates:
        input in;
        output out;
}
simple K2
{
    parameters:
        //volatile double interarrival_time;       
									 					  
    gates:
        input in;
        output out;
}

//////////////////////////////////////////////////
simple Lacze
{
    parameters:
        
    gates:
        input in;
        input in2;
        output out;
        output out2;
}

//////////////////////////////////////////////////
module GG1
{
	parameters:
	
    submodules:
        k1: K1 {
            parameters:
                @display("i=block/source");
        }
        k2: K2 {
            parameters:
                @display("i=block/source");
        }
        lacze: Lacze {
            parameters:
                @display("i=block/sink");
        }
    connections:
        k1.out --> {  } --> lacze.in;
        lacze.out --> {  } --> k1.in;

        k2.out --> {  } --> lacze.in2;
        lacze.out2 --> {  } --> k2.in;
}

//////////////////////////////////////////////////
network gg1 extends GG1
{
    parameters:
}
