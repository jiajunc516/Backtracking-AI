// ======================================================================
// FILE:        MyAI.hpp
//
// AUTHOR:      Abdullah Younis
//
// DESCRIPTION: This file contains your agent class, which you will
//              implement. You are responsible for implementing the
//              'getAction' function and any helper methods you feel you
//              need.
//
// NOTES:       - If you are having trouble understanding how the shell
//                works, look at the other parts of the code, as well as
//                the documentation.
//
//              - You are only allowed to make changes to this portion of
//                the code. Any changes to other portions of the code will
//                be lost when the tournament runs your code.
// ======================================================================
// Name: JIAJUN CHANG
// ID: 73041847

#ifndef MYAI_LOCK
#define MYAI_LOCK

#include <vector>
#include <map>
#include <utility>
#include <set>
#include <iterator>		/* begin, end */
#include <iostream>		/* cout, endl */
#include <stdlib.h>     /* rand */
#include "Agent.hpp"

class MyAI : public Agent
{
public:
	MyAI ( void );

	Action getAction
	(
		bool stench,
		bool breeze,
		bool glitter,
		bool bump,
		bool scream
	);

	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
	enum Direction
	{
		RIGHT,
		DOWN,
		LEFT,
		UP
	};
	~MyAI();

private:
	Direction my_dir;	// 0 - right, 1 - down, 2 - left, 3 - up
	int my_x, my_y;
	int max_X, max_Y;

	bool hasArrow;	 // True if the Agent can shoot;
	bool goldGrab;   // True if the gold is grabed;
	bool wumpusDead; // True if the wumpus is dead;
	bool die;        // Decide turn or shoot in an extreme start scene;

	std::map<std::pair<int,int>, std::string> Coords_status; // Store the safety status
	std::vector<std::pair<int,int>> myNeighbors;
	std::vector<Direction> path;	// Record the direction after each action;

	void getNeighbors(Direction dir);
	bool outOfBound(int x, int y);
	bool hasStatus(int x, int y);
	bool allVisited();
	void statusInsert(int x, int y, std::string s);
	void statusDelete(int x, int y);
	void statusClear();
	void setDirection(int d);
	Direction getDirection(int x, int y);
	Direction getOppositeDir(Direction dir);
	Action faceDirectionTo(Direction dir, bool add = true);
	Action turnRandomly();
	Action turnRightOrLeft(Direction dir);
	Action goBack();
	Action makeDecision();
	Action theLastStand(bool breeze, bool stench);
	Action returnHome();

	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
};

#endif