#pragma once

#include <memory>
#include <vector>
#include "Shape.h"
#include <queue>
#include <unordered_map> 
#include <stack>

struct Point {
    int R;
    int C;
};

struct State {
    std::shared_ptr<State> parent; //Points to the parent that generated the current state.
    std::vector<std::pair<Point, Point>> shapeLocations; //Holds the coordinates of the state.
    int matrix[5][9]{ 0 }; //Will hold the current matrix outline for the state.
    int cost = 0; //Should go up by 1 every time a new state is created. 
    int heuristic = 0; //Can be 0 for now.
};

class Board {

private:
    std::vector<Shape> shapes;
    std::vector<std::pair<Point, Point>> initialLocations;
    std::shared_ptr<State> initialState;

    int matrix[5][9];

public:
    
    //Null constructor
    Board();
    //Constructor for a Board object. Initializes State struct members.
    Board(std::vector<Shape> shapes, std::vector<std::pair<Point, Point>> initialLocations, int matrix[5][9]);
    
    //Functions of the class
    bool insert(int written);

    //Used by the A* and user to move shapes based on the 'written' variable.
    bool collisionNorth(int written, std::shared_ptr<State> state);
    bool collisionSouth(int written, std::shared_ptr<State> state);
    bool collisionEast(int written, std::shared_ptr<State> state);
    bool collisionWest(int written, std::shared_ptr<State> state);

    void moveNorth(int written, std::shared_ptr<State> state);
    void moveSouth(int written, std::shared_ptr<State> state);
    void moveWest(int written, std::shared_ptr<State> state);
    void moveEast(int written, std::shared_ptr<State> state);

    std::shared_ptr<State> asearch();
    std::shared_ptr<State> getStart();
    std::vector<std::shared_ptr<State>> successors(std::shared_ptr<State> state);
    std::string generateKey(const std::shared_ptr<State>& state);
    bool win(std::shared_ptr<State> state);

    void getPath(std::shared_ptr<State> goalState);
};



