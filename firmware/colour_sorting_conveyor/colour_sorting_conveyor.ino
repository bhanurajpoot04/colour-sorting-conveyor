/*
 * Colour Sorting Conveyor Belt
 * ----------------------------------------------------------
 * Board   : Arduino Uno
 * Sensors : TCS3200 colour sensor, IR obstacle sensor (active LOW)
 * Driver  : L298N (DC motor for belt)
 * Actuator: SG90 servo (diverter flap)
 *
 * Working:
 *  1. IR sensor object ko detect karta hai -> belt chalu.
 *  2. Object TCS3200 ke neeche pahunchta hai -> belt rukti hai.
 *  3. Colour sensor RGB read karta hai (average of N samples).
 *  4. Colour classify hota hai: RED / GREEN / BLUE / UNKNOWN.
 *  5. Servo us colour ke angle par ghoomta hai.
 *  6. Belt dobara chalti hai, object bin me girta hai, phir belt band.
 *
 * Serial Monitor (9600 baud):
 *   'c' bhejo -> calibration mode ON/OFF (raw RGB values print hongi)
 *
 * NOTE: Servo library Uno par pin 9 aur 10 ka analogWrite (PWM) disable
 *       kar deti hai. Isliye motor speed (ENA) pin 3 par rakhi hai.
 */

#include <Servo.h>

// ===================== PIN DEFINITIONS =====================
// TCS3200 colour sensor
#define S0        4
#define S1        5
#define S2        6
#define S3        7
#define SENSOR_OUT 8

// L298N motor driver
#define IN1       10
#define IN2       11
#define ENA       3      // PWM pin (Timer2) - Servo se conflict nahi

// IR sensor (active LOW: object aaye to LOW)
#define IR_PIN    2

// Servo
#define SERVO_PIN 9

// ===================== SETTINGS (yahan tune karo) =====================
#define BELT_SPEED          150    // 0-255
#define TRAVEL_TO_SENSOR_MS 2500   // IR se colour sensor tak object pahunchne ka time
#define SORT_RUN_MS         3000   // sorting ke baad belt kitni der chale (object girne tak)
#define SERVO_SETTLE_MS     400    // servo ko ghoomne ka time
#define NUM_SAMPLES         5      // average ke liye readings

// Servo angles (apne diverter ke hisab se adjust karo)
#define ANGLE_NEUTRAL  90    // default / UNKNOWN (seedha nikal jaye)
#define ANGLE_RED      30
#define ANGLE_GREEN    90
#define ANGLE_BLUE     150

// Colour detection thresholds
// TCS3200 frequency (pulse width in us) jitna CHOTA, utna zyada us colour ka light.
// Agar teeno values iss se badi hain -> koi object nahi / bahut dark.
#define NO_OBJECT_THRESHOLD 400
// Dominant colour baaki dono se kam se kam itna % chhota hona chahiye
#define DOMINANCE_PERCENT   85     // e.g. R < 0.85 * G aur R < 0.85 * B

// ===================== GLOBALS =====================
Servo sorter;

enum ColourType { COL_UNKNOWN, COL_RED, COL_GREEN, COL_BLUE };

bool calibrationMode = false;

// ===================== MOTOR HELPERS =====================
void beltOn() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, BELT_SPEED);
}

void beltOff() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);
}

// ===================== COLOUR SENSOR =====================
unsigned long readChannel(bool s2, bool s3) {
  digitalWrite(S2, s2);
  digitalWrite(S3, s3);
  delay(20);                              // filter settle
  return pulseIn(SENSOR_OUT, LOW, 50000UL); // timeout 50 ms
}

void readRGB(unsigned long &r, unsigned long &g, unsigned long &b) {
  unsigned long sumR = 0, sumG = 0, sumB = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sumR += readChannel(LOW,  LOW);    // RED filter
    sumG += readChannel(HIGH, HIGH);   // GREEN filter
    sumB += readChannel(LOW,  HIGH);   // BLUE filter
  }
  r = sumR / NUM_SAMPLES;
  g = sumG / NUM_SAMPLES;
  b = sumB / NUM_SAMPLES;
}

