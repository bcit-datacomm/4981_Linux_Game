#include "NetworkQueue.hpp"
#include "Client.h"
#include <iostream>

#define DEFAULT_IP  "142.232.164.129"
#define DEFAULT_USERNAME "defaultusername"

void run(NetworkQueue *);

int main(int argc, char **argv)
{
	const char *ip;
	const char *username;
	if(argc > 1) {
		ip = argv[1];
	} else {
		ip = DEFAULT_IP;
		std::cout << "No IP entered... using default: "
			<< DEFAULT_IP << std::endl;
	}

	if(argc > 2) {
	 	username = argv[2];
	} else {
		username = DEFAULT_USERNAME;
		std::cout << "No username entered... using default: "
			<< DEFAULT_USERNAME <<  std::endl;
	}

	NetworkQueue in, out;
	Client client(&in, &out, ip, username);

	run(&in);
}

void run(NetworkQueue *in) {
    while(true) {
        if(in->empty() == false) {
            std::cout << in->read() << std::endl;
        }
    }
}
