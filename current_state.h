///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///																							///
///		Author: Brad Stell																	///
///																							///
///		Date: 9/6/2015																		///
///																							///
///		Description:																		///
///			Class CurrentState declaration. Represents a node's state, and information		///
///			about that state, along with various methods to manipulate the state.			///
///																							///
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

#ifndef A_STAR_CURRENTSTATE_H
#define A_STAR_CURRENTSTATE_H

class CurrentState
{
private:
	int heuristic;
	CurrentState* parent;
	int depth;
public:
	int state[3][3];
	CurrentState();
	CurrentState(int** state, int depth);
	CurrentState(int state[][3], int depth);
	CurrentState(int state[9], int depth);
	CurrentState(std::vector<int> state, int depth);
	bool checkFinishedState();
	void calculateHeuristic();
	void manhattenDistance();
	void misplacedTiles();
	int getHeuristic();
	void setState(int** state);
	void setState(int state[][3]);
	void setState(int state[9]);
	void setState(std::vector<int> state);
	int** getState();
	void setParent(CurrentState* parent);
	CurrentState* getParent();
	friend bool operator< (CurrentState & a, CurrentState & b);
};

#endif //A_STAR_CURRENTSTATE_H
