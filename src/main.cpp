#include<Arduino.h>
#include<Wire.h>

#define WHEELS_ADDRESS 0x08
#define SENSORS_ADDRESS 0x09
#define I2C_TRANSACTION_MAX_SIZE 32

#define BAUDRATE 115200

#define SET_SETTINGS_COMMAND 'S'
#define GET_SETTINGS_COMMAND 'T'
#define SET_SPEED_COMMAND 'M'
#define GET_SENSOR_DATA '$'

#define ALARM_ON '!'
#define ALARM_OFF '~'

#define WHEELS_UP "Wheels up"
#define SENSORS_UP "Sensors up"

char buffer[256];
u_int8_t pos = 0;

void await_confirmation() {
    while (true) {
        if (Serial.available()) {
            char c = Serial.read();

            if (c == '*') {
                return;
            }
        }
    }
}

bool confirm_wheels() {
    Wire.beginTransmission(WHEELS_ADDRESS);
    Wire.write('*');
    Wire.endTransmission();

    delay(100);

    Wire.requestFrom(WHEELS_ADDRESS, I2C_TRANSACTION_MAX_SIZE);

    delay(100);

    if (!Wire.available()) {
        return false;
    }

    String confirm_buffer = "";

    while (Wire.available()) {
        confirm_buffer += Wire.read();
    }

    return confirm_buffer == WHEELS_UP;
}

bool confirm_sensors() {
    Wire.beginTransmission(SENSORS_ADDRESS);
    Wire.write('*');
    Wire.endTransmission();

    delay(100);

    Wire.requestFrom(SENSORS_ADDRESS, I2C_TRANSACTION_MAX_SIZE);

    delay(100);

    if (!Wire.available()) {
        return false;
    }

    String confirm_buffer = "";

    while (Wire.available()) {
        confirm_buffer += Wire.read();
    }

    return confirm_buffer == SENSORS_UP;
}

void confirm_slaves() {
    bool wheels_confirmed = false, sensors_confirmed = false;

    while (!wheels_confirmed || !sensors_confirmed) {
        if (!wheels_confirmed) {
            wheels_confirmed = confirm_wheels();

            if (wheels_confirmed) {
                Serial.println(WHEELS_UP);
            }
        }

        if (!sensors_confirmed) {
            sensors_confirmed = confirm_sensors();
            
            if (sensors_confirmed) {
                Serial.println(SENSORS_UP);
            }
        }

        delay(500);
    }
}

void handle_uart() {
    if (Serial.available()) {
        char c = Serial.read();

        if (c == '\n') {
            buffer[pos] = 0;

            switch (buffer[0]) {
                case ALARM_ON:
                    break;

                case ALARM_OFF:
                    break;

                case SET_SETTINGS_COMMAND:
                    break;

                case GET_SETTINGS_COMMAND:
                    break;

                case SET_SPEED_COMMAND:
                    break;

                case GET_SENSOR_DATA:
                    break;
            }

            pos = 0;
        } else {
            buffer[pos++] = c;
        }
    }
}

void handle_wheels() {

}

void handle_sensors() {

}

void setup() {
    Serial.begin(BAUDRATE);
    delay(500);

    await_confirmation();

    Serial.println("Base up");

    Wire.begin();
    confirm_slaves();
}

void loop() {
    handle_uart();
    handle_wheels();
    handle_sensors();
}