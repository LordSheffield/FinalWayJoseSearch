#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "vector"
#include "Shape.h"
#include <thread>
#include <iomanip> 
#include "Tokenizer.h"
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

using namespace std;

//Functions of the main file
void setDomain(int(&setMatrix)[5][9], vector<Shape>& setShapes);
void testDomain(int(&testMatrix)[5][9], vector<Shape>& testShapes);
void randomDomain(int(&randomMatrix)[5][9], vector<Shape>& randomShapes);
void displayMatrix(int(matrix)[5][9]);
bool win(vector<Shape> shapes);
void movetoWin(const string& direction, Shape* selectedShape, vector<Shape>& shapes, int(&matrix)[5][9]);
bool processText(istream& is, vector<Shape>& randomShapes, int(&randomMatrix)[5][9], vector<Shape>& setShapes, int(&setMatrix)[5][9], vector<Shape>& testShapes, int(&testMatrix)[5][9], bool interactive = true);

//FunctionsThatMightNotWork
int convert2DTo1D(int(&matrix)[5][9]);
int heuristic(vector<Shape>& shapes);

int main() {

    srand(time(0)); //Seeds srand

    vector<Shape> randomShapes; //Vector to hold shapes of the random generator
    vector<Shape> setShapes; //Vector to hold of the set domain
    vector<Shape> testShapes;


    int randomMatrix[5][9] = { 0 }; //Matrix to hold the infromation of the random generator
    int setMatrix[5][9] = { 0 }; //Matrix to hold the information of the set domain
    int testMatrix[5][9] = { 0 };

    if (!processText(cin, randomShapes, randomMatrix, setShapes, setMatrix, testShapes, testMatrix, true)) {
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
void displayMatrix(int(matrix)[5][9]) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 9; j++) {
            cout << setw(3) << matrix[i][j];
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

void testDomain(int(&testMatrix)[5][9], vector<Shape>& testShapes) {
    testShapes.push_back(Shape(1, 4, 0, 0)); //Jose is 1
    testShapes.push_back(Shape(2, 0, 7, 0)); //Potion is 2
    testShapes.push_back(Shape(5, 3, 4, 0)); //Potion is 2
    testShapes.push_back(Shape(6, 3, 2, true)); //Potion is 2
    testShapes[0].insert(testMatrix);
    testShapes[1].insert(testMatrix);
    testShapes[2].insert(testMatrix);
    testShapes[3].insertWeird(testMatrix);

}

//Hardcoded lvl37 of the game.
void setDomain(int(&setMatrix)[5][9], vector<Shape>& setShapes) {

    //Add setShapes to the vector without giving them names, they are recognized by their inherent id represented in the memory address, reference them by their index in the matrix and their actual member id
    setShapes.push_back(Shape(1, 4, 0, 0)); //Jose is 1
    setShapes.push_back(Shape(2, 0, 7, 0)); //Potion is 2

    setShapes.push_back(Shape(3, 0, 4)); //Barrier is 3
    setShapes.push_back(Shape(3, 0, 5)); //
    setShapes.push_back(Shape(3, 2, 4)); //
    setShapes.push_back(Shape(3, 4, 3)); //
    setShapes.push_back(Shape(3, 4, 4)); //

    setShapes.push_back(Shape(4, 2, 2, 0)); //Lamp is 4
    setShapes.push_back(Shape(4, 2, 5, 0)); //
    setShapes.push_back(Shape(5, 1, 5, 0)); //Camel is 5
    setShapes.push_back(Shape(5, 3, 0, 0)); //
    setShapes.push_back(Shape(6, 0, 0, true)); //Weird is 6
    setShapes.push_back(Shape(6, 3, 6, true)); //


    //Uses switch to insert things properly into the matrix by calling the correct insert functions based on ids
    for (int i = 0; i < setShapes.size(); ++i) {
        int id = setShapes[i].getID();
        switch (id) {
        case 1:
            setShapes[i].insert(setMatrix);
            break;
        case 2:
            setShapes[i].insert(setMatrix);
            break;
        case 3:
            setShapes[i].insertBarrier(setMatrix);
            break;
        case 4:
            setShapes[i].insert(setMatrix);
            break;
        case 5:
            setShapes[i].insert(setMatrix);
            break;
        case 6:
            setShapes[i].insertWeird(setMatrix);
            break;
        default:
            //There's no case where it will fail here
            break;
        }
    }
}

//Credits to Mr. Aubrey Knight. I stole the structure of the Tokenizer class from my old projects in my Data Structures class.
bool processText(istream& is, vector<Shape>& randomShapes, int(&randomMatrix)[5][9], vector<Shape>& setShapes, int(&setMatrix)[5][9], vector<Shape>& testShapes, int(&testMatrix)[5][9], bool interactive) {
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
        if (command == "exit") { //Exist console.
            cout << "Exiting ...." << endl;
            return false;
        }
        if (command == "clear") { //Clear console window.
            system("cls");
        }
        if (command == "set") { //Sets the domain for the setDomain faster
            setDomain(setMatrix, setShapes);
            displayMatrix(setMatrix);
            cout << endl;
        }
        if (command == "test") {  //Thoubleshooting domain, can have individual pieces inserted and tested freely.
            testDomain(testMatrix, testShapes);
            displayMatrix(testMatrix);
            cout << endl;
        }

        if (command == "solve") {
            ifstream file("solve.txt");
            if (file.is_open()) {
                string fileCommand;
                while (getline(file, fileCommand)) {
                    //Execute each command in the file
                    istringstream iss(fileCommand);
                    processText(iss, randomShapes, randomMatrix, setShapes, setMatrix, testShapes, testMatrix, false);

                    // Sleep for 1 second
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
                file.close();
            }
            else {
                cout << "Unable to open file" << endl;
            }
            continue;
        }


        //testMatrix stuff
        if (command == "t") {
            tkn.readInteger(int1);
            tkn.readWord(arg2);

            if (int1 == 3) {
                cout << "Error, you attempted to move a barrier." << endl;
                continue;
            }

            for (int i = 0; i < testShapes.size(); i++) {
                if (testShapes[i].getWritten() == int1) {
                    selectedShape = &testShapes[i];
                }
            }

            if (arg2 == "s" || arg2 == "S") {
                movetoWin(arg2, selectedShape, testShapes, testMatrix);
            }
            else if (arg2 == "n" || arg2 == "N") {
                movetoWin(arg2, selectedShape, testShapes, testMatrix);
            }
            else if (arg2 == "e" || arg2 == "E") {
                movetoWin(arg2, selectedShape, testShapes, testMatrix);
            }
            else if (arg2 == "w" || arg2 == "W") {
                movetoWin(arg2, selectedShape, testShapes, testMatrix);
            }
        }

        //setMatrix stuff
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
            else if (arg1 == "test") {
                setDomain(testMatrix, testShapes);
                tkn.readWord(arg2);
                if (arg2 == "-d") {
                    displayMatrix(testMatrix);
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
        
        //Direct display handlers
        else if (command == "-d") {
            tkn.readWord(arg1);
            if (arg1 == "s") {
                displayMatrix(setMatrix);
            }
            else if (arg1 == "r") {
                displayMatrix(randomMatrix);
            }
            else if (arg1 == "t") {
                displayMatrix(testMatrix);
            }
            else {
                cout << "Unexpected input." << endl;
            }
        }
        
        //Moving function calls
        else if (command == "m") { //move 23 N
            tkn.readInteger(int1);
            tkn.readWord(arg2);

            if (int1 == 3) {
                cout << "Error, you attempted to move a barrier." << endl;
                continue;
            }

            for (int i = 0; i < setShapes.size(); i++) {
                if (setShapes[i].getWritten() == int1) {
                    selectedShape = &setShapes[i];
                }
            }

            if (arg2 == "s" || arg2 == "S" || arg2 == "n" || arg2 == "N" || arg2 == "e" || arg2 == "E" || arg2 == "w" || arg2 == "W") {
                movetoWin(arg2, selectedShape, setShapes, setMatrix);
            }

            else {
                cout << "Unexpected input." << endl;
            }

            continue;
        }
        

        //TO BE REMOVED
        /*
        else if(command == "convert") {
            int flattenedMatrix = convert2DTo1D(setMatrix);
            for (int i = 0; i < flattenedMatrix.size(); i++) {
                cout << flattenedMatrix[i] << ", ";
            }
        }
        */

        //Heuristics calculator.
        else if(command == "h") {
            cout << heuristic(setShapes) << endl;
        }
    }
}

//Checks for win condition.
bool win(vector<Shape> shapes) {
    //Check if Jose's indexes are the same as the potion's indexes
    if (shapes[0].getRoot().R == shapes[1].getRoot().R && shapes[0].getRoot().C == shapes[1].getRoot().C) {
        return true; //WIN
    }
    return false;  //Not yet
}


//This function is used to move pieces without oversaturating the processText function, since it's where we do the most handling
void movetoWin(const string& direction, Shape* selectedShape, vector<Shape>& shapes, int(&matrix)[5][9]) {

    //For every cardinal direction, we simply check collion, and the move the piece. 
    //Those are functions already set up in the Shape class, and we also call the display and win functions from the main file.
    if (direction == "s" || direction == "S") {
        if (selectedShape->collisionSouth(shapes[1], matrix)) { //Calls collision
            cout << "You can move South." << endl; //Success message.
            selectedShape->moveSouth(matrix); //Calls move function.
        }
        else if (selectedShape->getID() == 2) { //Potion cannot be moved. 
            cout << "You cannot move the potion." << endl;
        }
        else {
            cout << "You cannot move South." << endl; //Error message in case of false.
        }
    }
    else if (direction == "n" || direction == "N") {
        if (selectedShape->collisionNorth(shapes[1], matrix)) { //In each call of collision, shapes[1], the potion index is passed, because there is an exception of movement, for when jose moves into the potion, which should be true.
            cout << "You can move North." << endl;
            selectedShape->moveNorth(matrix);
        }
        else if (selectedShape->getID() == 2) { //Potion cannot be moved. 
            cout << "You cannot move the potion." << endl;
        }
        else {
            cout << "You cannot move " << selectedShape->getWritten() << " North." << endl;
        }
    }
    else if (direction == "e" || direction == "E") {
        if (selectedShape->collisionEast(shapes[1], matrix)) {
            cout << "You can move East." << endl;
            selectedShape->moveEast(matrix);
        }
        else if (selectedShape->getID() == 2) { //Potion cannot be moved. 
            cout << "You cannot move the potion." << endl;
        }
        else {
            cout << "You cannot move " << selectedShape->getWritten() << " East." << endl;
        }
    }
    else if (direction == "w" || direction == "W") {
        if (selectedShape->collisionWest(shapes[1], matrix)) {
            cout << "You can move West." << endl;
            selectedShape->moveWest(matrix);
        }
        else if (selectedShape->getID() == 2) { //Potion cannot be moved. 
            cout << "You cannot move the potion." << endl;
        }
        else {
            cout << "You cannot move " << selectedShape->getWritten() << " West." << endl;
        }
    }
    else {
        cout << "Unexpected input." << endl;
        return;
    }

    displayMatrix(matrix); //Just displays the matrix every interaction that is allowed.
    cout << endl;

    if (win(shapes)) { //Checks for win condition, by calling the win function, and then prints Jose's image.
        cout << endl << "Jose found his way! Game over." << endl << endl;
        ifstream file("joseFoundaWay.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                cout << line << endl;
            }
            file.close();
        }
        else {
            cout << "Unable to open file" << endl;
        }

        // Play the win song
        PlaySound(TEXT("winsong.mp3"), NULL, SND_FILENAME | SND_ASYNC);

        exit(0); // End the game
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

int heuristic(vector<Shape>& shapes) {
    int joseC;
    int joseR;
    int potionC;
    int potionR;
    for (int i = 0; i < shapes.size(); ++i) { 
        if(shapes[i].getID() == 1) {
            joseC = shapes[i].getEndC();
            joseR = shapes[i].getEndR();
        }
        if(shapes[i].getID() == 2) {
            potionC = shapes[i].getEndC();
            potionR = shapes[i].getEndR();
        }
    }
    int h = abs(joseR + potionR) + abs(joseC - potionC);

    return h;
}

