#include <DS3231.h> // uses https://github.com/jarzebski/Arduino-DS3231

/* Create an rtc object */
DS3231 clock;

/* Relay switch on/off pin to use */
#define MOTOR_RELAY_PIN 7

const int  MORNING_ALARM_HOUR_ST = 6;
const int  MORNING_ALARM_MINUTES_ST = 0;
const int  MORNING_ALARM_HOUR_EN = 7;
const int  MORNING_ALARM_MINUTES_EN = 30;

const int EVENING_ALARM_HOUR_ST = 16;
const int EVENING_ALARM_MINUTES_ST = 0;
const int EVENING_ALARM_HOUR_EN = 17;
const int EVENING_ALARM_MINUTES_EN = 0;

/* Using Normally Closed(NC) on relay */

void setup() {
  pinMode(MOTOR_RELAY_PIN, OUTPUT);          // sets the digital pin as output
  digitalWrite(MOTOR_RELAY_PIN, LOW);       // offs motor
  
  pinMode(LED_BUILTIN, OUTPUT);          // sets the digital pin as output
  digitalWrite(LED_BUILTIN, LOW);       // offs error led to start with

  Serial.begin(9600);
  clock.begin();
  
  RTCDateTime dt;
  dt = clock.getDateTime();
  Serial.println(clock.dateFormat("d-m-Y H:i:s - l", dt));
  
  //initializeTime(); // comment this line after one time execution to set initial time of RTC
}

void loop() {
  RTCDateTime dt;
  dt = clock.getDateTime();
  
  Serial.println(clock.dateFormat("d-m-Y H:i:s - l", dt));
  
  if (! isValidDataTime(dt))
    digitalWrite(LED_BUILTIN, HIGH);
  else
    digitalWrite(LED_BUILTIN, LOW);

  if (isAlarmRange(dt))   // if morning or eve alarm then wait for timetorun motor and then switch off relay to stop motor
    triggerRelay(true);  
  else 
    triggerRelay(false);  
    
  delay(60000);  // sleep for 1 minutes before checking again
}

boolean isAlarmRange(RTCDateTime dt) {
  if (dt.hour >= MORNING_ALARM_HOUR_ST && dt.hour <= MORNING_ALARM_HOUR_EN) { // within morning alarm hour range
    if (dt.hour == MORNING_ALARM_HOUR_EN)         // current hour equal alarm end hour
      if (dt.minute <= MORNING_ALARM_MINUTES_EN)  // current minute within alarm end minute 
        return true;
      else 
        return false;
    if (dt.hour == MORNING_ALARM_HOUR_ST )
      if (dt.minute >= MORNING_ALARM_MINUTES_ST)  // current minute within alarm end minute 
        return true;
      else 
        return false;
    return true;
  } else 
    if (dt.hour >= EVENING_ALARM_HOUR_ST && dt.hour <= EVENING_ALARM_HOUR_EN) { // within evening alarm hour range
      if (dt.hour == EVENING_ALARM_HOUR_EN) // current hour equal alarm end hour
        if (dt.minute <= EVENING_ALARM_MINUTES_EN)  // current minute within alarm end minute
          return true;
        else                                          // current hour less than alarm end hour
          return false;
      if (dt.hour == EVENING_ALARM_HOUR_ST )
        if (dt.minute >= EVENING_ALARM_MINUTES_ST)  // current minute within alarm end minute 
          return true;
        else 
          return false;
      return true;
    } else
      return false;                             
}

boolean isValidDataTime(RTCDateTime dt) {
  if (dt.month == 01 && dt.day == 01 && dt.year == 2000)
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

void initializeTime() {
  // Manual (Year, Month, Day, Hour, Minute, Second)
  clock.setDateTime(2018, 10, 11, 00, 10, 0);
}
