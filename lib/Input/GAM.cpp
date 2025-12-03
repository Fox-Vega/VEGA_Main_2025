#include "Input.h"
#include "Output.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

void GAM::setup() {
    mybuzzer.start(400, 100);
    delay(50);
    mybuzzer.start(300, 100);
    delay(500);
    Wire.begin();
    if (!bno.begin()) Serial.println("BNO055 not detected.");
    bno.setExtCrystalUse(true);
    bno.setMode(OPERATION_MODE_AMG);
}

void GAM::read_azimuth() {
    sensors_event_t euler_event;
    bno.getEvent(&euler_event, Adafruit_BNO055::VECTOR_EULER);
    azimuth = (int)euler_event.orientation.x - yawtweak;
}

int GAM::get_azimuth() {
    return azimuth;
}

void GAM::dir_reset() {
    sensors_event_t euler_event;
    bno.getEvent(&euler_event, Adafruit_BNO055::VECTOR_EULER);

    yawtweak = euler_event.orientation.x;
}