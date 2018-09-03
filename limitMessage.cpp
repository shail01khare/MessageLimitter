// Temp.cpp : Defines the entry point for the console application.
//

#include <chrono>
#include <vector>
#include <queue>
#include <thread>
#include <iostream>
#include <iomanip>
#include "MessageLimiter.h"

using namespace std;
using namespace std::chrono;

MyQueue cancelQueue;
MyQueue normalQueue;

void QueueWriter(int i)
{
	while (true)
	{
		if (i == 3)
		{
			cancelQueue.push(MSG(i, i + 10));
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}
		else
		{
			normalQueue.push(MSG(i, i + 10));
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
}


void SendMessage()
{
	MessageLimitter objMessageLimitter(1000, 100, cancelQueue, normalQueue);

	while (true)
	{
		objMessageLimitter.processRequest();
	}
}
int main()
{
	//Create 3 thread for generating 3 types of order for testing
	std::thread t1(&QueueWriter, 1);
	std::thread t2(&QueueWriter, 2);
	std::thread t3(&QueueWriter, 3);

	//Create thread to send message for testing
	std::thread t(&SendMessage);

	t.join();
	t1.join();
	t2.join();
	t3.join();

	return 0;
}
