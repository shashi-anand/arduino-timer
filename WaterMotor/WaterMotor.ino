#include <DS3231.h> // uses https://github.com/jarzebski/Arduino-DS3231

/* Create an rtc object */
DS3231 clock;

/* Relay switch on/off pin to use */
#define MOTOR_RELAY_PIN 7

const int  MORNING_ALARM_HOUR = 6;
const int  MORNING_ALARM_MINUTES = 0;

const int EVENING_ALARM_HOUR = 16;
const int EVENING_ALARM_MINUTES = 0;

const long TIME_TO_RUN_MOTOR_MORNING = 5400000; //90 minutes = 90*60*1000
const long TIME_TO_RUN_MOTOR_EVENING = 3600000; //60 minutes = 60*60*1000
/* Using Normally Closed(NC) on relay */

void setup() {
  pinMode(MOTOR_RELAY_PIN, OUTPUT);          // sets the digital pin as output
  digitalWrite(MOTOR_RELAY_PIN, LOW);       // offs motor
  
  pinMode(LED_BUILTIN, OUTPUT);          // sets the digital pin as output
  digitalWrite(LED_BUILTIN, LOW);       // offs error led to start with

  Serial.begin(9600);
  clock.begin();

  clearAnyExistingAlarms();
  
  //initializeTime(); // comment this line after one time execution to set initial time of RTC
  
  setMorningAlarm();
  setEveningAlarm();
}

void loop() {
  RTCDateTime dt;
  dt = clock.getDateTime();
    
  Serial.println(clock.dateFormat("d-m-Y H:i:s - l", dt));
  
  if (! isValidDataTime(dt))
    digitalWrite(LED_BUILTIN, HIGH);
  else
    digitalWrite(LED_BUILTIN, LOW);

  if (clock.isAlarm1()) {  // if morning or eve alarm then wait for timetorun motor and then switch off relay to stop motor
    triggerRelay(true);  
    delay(TIME_TO_RUN_MOTOR_MORNING);
    triggerRelay(false);
  } else if (clock.isAlarm2()) {  // if morning or eve alarm then wait for timetorun motor and then switch off relay to stop motor
    triggerRelay(true);  
    delay(TIME_TO_RUN_MOTOR_EVENING);
    triggerRelay(false);
  } else 
    delay(60000);  // sleep for 1 minutes before checking again
}

boolean isValidDataTime(RTCDateTime dt) {
  if (dt.month == 01 || dt.day == 01 || dt.year == 2000)
    return false;
  else 
    return true;
}

void triggerRelay(bool on) {
  if (on) {
    digitalWrite(MOTOR_RELAY_PIN, HIGH);   // turns on motor
  } else {
    digitalWrite(MOTOR_RELAY_PIN, LOW);   // turn off motor
  }
}

void setEveningAlarm() {
  // Set Alarm - Every 01h:10m:30s in each day
  // setAlarm2(Date or Day, Hour, Minute, Mode, Armed = true)
  clock.setAlarm2(0, EVENING_ALARM_HOUR, EVENING_ALARM_MINUTES, DS3231_MATCH_H_M);
}

void setMorningAlarm() {
  // Set Alarm - Every 01h:10m:30s in each day
  // setAlarm1(Date or Day, Hour, Minute, Second, Mode, Armed = true)
  clock.setAlarm1(0, MORNING_ALARM_HOUR, MORNING_ALARM_MINUTES, 0, DS3231_MATCH_H_M_S);
}

void initializeTime() {
  // Manual (Year, Month, Day, Hour, Minute, Second)
  clock.setDateTime(2018, 10, 7, 15, 58, 0);
}

void clearAnyExistingAlarms() {
  // Disarm alarms and clear alarms for this example, because alarms is battery backed.
  // Under normal conditions, the settings should be reset after power and restart microcontroller.
  clock.armAlarm1(false);
  clock.armAlarm2(false);
  clock.clearAlarm1();
  clock.clearAlarm2();
}
