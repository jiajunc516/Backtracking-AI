// ======================================================================
// FILE:        MyAI.cpp
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

#include "MyAI.hpp"

MyAI::MyAI() : Agent()
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================
	my_x = 0;
	my_y = 0;
	max_X = -1;
	max_Y = -1;
	my_dir = RIGHT;
	hasArrow = true;
	goldGrab = false;
	wumpusDead = false;
	die = rand() % 2;
	statusInsert(my_x, my_y, "*");
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}

Agent::Action MyAI::getAction
(
	bool stench,
	bool breeze,
	bool glitter,
	bool bump,
	bool scream
)
{
	// ======================================================================
	// YOUR CODE BEGINS
	// ======================================================================

	if (scream)
	{
		wumpusDead = true;
		statusClear();
	}

	if (goldGrab)
	{
		return returnHome();
	}

	if (glitter)
	{
		goldGrab = true;
		return GRAB;
	}

	if (bump)
	{
		if (my_dir == RIGHT)
		{
			statusDelete(my_x, my_y);
			my_x--;
			max_X = my_x;
		}
		else if (my_dir == UP)
		{
			statusDelete(my_x, my_y);
			my_y--;
			max_Y = my_y;
		}
		return makeDecision();
	}

	std::pair<int,int> myLocation = std::make_pair(my_x, my_y);
	//std::cout << "current at (" << myLocation.first << ", " << myLocation.second <<") "<<
	//	Coords_status[myLocation] << std::endl;
	if (Coords_status[myLocation] == "*")
	{
		
		if (my_x == 0 && my_y == 0 && stench && !wumpusDead)
			return theLastStand(breeze, stench);
		
		Coords_status[myLocation] = ".";
		if (stench && !wumpusDead)
			Coords_status[myLocation] += "S";
		if (breeze)
			Coords_status[myLocation] += "B";
		if ((!breeze && !stench) || (stench && wumpusDead && !breeze))
		{
			getNeighbors(my_dir);
			for (int i = 0; i < myNeighbors.size(); i++)
			{
				std::pair<int,int> v = myNeighbors[i];
				int x = v.first;
				int y = v.second;
				//std::cout << "(" << x << ", " << y << ")" << std::endl;
				if (!hasStatus(x, y) && !outOfBound(x, y))
				{
					statusInsert(x, y, "*");
				}
			}
		}
	}
	if (allVisited())
		return returnHome();

	return makeDecision();
	// ======================================================================
	// YOUR CODE ENDS
	// ======================================================================
}

// ======================================================================
// YOUR CODE BEGINS
// ======================================================================
MyAI::~MyAI()
{}

void MyAI::getNeighbors(Direction d)
{
	if (!myNeighbors.empty())
		myNeighbors.clear();

	std::pair<int, int> v1,v2,v3,v4;
	v1 = std::make_pair(my_x+1, my_y);
	v2 = std::make_pair(my_x, my_y-1);
	v3 = std::make_pair(my_x-1, my_y);
	v4 = std::make_pair(my_x, my_y+1);
	if (d == RIGHT)
	{
		myNeighbors.push_back(v1); //  0
		myNeighbors.push_back(v2); //  1
		myNeighbors.push_back(v4); // -1
	}
	else if (d == DOWN)
	{
		myNeighbors.push_back(v2); //  0
		myNeighbors.push_back(v1); // -1
		myNeighbors.push_back(v3); //  1
	}
	else if (d == LEFT)
	{
		myNeighbors.push_back(v3); //  0
		myNeighbors.push_back(v2); // -1
		myNeighbors.push_back(v4); //  1
	}
	else
	{
		myNeighbors.push_back(v4); //  0
		myNeighbors.push_back(v3); // -1
		myNeighbors.push_back(v1); //  1
	}
}

void MyAI::statusInsert(int x, int y, std::string s)
{
	std::pair<int,int> v = std::make_pair(x, y);
	Coords_status[v] = s;
	//std::cout << "insert (" << x << ", " << y << "): " << s << std::endl;
}

void MyAI::statusDelete(int x, int y)
{
	if (hasStatus(x, y))
	{
		std::pair<int, int> v = std::make_pair(x, y);
		Coords_status.erase(v);
	}
}

void MyAI::statusClear()
{
	for (std::map<std::pair<int,int>, std::string>::iterator it = Coords_status.begin();
			it != Coords_status.end(); it++)
	{
		if (it->second.find("S") != std::string::npos && it->second.find("B") == std::string::npos)
		{
			//std::cout << "clear (" << it->first.first << ", " << it->first.second << ")" <<std::endl;
			it->second = ".";
		}
	}
}

bool MyAI::hasStatus(int x, int y)
{
	if (Coords_status.empty())
		return false;
	std::pair<int, int> v = std::make_pair(x ,y);
	if(Coords_status.find(v) != Coords_status.end())
		return true;
	return false;
}

bool MyAI::outOfBound(int x, int y)
{
	if (x < 0 || y < 0)
		return true;
	if (max_X > 0 && x > max_X)
		return true;
	if (max_Y > 0 && y > max_Y)
		return true;
	return false;
}

