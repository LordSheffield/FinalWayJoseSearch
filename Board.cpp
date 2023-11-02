#include "Board.h"
#include <vector>
#include "Shape.h"
#include "cstring"

Board::Board() {
    this->shapes = std::vector<Shape>();
    this->initialLocations = std::vector<std::pair<Point, Point>>();
    memset(this->matrix, 0, sizeof(int) * 5 * 9);

    auto s = std::make_shared<State>(); //Creates State.
    s->shapeLocations = this->initialLocations; //Pass copy of initialLocations to the State.
    memset(s->matrix, 0, sizeof(int) * 5 * 9); //Pass copy of matrix outline to the State.
    s->cost = 0;
    s->parent = nullptr;
}


Board::Board(std::vector<Shape> shapes, std::vector<std::pair<Point, Point>> initialLocations, int matrix[5][9]) {
    this->shapes = shapes;
    this->initialLocations = initialLocations;
    memcpy(this->matrix, matrix, sizeof(int) * 5 * 9);

    auto s = std::make_shared<State>(); //Creates State.
    s->shapeLocations = initialLocations; //Pass copy of initialLocations to the State.
    memcpy(s->matrix, matrix, sizeof(int) * 5 * 9); //Pass copy of matrix outline to the State.
    s->cost = 0;
    s->parent = nullptr;

    for (int i = 0; i < shapes.size(); i++) {
        insert(shapes[i].getWritten());
        std::cout << shapes[i].getWritten() << std::endl;
    }
}

bool Board::insert(int written) {
    for (int index = 0; index < this->initialLocations.size(); ++index) { //Find the shape that has a matching 'written' to be checked.
        auto& shapeLocation = this->initialLocations[index];
        Point root = shapeLocation.first; //Get Root and End points of the current shape.
        Point end = shapeLocation.second;

        if (this->shapes[index].getWritten() != written) {
            continue;
        } //Skip if this is not the shape to be inserted.

        //Get the ID of the current shape
        int id = this->shapes[index].getID();

        //Write the shape into the matrix based on its ID
        switch (id) {
        case 3: //Barrier
            if (this->matrix[root.R][root.C] != 0) {
                return false;
            }
            this->matrix[root.R][root.C] = written; //Writes to matrix
            return true;

        case 6: //Weird
            for (int j = root.C; j <= root.C + 1; ++j) { //row 0 from column n to n+1
                if (this->matrix[root.R][j] != 0) {
                    return false;
                }
                this->matrix[root.R][j] = written;
            }

            for (int j = root.C + 1; j <= end.C; ++j) { //row 1 from column n+1 to n+2
                if (this->matrix[root.R + 1][j] != 0) {
                    return false;
                }
                this->matrix[root.R + 1][j] = written;
            }
            return true;

        default: //1x2 and 1x4
            for (int j = root.C; j <= end.C; j++) {
                if (this->matrix[root.R][j] != 0) {
                    return false;
                }
                this->matrix[root.R][j] = written;
            }
            return true;
        }
    }
    return false;
}

//Move Functions
/*
shapeLocation is get from the initialState of the Struct, and making a copy to the shapeLocation local to the function.
for loop will parse the shapeLocation. 'id' will be called based on the shape[index] that the loop is currently in. 
then we check if shapes[index].getWritten() != written, if so, that's not the shape we want to move, so we loop to the next shape, and change the value for the location Pairs

*/
void Board::moveNorth(int written, std::shared_ptr<State> state) {

    for (int index = 0; index < state->shapeLocations.size(); ++index) { //Find the shape that has a matching 'written' to be checked.
        auto& shapeLocation = state->shapeLocations[index];
        Point root = shapeLocation.first;
        Point end = shapeLocation.second;

        //Determine the id based on the index, and using it to call the getID on the current shape of the shape vector. 
        int id = this->shapes[index].getID();
       
        if (this->shapes[index].getWritten() != written) {
            continue;
        } //Move to next loop interaction if the 'written' variable of the current shape[index] is different from the 'written' value passed to the function. 
         

        if (id != 6) {
            //Loop columns from root.C to end.C (inclusive)
            for (int i = root.C; i < end.C + 1; i++) {
                //Clear the index below the shape
                state->matrix[end.R][i] = 0;
                //Move the shape one position upward
                state->matrix[root.R - 1][i] = written;
            }
            //Update the root and end row positions
            root.R -= 1;
            end.R -= 1;
        }
        else if (id == 6) {
            state->matrix[root.R][root.C] = 0;
            state->matrix[end.R][end.C - 1] = 0;
            state->matrix[end.R][end.C] = 0;
            state->matrix[root.R - 1][root.C] = written;
            state->matrix[root.R - 1][root.C + 1] = written;
            state->matrix[end.R - 1][end.C] = written;

            root.R -= 1;
            end.R -= 1;
        }
    }
}

