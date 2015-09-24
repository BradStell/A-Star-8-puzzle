///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///																							///
///		Author: Brad Stell																	///
///																							///
///		Date: 9/6/2015																		///
///																							///
///		Description:																		///
///			Class CurrentState definition. Represents a node's state, and information		///
///			about that state, along with various methods to manipulate the state.			///
///																							///
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <cmath>
#include "current_state.h"

/////////////////////////
/// Empty constructor ///
CurrentState::CurrentState() {

	// Set parent to null & set heuristic to 99 (will be changed later)
	parent = NULL;
	heuristic = 99;
}

//////////////////////////////////////////////////
/// Constructor accepts a pointer to a pointer ///
///      aka 2d dynamic array				   ///
CurrentState::CurrentState(int** state, int depth)
{
	// Set parent to null & set heuristic to 99 (will be changed later) & set the depth, g(n)
	parent = NULL;
	heuristic = 99;
	this->depth = depth;

	// Fill in nodes state with incomming state
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			this->state[i][j] = state[i][j];
	}

	for (int i = 0; i < 3; i++)
		delete[] state[i];

	// Calculate the heuristic value of the nodes state ( really the whole f(n) )
	calculateHeuristic();
}

//////////////////////////////////////
/// Constructor accepts a 2D array ///
CurrentState::CurrentState(int state[][3], int depth)
{
	// Set parent to null & set heuristic to 99 (will be changed later) & set the depth, g(n)
	parent = NULL;
	heuristic = 99;
	this->depth = depth;

	// Fill in nodes state with incomming state
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			this->state[i][j] = state[i][j];
	}

	// Calculate the heuristic value of the nodes state ( really the whole f(n) )
	calculateHeuristic();
}

//////////////////////////////////////////
/// Constructor accepts a linear array ///
CurrentState::CurrentState(int state[9], int depth)
{
	// Set parent to null & set heuristic to 99 (will be changed later) & set the depth, g(n)
	parent = NULL;
	heuristic = 99;
	this->depth = depth;

	// Fill in nodes state with incomming state
	int c = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			this->state[i][j] = state[c];
			c++;
		}
	}

	// Calculate the heuristic value of the nodes state ( really the whole f(n) )
	calculateHeuristic();
}

//////////////////////////////////////////////
/// Constructor accepts a std::vector<int> ///
CurrentState::CurrentState(std::vector<int> state, int depth)
{
	// Set parent to null & set heuristic to 99 (will be changed later) & set the depth, g(n)
	parent = NULL;
	heuristic = 99;
	this->depth = depth;

	// Fill in nodes state with incomming state
	unsigned int c = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			this->state[i][j] = state.at(c);
			c++;
		}
	}

	// Calculate the heuristic value of the nodes state ( really the whole f(n) )
	calculateHeuristic();
}
/////////////////////////////////////////////
/// Returns the current state of the node ///
int** CurrentState::getState()
{
	// Make a new dynamic 2D array and populate it with the current nodes state
	int** array;
	array = new int*[3];
	for (int i = 0; i < 3; i++)
		array[i] = new int[3];

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			array[i][j] = this->state[i][j];
		}
	}

	return array;
}

////////////////////////////////////////////////////////////////////////
/// Checks to see if the nodes state is in a valid winning condition ///
bool CurrentState::checkFinishedState()
{
	bool winner = false;

	/////////////////////////////////////////////////
	/// Code to check against normal finish state ///
	if ((state[0][0] == 1 || state[0][0] == 0) &&
		(state[0][1] == 1 || state[0][1] == 2 || state[0][1] == 0) &&
		(state[0][2] == 2 || state[0][2] == 3 || state[0][2] == 0) &&
		(state[1][0] == 3 || state[1][0] == 4 || state[1][0] == 0) &&
		(state[1][1] == 4 || state[1][1] == 5 || state[1][1] == 0) &&
		(state[1][2] == 5 || state[1][2] == 6 || state[1][2] == 0) &&
		(state[2][0] == 6 || state[2][0] == 7 || state[2][0] == 0) &&
		(state[2][1] == 7 || state[2][1] == 8 || state[2][1] == 0) &&
		(state[2][2] == 8 || state[2][2] == 0))
		winner = true;
	else
		winner = false;
		

	return winner;
	return false;
}

//////////////////////////////////////////////////////
/// Sets the current state with a dynamic 2D array ///
void CurrentState::setState(int **state) {
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			this->state[i][j] = state[i][j];
	}

	for (int i = 0; i < 3; i++)
		delete[] state[i];

	// Calculate the heuristic value of the nodes state ( really the whole f(n) )
	calculateHeuristic();
}

//////////////////////////////////////////////////////
/// Sets the current state with a dynamic 2D array ///
void CurrentState::setState(int state[][3]) {

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			this->state[i][j] = state[i][j];
	}

	// Calculate the heuristic value of the nodes state ( really the whole f(n) )
	calculateHeuristic();
}

//////////////////////////////////////////////////
/// Sets the current state with a linear array ///
void CurrentState::setState(int state[9]) {

	int c = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			this->state[i][j] = state[c];
			c++;
		}
	}

	// Calculate the heuristic value of the nodes state ( really the whole f(n) )
	calculateHeuristic();
}

//////////////////////////////////////////////////////
/// Sets the current state with a std::vector<int> ///
void CurrentState::setState(std::vector<int> state)
{
	unsigned int c = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			this->state[i][j] = state.at(c);
			c++;
		}
	}

	// Calculate the heuristic value of the nodes state ( really the whole f(n) )
	calculateHeuristic();
}

