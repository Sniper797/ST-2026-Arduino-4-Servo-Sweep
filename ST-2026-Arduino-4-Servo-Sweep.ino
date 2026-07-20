/*
 * ST-2026 · Arduino 4-Servo Sweep
 *
 * Electrical task of the Smart Methods (ST 2026) summer training.
 *
 *   1. Sweep four servo motors together (the Servo library "Sweep" example)
 *      for exactly 2 seconds.
 *   2. Then move all four to 90 degrees and hold there.
 *
 * Wiring (Arduino Uno R3)
 *   Servo 1 signal (orange) -> D8      All servos:
 *   Servo 2 signal (orange) -> D9        red   -> 5V
 *   Servo 3 signal (orange) -> D10       black -> GND
 *   Servo 4 signal (orange) -> D11
 *
 *   In Tinkercad the board's 5V rail drives all four servos fine. On real
 *   hardware, power them from a separate 5V supply and tie its GND to the
 *   Arduino GND -- four servos can pull well over an amp when they move,
 *   which the USB-powered 5V pin cannot deliver.
 */

#include <Servo.h>

// --- Configuration ----------------------------------------------------------

const uint8_t SERVO_PINS[] = {8, 9, 10, 11};
const uint8_t SERVO_COUNT = sizeof(SERVO_PINS) / sizeof(SERVO_PINS[0]);

const unsigned long SWEEP_DURATION_MS = 2000;  // how long to sweep (task: 2 s)
const unsigned int  STEP_DELAY_MS     = 15;    // per-degree delay, as in Sweep
const int           MIN_ANGLE         = 0;
const int           MAX_ANGLE         = 180;
const int           HOLD_ANGLE        = 90;    // final resting position

Servo servos[SERVO_COUNT];

// --- Helpers ----------------------------------------------------------------

// Commands every servo to the same angle.
void writeAll(int angle) {
  for (uint8_t i = 0; i < SERVO_COUNT; i++) {
    servos[i].write(angle);
  }
}

// Runs the Sweep motion (0 -> 180 -> 0 -> ...) and returns after durationMs.
// millis() is used instead of counting degrees so the sweep stops on time
// even if it lands mid-travel -- a full 0->180 pass alone takes ~2.7 s.
void sweepFor(unsigned long durationMs) {
  const unsigned long startTime = millis();
  int angle = MIN_ANGLE;
  int step = 1;  // +1 while sweeping up, -1 while sweeping back down

  while (millis() - startTime < durationMs) {
    writeAll(angle);
    delay(STEP_DELAY_MS);

    angle += step;
    if (angle >= MAX_ANGLE) {
      angle = MAX_ANGLE;
      step = -1;
    } else if (angle <= MIN_ANGLE) {
      angle = MIN_ANGLE;
      step = 1;
    }
  }
}

// --- Program ----------------------------------------------------------------

void setup() {
  for (uint8_t i = 0; i < SERVO_COUNT; i++) {
    servos[i].attach(SERVO_PINS[i]);
  }

  sweepFor(SWEEP_DURATION_MS);  // step 1: sweep for 2 seconds
  writeAll(HOLD_ANGLE);         // step 2: hold at 90 degrees
}

void loop() {
  // Nothing to do. The servos stay at 90 degrees until reset or power-off.
}
