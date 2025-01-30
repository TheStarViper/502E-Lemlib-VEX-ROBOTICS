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


enum ArmStates {RESTING,GRAB_FROM_INTAKE,ALMOST_SCORED,SCORED,ALLIANCE};
ArmStates currentstate = RESTING;
short int RESTINGPOS = 8000;
short int GRABINTAKEPOS = 12400;
short int ALMOSTSCOREDPOS = 20000;
short int SCOREDPOS = 25000;
short int ALLIANCEPOS = 29000;


//how the states interact with each other
int ARMINTERATIONS[5][5] = {
  { 1, 1, 1, 1, 1}, //scored
  { 0, 0, 1, 1, 0}, //almost scored
  { 1, 1, 1, 0, 1}, //grab from intake
  { 1, 1, 1, 0, 1}, //resting
  { 1, 1, 1, 0, 1}  //alliance
//  R  G  AS  S  A
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

int armstatelistinintegers (){
  switch (currentstate){
    case RESTING: return 0;
    case GRAB_FROM_INTAKE: return 1;
    case ALMOST_SCORED: return 2;
    case SCORED: return 3;
    case ALLIANCE: return 4;
  }
}
void armstate(short int btn) {
  // btn = 0 is L1
  // btn = 1 is L1
  // btn = 2 is R2
  // btn = 3 is R1
  // btn = 4 is Y
  /*if (currentstate == SCORED||btn == 1){btn = 0;}
  if (ARMINTERATIONS[armstatelistinintegers()][btn]==1){
    switch(btn){
      case 0: currentstate = ALMOST_SCORED;
      case 1: currentstate = SCORED;
      case 2: currentstate = GRAB_FROM_INTAKE;
      case 3: currentstate = RESTING;
      case 4: currentstate = ALLIANCE;
    }
  }*/
  switch (currentstate) {
    case RESTING:
        switch (btn) {
        case 1:currentstate = ALMOST_SCORED;break;
        case 2:currentstate = GRAB_FROM_INTAKE;break;
        case 4:currentstate = ALLIANCE;break;
      }
      break;
    case GRAB_FROM_INTAKE:
        switch (btn) {
        case 1:currentstate = ALMOST_SCORED;break;
        case 3:currentstate = RESTING;break;
        case 4:currentstate = ALLIANCE;break;
      }
      break;
    case ALMOST_SCORED:
        switch (btn) {
        case 1:currentstate = SCORED;break;
        case 4:currentstate = ALLIANCE;break;
      }
      break;
    case SCORED:
        switch (btn) {
        case 1:currentstate = ALMOST_SCORED;break;
        case 2:currentstate = GRAB_FROM_INTAKE;break;
        case 3:currentstate = RESTING;break;
        case 4:currentstate = ALLIANCE;break;
      }
    case ALLIANCE:
    switch (btn) {
        case 1:currentstate = ALMOST_SCORED;break;
        case 2:currentstate = GRAB_FROM_INTAKE;break;
        case 3:currentstate = RESTING;break;
      }
      break;
  }
}

void getnewarmposition(){

}
int getcurrentstatepos(){
    switch(currentstate){
        case RESTING: return RESTINGPOS;
        case GRAB_FROM_INTAKE: return GRABINTAKEPOS;
        case ALMOST_SCORED: return ALMOSTSCOREDPOS;
        case SCORED: return SCOREDPOS;
        case ALLIANCE: return ALLIANCEPOS;
    }
}

void antistall(){
  long int abracadabra = pros::millis();
}

void colorsorter(){
  
}

int evenorodd(int number){ //though currently we only use this function once it's its seperate function just in case
  if (rounding(number,2)==number){
    return 0;
  } else{
    return 1;
  }
}