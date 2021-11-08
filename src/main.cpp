#include "main.h"
using namespace pros;
Controller master (E_CONTROLLER_MASTER); //Connects controller, Motors to port#
Motor frontLeft(4);
Motor frontRight(11);
Motor backLeft(7);
Motor backRight(8);
Motor liftLeft(1);
Motor liftRight(2);
Motor clamp(12);

class robotState  //naming functions for autonomous, all removed and will return when autonomous is programmed
{
public:
	void leftMotors(int voltage, int reps);
	void rightMotors(int voltage, int reps);
	void forward(int voltage, int reps);
	void backward(int voltage, int reps);
	void raise(int voltage, int reps);
	void lower(int voltage, int reps);
	void brakeLeft();
	void brakeRight();
	void clampUp(int voltage, int reps);
	void clampDown(int voltage, int reps);
public:
	int delay = 1; //initialize delay time
};

class Vec2d //for stick inputs
{
public:
	int x;
	int y;
public:
	Vec2d(int one,int two);
};

Vec2d::Vec2d(int one, int two)
{
	x = one;
	y = two;
}
class controllerState  //vars for button inputs
{
public:
	void getControllerState();
	void drive();
	controllerState();
public:
	bool leftBumper1;
	bool rightBumper1;
	bool leftBumper2;
  bool rightBumper2;
	Vec2d leftStick;
	Vec2d rightStick;
	bool a;
	bool b;
	bool y;
	bool x;
	bool larrow;
	bool rarrow;
	bool uarrow;
	bool darrow;
};

void controllerState::getControllerState()  //naming controller buttons
{
  leftBumper1 = master.get_digital(DIGITAL_L1);
	rightBumper1 = master.get_digital(DIGITAL_R1);
	leftBumper2 = master.get_digital(DIGITAL_L2);
	rightBumper2 = master.get_digital(DIGITAL_R2);
	a = master.get_digital_new_press(DIGITAL_A); //new_press to prevent spam
	b = master.get_digital_new_press(DIGITAL_B);
	y = master.get_digital_new_press(DIGITAL_Y);
	x = master.get_digital_new_press(DIGITAL_X);
	uarrow = master.get_digital(DIGITAL_UP);
	darrow = master.get_digital(DIGITAL_DOWN);
	larrow = master.get_digital(DIGITAL_LEFT);
	rarrow = master.get_digital(DIGITAL_RIGHT);
	leftStick.x = master.get_analog(ANALOG_LEFT_X);
	leftStick.y = master.get_analog(ANALOG_LEFT_Y);
	rightStick.x = master.get_analog(ANALOG_RIGHT_X);
	rightStick.y = master.get_analog(ANALOG_RIGHT_Y);
}

controllerState::controllerState()  // initializing all controller button values to false (not pressed)
: leftStick(0,0), rightStick(0,0)
{
	rightBumper1 = 0;
	leftBumper1 = 0;
	rightBumper2 = 0;
	leftBumper2 = 0;
	 a = 0;
	 b = 0;
	 y = 0;
	 x = 0;
	 rarrow = 0;
	 uarrow = 0;
	 darrow = 0;
	 larrow = 0;
}

 void stop(){                   //stop function (for autonomous)
    frontLeft.move(0);
    frontRight.move(0);
    backRight.move(0);
    backLeft.move(0);
}

void on_center_button() { // callback function from template (does the text actually display?)
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "Monkey Mode");
	} else {
		pros::lcd::clear_line(2);
	}
}


/**
 * Runs initialization code. This occurs as soon as the program is started.
 */
 void initialize() {
     pros::lcd::initialize();
     pros::lcd::set_text(1, "BONOBO"); // monke time
     pros::lcd::register_btn1_cb(on_center_button);
     frontLeft.set_brake_mode(E_MOTOR_BRAKE_COAST);//setting brake mode
     frontRight.set_brake_mode(E_MOTOR_BRAKE_COAST);
     backLeft.set_brake_mode(E_MOTOR_BRAKE_COAST);
     backRight.set_brake_mode(E_MOTOR_BRAKE_COAST);
		 liftLeft.set_brake_mode(E_MOTOR_BRAKE_HOLD);
		 liftRight.set_brake_mode(E_MOTOR_BRAKE_HOLD);
		 clamp.set_brake_mode(E_MOTOR_BRAKE_HOLD);
		 //opcontrol; (add when autonomous made)
 }

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

void competition_initialize() {}

 void autonomous() {
	 frontLeft.set_brake_mode(E_MOTOR_BRAKE_HOLD);//setting brake mode
	 frontRight.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	 backLeft.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	 backRight.set_brake_mode(E_MOTOR_BRAKE_HOLD);
 }

void opcontrol() { //DRIVING MODE
	double liftSpeed = 127;
	double clampSpeed = 200;
	double clampPos = 125;
	bool clampDown = false;
		pros::Controller master(pros::E_CONTROLLER_MASTER);
	controllerState controls;

	while (true) {
		controls.getControllerState();
		frontLeft.move(-controls.leftStick.y);
		backLeft.move(-controls.leftStick.y);
		frontRight.move(controls.rightStick.y);
		backRight.move(controls.rightStick.y);

		if(controls.leftBumper1)
		{
			liftLeft.move(-liftSpeed);
			liftRight.move(liftSpeed);
		} else if(controls.leftBumper2) {
			liftLeft.move(liftSpeed);
			liftRight.move(-liftSpeed);
		} else {
			liftLeft.move(0);
			liftRight.move(0);
		}

		if(controls.rightBumper1)
		{
			if(clampDown == false){
				clamp.move(0);
			} else {
					for(int i = 0; i < clampPos; i++)
					{
						clamp.move(-clampSpeed);
						pros::delay(1);
					}
				clampDown = !clampDown;
			}
		} else if (controls.rightBumper2) {
			if(clampDown == true)
			{
				clamp.move(0);
			} else {
				for(int i = 0; i < clampPos; i++)
				{
					clamp.move(clampSpeed);
					pros::delay(1);
				}
			  clampDown = !clampDown;
			}
		} else {
			clamp.move(0);
		}

		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		int left = master.get_analog(ANALOG_LEFT_Y);
		int right = master.get_analog(ANALOG_RIGHT_Y);

		pros::delay(10);
	}
}

int main()
{
	opcontrol;  //run manual operation
	return 0;
}