ColourType classify(unsigned long r, unsigned long g, unsigned long b) {
  // pulseIn timeout par 0 deta hai -> invalid
  if (r == 0 || g == 0 || b == 0) return COL_UNKNOWN;

  // Teeno bade -> object nahi / bahut andhera
  if (r > NO_OBJECT_THRESHOLD && g > NO_OBJECT_THRESHOLD && b > NO_OBJECT_THRESHOLD)
    return COL_UNKNOWN;

  if (r * 100UL < g * DOMINANCE_PERCENT && r * 100UL < b * DOMINANCE_PERCENT) return COL_RED;
  if (g * 100UL < r * DOMINANCE_PERCENT && g * 100UL < b * DOMINANCE_PERCENT) return COL_GREEN;
  if (b * 100UL < r * DOMINANCE_PERCENT && b * 100UL < g * DOMINANCE_PERCENT) return COL_BLUE;

  return COL_UNKNOWN;
}

const char* colourName(ColourType c) {
  switch (c) {
    case COL_RED:   return "RED";
    case COL_GREEN: return "GREEN";
    case COL_BLUE:  return "BLUE";
    default:        return "UNKNOWN";
  }
}

int colourAngle(ColourType c) {
  switch (c) {
    case COL_RED:   return ANGLE_RED;
    case COL_GREEN: return ANGLE_GREEN;
    case COL_BLUE:  return ANGLE_BLUE;
    default:        return ANGLE_NEUTRAL;
  }
}

// ===================== SETUP =====================
void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(SENSOR_OUT, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IR_PIN, INPUT);

  // TCS3200 frequency scaling 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  beltOff();

  Serial.begin(9600);
  sorter.attach(SERVO_PIN);
  sorter.write(ANGLE_NEUTRAL);

  Serial.println(F("=== Colour Sorting Conveyor Ready ==="));
  Serial.println(F("Calibration ke liye 'c' bhejo."));
}

// ===================== MAIN LOOP =====================
void loop() {

  // ---- Calibration mode: belt band, sirf RGB values print ----
  if (Serial.available()) {
    char ch = Serial.read();
    if (ch == 'c' || ch == 'C') {
      calibrationMode = !calibrationMode;
      Serial.println(calibrationMode ? F("Calibration ON") : F("Calibration OFF"));
    }
  }

  if (calibrationMode) {
    unsigned long r, g, b;
    readRGB(r, g, b);
    Serial.print(F("R: ")); Serial.print(r);
    Serial.print(F("  G: ")); Serial.print(g);
    Serial.print(F("  B: ")); Serial.print(b);
    Serial.print(F("  -> ")); Serial.println(colourName(classify(r, g, b)));
    delay(500);
    return;
  }

  // ---- Normal sorting cycle ----
  if (digitalRead(IR_PIN) == LOW) {          // object detect hua
    Serial.println(F("Object detected -> belt ON"));
    beltOn();
    delay(TRAVEL_TO_SENSOR_MS);              // colour sensor tak pahunchne do

    beltOff();                               // ruko aur colour padho
    delay(300);

    unsigned long r, g, b;
    readRGB(r, g, b);
    ColourType c = classify(r, g, b);

    Serial.print(F("R: ")); Serial.print(r);
    Serial.print(F("  G: ")); Serial.print(g);
    Serial.print(F("  B: ")); Serial.print(b);
    Serial.print(F("  => ")); Serial.println(colourName(c));

    sorter.write(colourAngle(c));            // diverter set karo
    delay(SERVO_SETTLE_MS);

    beltOn();                                // object ko bin tak bhejo
    delay(SORT_RUN_MS);
    beltOff();

    sorter.write(ANGLE_NEUTRAL);             // diverter wapas neutral
    delay(SERVO_SETTLE_MS);
    Serial.println(F("---------------------------"));
  }
}
