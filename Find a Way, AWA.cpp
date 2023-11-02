#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "Shape.h"
#include <thread>
#include <iomanip> 
#include "Tokenizer.h"
#include "board.h"


using namespace std;


//Functions of the main file
//void setDomain(int(&setMatrix)[5][9], std::vector<Shape>& setShapes);
void testDomain(int(&testMatrix)[5][9], std::vector<Shape>& testShapes);
void pushVector(std::vector<Shape>& setShapes, std::vector<std::pair<Point, Point>>& setLocation);
bool processText(istream& is, std::vector<std::pair<Point, Point>>& setLocation, std::vector<Shape>& setShapes, int(&setMatrix)[5][9], std::vector<Shape>& testShapes, int(&testMatrix)[5][9], bool interactive = true);

int main() {

    srand(time(0)); //Seeds srand

    std::vector<Shape> setShapes; //std::vector to hold of the set domain
    std::vector<std::pair<Point, Point>> setLocation;

    std::vector<Shape> testShapes;
    std::vector<std::pair<Point, Point>> testLocation;


    int setMatrix[5][9] = { 0 }; //Matrix to hold the information of the set domain
    int testMatrix[5][9] = { 0 };

    processText(cin, setLocation, setShapes, setMatrix, testShapes, testMatrix, true);
    
    return 0;
}

//Credits to Mr. Aubrey Knight. I stole the structure of the Tokenizer class from my old projects in my Data Structures class.
bool processText(istream& is, std::vector<std::pair<Point, Point>>& setLocation, std::vector<Shape>& setShapes, int(&setMatrix)[5][9], std::vector<Shape>& testShapes, int(&testMatrix)[5][9], bool interactive) {
    string line;
    string command;
    Tokenizer tkn; 
    Board board;

    /*///////////////////////////////////////////////////////////////////////////

    *////////////////////////////////////////////////////////////////////////////
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

        if (command == "set") { //Sets the domain for the setDomain faster.
            pushVector(setShapes, setLocation);
            board = Board(setShapes, setLocation, setMatrix);
        }

        /*if (command == "test") {  //Thoubleshooting domain, can have individual pieces inserted and tested freely.
            testDomain(testMatrix, testShapes);
            displayMatrix(testMatrix);
            cout << endl;
        }*/

        if (command == "solve") {
            board.getPath(board.asearch());
        }
        
    }
}

//This function will create the shapes by passing their ID, and then will create the pair for the Root and End points of each of the objects.
//NOTICE: Ojects maintain the same index in both vectors. Index 0 in setShapes will have Jose, index 0 in setLocation will have <Root, End> for Jose.
void pushVector(std::vector<Shape>& setShapes, std::vector<std::pair<Point, Point>>& setLocation) {
    Point Root, End;

    //Add setShapes to the std::vector without giving them names, they are recognized by their inherent id represented in the memory address, reference them by their index in the matrix and their actual member id
    setShapes.push_back(Shape(1)); //Jose is 1
    setShapes.push_back(Shape(2)); //Potion is 2
    setShapes.push_back(Shape(3)); //Barrier is 3
    setShapes.push_back(Shape(3)); //
    setShapes.push_back(Shape(3)); //
    setShapes.push_back(Shape(3)); //
    setShapes.push_back(Shape(3)); //
    setShapes.push_back(Shape(4)); //Lamp is 4
    setShapes.push_back(Shape(4)); //
    setShapes.push_back(Shape(5)); //Camel is 5
    setShapes.push_back(Shape(5)); //
    setShapes.push_back(Shape(6)); //Weird is 6
    setShapes.push_back(Shape(6)); //

    Root.R = 4; Root.C = 0; End.R = 4; End.C = 1; setLocation.push_back(std::make_pair(Root, End)); //Jose
    Root.R = 0; Root.C = 7; End.R = 0; End.C = 8; setLocation.push_back(std::make_pair(Root, End)); //Potion
    Root.R = 0; Root.C = 4; End.R = 0; End.C = 4; setLocation.push_back(std::make_pair(Root, End)); //Barrier 1
    Root.R = 0; Root.C = 5; End.R = 0; End.C = 5; setLocation.push_back(std::make_pair(Root, End)); //Barrier 2
    Root.R = 2; Root.C = 4; End.R = 2; End.C = 4; setLocation.push_back(std::make_pair(Root, End)); //Barrier 3
    Root.R = 4; Root.C = 3; End.R = 4; End.C = 3; setLocation.push_back(std::make_pair(Root, End)); //Barrier 4
    Root.R = 4; Root.C = 4; End.R = 4; End.C = 4; setLocation.push_back(std::make_pair(Root, End)); //Barrier 5
    Root.R = 2; Root.C = 2; End.R = 2; End.C = 3; setLocation.push_back(std::make_pair(Root, End)); //Lamp 1
    Root.R = 2; Root.C = 5; End.R = 2; End.C = 6; setLocation.push_back(std::make_pair(Root, End)); //Lamp 2
    Root.R = 3; Root.C = 0; End.R = 3; End.C = 3; setLocation.push_back(std::make_pair(Root, End)); //Camel 1
    Root.R = 1; Root.C = 5; End.R = 1; End.C = 8; setLocation.push_back(std::make_pair(Root, End)); //Camel 2
    Root.R = 0; Root.C = 0; End.R = 1; End.C = 2; setLocation.push_back(std::make_pair(Root, End)); //Weird 1
    Root.R = 3; Root.C = 6; End.R = 4; End.C = 8; setLocation.push_back(std::make_pair(Root, End)); //Weird 2
}

void testDomain(int(&testMatrix)[5][9], std::vector<Shape>& testShapes) {
    /*testShapes.push_back(Shape(1, 4, 0, 0)); //Jose is 1
    testShapes.push_back(Shape(2, 0, 7, 0)); //Potion is 2
    testShapes.push_back(Shape(5, 3, 4, 0)); //Potion is 2
    testShapes.push_back(Shape(6, 3, 2, true)); //Potion is 2
    testShapes[0].insert(testMatrix);
    testShapes[1].insert(testMatrix);
    testShapes[2].insert(testMatrix);
    testShapes[3].insertWeird(testMatrix);
    */
}
