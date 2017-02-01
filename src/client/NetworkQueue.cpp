#include "NetworkQueue.hpp"
#include <iostream>

void NetworkQueue::write(std::string data) 
{
	std::lock_guard<std::mutex> guard(lock);
	_queue.push(data);
}

std::string NetworkQueue::read() 
{
	std::lock_guard<std::mutex> guard(lock);
	std::string firstElement(_queue.front());
	_queue.pop();
	return firstElement;
}

bool NetworkQueue::empty() 
{
	return _queue.empty();
}
