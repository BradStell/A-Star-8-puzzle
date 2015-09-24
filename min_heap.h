///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///																							///
///		Author: Brad Stell																	///
///																							///
///		Date: 9/6/2015																		///
///																							///
///		Description:																		///
///			A min-heap, keeps the node with the lowest f(n) = g(n) + h(n) value at the 		///
///			top of the tree. Upon inserting the heap automatically reorganizes the heap		///
///			against the f(n) value, or heuristic.											///
///																							///
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PRIORITYQUEUE_MINHEAP_H
#define PRIORITYQUEUE_MINHEAP_H

#include <iostream>
#include "custom_exception.h"

///////////////////////////////////////////////////////////////
///															///
/// Class declaration, dynamic class: can accept any object ///
///															///
///////////////////////////////////////////////////////////////
template <class T>
class MinHeap {
private:
	std::vector<T*> heap;
	int heapSize;
	void heapifyAdd();
	void heapifyRemove();

public:
	MinHeap();
	MinHeap(T* root);
	~MinHeap();
	void add(T* element);
	T* remove();
	bool isEmpty();
	int size();
	void showHeap();	
};


///////////////////////////////////////////////////////////////
///															///
/// Class definition, dynamic class: can accept any object  ///
///															///
///////////////////////////////////////////////////////////////


///////////////////////////////////////////////////
/// Default constructor, currently does nothing ///
template <class T>
MinHeap<T>::MinHeap()
{

}

//////////////////////////////////////////////////////////////
/// Constructor that accepts an element to add to the heap ///
template <class T>
MinHeap<T>::MinHeap(T* root)
{
	// Add the element to the heap
	heap.push_back(root);
}

///////////////////////////////////////////////////////////////////////////////////////////
/// Deconstructor, used to cleanup dynamic memory, called when object goes out of scope ///
template <class T>
MinHeap<T>::~MinHeap()
{
	// Do not have any dynamically created objects, nothing to cleanup
}

///////////////////////////////////
/// Adds an element to the heap ///
template <class T>
void MinHeap<T>::add(T* element)
{
	// Add the element to the heap
	heap.push_back(element);

	// Reorganize the heap based on the heuristic value, lowest value at the top
	heapifyAdd();
}

/////////////////////////////////////////////////////////////////////////////////
/// Reorganize the heap based on the heuristic value, lowest value at the top ///
template <class T>
void MinHeap<T>::heapifyAdd()
{
	/// If the heap only has one element it does not need reorganizing
	if (heap.size() == 1)
		return;

	// Check the new node against its parent, if its heuristic value is lower then swap places
	// Continue this process all the way to the root node
	int index = heap.size() - 1;
	while (index > 0) {
		int p = (index - 1) / 2;
		T* item = heap[index];
		T* parent = heap[p];

		if (item < parent) {
			T* temp = heap[index];
			heap[index] = heap[p];
			heap[p] = temp;

			index = p;
		}
		else {
			return;
		}
	}
}

////////////////////////////////////////////////////////
/// Removes the node with the lowest heuristic value ///
template <class T>
T* MinHeap<T>::remove()
{
	// Make a pointer that will point to the first element of the heap
	T* toRemove;

	// If the heap has no elements throw an error
	if (heap.size() == 0) {
		CustomException myex("Empty Queue");
		throw myex;
	}

	// If the heap has one element then grab a reference to it and remove it from the heap
	if (heap.size() == 1)
	{
		toRemove = heap[0];
		heap.erase(heap.end() - 1);
	}

	// If there are more than one nodes in the heap, remove the first and re heapify the heap
	else
	{
		toRemove = heap[0];
		heap[0] = heap[heap.size() - 1];
		heap.erase(heap.end() - 1);

		// Reorganizes the heap back into new correct order
		heapifyRemove();
	}

	return toRemove;
}

////////////////////////////////////////////////////////
/// Reorganizes the heap back into new correct order ///
template <class T>
void MinHeap<T>::heapifyRemove()
{
	// index will point to the root node in the tree
	std::size_t index = 0;

	// left will point to the root nodes left child
	std::size_t left = 2 * index + 1;

	// Loop as long as the left child is not outside the range of the heap (as long as it has a left child)
	while (left < heap.size()) {

		// Find the child (left or right) with the lowest heuristic value
		std::size_t min = left;

		// right now holds the parents right childs index
		std::size_t right = left + 1;

		// Figure out which of the children has the lowest heuristic value
		if (right < heap.size()) {
			if (heap[right] < heap[left]) {
				min = right;
			}
		}

		// Check to see if the parent has a higher heuristic value than it's child with the lowest heuristic value
		if (heap[index] > heap[min]) {

			// Swap positions
			T* temp = heap[index];
			heap[index] = heap[min];
			heap[min] = temp;

			// The new parent node becomes the child node that had the lowest h value
			index = min;

			// Grab the new parents left child
			left = 2 * index + 1;
		}
		else {
			return;
		}
	}
}

//////////////////////////////////////////////////////////
/// Returns true if the heap is empty, false otherwise ///
template <class T>
bool MinHeap<T>::isEmpty()
{
	return heap.size() == 0;
}

////////////////////////////////////
/// Returns the size of the heap ///
template <class T>
int MinHeap<T>::size()
{
	return heap.size();
}

///////////////////////////////////////////////////
/// Prints the heap: For debugging. Deactivated ///
template <class T>
void MinHeap<T>::showHeap()
{
	std::cout << "\n\nUnimplemented\n\n";
}

#endif //PRIORITYQUEUE_MINHEAP_H
