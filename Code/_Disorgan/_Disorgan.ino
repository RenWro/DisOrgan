/*
  Made by Gustavo Silveira, 2023.

  http://www.musiconerd.com
  http://www.youtube.com/musiconerd
  http://facebook.com/musiconerdmusiconerd
  http://instagram.com/musiconerd/
  http://www.gustavosilveira.net
  gustavosilveira@musiconerd.com

  If you are using for anything that's not for personal use don't forget to give credit.

  PS: Just change the value that has a comment like " //* "

*/

/////////////////////////////////////////////
// Choosing your board
// Define your board, choose:
// "ATMEGA328" if using ATmega328 - Uno, Mega, Nano...
// "ATMEGA32U4" if using with ATmega32U4 - Micro, Pro Micro, Leonardo...
// "TEENSY" if using a Teensy board
// "BLEMIDI" if using BLE MIDI (ESP32)
// "DEBUG" if you just want to debug the code in the serial monitor
// "SERIAL_OUT" Outputs serial data

#define SERIAL_OUT 1  //* put here the uC you are using, like in the lines above followed by "1", like "ATMEGA328 1", "DEBUG 1", etc.

/////////////////////////////////////////////
// Are you using buttons?
//#define USING_BUTTONS 1  //* comment if not using buttons

/////////////////////////////////////////////
// Are you using potentiometers?
#define USING_POTENTIOMETERS 1  //* comment if not using potentiometers


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////
// LIBRARIES
// -- Defines the MIDI library -- //

//////////////////////////////////////

// if using with ATmega328 - Uno, Mega, Nano...
#ifdef ATMEGA328
#include <MIDI.h>
//MIDI_CREATE_DEFAULT_INSTANCE();
#endif

// if using with ATmega32U4 - Micro, Pro Micro, Leonardo...
#ifdef ATMEGA32U4
#include <MIDIUSB.h>

#ifdef MIDI_DIN
#include <MIDI.h>  // adds the MIDI library to use the hardware serial with a MIDI cable
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midi2);
#endif  //MIDI_DIN

#endif  // ATMEGA32U4

#ifdef BLEMIDI
#include <BLEMidi.h>  // https://github.com/max22-/ESP32-BLE-MIDI
// Documentation: https://www.arduino.cc/reference/en/libraries/esp32-ble-midi/
char bleMIDIDeviceName[] = { "BLE Controller" };  // put here the name you want for your device
#endif


//////////////////////
// Threads
#include <Thread.h>            // Threads library >> https://github.com/ivanseidel/ArduinoThread
#include <ThreadController.h>  // Same as above


#ifdef USING_POTENTIOMETERS
// include the ResponsiveAnalogRead library
#include <ResponsiveAnalogRead.h>  // https://github.com/dxinteractive/ResponsiveAnalogRead
#endif



// - - - - - - - - - - - - - - - - - - - - - - - - - - - -

// Types of MIDI messages to send - DO NOT TOUCH
byte NN = 0;  // Note number or MACKIE
byte CC = 1;  // Control change
byte T = 2;   // Toggle
byte PC = 3;  // Program Change
byte PB = 4;  // Pitch Bend



/////////////////////////////////////////////
// POTENTIOMETERS

#ifdef USING_POTENTIOMETERS

const byte N_POTS = 12;  //* total numbers of pots (slide & rotary). Number of pots in the Arduino + number of pots on multiplexer 1 + number of pots on multiplexer 2...

const byte N_POTS_ARDUINO = 12;  //* number of pots connected straight to the Arduino
// If using the Arduino declare as "A1, A2"
// If using ESP32 only use the GPIO number as "11, 10"
const byte POT_ARDUINO_PIN[N_POTS_ARDUINO] = { A6, A7, A8, A9, A10, A11, A5, A4, A3, A2, A1, A0 };  //* pins of each pot connected straight to the Arduino (don't use "A" if you are using ESP32, only the number)

int POT_MIDI_CH = 1;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#define USING_CUSTOM_CC_N 1  //* comment if not using CUSTOM CC NUMBERS, uncomment if using it.
#ifdef USING_CUSTOM_CC_N

// What type of message do you want to send?
// Control Change - Pitch Bend

// CC: Control change
// PB: Pitch Bend

//* Put here the type of message you want to send, in the same order you declared the button pins
// "CC" for Control Change | "PB" for Pitch Bend
byte MESSAGE_TYPE_POT[N_POTS] = {
  CC,
  CC,
  CC,
  CC,
  CC,
  CC,
  CC,
  CC,
  CC,
  CC,
  CC,
  CC,
};

byte POT_CC_N[N_POTS] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };  // Add the CC NUMBER or MACKIE of each pot you want

#endif

// - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const int TIMEOUT = 300;      //* Amount of time the potentiometer will be read after it exceeds the varThreshold
const byte varThreshold = 8;  //* Threshold for the potentiometer signal variation - Increase if using ESP32
// For varThreshold I usually use "8" for normal Arduino and "30" for ESP32

// put here the min and max reading in the potCState
// in the potMin put a little bit more and in the potMax put a little bit less
// IMPORTANT:
// Regular Arduinos have 10 bit resolution: 0 - 1023
// ESP32 boards have 12 bit resolution: 0 - 4095
const int potMin = 20;
const unsigned int potMax = 940;

#endif  // USING_POTENTIOMETERS

// - - - - - - - - - - - - - - - - - - - - - - - - - - - -
