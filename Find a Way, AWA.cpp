#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "vector"
#include "Shape.h"
#include "Tokenizer.h"

using namespace std;

//Functions of the main file
void setDomain(int(&setMatrix)[5][9], vector<Shape>& shapes);
void randomDomain(int(&randomMatrix)[5][9], vector<Shape>& randomShapes);
void displayMatrix(int(&matrix)[5][9]);
bool processText(istream& is, vector<Shape>& randomShapes, int(&randomMatrix)[5][9], vector<Shape>& setShapes, int(&setMatrix)[5][9], bool interactive = true);

int main() {

    srand(time(0)); //Seeds srand

    vector<Shape> randomShapes; //Vector to hold shapes of the random generator
    vector<Shape> setShapes; //Vector to hold of the set domain

    int randomMatrix[5][9] = { 0 }; //Matrix to hold the infromation of the random generator
    int setMatrix[5][9] = { 0 }; //Matrix to hold the information of the set domain

    if (!processText(cin, randomShapes, randomMatrix, setShapes, setMatrix, true)) {
        //It cleans up all structures used. 
        randomShapes.clear();
        setShapes.clear();
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 9; j++) {
                randomMatrix[i][j] = 0;
                setMatrix[i][j] = 0;
            }
        }
    }

    return 0;
}


//Print out the matrix
void displayMatrix(int(&matrix)[5][9]) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 9; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

//Creates a random domain (a work in progress)
void randomDomain(int(&randomMatrix)[5][9], vector<Shape>& randomShapes) {
    //Add main piece type 2 at index 0 and main piece type 1 at index 1
    int rR, cR;
    do {
        rR = rand() % 5;
        cR = rand() % 9;
    } while (cR + 1 > 8);
    randomShapes.push_back(Shape(1, rR, cR, 1)); //Potion - Both of these are randomly assigned a rR and cR, for now. Potion is type 1.

    do {
        rR = rand() % 5;
        cR = rand() % 9;
    } while (cR + 1 > 8);
    randomShapes.push_back(Shape(2, rR, cR, 2)); //Jose - Will always be index 1, and Potion always index 2. Jose is type 2.

    for (int i = 2; i < 8; i++) {
        int type = rand() % 8 + 3; //Type is between 3 and 10

        if (type == 4 || type == 5) { //Lamp or Camel
            do {
                rR = rand() % 5;
                cR = rand() % 9;
            } while (cR + (type == 4 ? 1 : 3) > 8);
            randomShapes.push_back(Shape(type, rR, cR, type));
        }
        else if (type == 6) { //Weird
            do {
                rR = rand() % 5;
                cR = rand() % 9;
            } while (cR + 1 > 4);
            randomShapes.push_back(Shape(type, rR, cR, true));
        }
        else { //Other types
            randomShapes.push_back(Shape(type, rand() % 5, rand() % 9, type));
        }
    }

    for (int i = 0; i < randomShapes.size(); ++i) {
        int id = randomShapes[i].getID();
        switch (id) {
        case 1:
            randomShapes[i].insert(randomMatrix);
            break;
        case 2:
            randomShapes[i].insert(randomMatrix);
            break;
        case 3:
            randomShapes[i].insertBarrier(randomMatrix);
            break;
        case 4:
            randomShapes[i].insert(randomMatrix);
            break;
        case 5:
            randomShapes[i].insert(randomMatrix);
            break;
        case 6:
            randomShapes[i].insertWeird(randomMatrix);
            break;
        default:
            //Handle the case when the ID is not recognized
            break;
        }
    }
}

//Hardcoded lvl37 of the game.
void setDomain(int(&setMatrix)[5][9], vector<Shape>& shapes) {

    //Add shapes to the vector without giving them names, they are recognized by their inherent id represented in the memory address, reference them by their index in the matrix and their actual member id
    shapes.push_back(Shape(1, 4, 0, 0)); //Jose is 1
    shapes.push_back(Shape(2, 0, 7, 0)); //Potion is 2

    shapes.push_back(Shape(3, 0, 4)); //Barrier is 3
    shapes.push_back(Shape(3, 0, 5)); //
    shapes.push_back(Shape(3, 2, 4)); //
    shapes.push_back(Shape(3, 4, 3)); //
    shapes.push_back(Shape(3, 4, 4)); //

    shapes.push_back(Shape(4, 2, 2, 0)); //Lamp is 4
    shapes.push_back(Shape(4, 2, 5, 0)); //
    shapes.push_back(Shape(5, 1, 5, 0)); //Camel is 5
    shapes.push_back(Shape(5, 3, 0, 0)); //
    shapes.push_back(Shape(6, 0, 0, true)); //Weird is 6
    shapes.push_back(Shape(6, 3, 6, true)); //


    //Uses switch to insert things properly into the matrix by calling the correct insert functions based on ids
    for (int i = 0; i < shapes.size(); ++i) {
        int id = shapes[i].getID();
        switch (id) {
        case 1:
            shapes[i].insert(setMatrix);
            break;
        case 2:
            shapes[i].insert(setMatrix);
            break;
        case 3:
            shapes[i].insertBarrier(setMatrix);
            break;
        case 4:
            shapes[i].insert(setMatrix);
            break;
        case 5:
            shapes[i].insert(setMatrix);
            break;
        case 6:
            shapes[i].insertWeird(setMatrix);
            break;
        default:
            //There's no case where it will fail here
            break;
        }
    }
}

