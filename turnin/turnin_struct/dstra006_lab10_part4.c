/*	Author: David Strathman
 *  Partner(s) Name:
 *	Lab Section: 022
 *	Assignment: Lab #10  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *  Link to Vid: https://youtu.be/B-infQdUtBg
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "../header/timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

typedef struct task {
  int state; // Current state of the task
  unsigned long period; // Rate at which the task should tick
  unsigned long elapsedTime; // Time since task's previous tick
  int (*TickFct)(int); // Function to call for task's tick
} task;

task tasks[5];

const unsigned char tasksNum = 5;
const unsigned long timerPeriod = 1;
const unsigned long periodBlinkLED = 1000;
const unsigned long periodThreeLEDs = 300;
const unsigned long periodCombineLEDs = 1;
const unsigned long periodSpeaker = 1;
const unsigned long periodSpeakerFreq = 300;

void TimerISR() {
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) { // Heart of the scheduler code
     if ( tasks[i].elapsedTime >= tasks[i].period ) { // Ready
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = 0;
     }
     tasks[i].elapsedTime += timerPeriod;
  }
}

//global variables
unsigned char threeLEDS = 0x00;
unsigned char blinkingLED = 0x00;
unsigned char speaker = 0x00;
unsigned char currA = 0;
unsigned long counter = 0;
unsigned long threshold = 0;
unsigned char speakerA = 0;
unsigned char sounds[] = {2, 3, 4, 5, 6, 7, 8, 9};
signed char i = 0;

enum ThreeLEDsStates { SM_LOOP };
int ThreeLEDsSM(int state) {
  switch (state) {
    case SM_LOOP:
      state = SM_LOOP;
  }

  switch (state) {
    case SM_LOOP:
      if (threeLEDS == 0x00) {
        threeLEDS = 0x01;
      }
      else if (threeLEDS == 0x01) {
        threeLEDS = 0x02;
      }
      else if (threeLEDS == 0x02) {
        threeLEDS = 0x04;
      }
      else if (threeLEDS == 0x04) {
        threeLEDS = 0x01;
      }
  }
  return state;
}

enum BlinkingLEDStates { SM_LOOP2 };
int BlinkingLEDSM(int state) {
  switch (state) {
    case SM_LOOP2:
      state = SM_LOOP2;
  }

  switch (state) {
    case SM_LOOP2:
      if (blinkingLED == 0x00) {
        blinkingLED = 0x08;
      } else {
        blinkingLED = 0x00;
      }
  }
  return state;
}

enum SpeakerStates {SM_LOOP4};
int Speaker(int state) {
  switch (state) {
    case SM_LOOP4:
      state = SM_LOOP4;
  }

  switch (state) {
    case SM_LOOP4:
      threshold = sounds[i] * 2;
      if (counter < threshold) {
        counter++;
      }
      if (counter >= (threshold / 2)) {
        if (counter >= threshold) {
          counter = 0;
        }
        currA = ~PINA;
        if ((currA & 0x04) == 0x04) {
          speaker = 0x10;
        }
        else {
          speaker = 0x00;
        }
      } else {
        speaker = 0x00;
      }
   }
   return state;
}


enum SpeakerFreqStates {SM_LOOP5};
int SpeakerFreq(int state) {
  switch (state) {
    case SM_LOOP5:
      state = SM_LOOP5;
  }

  switch (state) {
    case SM_LOOP5:
      speakerA = ~PINA;
      if ((speakerA & 0x02) == 0x02 && i <= 6) {
        i++;
      }
      else if ((speakerA & 0x01) == 0x01 && i >= 1) {
        i--;
      }
  }
  return state;
}

enum CombineStates {SM_LOOP3};
int CombineLEDsSM(int state) {
  switch (state) {
    case SM_LOOP3:
      state = SM_LOOP3;
  }

  switch (state) {
    case SM_LOOP3:
      PORTB = threeLEDS | blinkingLED | speaker;
  }
  return state;
}

int main(void) {
  unsigned char i=0;
    tasks[i].state = SM_LOOP;
    tasks[i].period = periodThreeLEDs;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &ThreeLEDsSM;
    ++i;
    tasks[i].state = SM_LOOP2;
    tasks[i].period = periodBlinkLED;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &BlinkingLEDSM;
    ++i;
    tasks[i].state = SM_LOOP3;
    tasks[i].period = periodCombineLEDs;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &CombineLEDsSM;
    ++i;
    tasks[i].state = SM_LOOP4;
    tasks[i].period = periodSpeaker;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &Speaker;
    ++i;
    tasks[i].state = SM_LOOP5;
    tasks[i].period = periodSpeakerFreq;
    tasks[i].elapsedTime = tasks[i].period;
    tasks[i].TickFct = &SpeakerFreq;

    DDRA = 0x00; PORTA = 0xFF; //A is input
    DDRB = 0xFF; PORTB = 0x00; //Set both to outputs */

    TimerSet(timerPeriod);
    TimerOn();

    while(1) {
       //Sleep();
    }
    return 0;


  /*
  unsigned long SM1_elapsedTime = 1000;
  unsigned long SM2_elapsedTime = 1000;
  unsigned long SM3_elapsedTime = 1000;
  unsigned char timerPeriod = 50;
  DDRA = 0x00; PORTA = 0xFF; //A is input
  DDRB = 0xFF; PORTB = 0x00; //Set both to outputs */
  /* Insert your solution below */
  /*
  TimerSet(timerPeriod);
  TimerOn();
  ThreeLEDsState = SM_LOOP;
  BlinkingLEDState = SM_LOOP2;
  CombineState = SM_LOOP3;
  while (1) {
    if (SM1_elapsedTime >= 1000) {
      ThreeLEDsSM();
      SM1_elapsedTime = 0;
    }
    if (SM2_elapsedTime >= 1000) {
      BlinkingLEDSM();
      SM2_elapsedTime = 0;
    }
    if (SM3_elapsedTime >= 1000) {
      CombineLEDsSM();
      SM3_elapsedTime = 0;
    }
    while (!TimerFlag);
    TimerFlag = 0;
    SM1_elapsedTime += timerPeriod;
    SM2_elapsedTime += timerPeriod;
    SM3_elapsedTime += timerPeriod;
  }
  return 1;
  */
}
