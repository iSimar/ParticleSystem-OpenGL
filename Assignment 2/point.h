//
//  point.h
//  Assignment 2
//
//  Created by Simar Singh on 2014-11-02.
//  Copyright (c) 2014 Simar Singh. All rights reserved.
//

#ifndef __Assignment_2__point__
#define __Assignment_2__point__

#include <stdio.h>

class point{
public:
    float x, y, z; //float variables x y and z
    
    //point constructors
    point(float x, float y, float z); //with the given x, y, z
    point(); //sets the x, y, z to 0
};

#endif /* defined(__Assignment_2__point__) */
