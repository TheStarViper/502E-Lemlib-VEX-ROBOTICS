#include "funcs.hpp"
/*
 ____          __  _         _                             _            __   __
|  _ \   ___  / _|(_) _ __  (_) _ __    __ _          ___ | |_  _   _  / _| / _|
| | | | / _ \| |_ | || '_ \ | || '_ \  / _` |        / __|| __|| | | || |_ | |_
| |_| ||  __/|  _|| || | | || || | | || (_| |        \__ \| |_ | |_| ||  _||  _|
|____/  \___||_|  |_||_| |_||_||_| |_| \__, |        |___/ \__| \__,_||_|  |_|
| |__    ___  _ __  ___                |___/
| '_ \  / _ \| '__|/ _ \
| | | ||  __/| |  |  __/
|_| |_| \___||_|   \___|                                             */


enum ArmStates {RESTING=0,GRAB_FROM_INTAKE=1,ALMOST_SCORED=2,SCORED=3,ALLIANCE=4};
ArmStates currentstate = RESTING;
short int RESTINGPOS = 8000;
short int GRABINTAKEPOS = 13750;
short int ALMOSTSCOREDPOS = 20000;
short int SCOREDPOS = 24800;
short int ALLIANCEPOS = 29000;
short int dial1foroutsidethisfunction;
short int dial2foroutsidethisfunction;
bool antistallactive = true;
bool stalled = false;
bool colorsorteractive = true;
bool colorsortersetting = true; // true = red /////////////// false = blue
bool skillsrun;
int selectedautonlocationonarrayandareallylongvariablenamebecausewhynotlol[2];

//how the states interact with each other
int ARMITERATIONSSKILLS[5][5] = {
  { 1, 0, 1, 1, 1}, //resting
  { 1, 0, 1, 1, 1}, //grab from intake
  { 0, 0, 0, 0, 0}, //almost scored
  { 1, 0, 1, 1, 1}, //scored
  { 1, 0, 1, 1, 1}  //alliance
//  S  AS G  R  A
};
int ARMITERATIONS[5][5] = {
  { 1, 0, 1, 1, 1}, //resting
  { 1, 0, 1, 1, 1}, //grab from intake
  { 1, 1, 0, 0, 0}, //almost scored
  { 1, 1, 1, 1, 1}, //scored
  { 1, 1, 1, 1, 1}  //alliance
//  S  AS G  R  A
};

int auton_array[5][4]{
    { 1, 2, 3, 4}, // red neg
    { 5, 6, 7, 8}, // red pos
    { 9, 10, 11, 12}, // blue neg
    { 13, 14, 15, 16}, // blue pos
    { 17, 18, 19, 20} // skills
};

int auton_array_color_sorter[5][4]{
    //0 = red
    //1 = blue
    { 0, 0, 0, 0}, // red neg
    { 0, 0, 0, 0}, // red pos
    { 1, 1, 1, 1}, // blue neg
    { 1, 1, 1, 1}, // blue pos
    { 0, 0, 0, 0} // skills
};

/*
 _____                     _    _                               _
|  ___|_   _  _ __    ___ | |_ (_)  ___   _ __   ___           | |__    ___  _ __  ___
| |_  | | | || '_ \  / __|| __|| | / _ \ | '_ \ / __|          | '_ \  / _ \| '__|/ _ \
|  _| | |_| || | | || (__ | |_ | || (_) || | | |\__ \          | | | ||  __/| |  |  __/
|_|    \__,_||_| |_| \___| \__||_| \___/ |_| |_||___/          |_| |_| \___||_|   \___| */

int rounding(short int num, short int roundto) {
  short int rnddown = num - (num % roundto);
  if (rnddown >= roundto / 2) {
    return rnddown + roundto;
  } else {
    return rnddown;
  }
}
/*
int armstatelistinintegers (){
  switch (currentstate){
    case RESTING: return 0;
    case GRAB_FROM_INTAKE: return 1;
    case ALMOST_SCORED: return 2;
    case SCORED: return 3;
    case ALLIANCE: return 4;
  }
}*/

void armstate(short int btn) {
  // btn = 0 is L1
  // btn = 1 is L1
  // btn = 2 is R2
  // btn = 3 is R1
  // btn = 4 is Y
  if (currentstate != ALMOST_SCORED&&btn == 1){btn = 0;}
  if ((ARMITERATIONS[currentstate][btn])==1){
    switch(btn){
      case 0: currentstate = ALMOST_SCORED; break;
      case 1: currentstate = SCORED; break;
      case 2: currentstate = GRAB_FROM_INTAKE; break;
      case 3: currentstate = RESTING; break;
      case 4: currentstate = ALLIANCE; break;
    }}}