void Board::moveSouth(int written, std::shared_ptr<State> state) {

    for (int index = 0; index < state->shapeLocations.size(); ++index) { //Find the shape that has a matching 'written' to be checked.
        auto& shapeLocation = state->shapeLocations[index];
        Point root = shapeLocation.first;
        Point end = shapeLocation.second;

        //Determine the id based on the index, and using it to call the getID on the current shape of the shape vector. 
        int id = this->shapes[index].getID();

        if (this->shapes[index].getWritten() != written) {
            continue;
        } //Move to next loop interaction if the 'written' variable of the current shape[index] is different from the 'written' value passed to the function. 


        if (id != 6) {
            //Loop columns from root.C to end.C (inclusive)
            for (int i = root.C; i < end.C + 1; i++) {
                //Clear the inxed above the shape
                state->matrix[root.R][i] = 0;
                //Move the shape one position downward
                state->matrix[end.R + 1][i] = written;
            }
            //Update the root and end row positions
            root.R += 1;
            end.R += 1;
        }
        else if (id == 6) {
            //Clear the previous position of the shape
            state->matrix[root.R][root.C] = 0;
            state->matrix[root.R][root.C + 1] = 0;
            state->matrix[end.R][end.C] = 0;

            //Move the shape one position downward
            state->matrix[root.R + 1][root.C] = written;
            state->matrix[end.R + 1][end.C] = written;
            state->matrix[end.R + 1][end.C - 1] = written;

            //Update the root and end row positions
            root.R += 1;
            end.R += 1;
        }
    }
}

void Board::moveWest(int written, std::shared_ptr<State> state) {

    for (int index = 0; index < state->shapeLocations.size(); ++index) { //Find the shape that has a matching 'written' to be checked.
        auto& shapeLocation = state->shapeLocations[index];
        Point root = shapeLocation.first;
        Point end = shapeLocation.second;

        //Determine the id based on the index, and using it to call the getID on the current shape of the shape vector. 
        int id = this->shapes[index].getID();

        if (this->shapes[index].getWritten() != written) {
            continue;
        } //Move to next loop interaction if the 'written' variable of the current shape[index] is different from the 'written' value passed to the function. 

        if (id != 6) {
            //Loop rows from root.R to end.R (inclusive)
            for (int i = root.R; i < end.R + 1; i++) {
                //Clear the index on the right of the shape
                state->matrix[i][end.C] = 0;
                //Move the shape one position to the left
                state->matrix[i][root.C - 1] = written;
            }
            //Update the root and end column positions
            root.C -= 1;
            end.C -= 1;
        }
        else if (id == 6) {
            state->matrix[root.R][root.C + 1] = 0;
            state->matrix[end.R][end.C] = 0;
            state->matrix[root.R][root.C - 1] = written;
            state->matrix[end.R][end.C - 2] = written;

            root.C -= 1;
            end.C -= 1;
        }
    }
}

void Board::moveEast(int written, std::shared_ptr<State> state) {

    for (int index = 0; index < state->shapeLocations.size(); ++index) { //Find the shape that has a matching 'written' to be checked.
        auto& shapeLocation = state->shapeLocations[index];
        Point root = shapeLocation.first;
        Point end = shapeLocation.second;

        //Determine the id based on the index, and using it to call the getID on the current shape of the shape vector. 
        int id = this->shapes[index].getID();

        if (this->shapes[index].getWritten() != written) {
            continue;
        } //Move to next loop interaction if the 'written' variable of the current shape[index] is different from the 'written' value passed to the function. 

        if (id != 6) {
            //Loop rows from root.R to end.R (inclusive)
            for (int i = root.R; i < end.R + 1; i++) {
                //Clear the index on the left of the shape
                state->matrix[i][root.C] = 0;
                //Move the shape one position to the right
                state->matrix[i][end.C + 1] = written;
            }
            //Update the root and end column positions
            root.C += 1;
            end.C += 1;
        }
        else if (id == 6) {
            state->matrix[root.R][root.C] = 0;
            state->matrix[end.R][end.C - 1] = 0;
            state->matrix[root.R][root.C + 2] = written;
            state->matrix[end.R][end.C + 1] = written;

            root.C += 1;
            end.C += 1;
        }
    }
}

