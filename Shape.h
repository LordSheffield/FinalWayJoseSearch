#pragma once
#include <iostream>
#include <string>

using namespace std;

struct Point {
	int R;
	int C;
};

class Shape
{
private: 
	Point root;
	Point end;

	int id;
	int written = 0; //Initialized written variable to store number used in matrix
	string spriteType;

	static int instanceCounter;

public:
	//Null constructor

	Shape();

	//Barrier shape constructor
	Shape(int id, int rR, int cR);

	//Constructor for creating Jose, Potion, and 1x2 an 1x4
	Shape(int id, int rR, int cR, int dummy1);

	//Constructor for creating weirds
	Shape(int id, int rR, int rC, bool dummy2);

	bool collisionNorth(Point root, int type, int(&matrix)[5][9]);
	bool collisionEast(Point end, int type, int(&matrix)[5][9]);
	bool collisionSouth(Point end, int type, int(&matrix)[5][9]);
	bool collisionWest(Point root, int type, int(&matrix)[5][9]);

	void displayPoints() {
		cout << "Root(R,C): (" << root.R << "," << root.C << ")" << endl;
		cout << "End(R,C): (" << end.R << "," << end.C << ")" << endl;
	}

	Shape moveN(Shape obj, int(&matrix)[5][9]);
	Shape moveS(Shape obj, int(&matrix)[5][9]);
	Shape moveW(Shape obj, int(&matrix)[5][9]);
	Shape moveE(Shape obj, int(&matrix)[5][9]);

	bool insert(int(&matrix)[5][9]);
	bool insertWeird(int(&matrix)[5][9]);
	bool insertBarrier(int(&matrix)[5][9]);

	int getID() {
		return id;
	}

	int getWritten() {
		return written;
	}

	string getSpriteType() {
		return spriteType;
	}

};