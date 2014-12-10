//
//  particle.h
//  Assignment 2
//
//  Created by Simarpreet Singh on 2014-11-02.
//  Copyright (c) 2014 Simarpreet Singh. All rights reserved.
//

#ifndef __Assignment_2__particle__
#define __Assignment_2__particle__

#include <stdio.h>
#include <list>
#include "point.h"

using namespace std;

class particle{
public:
    point * position; //current particle position
    point * direction; //particle movement direction
    float speed; //particle movement speed
    int * rotation; //particle rotation angle
    float size; //size of the particle
    float age; //particle life span
    int * rgb_color; //rgb color array
    float lifespan; //lifespan of a particle
    list<particle *> trailingParticles; //list of trailing particles
    
    //Different types of constructors
    particle(point * position, float size); //meant for trailing particle no direction - don't move
    particle(point * position, point * direction, float size, float lifespan);
    particle(point * position, point * direction, float speed, float size, float lifespan);
    particle(point * position, point * direction, float speed, float size, float lifespan, int * color);
    
    //Utility Methods
    int * generateRandomRGB(); //returns a int array of size 3 with randomly generated int values from 0 to 255
    int * generateRandomRotationAngles(); //returns a int array of size 3 with randomly generated int values from 0 to 360
    
    //Accessor Methods for Particle
    float * getPosition(); // returns a pointer to float array with the position of the particle
    int * getRGBColor(); // returns a pointer to int array with the rgb color values of the particle
    int * getRotationAngles(); // returns a pointer int array with the rotation angles fo the particle
    float getSize(); // return a float size of the particle
    
    /*
     move - updates the position of the particle, updates the direction of the particle, rotation array 
            and increments the age.
     */
    void move(float gravity, float floorSize, int friction);
    
    /*
     incrementAge - increments the age by 1
     */
    void incrementAge();
    
    /*
     isExpired - returns a bool, checks if a particle is expired with the given floorsize
     */
    bool isExpired(float floorSize);
    
    /*
     isTouchingFloor - returns bool, checks if the a particle is touching the flat plane, given the floor size
     */
    bool isTouchingFloor(float floorSize);
    
    /*
     invertDirection - inverts the direction vector
     */
    void invertDirection(int friction);
    
    /*
     addTrailingParticle - adds a new particle to the listOfTrailingParticle at the current particle position
     */
    void addTrailingParticle();
    
};

#endif /* defined(__Assignment_2__particle__) */