//Credits to Mr. Aubrey Knight. I stole the structure of the Tokenizer class from my old projects in my Data Structures class.
bool processText(istream& is, vector<Shape>& randomShapes, int(&randomMatrix)[5][9], vector<Shape>& setShapes, int(&setMatrix)[5][9], bool interactive) {
    string line;
    string command;
    string arg1, arg2, arg3;
    int int1;
    Shape* selectedShape = nullptr;
    Tokenizer tkn;

    /*///////////////////////////////
    s set - creates matrix of set shapes
    s set -d - creates matrix, then displays it in the console
    s random - creates random matrix of shapes
    s random -d - creates random matrix and then displays it on the console

    -d s - displays the setMatrix on the console
    -d r - displays the randomMatrix on the console

    */
    while (true)
    {
        if (!interactive)
        {
            if (is.eof()) return true;
        }
        else { cout << ">> "; }

        getline(is, line);
        tkn.setString(line);

        tkn.readWord(command);
        if (command == "exit") {
            cout << "Exiting ...." << endl;
            return false;
        }
        if (command == "clear") {
            system("cls");
        }
        if (command == "set") {
            setDomain(setMatrix, setShapes);
            displayMatrix(setMatrix);
        }

        if (command == "s") {
            tkn.readWord(arg1);
            if (arg1 == "random") {
                randomDomain(randomMatrix, randomShapes);
                tkn.readWord(arg2);
                if (arg2 == "-d") {
                    displayMatrix(randomMatrix);
                }
                else if (arg2 == "") {
                    continue;
                }
                else {
                    cout << "Unexpected input." << endl;
                }
            }
            else if (arg1 == "set") {
                setDomain(setMatrix, setShapes);
                tkn.readWord(arg2);
                if (arg2 == "-d") {
                    displayMatrix(setMatrix);
                }
                else if (arg2 == "") {
                    continue;
                }
                else {
                    cout << "Unexpected input." << endl;
                }
            }
            else {
                cout << "Unexpected input." << endl;
            }
        }
        else if (command == "-d") {
            tkn.readWord(arg1);
            if (arg1 == "s") {
                displayMatrix(setMatrix);
            }
            else if (arg1 == "r") {
                displayMatrix(randomMatrix);
            }
            else {
                cout << "Unexpected input." << endl;
            }
        }
        else if (command == "move") { //move 23 N
            tkn.readInteger(int1);
            tkn.readWord(arg2);

            for (int i = 0; i < setShapes.size(); i++) {
                if (setShapes[i].getWritten() == int1) {
                    selectedShape = &setShapes[i];
                }
            }

            if (arg2 == "s" || arg2 == "S"){
                if (selectedShape->collisionSouth(setMatrix)) {
                    cout << "You can move South." << endl;
                    selectedShape->moveSouth(setMatrix);
                    displayMatrix(setMatrix);
                }
                else {
                    cout << "You cannot move South." << endl;
                }
            }
            else if (arg2 == "n" || arg2 == "N") {
                if (selectedShape->collisionNorth(setMatrix)) {
                    cout << "You can move North." << endl;
                    selectedShape->moveNorth(setMatrix);
                    displayMatrix(setMatrix);
                }
                else {
                    cout << "You cannot move North." << endl;
                }
            }
            else if (arg2 == "e" || arg2 == "E") {
                if (selectedShape->collisionEast(setMatrix)) {
                    cout << "You can move East." << endl;
                    selectedShape->moveEast(setMatrix);
                    displayMatrix(setMatrix);
                }
                else {
                    cout << "You cannot move East." << endl;
                }
            }
            else if (arg2 == "w" || arg2 == "W") {
                if (selectedShape->collisionWest(setMatrix)) {
                    cout << "You can move West." << endl;
                    selectedShape->moveWest(setMatrix);
                    displayMatrix(setMatrix);
                }
                else {
                    cout << "You cannot move West." << endl;
                }
            }
            else {
                cout << "Unexpected input." << endl;
            }

            continue;
        }
        else if(command == "convert") {
            int flattenedMatrix = convert2DTo1D(setMatrix);
            for (int i = 0; i < flattenedMatrix.length(); i++) {
                cout << flattenedMatrix[i] << ", ";
            }
        }
    }
}

int convert2DTo1D(int(&matrix)[5][9]) {
    int result[45];
    int x = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 9; j++) {
            result[x] = matrix[i][j];
            x += 1;
        }
    }
    return *result;
}