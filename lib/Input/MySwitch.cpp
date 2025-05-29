#include <MySwitch.h>

void MySWITCH::setup() {
    for (int i = 0; i < 3; i++){
        pinMode(tactswitchPIN[i], INPUT_PULLUP);
    }
    pinMode(toggleswitchPIN, INPUT);
}

int MySWITCH::check_tact() {
    pushed_tact = 0;
    for (int i = 0; i < 3; i++) {
        if (digitalRead(tactswitchPIN[i]) == LOW) {
            pushed_tact = i + 1;
        }
    }
    return pushed_tact;
}

int MySWITCH::check_toggle() {
    toggle_stat = 0;
    if (digitalRead(toggleswitchPIN) == HIGH) {
        toggle_stat = 1;
    }
    return toggle_stat;
}
