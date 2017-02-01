#ifndef NETWORKQUEUE_H
#define NETWORKQUEUE_H

#include <mutex>
#include <string>
#include <queue>

class NetworkQueue 
{
public:
	void write(std::string);
	std::string read();
	bool empty();
private:
	std::queue<std::string> _queue;
	std::mutex lock;
};

#endif
