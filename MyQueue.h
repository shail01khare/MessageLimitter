#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "Common.h"

#ifndef __MyQueue_H_
#define __MyQueue_H_
class MyQueue
{
public:
	bool empty()
	{
		return queue_.empty();
	}

	MSG& pop()
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		if (!queue_.empty())
		{
			auto item = queue_.front();
			queue_.pop();
			return item;
		}
	}


	void push(const MSG& item)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		queue_.push(item);
		mlock.unlock();
	}

private:
	std::queue<MSG> queue_;
	std::mutex mutex_;
	std::condition_variable cond_;
};
#endif