#include "NetworkQueue.hpp"
#include <iostream>

/*--------------------------------------------------------------------------
METHOD: write

DATE: February. 1, 2017

REVISIONS:
Version, Date and Description

DESIGNER: Brody McCrone

PROGRAMMER: Brody McCrone

INTERFACE: {5:returntype} {6:fucntionName} (std::string data)
data: A string to be inserted into the queue.

RETURNS:
void

NOTES:
Thread safe.
--------------------------------------------------------------------------*/
void NetworkQueue::write(std::string data) {
	std::lock_guard<std::mutex> guard(lock);
	_queue.push(data);
}

/*--------------------------------------------------------------------------
METHOD: read

DATE: February. 1, 2017

REVISIONS:
Version, Date and Description

DESIGNER: Brody McCrone

PROGRAMMER: Brody McCrone

INTERFACE: std::string read ()

RETURNS: The oldest element in the queue.

NOTES:
Thread safe. Use NetworkQueue::empty to make sure the queue isn't empty before
using.
--------------------------------------------------------------------------*/
std::string NetworkQueue::read() {
	std::lock_guard<std::mutex> guard(lock);
	std::string firstElement(_queue.front());
	_queue.pop();
	return firstElement;
}

/*--------------------------------------------------------------------------
METHOD: empty

DATE: February. 1, 2017

REVISIONS:
Version, Date and Description

DESIGNER: Brody McCrone

PROGRAMMER: Brody McCrone

INTERFACE: bool empty ()

RETURNS:
true: The queue is empty.
false: The queue is not empty.

NOTES:
Checks if the queue is empty or not.
--------------------------------------------------------------------------*/
bool NetworkQueue::empty() {
	return _queue.empty();
}
