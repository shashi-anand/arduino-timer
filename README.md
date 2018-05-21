# water-motor-timer
Arduino code to start/stop a water motor at two times through a day

This code base uses DS3231 timer module along with library  https://github.com/jarzebski/Arduino-DS3231 to initialize, create timers and check timers state

A relay is used to close the circuit for motor to start and stop water motor at the timer trigger

Arduino relay is defined by MOTOR_RELAY_PIN in code as pin 7

MORNING_ALARM_HOUR along with MORNING_ALARM_MINUTES defines the morning alarm time
EVENING_ALARM_HOUR along with EVENING_ALARM_MINUTES defines the evening alarm time

TIME_TO_RUN_MOTOR_MORNING defines the time motor should stay on once it is switched on at an alarm

