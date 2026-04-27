#include "Pedal.h"
#include <Arduino.h>

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  hw_init();
  network_init();
}

void loop()
{
  // Update voltage reading and check threshold
  update_pedal_voltage();
  
  // Handle voltage threshold crossing
  if (pedal_state->pedal_mode == PEDAL_SWITCH && pedal_state->voltage_threshold_crossed)
  {
    pedal_state->voltage_threshold_crossed = false;
    Serial.println("Voltage threshold crossed!");
    
    Serial.print("Sending event to Reaper\n");
    // Your threshold event logic here

    network->send_fx_switch();
  }

  if(pedal_state->pedal_mode == PEDAL_WAH)
  {
    // Handle switch mode
    Serial.println("Swapped to WAH mode!");
  }
}

// void eventUpdate()
// {
//   switch (pedalMode)
//   {
//   case PEDAL_SWITCH:
//     Serial.println("Event!");
//     break;

//   case PEDAL_WAH:
//     if (voltage > PEDAL_VOLTAGE_THRESHOLD)
//     {
//       direction = 1;
//     }
//     else
//     {
//       direction = 0;
//     }
//     if (direction)
//     {
//       wahPercent += PEDAL_RATE_OF_CHANGE;
//       if (wahPercent > 100)
//         wahPercent = 100;
//     }
//     else
//     {
//       wahPercent -= PEDAL_RATE_OF_CHANGE;
//       if (wahPercent < 0)
//         wahPercent = 0;
//     }
//     Serial.print("Wah: ");
//     Serial.println(wahPercent);
//     break;

//   default:
//     break;
//   }
// }
