#include <DS3231.h> // uses https://github.com/jarzebski/Arduino-DS3231

/* Create an rtc object */
DS3231 clock;

/* Relay switch on/off pin to use */
#define MOTOR_RELAY_PIN 7

const int  MORNING_ALARM_HOUR_ST = 6;
const int  MORNING_ALARM_MINUTES_ST = 5;
const int  MORNING_ALARM_HOUR_EN = 6;
const int  MORNING_ALARM_MINUTES_EN = 50;

const int EVENING_ALARM_HOUR_ST = 18;
const int EVENING_ALARM_MINUTES_ST = 05;
const int EVENING_ALARM_HOUR_EN = 19;
const int EVENING_ALARM_MINUTES_EN = 00;

/* Using Normally Closed(NC) on relay */

RTCDateTime dt;

void setup() {
  pinMode(MOTOR_RELAY_PIN, OUTPUT);          // sets the digital pin as output  
  pinMode(LED_BUILTIN, OUTPUT);          // sets the digital pin as output
  
  digitalWrite(LED_BUILTIN, LOW);       // offs error led to start with
  Serial.begin(9600);
  clock.begin();
  //initializeTime(); // comment this line after one time execution to set initial time of RTC
}

void loop() {
  dt = clock.getDateTime();  
  Serial.println(clock.dateFormat("d F Y H:i:s", dt));
  
  if (! isValidDataTime(dt))
    digitalWrite(LED_BUILTIN, HIGH);
  else {
    digitalWrite(LED_BUILTIN, LOW);
    if (isInAlarmRange(dt))  { // if morning or eve alarm then wait for timetorun motor and then switch off relay to stop motor
      Serial.println("In Alarm range TRUE");
      switchOnMotor();  
    } else {
      Serial.println("In Alarm range FALSE");
      switchOffMotor();  
    }
  }  
  delay(60000);  // sleep for 1 minutes before checking again
}

boolean isInAlarmRange(RTCDateTime dt) {
  if ( (dt.hour >= MORNING_ALARM_HOUR_ST && dt.hour <= MORNING_ALARM_HOUR_EN) && 
       (dt.minute >= MORNING_ALARM_MINUTES_ST && dt.minute <= MORNING_ALARM_MINUTES_EN)){
            Serial.println("In morning alarm range");
            return true;
       } else if ((dt.hour >= EVENING_ALARM_HOUR_ST && dt.hour <= EVENING_ALARM_HOUR_EN) && 
                  (dt.minute >= EVENING_ALARM_MINUTES_ST && dt.minute <= EVENING_ALARM_MINUTES_EN) ) {
            Serial.println("In evevning alarm range");
            return true;
       } 
  Serial.println("NOT in alarm range");
  return false;
}

boolean isValidDataTime(RTCDateTime dt) {
  String msg = String(String(dt.month) + " " + String(dt.day) + " "+ String(dt.year) + " " + String(dt.hour) + ":" + String(dt.minute));
  Serial.println(msg);
  if (dt.month == 01 && dt.day == 01 && dt.year == 2000)
    return false;
  else 
    return true;
}

void switchOnMotor(){
  digitalWrite(MOTOR_RELAY_PIN, HIGH);   // normally closed on relay closed
}

void switchOffMotor(){
  digitalWrite(MOTOR_RELAY_PIN, LOW);   // normally closed on relay open
}

void initializeTime() {
  // Manual (Year, Month, Day, Hour, Minute, Second)
  clock.setDateTime(2020, 6, 12, 12, 16, 0);
}
