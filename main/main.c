#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"

#define DSEAT_PIN  GPIO_NUM_5
#define PSEAT_PIN  GPIO_NUM_4
#define DBELT_PIN  GPIO_NUM_7
#define PBELT_PIN  GPIO_NUM_6
#define IGNITION_BUTTON  GPIO_NUM_8
#define READY_LED  GPIO_NUM_10
#define SUCCESS_LED  GPIO_NUM_11
#define ALARM_PIN GPIO_NUM_12

bool dseat = false;  //Detects when the driver is seated 
bool pseat = false;  //Detects when the passenger is seated
bool dbelt = false;  //Detects when the drivers seatbelt is on
bool pbelt = false;  //Detects when the passengers seatbelt is on
bool ignition = false; //Detects when the ignition is turned on
int executed = 0; //keep track of print statements
int ready_led = 0; //keep track of whether ready_led should be on or off
bool started = false; //Detects when engine is started

void app_main(void)
{
    gpio_reset_pin(DSEAT_PIN);
    gpio_set_direction(DSEAT_PIN, GPIO_MODE_INPUT);
    gpio_pullup_en(DSEAT_PIN);

    gpio_reset_pin(PSEAT_PIN);
    gpio_set_direction(PSEAT_PIN, GPIO_MODE_INPUT);
    gpio_pullup_en(PSEAT_PIN);

    gpio_reset_pin(DBELT_PIN);
    gpio_set_direction(DBELT_PIN, GPIO_MODE_INPUT);
    gpio_pullup_en(DBELT_PIN);

    gpio_reset_pin(PBELT_PIN);
    gpio_set_direction(PBELT_PIN, GPIO_MODE_INPUT);
    gpio_pullup_en(PBELT_PIN);

    gpio_reset_pin(IGNITION_BUTTON);
    gpio_set_direction(IGNITION_BUTTON, GPIO_MODE_INPUT);
    gpio_pullup_en(IGNITION_BUTTON);

    gpio_reset_pin(READY_LED);
    gpio_set_direction(READY_LED, GPIO_MODE_OUTPUT);
    gpio_pullup_en(READY_LED);

    gpio_reset_pin(SUCCESS_LED);
    gpio_set_direction(SUCCESS_LED, GPIO_MODE_OUTPUT);
    gpio_pullup_en(SUCCESS_LED);

    gpio_reset_pin(ALARM_PIN);
    gpio_set_direction(ALARM_PIN, GPIO_MODE_OUTPUT);
    gpio_pullup_en(ALARM_PIN);

    while (1){
        vTaskDelay(100 / portTICK_PERIOD_MS);

        dseat = gpio_get_level(DSEAT_PIN)==0;
        pseat = gpio_get_level(PSEAT_PIN)==0;
        dbelt = gpio_get_level(DBELT_PIN)==0;
        pbelt = gpio_get_level(PBELT_PIN)==0;
        ignition = gpio_get_level(IGNITION_BUTTON)==0;


        if (dseat){
            if (executed == 0){
                printf("Welcome to enhanced alarm system model 218-W25 \n"); 
                executed = 1;
            }
            if (pseat && dbelt && pbelt){
                while (ready_led == 0 && !started){
                    vTaskDelay(25 / portTICK_PERIOD_MS);
                    if (pseat && dbelt && pbelt){
                        gpio_set_level(READY_LED, 1);
                        ready_led = 1;
                    }
                
                    if ((ready_led == 1) && (!pseat || !dbelt || !pbelt)){
                        gpio_set_level(READY_LED,0);
                        ready_led = 0;
                    }
                    if (ignition == true && executed == 1 && ready_led == 1){
                        gpio_set_level(SUCCESS_LED, 1);
                        gpio_set_level(READY_LED, 0);
                        printf("Engine started!\n");
                        executed = 2;
                        started = true;
                    }
                }
            }
            else {
                if (ignition==true && executed == 1){
                    gpio_set_level(ALARM_PIN, 1);
                    if (!pseat){
                        printf("Passenger seat not occupied.\n");
                    }
                    if (!pbelt){
                        printf("Passenger seatbelt not fastened.\n");
                    }
                    if (!dbelt){
                        printf("Drivers seatbelt not fastened.\n");
                    }
                    executed = 2;
                }
            }
        }
    }
}
