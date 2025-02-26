#pragma once

#include "api.h"
#include "lemlib/api.hpp"


//motors
inline pros::Motor intake(6,pros::MotorCartridge::blue);
inline pros::Motor raiser(1,pros::MotorCartridge::green);
inline pros::Motor arm(2,pros::MotorCartridge::green);
inline pros::MotorGroup leftMotors({-4,-3,5}, pros::MotorGearset::blue); // left motor group drivetrain
inline pros::MotorGroup rightMotors({7,-8,9}, pros::MotorGearset::blue); // right motor group drivetrain

//sensors
inline pros::Rotation armrotation(10);
inline pros::Rotation autonslectorcornersorsmth(17);
inline pros::Rotation autonslectoractualautonyk(17);
inline pros::Rotation horizontalEnc(19); //vert encoder for odometry
//inline pros::Rotation verticalEnc(-17); //vert encoder for odometry
inline pros::Optical colorsensor(20);
inline pros::Imu imu(21); //inertial
          
//pneumatics
inline pros::adi::Pneumatics mogoclamp('A',false);

//controller
inline pros::Controller controller(pros::E_CONTROLLER_MASTER);


//PIDS
inline lemlib::PID armpid(.01, // kP
        .001, // kI
        20, // kD
        5, // integral anti windup range
        false); // don't reset integral when sign of error flips

//odomentry
inline lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_2, -1.4);// horizontal tracking wheel. 2" diameter, 1" offset, back of the robot (negative)
//inline lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_2, -.3);// vertical tracking wheel. 2" diameter, 0" offset, left of the robot (negative)

// drivetrain settings
inline lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              11.5, // 11.5 inch track width
                              lemlib::Omniwheel::NEW_275,
                              450, // drivetrain rpm is 360
                              8 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// lateral motion controller
inline lemlib::ControllerSettings linearController(10, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            3, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            20 // maximum acceleration (slew)
);

// angular motion controller
inline lemlib::ControllerSettings angularController(2, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             10, // derivative gain (kD)
                                             3, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);


// sensors for odometry
inline lemlib::OdomSensors sensors(nullptr  , // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            &horizontal, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// input curve for throttle input during driver control
inline lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
inline lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

inline lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);
