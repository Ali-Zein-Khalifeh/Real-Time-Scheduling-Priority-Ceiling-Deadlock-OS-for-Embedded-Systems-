#include "tpl_os.h"
#include "Arduino.h"

unsigned long start ;
unsigned long delta ;
void setup()
{
	Serial.begin(115200);
}

void do_things(int ms) 
{ 
	unsigned long mul = ms * 504UL; 
	unsigned long i; 
	for(i=0; i<mul; i++) millis(); 
} 

TASK(taskA)
{
	do_things(200);	
	TerminateTask();
}

TASK(taskB)
{
	do_things(700);
	TerminateTask();
}

TASK(taskC)
{
	start = millis();
	do_things(300);
	delta=millis() - start;
	Serial.println(delta);
	TerminateTask();
}

TASK(taskSTOP)
{
	CancelAlarm(alarmA);
  	CancelAlarm(alarmB);
	CancelAlarm(alarmC);
	Serial.print("shutdown");
  	TerminateTask();
}
