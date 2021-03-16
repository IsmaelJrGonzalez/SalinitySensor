#include <conductivity.h> // smu library
#include <Servo.h>

/*
 * these pins are used by the SMU arduino conductivity library, do not use them
 *
 * pin - use

 * A2  - reading the voltage off the copper plate #1
 * A3  - reading the voltage off the copper plate #2
 *
 *  2  - simulating an AC current across copper plate #1
 *  3  - simulating an AC current across copper plate #2
 */
 
 // determines if print statements are compiled and ran
#define DBG_VERBOSE 1  

// motor pins
Servo servo;					// motor for droping and raising conductivity & temperature probes
const unsigned int motorPin = 3;		// the pwm pin that allows the arduino to communicate with the motor
const unsigned int controllerPin = A0;		// dictates how fast or slow the motor spins and what direction
unsigned int currentState = 90;			// speed/direction of motor 90 = still, <90 = move <-, >90 = move ->

// temperature probe pins
const unsigned temperaturePin = A1;		// allows use to read voltage from the temperature voltage divider

void setup() 
{
	Serial.begin(9600);

	// setup motor: 
	// tells arduino that there is a motor on pin 3
	// tell motor not to move
	servo.attach(motorPin);
	servo.write(currentState);
}

/*
 * updates speed/direction of the motor based on the ADC code of the controllerPin
 *
 * controllerPin:
 *  the pin whom's adc code dictates the speed/direction of the motor
 *
 * servo:
 * 	the motor that needs to be moved/updated
 *
 * currentState:
 *	the current state of the motor
 */

void updateMotor(const unsigned int controllerPin, Servo& servo, unsigned int& currentState)
{
	// read the adc code from the potentiometer
	// converting it to number from 0 - 180, which represents speed/direction
	float newState = analogRead(controllerPin);	// get the voltage, on a 5v 0 - 1023 scale
	newState /= 1023;				// convert that voltage into a ratio
	newState *= 180;				// convert that ratio into a state

	// debuging print statements
#if DBG_VERBOSE
	Serial.print("New State: ");
	Serial.print(newState);
	Serial.print("\t");
#endif

	// update the servo to the new state if, and only if, the two states are different
	if(newState != currentState)
	{
		servo.write(currentState);
		currentState = currentState;
		delay(200);
	}
}

void loop() 
{
#if DBG_VERBOSE
	// temperature probe readings
	Serial.print("Temperature Probe: ");
	Serial.print(analogRead(temperaturePin)); // NOTE: this should be getTemp, but we just need an ADC code this sprint
	Serial.print("\t");

	// conductivity probe
	Serial.print("Conductivity Probe: ");
	Serial.print(getConductivity());
	Serial.print("\t");

	// Motor state
	Serial.print("Motor: ");
	Serial.print(currentState);
	Serial.print("\t");
#endif

	updateMotor(controllerPin, servo, currentState);

#if DBG_VERBOSE
	Serial.print("\n");
#endif
}
