#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

#define DSEAT_PIN  GPIO_NUM_5
#define PSEAT_PIN  GPIO_NUM_4
#define DBELT_PIN  GPIO_NUM_7
#define PBELT_PIN  GPIO_NUM_6
#define IGNITION_PIN  GPIO_NUM_8

bool dseat = false;  //Detects when the driver is seated 
bool pseat = false;  //Detects when the passenger is seated
bool dbelt = false;  //Detects when the drivers seatbelt is on
bool pbelt = false;  //Detects when the passengers seatbelt is on
bool ignition = false; //Detects when the ignition is turned on

void app_main(void)
{
    gpio_reset_pin()
}
