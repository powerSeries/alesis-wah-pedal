#include "Pedal.h"

Network* network;
UserConfig config;
PedalState* pedal_state;


void IRAM_ATTR pedal_button_isr()
{
    // Debounce: ignore if interrupt fires within PEDAL_DEBOUNCE_DELAY ms
    unsigned long current_time = millis();
    if (current_time - pedal_state->last_button_press_time < PEDAL_DEBOUNCE_DELAY)
    {
        return;
    }

    pedal_state->last_button_press_time = current_time;
    if(pedal_state->pedal_mode == PEDAL_SWITCH)
    {
        pedal_state->pedal_mode = PEDAL_WAH;
    }
    else
    {
        pedal_state->pedal_mode = PEDAL_SWITCH;
    }
}

void IRAM_ATTR pedal_wah_isr()
{
    // Handle wah event
}

void update_pedal_voltage()
{
    // Read analog voltage from pedal input (0-1023 for ESP8266)
    int raw_value = analogRead(PEDAL_INPUT_PIN);
    // Convert to voltage (0-1V on ESP8266 with default ADC range)
    pedal_state->voltage = raw_value / 1023.0f;
    
    // Check if voltage crossed the threshold
    if (pedal_state->voltage >= PEDAL_VOLTAGE_THRESHOLD && 
        pedal_state->prev_voltage < PEDAL_VOLTAGE_THRESHOLD)
    {
        // Voltage crossed threshold upward
        pedal_state->voltage_threshold_crossed = true;
    }
    
    pedal_state->prev_voltage = pedal_state->voltage;
}

void network_init()
{
    network = new Network(config);
}

void hw_init()
{
    EEPROM.begin(EEPROM_SIZE);

    EEPROM.get(FIRST_RUN_FLAG_ADDR, config);
    Serial.print("First run flag: " + String(config.first_run_flag) + "\n");

    if (config.first_run_flag == FIRST_RUN_FLAG_VALUE)
    {
        Serial.println("EEPROM already initialized.\n");
    }

    // Initialize pedal state before attaching interrupt
    pedal_state = new PedalState();
    pedal_state->voltage = 0;
    pedal_state->wah_percent = 0;
    pedal_state->prev_voltage = 0;
    pedal_state->direction = 0;
    pedal_state->pedal_mode = PEDAL_SWITCH;
    pedal_state->last_button_press_time = 0;
    pedal_state->voltage_threshold_crossed = false;

    // Initialize hardware components if needed
    pinMode(PEDAL_INPUT_PIN, INPUT);
    pinMode(PEDAL_BUTTON_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(PEDAL_BUTTON_PIN), pedal_button_isr, FALLING);
}