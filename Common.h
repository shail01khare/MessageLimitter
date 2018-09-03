#ifndef __COMMON_H_
#define __COMMON_H_

typedef struct OrderMessage
{
	//long OrderId;
	int OrderType;//1-newOrder,2-AmendOrder,3-PullOrder
	int price_;
	int size_;
	char symbol_[10];
	OrderMessage(int Otype,int OSize) :OrderType(Otype), size_(OSize){}
}MSG;

class MyComp
{
public:
	bool operator()(MSG a, MSG b)
	{
		return a.OrderType < b.OrderType;
	}
};

#endif // !1