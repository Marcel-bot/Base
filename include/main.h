#ifndef MAIN
#define MAIN

#include<Arduino.h>
#include<Wire.h>

#define WHEELS_ADDRESS 0x08
#define SENSORS_ADDRESS 0x09
#define I2C_TRANSACTION_MAX_SIZE 32

#define BAUDRATE 115200

#define UP_MESSAGE "Base up"

#define UP_CHAR '*'

#define SET_SETTINGS_COMMAND 'S'
#define GET_SETTINGS_COMMAND 'T'
#define SET_SPEED_COMMAND 'M'
#define GET_SENSOR_DATA '$'

#define ALARM_ON '!'
#define ALARM_OFF '~'

#define WHEELS_UP "Wheels up"
#define WHEELS_UP_SIZE 9

#define SENSORS_UP "Sensors up"
#define SENSORS_UP_SIZE 10

#endif