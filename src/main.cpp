#include "main.h"
using namespace pros;
Controller master (E_CONTROLLER_MASTER); //Connects controller, Motors to port#
Motor frontLeft(0);
Motor frontRight(0);
Motor backLeft(0);
Motor backRight(0);
Motor liftLeft(0);
Motor liftRight(0);
Motor clamp(0);

class robotState  //Functions to be used with controller
{
public:
	void rightMotors(int voltage, int reps);
	void leftMotors(int voltage, int reps);
	void forward(int voltage, int reps);
	void backward(int voltage, int reps);
	void raise(int voltage, int reps);
	void lower(int voltage, int reps);
	void brakeLeft();
	void brakeRight();
	void clampUp(); // not described or implemented yet
	void clampDown(); // not described or implemented yet
public:
	int delay = 10; //initialize delay time
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
	bool rightBumper1;
  bool leftBumper1;
  bool rightBumper2;
  bool leftBumper2;
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
	uArrow = master.get_digital(DIGITAL_UP);
	dArrow = master.get_digital(DIGITAL_DOWN);
	lArrow = master.get_digital(DIGITAL_LEFT);
	rArrow = master.get_digital(DIGITAL_RIGHT);
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

void setDriveVoltage(int voltL, int voltR){ //values inputted upon initialization. NOTE: Drive motor orientations are wack, edit directions to match.
	frontLeft=voltL;
	frontRight=-voltR;
	backLeft=voltL;
	backRight=-voltR;
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

void robotState::leftMotors(int voltage, int reps) // turn left side wheels
{
	for(int i = 0; i<reps; i++)
	{
	frontLeft.move(voltage);
	backLeft.move(voltage);
	pros::delay(delay);
  }
}

void robotState::rightMotors(int voltage, int reps) // turn right side wheels
{
	for(int i =0; i<reps; i++)
	{
		frontRight.move(voltage);
		backRight.move(voltage);
		pros::delay(delay);
	}
}

void robotState::raise(int voltage, int reps) // raise lift
{
	for(int i = 0; i < reps; i++)
	{
		liftLeft.move(voltage);
		liftRight.move(voltage);
		pros::delay(delay);
	}
	liftLeft.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	liftRight.set_brake_mode(E_MOTOR_BRAKE_HOLD);
}

void robotState::lower(int voltage, int reps) // lower lift
{
	for(int i = 0; i < reps; i++)
	{
		liftLeft.move(-voltage);
		liftRight.move(voltage);
		pros::delay(delay);
	}
	liftLeft.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	liftRight.set_brake_mode(E_MOTOR_BRAKE_HOLD);
}

void robotState::brakeLeft()  // the above drive commands don't stop on their own. This is the stop command.
{
	backLeft.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	frontLeft.set_brake_mode(E_MOTOR_BRAKE_HOLD);
}

void robotState::brakeRight() // right stop
{
	backRight.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	frontRight.set_brake_mode(E_MOTOR_BRAKE_HOLD);
}

void robotState::forward(int voltage, int reps) // drive forward
{

		rightMotors(voltage, reps);
		leftMotors(voltage, reps);
		pros::delay(delay);

	brakeLeft();
	brakeRight();

}

void robotState::backward(int voltage, int reps) // drive backward
{
	rightMotors(-voltage, reps);
	leftMotors(-voltage, reps);
		pros::delay(delay);
	brakeLeft();
	brakeRight();
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 */
 void initialize() {
     pros::lcd::initialize();
     pros::lcd::set_text(1, "BONOBO"); // monke time
     pros::lcd::register_btn1_cb(on_center_button);
     frontLeft.set_brake_mode(E_MOTOR_BRAKE_COAST);//setting brake mode (remove to revert to immediate stops.)
     frontRight.set_brake_mode(E_MOTOR_BRAKE_COAST); // coast = will continue on momentum when button no longer pressed
     backLeft.set_brake_mode(E_MOTOR_BRAKE_COAST);
     backRight.set_brake_mode(E_MOTOR_BRAKE_COAST);
		 //opcontrol; (add when autonomous made)
 }

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
 void autonomous() {}

 /** Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() { //FUNCTIONS TO BUTTONS
	double intakeSpeed = 127;
	double liftSpeed = 127;
	double intakeLiftSpeed = 75;
		pros::Controller master(pros::E_CONTROLLER_MASTER);
	controllerState controls;

	while (true) {
		controls.getControllerState();
		frontLeft.move(controls.leftStick.y);
		backLeft.move(controls.leftStick.y);
		frontRight.move(-controls.rightStick.y);
		backRight.move(-controls.rightStick.y);

		if(controls.rightBumper1)
		{
			intakeLeft.move(intakeSpeed);
			intakeRight.move(-intakeSpeed);
		} else if(controls.rightBumper2) {
			intakeLeft.move(-intakeSpeed);
			intakeRight.move(intakeSpeed);
		}	else {
			intakeLeft.move(0);
			intakeRight.move(0);
			intakeRight.set_brake_mode(E_MOTOR_BRAKE_HOLD);
			intakeLeft.set_brake_mode(E_MOTOR_BRAKE_HOLD);
		}

		if(controls.leftBumper1)
		{
			lift.move(liftSpeed);
		} else if(controls.leftBumper2) {
			lift.move(-liftSpeed);
		} else {
			lift.move(0);
			lift.set_brake_mode(E_MOTOR_BRAKE_HOLD);
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