//Adapted collisionNSWE functions, all functions should work as intended now.
bool Board::collisionNorth(int written, std::shared_ptr<State> state) {
 
    //Get the points for the potion based on state.shapeLocations, this is knowing potion is always going to be at index 1 of both vectors.
    auto& potionLocation = state->shapeLocations[1];
    Point potionRoot = potionLocation.first;
    Point potionEnd = potionLocation.second;


    //'written' is passed to the function. For loop will parse from 0 to shapeLocation.size(), and in every interaction, it will cretae a Pair of Points for the current index
    //of the shapeLocation. These Points will be used in the rest of the function to determine the checks.
    //The, we get the ID of the current shape in question by calling the getID() on the shapes[index], because shapes hold the actual objects of the shape class.

    //Then we check if the shape we are currently in is the one we want to check, if not, the continue will move to the second interaction.

    for (int index = 0; index < state->shapeLocations.size(); ++index) { //Find the shape that has a matching 'written' to be checked.
        auto& shapeLocation = state->shapeLocations[index];
        Point root = shapeLocation.first;
        Point end = shapeLocation.second;

        //Determine the id based on the index, and using it to call the getID on the current shape of the shape vector. 
        int id = this->shapes[index].getID();

        if (state->matrix[root.R][root.C] != written) {
            continue;
        } //Checks if this is not the shape we want to move, moves to next loop interaction.

        if (id == 1 && ((root.R - 1 == 0 && end.R - 1 == potionRoot.R) || (root.R - 1 == potionEnd.R && end.R - 1 == 0) || (root.R - 1 == potionRoot.R && end.R - 1 == potionRoot.R))) {
            return true;
        } //Jose must be able to fully enter Potion from all 3 possible combinations from South to North.

        if (id == 2 || id == 3) {
            return false;
        } //Potion or barrier cannot be moved. Based on the loop index.


        //\\\\\\\\\\Any other 1xn shape\\\\\\\\\\//
        if (id != 6) {
            for (int i = 0; i < end.C - root.C + 1; i++) { //Checks all the indexes to the right of the root until it reaches the end point. Checks if the cells directly above the shape are clear for movement.
                //Check if we're at the top row or if there's a cell above that's not empty
                if (root.R - 1 < 0 || state->matrix[root.R - 1][root.C + i] != 0) {
                    return false;
                }
            }
            continue;
        } //Takes care of shapes that are not weird.


        //\\\\\\\\\\Weird\\\\\\\\\\//
        //If the code did not continue or return earlier, that means it's a weird shape.
        for (int i = 0; i < end.C - root.C + 1; i++) { //Check the first row from root.R, root.c to end.R-1 to root.C+1 can be moved up, and checks for out of bounds.
            if (state->matrix[root.R][(root.C) + i] == written) {
                if ((root.R - 1) < 0 || state->matrix[(root.R) - 1][(root.C) + i] != 0) {
                    return false;
                }
            }
        }

        for (int i = 0; i < end.C - (root.C + 1) + 1; i++) {
            if (state->matrix[root.R - 1][(root.C + 1) + i] == written) {
                //If any of the cells in the second row are out of bounds or occupied, return false.
                if ((root.R - 2) < 0 || state->matrix[(root.R - 2)][(root.C + 1) + i] != 0) {
                    return false;
                }
            }
            //Check if end.R-1 and end.C is not occupied.
            if ((end.R - 1) < 0 || state->matrix[end.R - 1][end.C] != 0) {
                return false;
            }
        }
    }
    return true;
}

