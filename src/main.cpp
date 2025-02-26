#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    updateselectedauton();
    colorsensor.set_led_pwm(100);
    pros::lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors
    //Graphics::renderLoop();
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
        if (!pros::competition::is_connected()){
            updateselectedauton();
        }
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

    if(controller.get_digital(DIGITAL_R2)){intakefunc(3,600);armstate(2);}
    else if(controller.get_digital(DIGITAL_R1)){intakefunc(3,600);armstate(3);}
    else if(controller.get_digital(DIGITAL_A)){intakefunc(3,-600);}
    else {intake.brake();raiser.brake();}
    if (controller.get_digital_new_press(DIGITAL_L1)){armstate(1);}
    if (controller.get_digital_new_press(DIGITAL_Y)){armstate(4);}
    if (controller.get_digital_new_press(DIGITAL_L2)){mogoclamp.toggle();}
    if ((colorsensor.get_hue() > 210) && (colorsensor.get_hue() < 250)){colorsorter();}
    //if (controller.get_digital_new_press(DIGITAL_UP)){descore.toggle();}
    //if (skillsrun == true){intakefunc(2,127);}
    
    }
}

