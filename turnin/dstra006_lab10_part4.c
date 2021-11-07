/*	Author: David Strathman
 *  Partner(s) Name:
 *	Lab Section: 022
 *	Assignment: Lab #10  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
  * Link to vid:
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "../header/timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
//global variables
unsigned char threeLEDS = 0x00;
unsigned char blinkingLED = 0x00;
unsigned char speaker = 0x00;
unsigned char currA = 0;
unsigned long counter = 0;
unsigned long threshold = 2;
unsigned char speakerA = 0;

enum ThreeLEDsStates { SM_LOOP } ThreeLEDsState;
void ThreeLEDsSM() {
  switch (ThreeLEDsState) {
    case SM_LOOP:
      ThreeLEDsState = SM_LOOP;
  }

  switch (ThreeLEDsState) {
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
}

enum BlinkingLEDStates { SM_LOOP2 } BlinkingLEDState;
void BlinkingLEDSM() {
  switch (BlinkingLEDState) {
    case SM_LOOP2:
      BlinkingLEDState = SM_LOOP2;
  }

  switch (BlinkingLEDState) {
    case SM_LOOP2:
      if (blinkingLED == 0x00) {
        blinkingLED = 0x08;
      } else {
        blinkingLED = 0x00;
      }
  }
}

enum SpeakerStates {SM_LOOP4} SpeakerState;
void Speaker() {
  switch (SpeakerState) {
    case SM_LOOP4:
      SpeakerState = SM_LOOP4;
  }

  switch (SpeakerState) {
    case SM_LOOP4:
      if (counter >= threshold) {
        counter = 0;
        currA = ~PINA;
        if ((currA & 0x04) == 0x04) {
          if (speaker == 0x10) {
            speaker = 0x00;
          } else {
            speaker = 0x10;
          }
        } else {
          speaker = 0x00;
        }
      } else {
        counter++;
      }
   }
}

enum SpeakerFreqStates {SM_LOOP5} SpeakerFreqState;
void SpeakerFreq() {
  switch (SpeakerFreqState) {
    case SM_LOOP5:
      SpeakerFreqState = SM_LOOP5;
  }

  switch (SpeakerFreqState) {
    case SM_LOOP5:
      speakerA = ~PINA;
      if ((speakerA & 0x02) == 0x02 && threshold >= 2) {
        threshold--;
      }
      else if ((speakerA & 0x01) == 0x01) {
        threshold++;
      }
  }
}


enum CombineStates {SM_LOOP3} CombineState;
void CombineLEDsSM() {
  switch (CombineState) {
    case SM_LOOP3:
      CombineState = SM_LOOP3;
  }

  switch (CombineState) {
    case SM_LOOP3:
      PORTB = threeLEDS | blinkingLED | speaker;
  }
}

int main(void) {
  unsigned long SM1_elapsedTime = 300;
  unsigned long SM2_elapsedTime = 1000;
  unsigned long SM3_elapsedTime = 1;
  unsigned long SM4_elapsedTime = 2;
  unsigned long SM5_elapsedTime = 500;
  unsigned char timerPeriod = 1;
  DDRA = 0x00; PORTA = 0xFF; //A is input
  DDRB = 0xFF; PORTB = 0x00; //Set both to outputs
  /* Insert your solution below */
  TimerSet(timerPeriod);
  TimerOn();
  ThreeLEDsState = SM_LOOP;
  BlinkingLEDState = SM_LOOP2;
  CombineState = SM_LOOP3;
  SpeakerState = SM_LOOP4;
  while (1) {
    if (SM1_elapsedTime >= 300) {
      ThreeLEDsSM();
      SM1_elapsedTime = 0;
    }
    if (SM2_elapsedTime >= 1000) {
      BlinkingLEDSM();
      SM2_elapsedTime = 0;
    }
    if (SM3_elapsedTime >= 1) {
      CombineLEDsSM();
      SM3_elapsedTime = 0;
    }
    if (SM4_elapsedTime >= 1) {
      Speaker();
      SM4_elapsedTime = 0;
    }
    if (SM5_elapsedTime >= 500) {
      SpeakerFreq();
      SM5_elapsedTime = 0;
    }
    while (!TimerFlag);
    TimerFlag = 0;
    SM1_elapsedTime += timerPeriod;
    SM2_elapsedTime += timerPeriod;
    SM3_elapsedTime += timerPeriod;
    SM4_elapsedTime += timerPeriod;
    SM5_elapsedTime += timerPeriod;
  }
  return 1;
}