bool Board::collisionSouth(int written, std::shared_ptr<State> state) {

    //Get the points for the potion based on shapeLocations, this is knowing potion is always going to be at index 1 of both vectors.
    auto& potionLocation = state->shapeLocations[1];
    Point potionRoot = potionLocation.first;
    Point potionEnd = potionLocation.second;

    for (int index = 0; index < state->shapeLocations.size(); ++index) { //Find the shape that has a matching 'written' to be checked.
        auto& shapeLocation = state->shapeLocations[index];
        Point root = shapeLocation.first;
        Point end = shapeLocation.second;

        //Determine the id based on the index
        int id = this->shapes[index].getID();
        
        if (state->matrix[root.R][root.C] != written) {
            continue;
        } //Checks if this is not the shape we want to move.

        if (id == 1 && ((root.R + 1 == 0 && end.R + 1 == potionRoot.R) || (root.R + 1 == potionEnd.R && end.R + 1 == 0) || (root.R + 1 == potionRoot.R && end.R + 1 == potionRoot.R))) {
            return true;
        } //Jose must be able to fully enter Potion from all 3 possible combinations from South to North.

        if (id == 2 || id == 3) { 
            return false;
        } //Potion or barrier cannot be moved.

        //\\\\\\\\\\Any other 1xn shape\\\\\\\\\\//
        if (id != 6) {
            for (int i = 0; i < end.C - root.C + 1; i++) { //Check all the indexes to the right of the root until it reaches the end point.
                if (state->matrix[end.R][(root.C) + i] == written) {
                    if ((end.R + 1) >= 5 || state->matrix[(end.R) + 1][(root.C) + i] != 0) { //If any of the cells to the right are out of bounds or occupied, return false.
                        return false;
                    }
                }
            }
            continue;
        }

        //\\\\\\\\\\Weird\\\\\\\\\\//
        //If the code did not continue or return earlier, that means it's a weird shape.
        for (int i = 0; i < end.C - (root.C + 1) + 1; i++) { //Handle id == 6, weird shape. Check the first row from root.R to end.R+1 to root.C to end.C can be moved down, and check for out of bounds.
            if (state->matrix[root.R + 1][(root.C + 1) + i] == written) {
                //If any of the cells in the second row are out of bounds or occupied, return false.
                if ((root.R + 2) > 5 || state->matrix[(root.R + 2)][(root.C + 1) + i] != 0) {
                    return false;
                }
            }
            //Check if end.R-1 and end.C is not occupied.
            if ((end.R + 1) >= 5 || state->matrix[end.R + 1][end.C] != 0) {
                return false;
            }
        }

        //Checks if root.R+1 root.C is !=0, if so, fail. And check for out of bounds. No need to check root.C+1 because it will always be part of the shape. 
        if ((root.R + 1) >= 5 || state->matrix[(root.R) + 1][root.C] != 0) {
            return false;
        }
    }

    return true;
}

bool Board::collisionWest(int written, std::shared_ptr<State> state) {
    //Get the points for the potion based on shapeLocations, this is knowing potion is always going to be at index 1 of both vectors.
    auto& potionLocation = state->shapeLocations[1];
    Point potionRoot = potionLocation.first;
    Point potionEnd = potionLocation.second;

    for (int index = 0; index < state->shapeLocations.size(); ++index) { //Find the shape that has a matching 'written' to be checked.
        auto& shapeLocation = state->shapeLocations[index];
        Point root = shapeLocation.first;
        Point end = shapeLocation.second;

        //Determine the id based on the index
        int id = this->shapes[index].getID();

        if (state->matrix[root.R][root.C] != written) {
            continue;
        } //Checks if this is not the shape we want to move.

        if (id == 1 && (end.C - 1 >= potionRoot.C && end.C - 1 <= potionEnd.C)) {
            return true;
        } //Jose must be able to fully enter Potion from all 3 possible combinations from the West.

        if (id == 2 || id == 3) {
            return false;
        } //Potion or barrier cannot be moved.

        if ((root.C - 1) < 0 || state->matrix[root.R][root.C - 1] != 0) {
            return false;
        }

        else if (state->matrix[root.R + 1][root.C + 1] == written) {
            if (state->matrix[root.R + 1][root.C] != 0) {
                return false;
            }
        }
    }
    return true;
}

bool Board::collisionEast(int written, std::shared_ptr<State> state) {

    //Get the points for the potion based on shapeLocations, this is knowing potion is always going to be at index 1 of both vectors.
    auto& potionLocation = state->shapeLocations[1];
    Point potionRoot = potionLocation.first;
    Point potionEnd = potionLocation.second;

    for (int index = 0; index < state->shapeLocations.size(); ++index) { //Find the shape that has a matching 'written' to be checked.
        auto& shapeLocation = state->shapeLocations[index];
        Point root = shapeLocation.first;
        Point end = shapeLocation.second;

        //Determine the id based on the index
        int id = this->shapes[index].getID();

        if (state->matrix[root.R][root.C] != written) {
            continue;
        } //Checks if this is not the shape we want to move.

        if (id == 1 && (end.C + 1 >= potionRoot.C && end.C + 1 <= potionEnd.C)) {
            return true;
        } //Jose must be able to fully enter Potion from all 3 possible combinations from the East.


        if (id == 2 || id == 3) { //Potion or barrier cannot be moved.
            return false;
        }

        //If the index east of the calling shape's end is used or is out of bounds, return false.
        if ((end.C + 1) > 8 || state->matrix[end.R][(end.C) + 1] != 0) {
            return false;
        }

        else if (state->matrix[(end.R) - 1][(end.C) - 1] == written) {
            if (state->matrix[(end.R) - 1][end.C] != 0) {
                return false;
            }
        }
    }

    return true;
}


