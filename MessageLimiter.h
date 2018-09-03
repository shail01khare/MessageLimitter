#include <chrono>
#include <vector>
#include <queue>
#include <thread>
#include <iostream>
#include <iomanip>
#include "MyQueue.h"

using namespace std;
using namespace std::chrono;

class MessageLimitter
{
private:
	std::queue<std::pair<long long, unsigned int>> requestLimiterQueue;
	long long intervalMicroSeconds;
	unsigned int messageLimit;
	unsigned int totalSlot;
	long long SingleSlotLength;
	unsigned int reqsPerSlot;

	//MyPQueue & RefMyQueue;
	MyQueue &refCancelQueue;
	MyQueue &refNormalQueue;

public:
	MessageLimitter(long long intervalMs, unsigned int requestLimit, 
		MyQueue &refCQ, MyQueue &refNQ, unsigned int slotCount = 100 )
		: intervalMicroSeconds(intervalMs * 1000), 
		messageLimit(requestLimit), 
		totalSlot(slotCount),
		SingleSlotLength(intervalMs * 1000 / slotCount),
		reqsPerSlot(0),
		refCancelQueue(refCQ),
		refNormalQueue(refNQ)
	{
		if (SingleSlotLength == 0)
		{
			//throw invalid_argument("intervalMs > slotCount");
		}
	}

	void processRequest() 
	{
		long long nowUs = duration_cast<microseconds>(steady_clock::now().time_since_epoch()).count();
		long long slotId = nowUs / SingleSlotLength;

		// check for oldest slot expired, if expired remove it from queue
		if (!requestLimiterQueue.empty() && requestLimiterQueue.front().first <= slotId - totalSlot) 
		{
			cout << requestLimiterQueue.front().first << " : " << requestLimiterQueue.front().second << endl;

			reqsPerSlot -= requestLimiterQueue.front().second; // all that were in this slot get freed
			requestLimiterQueue.pop(); // remove it
		}

		// check for available space
		if (reqsPerSlot < messageLimit) 
		{
			reqsPerSlot++;

			if (!requestLimiterQueue.empty() && requestLimiterQueue.back().first == slotId) 
			{
				requestLimiterQueue.back().second++; // append to open slot
			}
			else 
			{
				requestLimiterQueue.push({ slotId, 1 }); // create new slot with count 1
			}

			sendMessage();
		}

	}

	void sendMessage()
	{
		if (!refCancelQueue.empty())
		{
			//cout << '\t' << RefMyQueue.top().OrderType << " : " << RefMyQueue.top().size_;
			refCancelQueue.pop();
		}
		else
		{
			if (!refNormalQueue.empty())
			{
				refNormalQueue.pop();
			}
		}
	}
}; 