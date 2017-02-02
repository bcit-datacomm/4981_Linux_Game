#ifndef NETWORKQUEUE_H
#define NETWORKQUEUE_H

#include <mutex>
#include <string>
#include <queue>

/*--------------------------------------------------------------------------
CLASS: NetworkQueue

DATE: February. 1, 2017

REVISIONS:

METHODS:
void write (std::string data)
std::string read ()
bool empty ()

NOTES:
Thread safe wrapper for std::queue.
--------------------------------------------------------------------------*/
class NetworkQueue
{
public:
    /*--------------------------------------------------------------------------
    METHOD: write

    DATE: February. 1, 2017

    REVISIONS:
    Version, Date and Description

    DESIGNER: Brody McCrone

    PROGRAMMER: Brody McCrone

    INTERFACE: void write (std::string data)
    data: A string to be inserted into the queue.

    RETURNS:
    void

    NOTES:
    Thread safe.
    --------------------------------------------------------------------------*/
	void write(std::string);
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

	std::string read();
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
	bool empty();
private:
	std::queue<std::string> _queue;
	std::mutex lock;
};

#endif
