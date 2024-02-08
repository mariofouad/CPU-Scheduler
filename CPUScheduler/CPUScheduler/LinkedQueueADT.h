#pragma once

template <typename T>
class LinkedQueueADT {

public:

	virtual bool IsEmpty() const = 0;			//return true if queue is empty
	virtual bool Enqueue(T a) = 0;				//Adds one element from the rear
	virtual bool Dequeue(T& a) = 0;				//Removes one element from the front
	virtual bool peek(T& a) const = 0;			//shows us the value of front's Node value
	~LinkedQueueADT() {}
};
