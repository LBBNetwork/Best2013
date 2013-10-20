#include "Main.h"

void main(void)
{
	int Servo1 = 0;
	int ServoFlagBits = 0;
	int Servo2Test = 0;
	int Openy1 = 0;
	int Openy2 = 0;
	int Openy3 = 0;
	int Tilty1 = 0;
	int Tilty2 = 0;
	int Tilty3 = 0;
	int Tilty4 = 0;
	int Flippy1 = 0;
	int Flippy2 = 0;
	int Flippy3 = 0;
	int Flippy4 = 0;
	int MotorStop = 0;
	int ProgramGo = 0;
	int MotorBringDown = 0;
	unsigned int ServoCount = 0;
	unsigned int MotorPos = 0;

	while(1)
	{
		/* Call the function that brings the arm down for compliance. */
		if(MotorBringDown == 0) /* Run if variable is 0 (false) */
		{
			/* This call runs the function and puts the return value (1) into a variable. */
			MotorBringDown = MotorCompliance(); 
		}

		/* Motor Lock buttons
		 *
		 * Any GetJoystickDigital calls return 1 if pressed or 0 if not pressed. */		 
		ProgramGo = GetJoystickDigital(1,5,1); //Channel 5, Button 1 (DOWN)
		if(ProgramGo == 1)
		{
			MotorStop = 1; //Block all Raisy activity untill this is set to 0
		}

		if(MotorStop == 1)
		{
			/* Run the motor at a small negative value so it stays in place.
			 * this works because of how the arm was balanced in 2013 */
			SetMotor(2,-30);

			/* Checking here to see if we need to unlock Raisy */
			ProgramGo = GetJoystickDigital(1,5,2);
			if(ProgramGo == 1) //If so...
			{
				MotorStop = 0; //...unlock it
			}
		}
		else
		{
			/* Get the value of Joystick channel 3 and return it to an unsigned
			 * int - it ranges from -127 to 127 */
			MotorPos = GetJoystickAnalog(1,3); 
			if(MotorPos > 10) 
			{
				if(MotorPos < 50)
				{
					SetMotor(2,30); //Slow down the motor if MotorPos is greater than 50
				}
				else
				{
					SetMotor(2,MotorPos); //Set the motor speed to whatever is in MotorPos
				}
			}
			else
			{
				SetMotor(2,-30); //Freeze Raisy if MotorPos is less than 10
			}
		}

		/* Extendy. Plain and simple. */
		JoystickToMotor(1,2,5,0);

		/* Code for Openy */
		Openy1 = GetJoystickDigital(1,6,1);
		Openy3 = GetJoystickDigital(1,6,2);

		if(Openy1 == 1)
		{
			SetServo(7,127);
			Openy2 = 1;
		}
		else
		{
			if(Openy2 == 1 && Openy3 == 1)
			{
				SetServo(7,-127);
				Openy2 = 0;
			}
		}

		/* Code for Tilty */
		Tilty1 = GetJoystickDigital(1,7,2);
		Tilty2 = GetJoystickDigital(1,7,3);
		Tilty3 = GetJoystickDigital(1,7,4);

		if(Tilty1 == 1)
		{
			SetServo(6,0);
			Tilty4 = 0;
		}
		else if(Tilty2 == 1)
		{
			SetServo(6,127);
			Tilty4 = 1;
		}
		else if(Tilty3 == 1)
		{
			SetServo(6,-127);
			Tilty4 = 1;
		}
		else
		{

		}

		/* Code for Flippy */
		Flippy1 = GetJoystickDigital(1,8,2);
		Flippy2 = GetJoystickDigital(1,8,3);
		Flippy3 = GetJoystickDigital(1,8,4);

		if(Flippy1 == 1)
		{
			SetServo(3,-127);
			SetServo(4,-64);
			Flippy4 = 1;
		}
		else if(Flippy2 == 1)
		{
			SetServo(3,0);
			SetServo(4,0);
			Flippy4 = 1;
		}
		else if(Flippy3 == 1)
		{
			SetServo(3,127);
			SetServo(4,64);
			Flippy4 = 1;
		}

		/* Unfinished test code. Do not use. */
		//Servo1 = GetJoystickDigital(1,8,2);
		if(ServoFlagBits == 0)
		{
			if(Servo1 == 1)
			{
				for(ServoCount = 0; ServoCount < 128; ServoCount++)
				{
					//SetServo(9,ServoCount);
					//Wait(5);
				}
				//ServoFlagBits = 1;
			}
		}

		//Servo2Test = GetJoystickDigital(1,8,1);
		if(ServoFlagBits == 1)
		{
			if(Servo2Test == 1)
			{
				/*
				 * for(ServoCount = 128; ServoCount --> 0;)
				 * {
				 *	 SetServo(9,ServoCount);
				 * }
				 * ServoFlagBits = 0;
				 */
			}
		}
	}
	//Servo1 = 0;
}

int MotorCompliance(void)
{
	int StopSwitch = 0;
	int ForLoop = 1;
	int ArmEmergencyRelease = 0;
	unsigned int LetsGo = 0;

	/* Bring the motor down at first powerup so the robot is compliant. */
	while(ForLoop) // This is the same while loop as in the main program,
	{              // but is using a variable set that starts set to 1

		/* Get digital input from the switch mounted to the tower.
		 * GetDigitalInput is weird - 1 is unpressed, 0 is pressed */
		StopSwitch = GetDigitalInput(7);
		if(StopSwitch == 1)
		{
			/* Button on the joypad to press in case the above DI doesn't trigger.
			 * Same button as the arm unlock button. */
			ArmEmergencyRelease = GetJoystickDigital(1,5,2);
			if(ArmEmergencyRelease == 1)
			{
				SetServo(6,0);
				ForLoop = 0;
			}

			/* Set various motors and servos to a compliant position */
			SetMotor(2,-90);
			SetServo(4,64);
			SetServo(3,127);
			SetServo(6,127);
			SetServo(7,127);
		}
		else
		{
			LetsGo = GetJoystickAnalog(1,3);
			if(LetsGo) //Move Raisy in any position where it won't be set to 0
			{
				SetServo(6,0); //Set Tilty to the straight position
				ForLoop = 0; //Exit the while loop
			}
			else
			{
				SetMotor(2,-40); // Keep Raisy down to the base until the joystick is moved
			}
		}
	}

	/* Return 1 when this function is called. We need this set to 1 so this init function only runs
	 * once. MotorBringDown gets set to 1 and thus fails the if check when we do this. */
	return 1; 
}