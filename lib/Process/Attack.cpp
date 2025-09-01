#include <Attack.h>
#include "Input.h"
#include "Output.h"
#include "Process.h"
#include "AIP.h"

void Attack::attack_() {
    // mymotor.run(1, 200, 0);
    line.read();
    if (line.get_magnitude() != 999) {
        mymotor.run(line.get_avoid(), avoid_speed, 0);
        mybuzzer.start(330, 999);
        mypixel.closest(line.get_avoid(), 50, 255, 50, 3);

    } else if (ball.get_value(99) != 0) {
        
    } else {
        mymotor.free();
    }
}