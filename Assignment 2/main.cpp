//
//  main.cpp
//  Assignment 2
//
//  Created by Simarpreet Singh (1216728) on 2014-11-02.
//  Copyright (c) 2014 Simarpreet Singh. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <list>

#include "point.h"
#include "particle.h"

using namespace std;

const int ON = 1;
const int OFF = 0;

float particleOrigin[3] = {-10, 10, -10};
float cameraPosition[3] = {10, 20, 0};

float flatQuadSize = 10;
float g = 0.1; //gravity
float particleSize = 0.2;
float particleLifeSpanMin = 200;

int friction = ON;
int simulation = ON;
int particleCameraMode = OFF;
int particleTrailMode = OFF;

float numOfParticlesPerUpdateMethod = 0.1;
float particleRenderCompletion = 0;

particle * cameraParticle = NULL;


list<particle *> listOfParticles;

/*
 getRandomParticle - returns back a pointer to a random particle from the list of particles
 */
particle * getRandomParticle(){
    if(!listOfParticles.empty()){
        list<particle *>::iterator i = listOfParticles.begin();
        for(int y; y<rand()%(listOfParticles.size()); y++){
            i++;
        }
        return *i;
    }
    else{
        return NULL;
    }
}


float randomNumExceptZero(int max){
    int output = (int)(rand() % (int)(2));
    float result = (rand()%max)+0.5;
    if(particleOrigin[0]>-1*flatQuadSize+max && particleOrigin[0]<flatQuadSize-max){
        if(particleOrigin[2]>-1*flatQuadSize+max && particleOrigin[2]<flatQuadSize-max){
                if(output == 1){
                    result*=-1;
                }
        }
    }
    return result;
}

/*
 pushParticleToListOfParticles - pushes back a new particle to the list of particles
 */
void pushParticleToListOfParticles(){
    listOfParticles.push_back(new particle(
                                           new point(particleOrigin[0], particleOrigin[1], particleOrigin[2]),
                                           new point(randomNumExceptZero(3), 0, randomNumExceptZero(3)),
                                           particleSize,
                                           (rand()%1000)+particleLifeSpanMin
                                           )
                              );
}

/*
 fillListOfParticles - takes a number and calls pushParticleToListOfParticles() that many times in a for loop
 */
void fillListOfParticles(int num){
    for(int i = 0; i<num; i++){
        pushParticleToListOfParticles();
    }
}

/*
 renerParticle - renders a given pointer of a particle as a sphere
 */
void renderParticle(particle * p){
    glPushMatrix(); // push back the matrix
    float * pPosition = p->getPosition(); // get particle position array
    int * pRGBColor = p->getRGBColor(); // get particle color array
    int * pRotationAngles = p->getRotationAngles(); // get particle rotation angle array
    float pSize = p->getSize(); // get particle size
    glTranslatef(pPosition[0], pPosition[1], pPosition[2]); //translate to the corrdinates of particle position
    glColor3b(pRGBColor[0], pRGBColor[1], pRGBColor[2]); // set the color to particle color
    glRotatef(pRotationAngles[0], 1, 0, 0); //rotation angle in x axis
    glRotatef(pRotationAngles[1], 0, 1, 0); //rotation angle in y axis
    glRotatef(pRotationAngles[2], 0, 0, 1); //rotation angle in z axis
    glutSolidSphere(pSize, 16, 16); //draw a solid sphere with radius as the particle size
    glPopMatrix(); // pop the matrix
}

/*
 renderListofParticles - iterates through the list of particles and called the renderParticle method on each of them.
                        if the particle trail mode is on it calls the renderParticle method on each of trailing particle of each particle aswell. 
 */
void renderListOfParticles(){
    for(list<particle *>::iterator i = listOfParticles.begin(); i != listOfParticles.end(); ++i){
        renderParticle(*i);
        if(particleTrailMode==ON){
            for(list<particle *>::iterator j = (*i)->trailingParticles.begin(); j != (*i)->trailingParticles.end(); ++j){
                renderParticle(*j);
            }
        }
    }
}

/*
 updateMethod  - called every 16 milliseconds,  meant to update the animation 
                 and add more particles to the list of particles
 */
