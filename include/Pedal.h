#ifndef PEDAL_H
#define PEDAL_H

#include "network.h"
#include "UserData.h"
#include <EEPROM.h>
#include <Arduino.h>

#define PEDAL_VOLTAGE_THRESHOLD 0.07f
#define PEDAL_RATE_OF_CHANGE 10

#define PEDAL_INPUT_PIN A0
#define PEDAL_BUTTON_PIN D1

#define PEDAL_DEBOUNCE_DELAY 500

enum PedalMode
{
    PEDAL_SWITCH = 0,
    PEDAL_WAH = 1,
};

struct PedalState
{
    float voltage;  
    int wah_percent;
    float prev_voltage;
    int direction;
    volatile PedalMode pedal_mode;
    volatile unsigned long last_button_press_time;
    volatile bool voltage_threshold_crossed;
};

extern Network* network;
extern UserConfig config;
extern PedalState* pedal_state;

void setup_pedal();

void network_init();
void hw_init();
void update_pedal_voltage();

void pedal_state_init();


#endif
