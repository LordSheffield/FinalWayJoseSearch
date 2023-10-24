#include "Shape.h"
#include <vector>

int Shape::instanceCounter = 20; 

//Just creates a null constructor
Shape::Shape() {
    root.R = 0;
    root.C = 0;
    end.R = 0;
    end.C = 0;
    id = 0;
    spriteType = "";
    written = 0;
    instanceCounter++;
}

//Creates barrier shape 
Shape::Shape(int id, int rR, int cR) {
    root.R = rR;
    root.C = cR;
    end.R = rR;
    end.C = cR;
    this->id = id;
    this->spriteType = "barrier";
    written = 3;
}


//Creates Jose, Potion, and 1x2 and 1x4
Shape::Shape(int id, int rR, int cR, int dummy1) {
    root.R = rR;
    root.C = cR;
    end.R = rR;
    end.C = cR;
    

    this->id = id;

    if (id == 1) { //If id is 1, then it's jose
        end.C = cR + 1; //Jose is 1x2
        spriteType = "jose";
    }
    else if (id == 2) { //If id is 2, then it's potion
        end.C = cR + 1; //Potion is 1x2
        spriteType = "potion";
    }
    else if (id == 4) { //If id is 4, then it's a lamp
        end.C = cR + 1; //Lamp is 1x2
        spriteType = "lamp";
    }
    else if (id == 5) { //If id is 5, then it's a camel
        end.C = cR + 3; //Camel is 1x4
        spriteType = "camel";
    }

    written = ++instanceCounter;
}

//Creates weird shapes
Shape::Shape(int id, int rR, int rC, bool dummy2) {
    root.R = rR;
    root.C = rC;
    end.R = rR + 1;
    end.C = rC + 2;
    this->id = id; //Use id instead of type
    spriteType = "weird";
    instanceCounter++;

    written = ++instanceCounter;
}


//Collisions Functions
bool Shape::collisionNorth(int(&matrix)[5][9]) {
    if(matrix[(root.R)-1][root.C] != 0 || (root.R-1) < 0) {
        return false;
    }
    else if(matrix[root.R][(root.C)+1] == written) {
        if(matrix[(root.R)-1][(root.C)+1] != 0) {
            return false;
        }
    }
    return true; 
}
bool Shape::collisionEast(int(&matrix)[5][9]) {
    if(matrix[end.R][(end.C)+1] != 0 || (end.C + 1) > 8) {
        return false;
    }
    else if(matrix[(end.R)-1][(end.C)-1] == written) {
        if(matrix[(end.R)-1][end.C] != 0) {
            return false;
        }
    }
    return true;
}
bool Shape::collisionSouth(int(&matrix)[5][9]) {
    if(matrix[(end.R)+1][end.C] != 0 || (end.R+1) > 5) {
        return false;
    }
    else if((matrix[end.R][(end.C)-1] == written) && (matrix[end.R][(end.C)-2] == written) && (matrix[end.R][(end.C)-3] == written)) {
        if((matrix[(end.R)+1][(end.C)-1] != 0) || (matrix[(end.R)+1][(end.C)-2] != 0) || (matrix[(end.R)+1][(end.C)-3] != 0)) {
            return false;
        }
    }
    else if((matrix[end.R][(end.C)-1] == written) && (matrix[end.R][(end.C)-2] != written)) {
        if(matrix[(end.R)+1][(end.C)-1] != 0) {
            return false;
        }
    }
    else if((matrix[end.R][(end.C)-1] == written) && (matrix[end.R][(end.C)-2] != written) && (matrix[(end.R)-1][(end.C)-2] == written)) {
        if((matrix[(end.R)+1][(end.C)-1] != 0) && (matrix[end.R][(end.C)-2] != 0)) {
            return false;
        }
    }
    return true;
}

bool Shape::collisionWest(int(&matrix)[5][9]) {
    if (matrix[root.R][root.C - 1] != 0 || (root.C - 1) < 0) {
        return false;
    }
    else if (matrix[root.R + 1][root.C + 1] == written) {
        if (matrix[root.R + 1][root.C] != 0) {
            return false;
        }
    }
    return true;
}

//Move Functions
void Shape::moveWest(int(&matrix)[5][9]) {
    if (id != 6) {
        //Loop rows from root.R to end.R (inclusive)
        for (int i = root.R; i < end.R + 1; i++) {
            //Clear the index on the right of the shape
            matrix[i][end.C] = 0;
            //Move the shape one position to the left
            matrix[i][root.C - 1] = written;
        }
        //Update the root and end column positions
        root.C -= 1;
        end.C -= 1;
    }
    else if (id == 6) {
        matrix[root.R][root.C + 1] = 0;
        matrix[end.R][end.C] = 0;
        matrix[root.R][root.C - 1] = written;
        matrix[end.R][end.C - 2] = written;

        root.C -= 1;
        end.C -= 1;
    }
}