void updateMethod(int value){
    if(simulation==ON){ //if the simulation is on
        particleRenderCompletion+=numOfParticlesPerUpdateMethod; //increment particleRenderCompletion by numOfParticlesPerMethod
        if(particleRenderCompletion>1){//if particleRenderCompletion is more than 1
            fillListOfParticles((int)particleRenderCompletion); //call fillListOfParticles function with the particleRenderCompletion as the arguement
            particleRenderCompletion = 0; //set particle RenderCompletion to zero
        }
    }
    //iterate through list of particles and call the move function of the particle has no expired else remove it from the list
    for(list<particle *>::iterator i = listOfParticles.begin(); i != listOfParticles.end(); ++i){
        particle * currentParticle = *i;
        if(!currentParticle->isExpired(flatQuadSize)){
            currentParticle->move(g, flatQuadSize, friction);
        }
        else{
            listOfParticles.erase(i);
        }
    }
    glutTimerFunc(16, updateMethod, 0); //call updateMethod after 16 seconds
    glutPostRedisplay(); //call display function
}

/*
 drawFlatQuad - meant to be called from the display method, draws the flat plane using GL_QUADS
 */
void drawFlatQuad(){
    glColor3f(0, 3, 0); //set the color
    glBegin(GL_QUADS); //GL_QUADS begin, draw a flat square plane of flatQuadSize size.
        glVertex3f(-1*flatQuadSize, 0, flatQuadSize);
        glVertex3f(flatQuadSize, 0, flatQuadSize);
        glVertex3f(flatQuadSize, 0, -1*flatQuadSize);
        glVertex3f(-1*flatQuadSize, 0, -1*flatQuadSize);
    glEnd(); //gl end
}

/*
 keyboard - meant to be a call back function to handle the cases of keys being press to controll the particle simulation,
           also used to enable/disable features.
 */
void keyboard(unsigned char key, int x, int y){
    switch (key){
        case 'q':
        case 27:
            printf("Exiting Particle System...\n");
            exit (0);
            break;
        case 'F':
        case 'f':
            if(friction==ON){
                friction=OFF;
                printf("Friction: OFF\n");
            }
            else{
                friction=ON;
                printf("Friction: ON\n");
            }
            break;
        case ' ':
            if(simulation==ON){
                simulation=OFF;
                printf("Simulation: OFF\n");
            }
            else{
                simulation=ON;
                printf("Simulation: ON\n");
            }
            break;
        case 'r':
            simulation = ON;
            particleCameraMode = OFF;
            printf("Simulation: ON\n");
            printf("Sumulation Restarted\n");
            listOfParticles.clear();
            break;
        case 'c':
            if(particleCameraMode==OFF){
                particleCameraMode = ON;
                printf("Particle Cam Mode: ON\n");
                list<particle *>::iterator i = listOfParticles.end(); //iterator of list set to the end of list of particles
                i--;
                if(*i){
                    cameraParticle = *i; // make it the camera particle
                }
            }
            break;
        case 't':
            if(particleTrailMode==ON){
                printf("Particle Trail: OFF\n");
                particleTrailMode = OFF;
            }
            else{
                printf("Particle Trail: ON\n");
                particleTrailMode=ON;
            }
            break;
            
    }
    glutPostRedisplay(); // call the display function
}

/*
 special - meant to be a call back function to handle the cases of arrows being press to controll the camera
 */
void special(int key, int x, int y){
    /* arrow key presses move the camera */
    switch(key){
        case GLUT_KEY_LEFT:
            cameraPosition[2]-= 1;
            break;
            
        case GLUT_KEY_RIGHT:
            cameraPosition[2]+= 1;
            break;
            
        case GLUT_KEY_UP:
            cameraPosition[1] -= 1;
            break;
            
        case GLUT_KEY_DOWN:
            cameraPosition[1] += 1;
            break;
    }
    glutPostRedisplay(); //call the display function
}

/*
 display - where all the rendering is performed, frame buffers are swapped
 */
