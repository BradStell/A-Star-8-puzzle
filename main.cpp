///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///																																	///
///		Author: Brad Stell																											///
///																																	///
///		Date: 9/6/2015																												///
///																																	///
///		Language: c++																												///
///																																	///
///		Program:																													///
///			This program attempts to solve the '8-puzzle' game. A game in which there is a 3x3 grid filled with 8					///
///			tiles and an empty space with no tile. Each tile has a different number ranging from 1 to 8. The object					///
///			of the game is to rearrange the tiles so that the numbers are in order, starting from the upper left					///
///			corner to the lower right corner. The space can be in any location, so long as the numbers are in sequencial			///
///			order.																													///
///				Example start and finish states:																					///
///					start			finish																							///
///				   |7|2|4|			|1|2|3|																							///
///				   |5| |6|			|4|5|6|																							///
///				   |8|3|1|			|7|8| |																							///
///																																	///
///			This program solves this puzzle by implementing the A* algorithm.  To help the A* algorithm I have used a				///
///			priority queue and a node that represents a state of the board.  The priority queue is implemented using				///
///			a min heap, sorted by the function f(n) = g(n) + h(n), where g(n) is the step cost from the root node, and				///
///			h(n) is the heuristic value assigned to each state, this is calculated using the 'Manhatten Distance'.					///
///																																	///
///		Files:																														///
///			current_state.h			<-- Header file for a node, represents a current state of the game								///
///			current_state.cpp		<-- Source file for a node																		///
///				*** CurrentState can accept multiple forms of an incomming starting state, here are the options:					///
///					int array[9]																									///
///				    int array[3][3]																									///
///				    int** array																										///
///				    std::vector<int> array;																							///
///			custom_exception.h		<-- Header file for a custom exception, thrown if popping from an empty queue					///
///			min_heap.h				<-- Min-Heap, dynamic, can accept any object													///
///			priority_queue.h		<-- Priority queue, dynamic, can accept any object												///
///			main.cpp																												///
///																																	///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <stdio.h>
#include "current_state.h"
#include "priority_queue.h"

using namespace std;

/////////////////////////////
/// Function Declarations ///

CurrentState* Astar();


////////////////////////
/// Global Variables ///
int NODES_GEN = 0;
int NODES_EXP = 0;



int main()
{
	//////////////////////////////////////////
	////  Call the A* Function    ////////////

	CurrentState* finalState = Astar();



	//////////////////////////////////////////
	//////   If solution not found    ////////

	if (finalState == NULL)
		cout << "\n\nFailed\n\n" << endl;


	//////////////////////////////////////////
	/////   If solution is found      ////////

	else
	{
		cout << "\n\nFound Solution!!\n" << endl
			<< "Nodes Generated: " << NODES_GEN
			<< "\nNodes Expanded: " << NODES_EXP << "\n\n";

		// Create a vector to collect the winning sequence of moves the 'program' made
		std::vector<CurrentState*> revSequence;

		// Put the winning state into the vector		
		revSequence.push_back(finalState);

		// Get the parent of the winning state
		CurrentState* pointer = finalState->getParent();

		// Add each states parent to the winning sequence vector back up to the starting state, or the root state
		while (pointer != NULL)
		{
			revSequence.push_back(pointer);
			pointer = pointer->getParent();
		}

		// Print the states of each node in the vector in reverse order,
		// Which starts from the starting state and works its way to the winning state
		for (int i = revSequence.size() - 1; i >= 0; i--)
		{
			cout << " ____ ____ ____\n";
			for (int k = 0; k < 3; k++)
			{
				for (int l = 0; l < 3; l++)
				{
					if (l < 2)
					{
						if (revSequence[i]->state[k][l] != 0)
							cout << "| " << revSequence[i]->state[k][l] << "  ";
						else
							cout << "|    ";
					}
					else
					{
						if (revSequence[i]->state[k][l] != 0)
							cout << "| " << revSequence[i]->state[k][l] << "  |";
						else
							cout << "|    |";
					}

				}
				cout << "\n|____|____|____|\n";
			}
			cout << "\n\n";
		}
	}

	return 0;
}

