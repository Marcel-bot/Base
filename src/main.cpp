#include <main.h>

bool requested_sensor_data = false;

char buffer[256];
u_int8_t pos = 0;

u_int16_t front_left = 1000, front_middle = 1000, front_right = 1000, back_left = 1000, back_middle = 1000, back_right = 1000;

unsigned long last_request = millis();

void await_confirmation() {
    while (true) {
        if (Serial.available()) {
            char c = Serial.read();

            if (c == UP_CHAR) {
                return;
            }
        }
    }
}

bool confirm_wheels() {
    Wire.beginTransmission(WHEELS_ADDRESS);
    Wire.write(UP_CHAR);
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
    Wire.write(UP_CHAR);
    Wire.endTransmission();

    delay(100);

    Wire.requestFrom(SENSORS_ADDRESS, SENSORS_UP_SIZE);

    delay(100);

    if (!Wire.available()) {
        return false;
    }

    String confirm_buffer = "";

    while (Wire.available()) {
        confirm_buffer += (char)Wire.read();
    }

    return confirm_buffer == SENSORS_UP;
}

void confirm_slaves() {
    bool wheels_confirmed = true, sensors_confirmed = false;

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

    delay(500);
}

void handle_uart() {
    if (Serial.available()) {
        char c = Serial.read();

        if (c == '\n') {
            buffer[pos] = 0;

            switch (buffer[0]) {
                // case ALARM_ON:
                //     break;

                // case ALARM_OFF:
                //     break;

                // case SET_SETTINGS_COMMAND:
                //     break;

                // case GET_SETTINGS_COMMAND:
                //     break;

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
    if (millis() - last_request > 500) {
        last_request = millis();

        Wire.requestFrom(SENSORS_ADDRESS, 12);

        delay(10);

        while (Wire.available()) {
            Serial.print("Value: ");
            Serial.println(Wire.read() << 8 | Wire.read());
        }
    }
}

void setup() {
    Serial.begin(BAUDRATE);
    delay(500);

    await_confirmation();

    Serial.println(UP_MESSAGE);

    Wire.begin();
    confirm_slaves();
}

void loop() {
    handle_uart();
    handle_wheels();
    handle_sensors();
}