/////////////////////////////////////////////////////////
/// Calculates the heuristic calue of the nodes state ///
void CurrentState::calculateHeuristic()
{
	// Calculate the heuristic value

	// Using the 'Manhatten Distance', or block distance of each tile to where it should be
	manhattenDistance();

	// Counting the missplaced tiles
	//misplacedTiles();
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// Calculate the 'Manhatten Distance', or block distance of each tile to where it should be ///
void CurrentState::manhattenDistance()
{
	// Set the heuristic to equal the g(n)
	heuristic = depth;

	// Ugly code, yet runs in O(32) time, or basically linear time

	// Check each tile and determine its distance to where the number should ideally located at
	switch (state[0][0])
	{
	case 1:
		heuristic += 1;
		break;
	case 2:
		heuristic += 2;
		break;
	case 3:
		heuristic += 1;
		break;
	case 4:
		heuristic += 2;
		break;
	case 5:
		heuristic += 3;
		break;
	case 6:
		heuristic += 2;
		break;
	case 7:
		heuristic += 3;
		break;
	case 8:
		heuristic += 4;
	}
	switch (state[0][1])
	{
	case 1:
		heuristic += 0;
		break;
	case 2:
		heuristic += 1;
		break;
	case 3:
		heuristic += 2;
		break;
	case 4:
		heuristic += 1;
		break;
	case 5:
		heuristic += 2;
		break;
	case 6:
		heuristic += 3;
		break;
	case 7:
		heuristic += 2;
		break;
	case 8:
		heuristic += 3;
	}
	switch (state[0][2])
	{
	case 1:
		heuristic += 1;
		break;
	case 2:
		heuristic += 0;
		break;
	case 3:
		heuristic += 3;
		break;
	case 4:
		heuristic += 2;
		break;
	case 5:
		heuristic += 1;
		break;
	case 6:
		heuristic += 4;
		break;
	case 7:
		heuristic += 3;
		break;
	case 8:
		heuristic += 2;
	}
	switch (state[1][0])
	{
	case 1:
		heuristic += 2;
		break;
	case 2:
		heuristic += 3;
		break;
	case 3:
		heuristic += 0;
		break;
	case 4:
		heuristic += 1;
		break;
	case 5:
		heuristic += 2;
		break;
	case 6:
		heuristic += 1;
		break;
	case 7:
		heuristic += 2;
		break;
	case 8:
		heuristic += 3;
	}
	switch (state[1][1])
	{
	case 1:
		heuristic += 1;
		break;
	case 2:
		heuristic += 2;
		break;
	case 3:
		heuristic += 1;
		break;
	case 4:
		heuristic += 0;
		break;
	case 5:
		heuristic += 1;
		break;
	case 6:
		heuristic += 2;
		break;
	case 7:
		heuristic += 1;
		break;
	case 8:
		heuristic += 2;
	}
	switch (state[1][2])
	{
	case 1:
		heuristic += 2;
		break;
	case 2:
		heuristic += 1;
		break;
	case 3:
		heuristic += 2;
		break;
	case 4:
		heuristic += 1;
		break;
	case 5:
		heuristic += 0;
		break;
	case 6:
		heuristic += 3;
		break;
	case 7:
		heuristic += 2;
		break;
	case 8:
		heuristic += 1;
	}
	switch (state[2][0])
	{
	case 1:
		heuristic += 3;
		break;
	case 2:
		heuristic += 4;
		break;
	case 3:
		heuristic += 1;
		break;
	case 4:
		heuristic += 2;
		break;
	case 5:
		heuristic += 3;
		break;
	case 6:
		heuristic += 0;
		break;
	case 7:
		heuristic += 1;
		break;
	case 8:
		heuristic += 2;
	}
	switch (state[2][1])
	{
	case 1:
		heuristic += 2;
		break;
	case 2:
		heuristic += 3;
		break;
	case 3:
		heuristic += 2;
		break;
	case 4:
		heuristic += 1;
		break;
	case 5:
		heuristic += 2;
		break;
	case 6:
		heuristic += 1;
		break;
	case 7:
		heuristic += 0;
		break;
	case 8:
		heuristic += 1;
	}
	switch (state[2][2])
	{
	case 1:
		heuristic += 3;
		break;
	case 2:
		heuristic += 2;
		break;
	case 3:
		heuristic += 3;
		break;
	case 4:
		heuristic += 2;
		break;
	case 5:
		heuristic += 1;
		break;
	case 6:
		heuristic += 2;
		break;
	case 7:
		heuristic += 1;
		break;
	case 8:
		heuristic += 0;
	}
}

//////////////////////////////////////////////////////////////////
/// Calculates the heuristic value by counting misplaced tiles ///
void CurrentState::misplacedTiles()
{
	heuristic = depth;

	if (state[0][1] != 1)
		heuristic++;
	if (state[0][2] != 2)
		heuristic++;
	if (state[1][0] != 3)
		heuristic++;
	if (state[1][1] != 4)
		heuristic++;
	if (state[1][2] != 5)
		heuristic++;
	if (state[2][0] != 6)
		heuristic++;
	if (state[2][1] != 7)
		heuristic++;
	if (state[2][2] != 8)
		heuristic++;
}

////////////////////////////////////////////////////////////////////////
/// Returns the heuristic value, really the f(n) = g(n) + h(n) value ///
int CurrentState::getHeuristic() {
	return heuristic;
}

/////////////////////////////////////////////////////////////////////////
/// Sets this nodes parent node ( the game state that created it )    ///
void CurrentState::setParent(CurrentState* parent)
{
	this->parent = parent;
}

/////////////////////////////////////////////////////////////////////////
/// Returns this nodes parent node ( the game state that created it ) ///
CurrentState* CurrentState::getParent()
{
	return parent;
}

bool operator< (CurrentState & a, CurrentState & b)
{
	return a.getHeuristic() < b.getHeuristic();
}