CurrentState* Astar()
{
	///////////////////////////////////////////////////////////////////////////////////////////////
	/// Enter your starting game state here, the constructor for CurrentState is overloaded		///
	/// and can accept the following inputs:													///
	///    int array[9]																			///
	///    int array[3][3]																		///
	///    int** array																			///
	///    std::vector<int> array;																///
	/// So use the data type you prefere.														///
	///////////////////////////////////////////////////////////////////////////////////////////////

	int initialState[9] = { 3, 1, 2, 4, 7, 5, 6, 8, 0 };    // Simple state, finishes immediately
															//int initialState[9] = { 5, 4, 0, 6, 1, 8, 7, 3, 2 };	// This state takes about 27 seconds to find the correct answer
															//int initialState[9] = { 7, 2, 4, 5, 0, 6, 8, 3, 1 };	// This state takes about 27 seconds to find the correct answer

															// A* evaluates the correctness of each state with a function, f(n) = g(n) + h(n), where g(n) is the step cost from the root node
															// and h(n) is the heuristic value of the state, implemented here with the 'Manhatten Distance'.
															// depth is the g(n) value, it represents each level in the tree structure, so it starts at 0 and will increment by 1 each level dropped
															// in the tree
	int depth = 0;

	// Create the root node with the current state and depth
	CurrentState* root = new CurrentState(initialState, depth);

	// Create a priority queue (implemented with a min heap) and add to it the root state
	PriorityQueue<CurrentState>* queue = new PriorityQueue<CurrentState>(root);


	/////////////////////
	/// Main A* loop  ///

	// Loop until the queue is empty
	while (!queue->isEmpty())
	{
		// Increment depth since we are dropping to a new level in the tree
		depth++;

		// Remove the best node from the queue ( the node with the lowest 'f(n) = g(n) + h(n)' )
		CurrentState* bestNode = queue->remove();
		NODES_EXP++;

		// Check to see if the node is in the finished state
		if (bestNode->checkFinishedState())
			return bestNode;

		/////////////////////////////////////////////
		/// Generate successor nodes of bestNode  ///

		// Find the zero, or the 'empty tile', save the indexes in zeroR and zeroC
		int zeroR = 99, zeroC = 99;
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (bestNode->state[i][j] == 0)
				{
					zeroR = i;
					zeroC = j;
					break;
				}
			}
		}

		/////////////////
		/// Move left ///

		// Can only move left if the space is not in the first column
		if (zeroC != 0)
		{
			// Get the state of bestNode
			int** parentArray = bestNode->getState();

			// Swap the 0 with the number to the left
			int temp = parentArray[zeroR][zeroC - 1];
			parentArray[zeroR][zeroC - 1] = 0;
			parentArray[zeroR][zeroC] = temp;

			// Create a new state with the new array and new depth
			CurrentState* leftState = new CurrentState(parentArray, depth);

			// Set the parent of the new state
			leftState->setParent(bestNode);

			// Check all ancestors of the new node and make sure it is not a duplicate state
			//CurrentState* parent = leftState->getParent();
			//bool same = true;
			//while (parent != NULL)
			//{
			//	same = true;
			//	// Check states
			//	for (int i = 0; i < 3; i++)
			//	{
			//		for (int j = 0; j < 3; j++)
			//		{
			//			if (leftState->state[i][j] != parent->state[i][j])
			//			{
			//				// Not same states
			//				same = false;
			//				break;
			//			}
			//		}
			//		if (!same)
			//			break;
			//	}
			//	parent = parent->getParent();
			//}

			//// If it is not a dup state then add it to the queue
			//if (!same)
			//{
			//	queue->add(leftState);
			//	NODES_GEN++;
			//}


			//// If it is a dup state delete it from the heap
			//else
			//	delete leftState;
			queue->add(leftState, NODES_GEN);

		}

		//////////////////
		/// Move Right ///

		// Can only move right if space is not in the third column
		// Logis mirrors that of the Move Left function above
		if (zeroC != 2)
		{
			int** parentArray = bestNode->getState();
			int temp = parentArray[zeroR][zeroC + 1];
			parentArray[zeroR][zeroC + 1] = 0;
			parentArray[zeroR][zeroC] = temp;

			CurrentState* rightState = new CurrentState(parentArray, depth);
			rightState->setParent(bestNode);

			//// Check for same state in ancestor nodes
			//CurrentState* parent = rightState->getParent();
			//bool same = true;
			//while (parent != NULL)
			//{
			//	same = true;
			//	// Check states
			//	for (int i = 0; i < 3; i++)
			//	{
			//		for (int j = 0; j < 3; j++)
			//		{
			//			if (rightState->state[i][j] != parent->state[i][j])
			//			{
			//				// Not same states
			//				same = false;
			//				break;
			//			}
			//		}
			//		if (!same)
			//			break;
			//	}
			//	parent = parent->getParent();
			//}

			//if (!same)
			//{
			//	queue->add(rightState);
			//	NODES_GEN++;
			//}

			//else
			//	delete rightState;
			queue->add(rightState, NODES_GEN);
		}

		// ////////////
		/// Move Up ///

		// Can only move up if the 0 is not in the first row
		// Logic mirrors that of above
		if (zeroR != 0)
		{
			int** parentArray = bestNode->getState();
			int temp = parentArray[zeroR - 1][zeroC];
			parentArray[zeroR - 1][zeroC] = 0;
			parentArray[zeroR][zeroC] = temp;


			CurrentState* upState = new CurrentState(parentArray, depth);
			upState->setParent(bestNode);

			//// Check for same state in ancestor nodes
			//CurrentState* parent = upState->getParent();
			//bool same = true;
			//while (parent != NULL)
			//{
			//	same = true;
			//	// Check states
			//	for (int i = 0; i < 3; i++)
			//	{
			//		for (int j = 0; j < 3; j++)
			//		{
			//			if (upState->state[i][j] != parent->state[i][j])
			//			{
			//				// Not same states
			//				same = false;
			//				break;
			//			}
			//		}
			//		if (!same)
			//			break;
			//	}
			//	parent = parent->getParent();
			//}

			//if (!same)
			//{
			//	queue->add(upState);
			//	NODES_GEN++;
			//}

			//else
			//	delete upState;

			queue->add(upState, NODES_GEN);
		}

		// //////////////
		/// Move Down ///

		// Can only move down if the 0 is not in the third row
		// Logic mirrors that of above
		if (zeroR != 2)
		{
			int** parentArray = bestNode->getState();
			int temp = parentArray[zeroR + 1][zeroC];
			parentArray[zeroR + 1][zeroC] = 0;
			parentArray[zeroR][zeroC] = temp;


			CurrentState* downState = new CurrentState(parentArray, depth);
			downState->setParent(bestNode);


			//// Check for same state in ancestor nodes
			//CurrentState* parent = downState->getParent();
			//bool same = true;
			//while (parent != NULL)
			//{
			//	same = true;
			//	// Check states
			//	for (int i = 0; i < 3; i++)
			//	{
			//		for (int j = 0; j < 3; j++)
			//		{
			//			if (downState->state[i][j] != parent->state[i][j])
			//			{
			//				// Not same states
			//				same = false;
			//				break;
			//			}
			//		}
			//		if (!same)
			//			break;
			//	}
			//	parent = parent->getParent();
			//}

			//if (!same)
			//{
			//	queue->add(downState);
			//	NODES_GEN++;
			//}

			//else
			//	delete downState;

			queue->add(downState, NODES_GEN);
		}
	}

	// If the queue is empty and no winning state has been found
	return NULL;
}
