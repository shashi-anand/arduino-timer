#include <DS3231.h> // uses https://github.com/jarzebski/Arduino-DS3231

/* Create an rtc object */
DS3231 clock;

/* Relay switch on/off pin to use */
#define MOTOR_RELAY_PIN 7

const int  MORNING_ALARM_HOUR_ST = 6;
const int  MORNING_ALARM_MINUTES_ST = 05;
const int  MORNING_ALARM_HOUR_EN = 7;
const int  MORNING_ALARM_MINUTES_EN = 30; // Make sure this is higher number like 58,59

const int EVENING_ALARM_HOUR_ST = 18;
const int EVENING_ALARM_MINUTES_ST = 05;
const int EVENING_ALARM_HOUR_EN = 19;
const int EVENING_ALARM_MINUTES_EN = 15;  // Make sure this is higher number like 58,59

/* Using Normally Closed(NC) on relay */

void setup() {
  pinMode(MOTOR_RELAY_PIN, OUTPUT);          // sets the digital pin as output
  pinMode(LED_BUILTIN, OUTPUT);          // sets the digital pin as output

  digitalWrite(LED_BUILTIN, LOW);       // offs error led to start with
  Serial.begin(9600);
  clock.begin();
  //initializeTime(); // comment this line after one time execution to set initial time of RTC
}

void loop() {
  RTCDateTime dt = clock.getDateTime();
  Serial.println("System clock: ");
  printDate(dt);

  if (!isValidDataTime(dt)) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    if (isInAlarmRange(dt)){
      switchOnMotor();
    } else {
      switchOffMotor();
    }
  }
  delay(60000);  // sleep for 1 minutes before checking again
}

boolean isInAlarmRange(RTCDateTime dt){
  if (isInAlarmRange(dt, MORNING_ALARM_HOUR_ST, MORNING_ALARM_MINUTES_ST, MORNING_ALARM_HOUR_EN, MORNING_ALARM_MINUTES_EN)){
    Serial.println("In morning alarm range");
    return true;  
  } else if (isInAlarmRange(dt, EVENING_ALARM_HOUR_ST, EVENING_ALARM_MINUTES_ST, EVENING_ALARM_HOUR_EN, EVENING_ALARM_MINUTES_EN)){
    Serial.println("In evening alarm range");
    return true;
  } else {
    return false;
  }
}

boolean isInAlarmRange(RTCDateTime dt, int startHour, int startMinute, int endHour, int endMinute) {
  int currentHour = dt.hour;
  int currentMinute = dt.minute;
  return isInAlarmRange(currentHour, currentMinute, startHour, startMinute, endHour, endMinute);
}

boolean isInAlarmRange(int currentHour, int currentMinute, int startHour, int startMinute, int endHour, int endMinute) {
  if (currentHour == startHour && currentHour == endHour){
    if (currentMinute >= startMinute && currentMinute <= endMinute){
      return true;
    }
  }
  if (currentHour == startHour){
    if (currentMinute >= startMinute){
      return true;
    }
  } 
  if (currentHour == endHour){
    if (currentMinute <= endMinute){
      return true;
    }
  } 
  if (currentHour > startHour && currentHour < endHour){
    if (currentMinute >= startMinute){
      return true;
    }
  } 
  return false;
}

boolean isValidDataTime(RTCDateTime dt) {
  if (dt.month == 01 && dt.day == 01 && dt.year == 2000)
    return false;
  else
    return true;
}

void printDate(RTCDateTime dt) {
  String msg = String(String(dt.month) + " " + String(dt.day) + " " + String(dt.year) + " " + String(dt.hour) + ":" + String(dt.minute));
  Serial.println(msg);
}

void switchOnMotor() {
  digitalWrite(MOTOR_RELAY_PIN, HIGH);   // normally open gets closed
}

void switchOffMotor() {
  digitalWrite(MOTOR_RELAY_PIN, LOW);   // normally open gets closed
}

void initializeTime() {
  // Manual (Year, Month, Day, Hour, Minute, Second)
  clock.setDateTime(2020, 10, 25, 22, 27, 0);
}
