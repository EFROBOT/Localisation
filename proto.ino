#include <Arduino.h>
#include "A4988.h"
#include "MultiDriver.h"

// robot 
const float L = ;          // distance centre - axe 
const float LAMBDA = ;     // largeur centre - axe
const float R = ;          // rayon des roues

// Nema 14 
const int STEPS_PER_REV = 200;   // 360 / 1.8 
const int MICROSTEPPING = 16;    // A4988
const float NB_PAS_PAR_METRE = (STEPS_PER_REV * MICROSTEPPING) / (2 * PI * R);

A4988 step_1(STEPS_PER_REV, , ); // haut gauche
A4988 step_2(STEPS_PER_REV, , ); // haut droite
A4988 step_3(STEPS_PER_REV, , ); // bas gauche
A4988 step_4(STEPS_PER_REV, , ); // bas droite


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
    step_1.move(steps1);
    step_2.move(steps2);
    step_3.move(steps3);
    step_4.move(steps4);

    while(step_1.getStepsRemaining() || step_2.getStepsRemaining() || step_3.getStepsRemaining() || step_4.getStepsRemaining()){
        step_1.nextAction();
        step_2.nextAction();
        step_3.nextAction();
        step_4.nextAction();
    }
}


void avancer(float distance){
    long steps = calcul_steps(distance);
    sync_4_driver(steps, steps, steps, steps);
}

void reculer(float distance){
    long steps = calcul_steps(distance);
    sync_4_driver(-steps, -steps, -steps, -steps);
}

void gauche(float distance){
    long steps = calcul_steps(distance);
    sync_4_driver(-steps, steps, steps, -steps);
}

void droite(float distance){
    long steps = calcul_steps(distance);
    sync_4_driver(steps, -steps, -steps, steps);
}

void loop() {

    avancer(1);
    delay(200);

    reculer(1);
    delay(200);

    gauche(1);
    delay(200);

    droite(1);
    delay(200);
}