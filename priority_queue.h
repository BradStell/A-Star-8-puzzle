///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///																							///
///		Author: Brad Stell																	///
///																							///
///		Date: 9/6/2015																		///
///																							///
///		Description:																		///
///			A priority queue, implemented using a min heap from file 'min_heap.h'.			///
///			Serves as an interface into the min-heap, abstracting away the heap's			///
///			internal workings.																///
///																							///
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PRIORITYQUEUE_PRIORITYQUEUE_H
#define PRIORITYQUEUE_PRIORITYQUEUE_H

#include "min_heap.h"

///////////////////////////////////////////////////////////////////////////
///																		///
///		Class Declaration, Dynamic class: can accept any type object	///
///																		///
///////////////////////////////////////////////////////////////////////////
template <class T>
class PriorityQueue
{
private:
	MinHeap<T> heap;
	std::vector<T*> godHeap;
public:
	PriorityQueue();
	PriorityQueue(T* element);
	~PriorityQueue();
	void add(T* element, int &nodes_gen);
	T* remove();
	int size();
	bool isEmpty();
	void printQueue();
	bool alreadyCreated(T* element);
};


///////////////////////////////////////////////////////////////////////////
///																		///
///		Class Definition, Dynamic class: can accept any type object		///
///																		///
///////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////
/// Empty Constructor, currently not used ///
template <class T>
PriorityQueue<T>::PriorityQueue()
{

}

////////////////////////////////////////////////////////////////
/// Constructor accepting an element to start the queue with ///
template <class T>
PriorityQueue<T>::PriorityQueue(T* element)
{
	// godHeap is the master heap that keeps track of every inserted node into the queue, even ones that have been removed from the queue
	// used to make sure dup states do not go into the queue, even after they have been dequeued.
	// Add the first root element into the godHeap.
	godHeap.push_back(element);

	// Add element into the min heap
	heap.add(element);
}

////////////////////////////////////////////////////////////////////////////////
/// Deconstructor, called once the queue goes out of scope, used for cleanup ///
template <class T>
PriorityQueue<T>::~PriorityQueue()
{
	// Don't need to do any cleanup since there are no dynamically created elements
}

////////////////////////////////////
/// Adds an element to the queue ///
template <class T>
void PriorityQueue<T>::add(T* element, int &nodes_gen)
{
	// Check to see if the node has already been inserted before,
	// if it has not been inserted before, insert it into the min heap
	if (!alreadyCreated(element))
	{
		heap.add(element);
		godHeap.push_back(element);
		nodes_gen++;
	}

	// If it has been inserted before then delete it
	else
		delete element;

}

/////////////////////////////////////////////////////////////////////////////////////////
/// Removes the first element from the queue: has the lowest f(n) = g(n) + h(n) value ///
template <class T>
T* PriorityQueue<T>::remove()
{
	return heap.remove();
}

/////////////////////////////////////////////
/// Returns the current size of the queue ///
template <class T>
int PriorityQueue<T>::size()
{
	return heap.size();
}

///////////////////////////////////////////////////////////
/// Returns true if the queue is empty, false otherwise ///
template <class T>
bool PriorityQueue<T>::isEmpty()
{
	return heap.isEmpty();
}

//////////////////////////////////////////////////////////////////////////////////
/// Prints the contents of the heap, used for debugging. Has been disactivated ///
template <class T>
void PriorityQueue<T>::printQueue()
{
	heap.showHeap();
}

//////////////////////////////////////////////////////////////////////////////
/// Checks against the godHeap to see if the node has been inserted before ///
///			returns true if it has been inserted, false otherwise		   ///
template <class T>
bool PriorityQueue<T>::alreadyCreated(T* element)
{
	bool same = true;

	// Check every node in the godHeap to see if the element has been inserted before
	for (std::size_t i = 0; i < godHeap.size(); i++)
	{
		same = true;
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				if (godHeap[i]->state[j][k] != element->state[j][k])
				{
					same = false;
					break;
				}
			}
			if (!same)
				break;
		}
		if (same)
			break;
	}

	if (!same)
		return false;
	else
		return true;
}

#endif //PRIORITYQUEUE_PRIORITYQUEUE_H
