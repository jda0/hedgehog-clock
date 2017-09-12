/*
 *    DualStepper.h - Library for controlling 228BYJ-48 stepper Motors using the esp8266
 *    Created by The Makron, April 16th, 2016
 *
 */

#include "espDualStepper.h"

DualStepper Motor;
void stepperISR(void *) {
    Motor.motorISR();
}

//this sets up the left and right Motor outputs and timer1 for the pulse duration of the stepper Motors
void DualStepper::start(int leftMotor1,int leftMotor2 ,int leftMotor3 ,int leftMotor4 ,int rightMotor1 ,int rightMotor2 ,int rightMotor3 ,int rightMotor4){
    //Sets user defined left Motor pins as outputs
    pinMode(leftMotor1,OUTPUT);
    pinMode(leftMotor2,OUTPUT);
    pinMode(leftMotor3,OUTPUT);
    pinMode(leftMotor4,OUTPUT);
    //Sets user defined right Motor pins as outputs
    pinMode(rightMotor1,OUTPUT);
    pinMode(rightMotor2,OUTPUT);
    pinMode(rightMotor3,OUTPUT);
    pinMode(rightMotor4,OUTPUT);

    //passes Motor pin numbers to private
    _leftMotor1=leftMotor1;
    _leftMotor2=leftMotor2;
    _leftMotor3=leftMotor3;
    _leftMotor4=leftMotor4;
    _rightMotor1=rightMotor1;
    _rightMotor2=rightMotor2;
    _rightMotor3=rightMotor3;
    _rightMotor4=rightMotor4;

    digitalWrite(_leftMotor1, LOW);
    digitalWrite(_leftMotor2, LOW);
    digitalWrite(_leftMotor3, LOW);
    digitalWrite(_leftMotor4, LOW);
    digitalWrite(_rightMotor1, LOW);
    digitalWrite(_rightMotor2, LOW);
    digitalWrite(_rightMotor3, LOW);
    digitalWrite(_rightMotor4, LOW);

    os_timer_setfn(&this->stepperISRTimer, &stepperISR, NULL);
    os_timer_arm(&this->stepperISRTimer, 2, true);
}


void DualStepper::motorISR(){

    if ((_leftMotorStateIndex <= 3) && (_lhMotorDir == -1)) {        //if the left Motor spins counter clockwise this resets the left Motor indexer (_leftMotorStateIndex) to 31
        _leftMotorStateIndex = 31;
    }
    else if ((_leftMotorStateIndex > 3) && (_lhMotorDir == -1)) {    //_leftMotorStateIndex decrments through state in steps of 4 so that the left stepper Motor can use the correct output sequence
        _leftMotorStateIndex -= 4;
    }
    else if ((_leftMotorStateIndex >= 31) && (_lhMotorDir == 1)) {    //if the left Motor spins  clockwise this resets the left Motor indexer (_leftMotorStateIndex) to 3
        _leftMotorStateIndex = 3;
    }
    else if ((_leftMotorStateIndex < 31) && (_lhMotorDir == 1)) {
        _leftMotorStateIndex += 4;
    }

    if ((_rightMotorStateIndex <= 3) && (_rhMotorDir == -1)) {
        _rightMotorStateIndex = 31;
    }
    else if ((_rightMotorStateIndex > 3) && (_rhMotorDir == -1)) {
        _rightMotorStateIndex -= 4;
    }
    else if ((_rightMotorStateIndex >= 31) && (_rhMotorDir == 1)) {
        _rightMotorStateIndex = 3;
    }
    else if ((_rightMotorStateIndex < 31) && (_rhMotorDir == 1)) {
        _rightMotorStateIndex += 4;
    }

    if(_lhMotorDir != 0){
        digitalWrite(_leftMotor1, _state[_leftMotorStateIndex - 3]);
        digitalWrite(_leftMotor2, _state[_leftMotorStateIndex - 2]);
        digitalWrite(_leftMotor3, _state[_leftMotorStateIndex - 1]);
        digitalWrite(_leftMotor4, _state[_leftMotorStateIndex]);
    }
    else{
        digitalWrite(_leftMotor1, LOW);
        digitalWrite(_leftMotor2, LOW);
        digitalWrite(_leftMotor3, LOW);
        digitalWrite(_leftMotor4, LOW);
    }

    if(_rhMotorDir != 0){
        digitalWrite(_rightMotor1, _state[_rightMotorStateIndex - 3]);
        digitalWrite(_rightMotor2, _state[_rightMotorStateIndex - 2]);
        digitalWrite(_rightMotor3, _state[_rightMotorStateIndex - 1]);
        digitalWrite(_rightMotor4, _state[_rightMotorStateIndex]);
    }
    else{
        digitalWrite(_rightMotor1, LOW);
        digitalWrite(_rightMotor2, LOW);
        digitalWrite(_rightMotor3, LOW);
        digitalWrite(_rightMotor4, LOW);
    }

}

void DualStepper::stop(){
    _lhMotorDir = 0;
    _rhMotorDir = 0;
}

void DualStepper::forward(){
    _lhMotorDir = 1;
    _rhMotorDir = -1;
}

void DualStepper::reverse(){
    _lhMotorDir = -1;
    _rhMotorDir = 1;

}
void DualStepper::left(){
    _lhMotorDir = 1;
    _rhMotorDir = 1;
}
void DualStepper::right(){
    _lhMotorDir = -1;
    _rhMotorDir = -1;
}
void DualStepper::pitchLeft(){
    _lhMotorDir = 0;
    _rhMotorDir = -1;

}
void DualStepper::pitchRight(){
    _lhMotorDir = 1;
    _rhMotorDir = 0;
}