std::shared_ptr<State> Board::asearch() {

    auto comp = [](const std::shared_ptr<State>& state1, const std::shared_ptr<State>& state2) {
        return state1->cost + state1->heuristic > state2->cost + state2->heuristic;
    }; //Lambda function to compare states using cost + heuristic of state1 and state2.

    std::priority_queue<std::shared_ptr<State>, std::vector<std::shared_ptr<State>>, decltype(comp)> openList(comp); //Priority queue for openList.

    std::unordered_map<std::string, std::shared_ptr<State>> closedList; //Hash table for closedList

    openList.push(getStart());  //Add start state to openList
     
    while (!openList.empty()) {
        auto current = openList.top(); //Get the state with the lowest cost + heuristic.
        openList.pop(); //Remove from the openList

        std::string key = generateKey(current); //Generate a unique key for the current state. Concatenates the matrix into a string.
        closedList[key] = current; //Add current state to closedList.

        if (closedList.count(key)) {
            continue;
        } //If current state is in closedList, skip.

        if (win(current)) {
            return current;
        } //If current state is the goal, return it.

        auto successors = this->successors(current); //Generate successors of the current state.

        for (auto& successor : successors) { //For each successor.
            std::string succKey = generateKey(successor);

            if (closedList.count(succKey) && successor->cost >= closedList[succKey]->cost) {
                continue;
            } //If successor is in closed list and has a lower cost, skip it.

            openList.push(successor);
        }
    }

    return nullptr; //If no solution found, return nullptr
}

std::shared_ptr<State> Board::getStart() {
    auto startState = std::make_shared<State>();

    startState->cost = 0;
    startState->heuristic = 0;
    startState->parent = nullptr;

    startState->shapeLocations = initialLocations;
    std::memcpy(startState->matrix, matrix, sizeof(int) * 5 * 9);

    return startState;
}

std::vector<std::shared_ptr<State>> Board::successors(std::shared_ptr<State> state) {
    std::vector<std::shared_ptr<State>> successors;

    for (int i = 0; i < shapes.size(); i++) {
        if (collisionNorth(shapes[i].getWritten(), state)) {
            auto s = std::make_shared<State>(*state);
            s->cost++;
            s->parent = state;
            //update changed location
            moveNorth(shapes[i].getWritten(), s);

            successors.push_back(s);
        }
        if (collisionEast(shapes[i].getWritten(), state)) {
            auto s = std::make_shared<State>(*state);
            s->cost++;
            s->parent = state;
            //update changed location
            moveEast(shapes[i].getWritten(), s);

            successors.push_back(s);
        }
        if (collisionSouth(shapes[i].getWritten(), state)) {
            auto s = std::make_shared<State>(*state);
            s->cost++;
            s->parent = state;
            //update changed location
            moveSouth(shapes[i].getWritten(), s);

            successors.push_back(s);
        }
        if (collisionWest(shapes[i].getWritten(), state)) {
            auto s = std::make_shared<State>(*state);
            s->cost++;
            s->parent = state;
            //update changed location
            moveWest(shapes[i].getWritten(), s);

            successors.push_back(s);
        }
    }

    return successors;
}

std::string Board::generateKey(const std::shared_ptr<State>& state) {
    std::string key;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 9; ++j) {
            key += std::to_string(state->matrix[i][j]);
        }
    }
    return key;
}

bool Board::win(std::shared_ptr<State> state) {
    if (state->shapeLocations[0].first.R == state->shapeLocations[1].first.R && state->shapeLocations[0].first.C == state->shapeLocations[1].first.C) {
        return true;
    }
    return false;  //Not yet
}

void Board::getPath(std::shared_ptr<State> goalState) {
    std::stack<std::shared_ptr<State>> path;
    std::shared_ptr<State> current = goalState;

    while (current != nullptr) {
        path.push(current);
        current = current->parent;
    } //Backtracks the path from the goal state to the start state by using the parent of each state, and pushes it to the stack.

    while (!path.empty()) {
        current = path.top(); //Gets the first element of the stack.
        path.pop(); //Pops the first element of the stack to remove it.

        //Prints the cost and heuristic of each state.
        std::cout << "Cost: " << current->cost << ", Heuristic: " << current->heuristic << std::endl;
        
        for (int i = 0; i < 5; ++i) { //Prints the matrix for each state.
            for (int j = 0; j < 9; ++j) {
                std::cout << current->matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}
