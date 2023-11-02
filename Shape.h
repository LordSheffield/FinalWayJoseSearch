#pragma once
#include <iostream>
#include <string>

class Shape
{
private:
	int id; //jose = 1, potion = 2, barrier = 3, lamp = 4, camel = 5, weird = 6.
	int written = 0; //Initialized written variable to store number used in matrix.
	static int instanceCounter; //Ensures that each shape has a different number in the matrix.
	std::string spriteType; //Holds the spriteType for the graphical library.

public:
	//Null constructor

	Shape(); //Null constructor.
	Shape(int id); //Constructor with 'id' sets all the other member variables.

	int getID() {
		return id;
	} //Return 'id'.

	int getWritten() {
		return written;
	} //Returns written.
	std::string getSpriteType() {
		return spriteType;
	} //Returns spriteType for graphics branch.
};
