#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

enum Autons { TESTING, REDNEG, BLUENEG, REDPOS, BLUEPOS };
            //blue     red     blue     red     blue
Autons activeauton = TESTING;

lemlib::PID armpid(.01, // kP
        .001, // kI
        20, // kD
        5, // integral anti windup range
        false); // don't reset integral when sign of error flips

// controller
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// motor groups
pros::MotorGroup leftMotors({-10, 5, -9}, pros::MotorGearset::blue); // left motor group
pros::MotorGroup rightMotors({-1, 2, 12}, pros::MotorGearset::blue); // right motor group

pros::Motor intake(19,pros::MotorCartridge::blue);
pros::Motor raiser(21,pros::MotorCartridge::green);
pros::Motor arm(7,pros::MotorCartridge::green);

pros::Rotation armrotation(6);
pros::Optical colorsensor();

pros::adi::Pneumatics mogoclamp('A',false);

pros::Imu imu(4); // Inertial sensor

// tracking wheels
// horizontal tracking wheel encoder. Rotation sensor, port 20, not reversed
pros::Rotation horizontalEnc(20);
// vertical tracking wheel encoder. Rotation sensor, port 11, reversed
pros::Rotation verticalEnc(-15);

// horizontal tracking wheel. 2" diameter, 1" offset, back of the robot (negative)
lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_2, 2.5);

// vertical tracking wheel. 2" diameter, 0" offset, left of the robot (negative)
lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_2, -.3);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              11.5, // 11.5 inch track width
                              lemlib::Omniwheel::NEW_275, // using new 4" omnis
                              450, // drivetrain rpm is 360
                              8 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// lateral motion controller
lemlib::ControllerSettings linearController(10, // proportional gain (kP)
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
lemlib::ControllerSettings angularController(2, // proportional gain (kP)
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
lemlib::OdomSensors sensors(&vertical, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            &horizontal, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors
    
    // the default rate is 50. however, if you need to change the rate, you
    // can do the following.
    // lemlib::bufferedStdout().setRate(...);
    // If you use bluetooth or a wired connection, you will want to have a rate of 10ms

    // for more information on how the formatting for the loggers
    // works, refer to the fmtlib docs

    // thread to for brain screen and position logging
    pros::Task screenTask([&]() {
        while (true) {
            // print robot location to the brain screen
            pros::lcd::print(0, "X: %f", chassis.getPose().x); // x
            pros::lcd::print(1, "Y: %f", chassis.getPose().y); // y
            pros::lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            // log position telemetry
            lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
            // delay to save resources
            pros::delay(50);
        }
    });
    controller.rumble(".");
}


    


void updateselectedauton() {
    pros::delay(2000);
    while (true) {
        // auton selector with rotation sensor code goes here
    }
}
pros::Task Autonselector_Task(updateselectedauton);

int getselectedauton(){
    if (evenorodd(activeauton)==1){
        return 1;
    } else {
        return 2;
    }
}

void runselectedauton() {
    switch (activeauton) {
        case TESTING: testauton(); // test auton
        case REDNEG: redneg(); // red negative
        case BLUENEG: blueneg(); // blue negative
        case REDPOS: redpos(); // red positive
        case BLUEPOS: bluepos(); // blue positive
    }
}

void testauton() {
    // Move to x: 20 and y: 15, and face heading 90. Timeout set to 4000 ms
    chassis.moveToPose(20, 15, 90, 4000);
    
}

void redneg() {}

void blueneg() {}

void redpos() {}

void bluepos() {}

void arm_task() {
  pros::delay(2000);
  while (true) {
    float speed = armpid.update(getcurrentstatepos() - armrotation.get_position());
    arm.move(speed);
  }
}
pros::Task Lift_Task(arm_task);  // Create the task, this will cause the function to start running
/** 
 * Runs while the robot is disabled
 */
void disabled() {}

/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {}

// get a path used for pure pursuit
// this needs to be put outside a function
ASSET(example_txt); // '.' replaced with "_" to make c++ happy


void autonomous() {runselectedauton();}

void opcontrol() {
    
    
    // controller
    // loop to continuously update motors
    while (true) {
        // get joystick positions
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightX = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
        // move the chassis with curvature drive
        chassis.arcade(leftY, rightX);
        // delay to save resources
        pros::delay(10);
    if (controller.get_digital(DIGITAL_B) && controller.get_digital(DIGITAL_DOWN)) {
        autonomous();
      } 

    if(controller.get_digital(DIGITAL_R2)){
      intake.move(127);
      raiser.move(127);
      armstate(2);
    }
    else if(controller.get_digital(DIGITAL_R1)){
      intake.move(127);
      raiser.move(127);
      armstate(3);
    }
    else if(controller.get_digital(DIGITAL_A)){
      intake.move(-127);
      raiser.move(-127);
    }
    else {
      intake.move(0);
      raiser.move(0);
    }
    if (controller.get_digital_new_press(DIGITAL_L1)){armstate(1);}
    if (controller.get_digital_new_press(DIGITAL_Y)){armstate(4);}
    if (controller.get_digital_new_press(DIGITAL_L2)){mogoclamp.toggle();}
    }
}