bool MyAI::allVisited()
{
	for (std::map<std::pair<int,int>, std::string>::iterator it = Coords_status.begin();
			it != Coords_status.end(); it++)
	{
		if (it->second.find("*") != std::string::npos)
		{
			return false;
		}
	}
	return true;
}

MyAI::Direction MyAI::getDirection(int x, int y)
{
	if (x == my_x + 1)
		return RIGHT;
	else if (x == my_x - 1)
		return LEFT;
	else if (y == my_y + 1)
		return UP;
	else // if (y == my_y - 1)
		return DOWN;
}

MyAI::Direction MyAI::getOppositeDir(Direction d)
{
	if (d == RIGHT)
		return LEFT;
	if (d == DOWN)
		return UP;
	if (d == LEFT)
		return RIGHT;
	//if (d == UP)
	return DOWN;
}

Agent::Action MyAI::faceDirectionTo(Direction d, bool add)
{
	if (my_dir == d)
	{
		switch (my_dir)
		{
			case 0: my_x++;
					break;
			case 1: my_y--;
					break;
			case 2: my_x--;
					break;
			case 3: my_y++;
					break;
			default: break;
		}
		if (add)
			path.push_back(my_dir);
		return FORWARD;
	}
	if (getOppositeDir(d) == my_dir)
		return turnRandomly();
	return turnRightOrLeft(d);
}

void MyAI::setDirection(int d)
{
	switch (d)
	{
		case 0: my_dir = RIGHT;
				break;
		case 1: my_dir = DOWN;
				break;
		case 2: my_dir = LEFT;
				break;
		case 3: my_dir = UP;
				break;
		default: break;
	}
}

Agent::Action MyAI::turnRandomly()
{
	bool r = rand() % 2;
	if (r)
	{
		setDirection((my_dir + 4 - 1) % 4);
		return TURN_LEFT;
	}
	setDirection((my_dir + 4 - 3) % 4);
	return TURN_RIGHT;
}

Agent::Action MyAI::turnRightOrLeft(Direction d)
{
	if ((my_dir + 4 - d) % 4 == 1)
	{
		my_dir = d;
		return TURN_LEFT;
	}
	my_dir = d;
	return TURN_RIGHT;
}

Agent::Action MyAI::goBack()
{
	Action result = CLIMB;
	if (!path.empty())
	{
		Direction lastDir = path.back();
		Direction nextDir = getOppositeDir(lastDir);
		if (my_dir == nextDir)
			path.pop_back();
		result = faceDirectionTo(nextDir, false);
	}
	return result;
}

Agent::Action MyAI::returnHome()
{
	if (my_x == 0 && my_y == 0)
		return CLIMB;
	std::vector<Direction> dirOpt;
	if (my_x > 0 && hasStatus(my_x-1, my_y))
	{
		std::pair<int,int> v1 = std::make_pair(my_x-1, my_y);
		if (Coords_status[v1].find(".") != std::string::npos || Coords_status[v1].find("*") != std::string::npos)
			dirOpt.push_back(LEFT);
	}
	if (my_y >0 && hasStatus(my_x, my_y-1))
	{
		std::pair<int,int> v2 = std::make_pair(my_x, my_y-1);
		if (Coords_status[v2].find(".") != std::string::npos || Coords_status[v2].find("*") != std::string::npos)
			dirOpt.push_back(DOWN);
	}
	if (!dirOpt.empty())
	{
		for (int i = 0; i < dirOpt.size(); i++)
		{
			if (dirOpt[i] == my_dir)
				return faceDirectionTo(dirOpt[i]);
		}
		for (int i = 0; i < dirOpt.size(); i++)
		{
			if (dirOpt[i] != getOppositeDir(my_dir))
				return faceDirectionTo(dirOpt[i]);
		}
		return faceDirectionTo(dirOpt[0]);
	}
	std::pair<int,int> myLocation = std::make_pair(my_x, my_y);
	Coords_status[myLocation] = "NO";
	return goBack();

}

Agent::Action MyAI::makeDecision()
{
	getNeighbors(my_dir);
	for (int i = 0; i < myNeighbors.size(); i++)
	{
		std::pair<int,int> v = myNeighbors[i];
		Direction nextDir = getDirection(v.first, v.second);
		if (hasStatus(v.first, v.second))
		{
			if (Coords_status[v] == "*")
				return faceDirectionTo(nextDir);
		}
	}
	return goBack();
}

Agent::Action MyAI::theLastStand(bool breeze, bool stench)
{
	if (breeze)
		return CLIMB;
	if (stench && hasArrow)
	{
		if (die)
		{
			hasArrow = false;
			return SHOOT;
		}
		else
		{
			die = true;
			my_dir = UP;
			return TURN_LEFT;
		}
	}
	return CLIMB; // if the fire try did not kill the wumpus, agent has to climb
}

// ======================================================================
// YOUR CODE ENDS
// ======================================================================