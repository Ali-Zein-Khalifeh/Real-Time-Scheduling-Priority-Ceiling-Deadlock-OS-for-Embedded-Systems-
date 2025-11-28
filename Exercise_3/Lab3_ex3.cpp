#include "tpl_os.h"
#include "Arduino.h"

static bool M;
static bool L;


void setup()
{
    Serial.begin(115200);
}

void do_things(int ms)
{
    unsigned long mul = ms * 504UL;
    unsigned long i;
    for (i = 0; i < mul; i++) millis();
}

TASK(taskI)
{
	if (millis() < 200) {
		M = true;
		SendMessage(from_taskI, &M);
	} else {
		if (ReceiveMessage(taskI_receive_from_A, &M)) {
			SendMessage(from_taskI, &M);
		}
		if (ReceiveMessage(taskI_receive_from_C, &M)) {
			SendMessage(from_taskI, &M);
		}
	}
	TerminateTask();
}

TASK(taskA)
{
	ReceiveMessage(taskA_receive, &M);
    while (!M) {}
	ReceiveMessage(taskA_receive_from_C, &L);
	while (M && !L) {
		SendMessage(from_taskA, &M);
		Serial.println("taskA starts");
		do_things(200);  // Critical section
		Serial.println("taskA ends");

		// Leave the critical section
		M = false;
	}
	M = true;
	SendMessage(from_taskA, &M);
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
    Serial.println("taskC non-critical starts");
    do_things(100);  // Non-critical section
    Serial.println("taskC non-critical ends");

	ReceiveMessage(taskC_receive, &M);
    while (!M) {}

    // Enter the critical section
	
	while (M) {
		L = true;
		SendMessage(from_taskC_to_A, &L);
		Serial.println("taskC critical starts");
		do_things(200);  // Critical section
		Serial.println("taskC critical ends");

		// Leave the critical section
		M = false;
		SendMessage(from_taskC, &M);
	}
	L = false;
	M = true;
	SendMessage(from_taskC, &M);
	SendMessage(from_taskC_to_A, &L);
    TerminateTask();
}

TASK(taskSTOP)
{
    Serial.println("Shutdown");
	CancelAlarm(alarmA);
	CancelAlarm(alarmB);
	CancelAlarm(alarmC);

    TerminateTask();
}
