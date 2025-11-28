#include "tpl_os.h"
#include "Arduino.h"

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
	GetResource(resource);
	Serial.println("taskA holds resource");	
	do_things(200);
	Serial.println("taskA releases resource");	
	ReleaseResource(resource);
	TerminateTask();

}

TASK(taskB)
{
	Serial.println("taskB starts");	
	do_things(700);
	Serial.println("taskB ends");	
	TerminateTask();
	
}

TASK(taskC)
{
	int start = millis();
	do_things(100);
	GetResource(resource);
	Serial.println("taskC holds resource");	
	do_things(200);
	Serial.println(millis()-start);
	Serial.println("taskC releases resource");
	ReleaseResource(resource);
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
