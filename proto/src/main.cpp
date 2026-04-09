#include <Arduino.h>
#include "A4988.h"
#include "MultiDriver.h"

// robot 
const float L = 0.0115;        // distance centre - axe 
const float LAMBDA = 0.0115;   // largeur centre - axe
const float R = 0.024;         // rayon des roues

// Nema 14 
const int STEPS_PER_REV = 200; 
const int MICROSTEPPING = 16;    // A4988
const float NB_PAS_PAR_METRE = (STEPS_PER_REV * MICROSTEPPING) / (2 * PI * R);

// Position initial 
float x_robot = 0.0;
float y_robot = 0.0;
float angle_robot = 0.0;

A4988 step_1(STEPS_PER_REV, D2, D3); // haut gauche
A4988 step_2(STEPS_PER_REV, D6, D7); // haut droite
A4988 step_3(STEPS_PER_REV, D8, D9); // bas gauche
A4988 step_4(STEPS_PER_REV, D10, D11); // bas droite


void setup(){
    step_1.begin(100, MICROSTEPPING);
    step_2.begin(100, MICROSTEPPING);
    step_3.begin(100, MICROSTEPPING);
    step_4.begin(100, MICROSTEPPING);

    /*
     * Set speed profile - CONSTANT_SPEED, LINEAR_SPEED (accelerated)
     * accel and decel are given in [full steps/s^2]
     */

    step_1.setSpeedProfile(step_1.LINEAR_SPEED, 1000, 1000);
    step_2.setSpeedProfile(step_2.LINEAR_SPEED, 1000, 1000);
    step_3.setSpeedProfile(step_3.LINEAR_SPEED, 1000, 1000);
    step_4.setSpeedProfile(step_4.LINEAR_SPEED, 1000, 1000);
}


int calcul_steps(float distance){
    long steps = distance * NB_PAS_PAR_METRE;
    return steps;
}

/*
    * Function move : 
    * Rotate the motor a given number of degrees (1-360).
*/
void sync_4_driver(long steps1, long steps2, long steps3, long steps4){
    step_1.startMove(steps1);
    step_2.startMove(steps2);
    step_3.startMove(steps3);
    step_4.startMove(steps4);

    while(step_1.getStepsRemaining() || step_2.getStepsRemaining() || step_3.getStepsRemaining() || step_4.getStepsRemaining()){
        step_1.nextAction();
        step_2.nextAction();
        step_3.nextAction();
        step_4.nextAction();
    }
}


void avancer(float distance){
    long steps = calcul_steps(distance);
    sync_4_driver(steps, -steps, steps, -steps);
    x_robot += distance * cos(angle_robot);
    y_robot += distance * sin(angle_robot);
}

void reculer(float distance){
    long steps = calcul_steps(distance);
    sync_4_driver(-steps, steps, -steps, steps);
    x_robot -= distance * cos(angle_robot);
    y_robot -= distance * sin(angle_robot);
}

void gauche(float distance){
    long steps = calcul_steps(distance);
    sync_4_driver(-steps, -steps, steps, steps);
    x_robot -= distance * sin(angle_robot); 
    y_robot += distance * cos(angle_robot);
}

void droite(float distance){
    long steps = calcul_steps(distance);
    sync_4_driver(steps, steps, -steps, -steps);
    x_robot += distance * sin(angle_robot);
    y_robot -= distance * cos(angle_robot);
}

void rotation_droite(float angle){
    float arc = (angle / 360.0) * 2 * PI * R;
    long steps = calcul_steps(arc);
    sync_4_driver(steps, steps, steps, steps);
}

void rotation_gauche(float angle){
    float arc = (angle / 360.0) * 2 * PI * R;
    long steps = calcul_steps(arc);
    sync_4_driver(-steps, -steps, -steps, -steps);
}

void go_to_coord(float x, float y){

    // difference 
    float dx = x - x_robot;
    float dy = y - y_robot;
    float angle_cible = atan2(dy, dx);
    float dangle = angle_cible - angle_robot;

    // turn to the cible 
    float arc = (dangle / 360.0) * 2 * PI * R;
    long steps_for_rotate = calcul_steps(arc);
    if (dangle > 0.01) {
        sync_4_driver(-steps_for_rotate, -steps_for_rotate, -steps_for_rotate, -steps_for_rotate);
    } else if (dangle < -0.01) {
        sync_4_driver(steps_for_rotate, steps_for_rotate, steps_for_rotate, steps_for_rotate);
    }

    // move to cible 
    float distance = sqrt(dx * dx + dy * dy);
    long steps_for_move = calcul_steps(distance);
    sync_4_driver(steps_for_move, -steps_for_move, steps_for_move, -steps_for_move);

    //update robot coord 
    x_robot = x;
    y_robot = y;
    angle_robot = angle_cible;
}

void loop() {
    avancer(1);
    delay(600);

    reculer(1);
    delay(600);

    gauche(0.5);
    delay(600);

    droite(0.5);
    delay(600);
}

/*
void loop() {
    if (Serial.available() > 0) {
        char commande = Serial.read();

        switch (commande) {
            case 'z':
                avancer(0.1); 
                break;
            case 's':
                reculer(0.1);
                break;
            case 'q':
                gauche(0.1);
                break;
            case 'd':
                droite(0.1);
                break;
            case 'a':
                rotation_gauche(90.0);
                break;
            case 'e': 
                rotation_droite(90.0);
                break;
        }
    }
}
*/
