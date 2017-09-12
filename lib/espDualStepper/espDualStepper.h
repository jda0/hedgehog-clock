#ifndef espDualStepper_h
#define espDualStepper_h
#include "Arduino.h"

extern "C" {
#include "user_interface.h"
}

class DualStepper {
    public:
        void start(int leftMotor1,int leftMotor2 ,int leftMotor3 ,int leftMotor4 ,int rightMotor1 ,int rightMotor2 ,int rightMotor3 ,int rightMotor4);
        void motorISR();
        void stop();
        void forward();
        void reverse();
        void left();
        void right();
        void pitchLeft();
        void pitchRight();

    private:
        os_timer_t stepperISRTimer;
        int _lhMotorDir = 0;//1=clockwise -1=counterclockwise
        int _rhMotorDir = 0;//1=clockwise -1=counterclockwise
        int _leftMotor1;
        int _leftMotor2;
        int _leftMotor3;
        int _leftMotor4;
        int _rightMotor1;
        int _rightMotor2;
        int _rightMotor3;
        int _rightMotor4;

        char _state[32] = {
            1, 0, 0, 0,
            1, 1, 0, 0,
            0, 1, 0, 0,
            0, 1, 1, 0,
            0, 0, 1, 0,
            0, 0, 1, 1,
            0, 0, 0, 1,
            1, 0, 0, 1
        };
        int _leftMotorStateIndex = 4; //index increments through state for the lefthand Motor
        int _rightMotorStateIndex = 4; //index increments through state for the righthand Motor
};


extern DualStepper Motor;

#endif