int getcurrentstatepos(){
    switch(currentstate){
        case RESTING: return RESTINGPOS;
        case GRAB_FROM_INTAKE: return GRABINTAKEPOS;
        case ALMOST_SCORED: return ALMOSTSCOREDPOS;
        case SCORED: return SCOREDPOS;
        case ALLIANCE: return ALLIANCEPOS;}}

void arm_task() {
  pros::delay(2000);
  while (true) {
    float speed = armpid.update(getcurrentstatepos() - armrotation.get_position());
    arm.move(speed);
  }
}
pros::Task Lift_Task(arm_task,TASK_PRIORITY_DEFAULT-1);



void antistall(){
  if (intake.get_target_velocity() - intake.get_actual_velocity() >= 200){
    pros::Task::delay(200);
    if (intake.get_target_velocity() - intake.get_actual_velocity() >= 200){
       stalled = true;
       intake.move_relative(20000,-12700);
       stalled = false;
    }else{return;}
    }else{return;}}

void intakefunc(short int mode,short int speed){
  if (stalled == false){
  switch (mode){
    case 1: intake.move_velocity(speed); /*antistall();*/ break;
    case 2: raiser.move_velocity(speed); /*antistall();*/ break;
    case 3: intake.move_velocity(speed); raiser.move_velocity(speed); /*antistall();*/ break;
  }}
}

void colorsorter(){
  if (colorsorteractive == true){
  stalled = true;
  intake.move_relative(50,-127);
  stalled = false;}
}

int evenorodd(int number){ //though currently we only use this function once it's its seperate function just in case
  if (rounding(number,2)==number){
    return 0;
  } else{
    return 1;
  }
}

void colorsortersortingpart(){
  if (colorsortersetting == false){if ((colorsensor.get_hue() > 210) && (colorsensor.get_hue() < 250)){colorsorter();}}
  if (colorsortersetting == true){if ((colorsensor.get_hue() > 0) && (colorsensor.get_hue() < 20)){colorsorter();}}
}
pros::Task colorsort(colorsortersortingpart);

/*
    _         _                ____       _           _             
   / \  _   _| |_ ___  _ __   / ___|  ___| | ___  ___| |_ ___  _ __ 
  / _ \| | | | __/ _ \| '_ \  \___ \ / _ \ |/ _ \/ __| __/ _ \| '__|
 / ___ \ |_| | || (_) | | | |  ___) |  __/ |  __/ (__| || (_) | |   
/_/   \_\__,_|\__\___/|_| |_| |____/ \___|_|\___|\___|\__\___/|_|   */

void updateselectedauton() {
    int dial1val = rounding(static_cast<int>(round(autonslectorcornersorsmth.get_angle()/100)),72);
    dial1val /= 72;
    if (dial1val == 5){dial1val =0;}
    int dial2val = rounding(static_cast<int>(round(autonslectorcornersorsmth.get_angle()/100)),90);
    dial2val /= 90;
    if (dial2val == 4){dial2val =0;}
    selectedautonlocationonarrayandareallylongvariablenamebecausewhynotlol[0] = dial1val;
    selectedautonlocationonarrayandareallylongvariablenamebecausewhynotlol[1] = dial2val;
    if (dial1val==4){skillsrun =true;}else{skillsrun = false;}
    setting_colorsensor_mode();
    //dial1foroutsidethisfunction = dial1val;
    //dial2foroutsidethisfunction = dial2val;
    dial1foroutsidethisfunction = 4;
    dial2foroutsidethisfunction = 3;
}

void setting_colorsensor_mode(){
  if (auton_array_color_sorter[selectedautonlocationonarrayandareallylongvariablenamebecausewhynotlol[0]][1] == 0){colorsortersetting = true;}
  else if (auton_array_color_sorter[selectedautonlocationonarrayandareallylongvariablenamebecausewhynotlol[0]][1] == 1){colorsortersetting = false;}
}

void runselectedauton(){
  switch(auton_array[dial1foroutsidethisfunction][dial2foroutsidethisfunction]){
    case 1:redneg1();
    case 2:redneg2();
    case 3:redneg3();
    case 4:redneg4();
    case 5:redpos1();
    case 6:redpos2();
    case 7:redpos3();
    case 8:redpos4();
    case 9:blueneg1();
    case 10:blueneg2();
    case 11:blueneg3();
    case 12:blueneg4();
    case 13:bluepos1();
    case 14:bluepos2();
    case 15:bluepos3();
    case 16:bluepos4();
    case 17:skills1();
    case 18:skills2();
    case 19:skills3();
    case 20:testauton();
  }
}