void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); //change matrix mode to model view
    glLoadIdentity(); //load identity matrix
    
    if(particleCameraMode==OFF){ //if the particle camera mode is off
        gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
                  0, 0, 0,
                  0, 1, 0); // look at the origin from cameraPosition
    }
    else{ //else - the particle camera mode is on
        if(cameraParticle){ //if cameraParticle is not null
            float * cameraParticlePosition = cameraParticle->getPosition(); //get the camera particle position
            gluLookAt(cameraParticlePosition[0], cameraParticlePosition[1], cameraParticlePosition[2],
                      particleOrigin[0], particleOrigin[1], particleOrigin[2],
                      0, 1, 0); // look at the particle origin from the camera particle position
            if(cameraParticle->isExpired(flatQuadSize)){ // if the camera particle expires
                particleCameraMode = OFF; //set the particleCameraMode to off
                printf("Particle Cam Mode: OFF\n"); //print this
            }
        }
    }

    //lighting settings
    float m_amb[] = {0.33, 0.33, 0.33, 1.0};
    float m_dif[] = {0.78, 0.57, 0.11, 1.0};
    float m_spec[] = {0.99, 0.91, 0.81, 1.0};
    float shiny = 27;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_dif);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    
    drawFlatQuad(); //draw the flat plane
    renderListOfParticles(); // draw all the particles in the list of particles
    
    glutSwapBuffers(); //swap the frame buffers
}
/*
 init - procedure sets up the lighting and the projection
 */
void init(){
    glEnable(GL_COLOR_MATERIAL); //enable color material
    glEnable(GL_LIGHTING); //enable lighting
    glEnable(GL_LIGHT0); //enable light
    
    float position[4] = {1.5, 0, 0, 0};
    
    float amb[4] = {1.0, 1, 1, 1};
    float diff[4] = {0,0,0, 1};
    float spec[4] = {0,0,1, 1};
    
    glLightfv(GL_LIGHT1, GL_POSITION, position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diff);
    glLightfv(GL_LIGHT1, GL_AMBIENT, amb);
    glLightfv(GL_LIGHT1, GL_SPECULAR, spec);
    
    glMatrixMode(GL_PROJECTION); //change matrix mode to gl projection
    glLoadIdentity(); //load an identity matrix
    gluPerspective(80, 1, 1, 100); //set up a perspective projection
}

/*
 main - where the program starts off, commands are printed off, glut is set up, call back functions
        are assigned.
 */
int main(int argc, char** argv){
    //print out the welcome message and the commangs
    printf("=======================================================\n");
    printf("Welcome To Particle System By Simarpreet Singh(1216728)\n");
    printf("=======================================================\n");
    printf("\n");
    printf("       CONTROLS    \n");
    printf("+----------------------------+\n");
    printf("|  f  | Toggle Friction      |\n");
    printf("|-----+----------------------|\n");
    printf("|  r  | Restart Simulation   |\n");
    printf("|-----+----------------------|\n");
    printf("|space| Start/Stop Simulation|\n");
    printf("| bar |                      |\n");
    printf("|-----+----------------------|\n");
    printf("|  c  | Particle Camera Mode |\n");
    printf("|-----+----------------------|\n");
    printf("|  t  | Show Particle Trails |\n");
    printf("|-----+----------------------|\n");
    printf("|< ^ >| Control The Camera   |\n");
    printf("|-----+----------------------|\n");
    printf("|  q  | Quit Particle System |\n");
    printf("+----------------------------+\n");
    printf("\n");
    printf("Please look here for feedback while using the particle system:\n");
    
    glutInit(&argc, argv); //starts up GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //initiate glut display mode
    glutInitWindowSize(800, 800); //set the window size, 800 by 800
    glutInitWindowPosition(100, 100); //position the window at 100, 100
    glutCreateWindow("Particle System"); //creates the window with the given title
    glutDisplayFunc(display); //registers "display" as the display callback function
    glutKeyboardFunc(keyboard); //registers "keyboard" as the keyboard callback function
    glutSpecialFunc(special); //registers "special" as the special callback function
    
    glClearColor(1, 1, 1, 1.0f); //set the clear color to white
    
    glEnable(GL_DEPTH_TEST); //enable gl depth test
    
    init(); //call the init procedure
    
    glutTimerFunc(16, updateMethod, 0); // call the updateMethod after 16 milliseconds
    
    glutMainLoop();	//starts the main loop
    
    return(0); //return may not be necessary on all compilers
}