void Shape::moveNorth(int(&matrix)[5][9]) {
    if (id != 6) {
        //Loop columns from root.C to end.C (inclusive)
        for (int i = root.C; i < end.C + 1; i++) {
            //Clear the index below the shape
            matrix[end.R][i] = 0;
            //Move the shape one position upward
            matrix[root.R - 1][i] = written;
        }
        //Update the root and end row positions
        root.R -= 1;
        end.R -= 1;
    }
    else if (id == 6) {
        matrix[root.R][root.C] = 0;
        matrix[end.R][end.C - 1] = 0;
        matrix[end.R][end.C] = 0;
        matrix[root.R - 1][root.C] = written;
        matrix[root.R - 1][root.C + 1] = written;
        matrix[end.R - 1][end.C] = written;

        root.R -= 1;
        end.R -= 1;
    }
}

void Shape::moveEast(int(&matrix)[5][9]) {
    if (id != 6) {
        //Loop rows from root.R to end.R (inclusive)
        for (int i = root.R; i < end.R + 1; i++) {
            //Clear the index on the left of the shape
            matrix[i][root.C] = 0;
            //Move the shape one position to the right
            matrix[i][end.C + 1] = written;
        }
        //Update the root and end column positions
        root.C += 1;
        end.C += 1;
    }
    else if (id == 6) {
        matrix[root.R][root.C] = 0;
        matrix[end.R][end.C - 1] = 0;
        matrix[root.R][root.C + 2] = written;
        matrix[end.R][end.C + 1] = written;

        root.C += 1;
        end.C += 1;
    }
}

void Shape::moveSouth(int(&matrix)[5][9]) {
    if (id != 6) {
        //Loop columns from root.C to end.C (inclusive)
        for (int i = root.C; i < end.C + 1; i++) {
            //Clear the inxed above the shape
            matrix[root.R][i] = 0;
            //Move the shape one position downward
            matrix[end.R + 1][i] = written;
        }
        //Update the root and end row positions
        root.R += 1;
        end.R += 1;
    }
    else if (id == 6) {
        matrix[root.R][root.C] = 0;
        matrix[root.R][root.C + 1] = 0;
        matrix[end.R][end.C] = 0;
        matrix[root.R + 1][root.C] = written;
        matrix[end.R + 1][end.C] = written;
        matrix[end.R + 1][end.C - 1] = written;

        root.R -= 1;
        end.R -= 1;
    }
}




/*
///////////////////////////////////////////////////////////////////////// IMPORTANT //////////////////////////////////////////////////////////////////////////////////////////
Insert functions will all perform a index check if index is !=0. That ensures that a shape won't overwrite another one already in the matrix. Function fails is so. 
Insert functions will all perform a bound test, checking the root of each shape is beyond the far edges of the matrix. Really important for the randomDomain only.
Insert functions will all have member "written" be "id * 10 + instanceCounter", this ensures that shapes with the same "id" are written with different numbers in the matrix.
    Otherwise, it would be hard to distinguish where one thing ends and the other start for the collison checks.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/
bool Shape::insert(int(&matrix)[5][9]) {
    //Check if the shape fits within the matrix bounds
    if (this->end.R >= 5 || this->end.C >= 9) {
        return false;
    }

    //Write the shape into the matrix
    for (int j = this->root.C; j <= this->end.C; j++) {
        if (matrix[this->root.R][j] != 0) { 
            return false;
        }
        matrix[this->root.R][j] = written;
    }

    return true;
}

bool Shape::insertWeird(int(&matrix)[5][9]) {
    //Check if the shape fits within the matrix bounds
    if (this->end.R >= 5 || this->end.C >= 9) {
        return false;
    }

    //Write the shape into the matrix
    for (int j = this->root.C; j <= this->root.C + 1; ++j) { //row 0 from column n to n+1
        if (matrix[this->root.R][j] != 0) {
            return false;
        }
        matrix[this->root.R][j] = written;
    }

    for (int j = this->root.C + 1; j <= this->end.C; ++j) { //row 1 from column n+1 to n+2
        if (matrix[this->root.R + 1][j] != 0) {
            return false;
        }
        matrix[this->root.R + 1][j] = written;
    }


    return true;
}

bool Shape::insertBarrier(int(&matrix)[5][9]) {
    //Check if the shape fits within the matrix bounds
    if (this->root.R >= 5 || this->root.C >= 9) {
        return false;
    }

    //Write the barrier into the matrix
    if (matrix[this->root.R][this->root.C] != 0) {
        return false;
    }

    matrix[this->root.R][this->root.C] = written; //Writes to matrix

    return true;
}