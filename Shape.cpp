#include "Shape.h"
#include <vector>

int Shape::instanceCounter = 20;

//Null constructor
Shape::Shape() {
    id = 0;
    written = 0;
    spriteType = "";
    instanceCounter++;
}

//Creates shapes and set their sprites in a different way based on their ID.
Shape::Shape(int id) {
    this->id = id;
    if (id == 1) { //If id is 1, then it's jose.
        spriteType = "jose";
    }
    else if (id == 2) { //If id is 2, then it's potion.
        spriteType = "potion";
    }
    else if (id == 3) { //If id is 3, then it's barrier.
        spriteType = "barrier";
    }
    else if (id == 4) { //If id is 4, then it's a lamp.
        spriteType = "lamp";
    }
    else if (id == 5) { //If id is 5, then it's a camel.
        spriteType = "camel";
    }
    else if (id == 6) { //If id is 6, then it's weird.
        spriteType = "weird";
    }

    written = ++instanceCounter;
}