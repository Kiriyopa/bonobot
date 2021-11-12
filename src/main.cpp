#include "main.h"
using namespace pros;

//To do: implement controls for back lift, switch lift controls to stick operation
//fix front clamp operation (motor is now in the other orientation)
//autonomous!!!!!!

Controller master (E_CONTROLLER_MASTER); //Connects controller, Motors to port#
Motor frontLeft(8);
Motor frontRight(11); //correct
Motor backLeft(4);
Motor backRight(2); //correct
Motor liftLeft(13);
Motor liftRight(1); //correct
Motor clamp(12); //correct
Motor backLift(7); //correct

class robotState  //naming functions for autonomous, all removed and will return when autonomous is programmed
{
public:
	void leftMotors(int voltage, int reps);
	void rightMotors(int voltage, int reps);
	void forward(int voltage, int reps);
	void backward(int voltage, int reps);
	void raiseFront(int voltage, int reps);
	void lowerFront(int voltage, int reps);
	void raiseBack(int voltage, int reps);
	void lowerBack(int voltage, int reps);
	void clampUp(int voltage, int reps);
	void clampDown(int voltage, int reps);
	void clampStartDown();
public:
	int delay = 1; //initialize delay timedouble clampSpeed = 500;
	bool clampIsDown = false;
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
		liftLeft.move(0);
		liftRight.move(0);
}

void robotState::rightMotors(int voltage, int reps)
{
 for(int i =0; i<reps; i++)
 {
	 frontRight.move(voltage);
	 backRight.move(voltage);
	 pros::delay(delay);
 }
}

void robotState::leftMotors(int voltage, int reps)
{
 for(int i = 0; i<reps; i++)
 {
 frontLeft.move(voltage);
 backLeft.move(voltage);
 pros::delay(delay);
 }
}

void robotState::lowerFront(int voltage, int reps)
{
 for(int i = 0; i < reps; i++)
 {
	 liftLeft.move(voltage);
	 liftRight.move(-voltage);
	 pros::delay(delay);
 }
}

void robotState::raiseFront(int voltage, int reps)
{
 for(int i = 0; i < reps; i++)
 {
	 liftLeft.move(-voltage);
	 liftRight.move(voltage);
	 pros::delay(delay);
 }
}

void robotState::lowerBack(int voltage, int reps)
{
 for(int i = 0; i < reps; i++)
 {
   backLift.move(-voltage);
	 pros::delay(delay);
 }
}

void robotState::raiseBack(int voltage, int reps)
{
 for(int i = 0; i < reps; i++)
 {
	 backLift.move(voltage);
	 pros::delay(delay);
 }
}

void robotState::forward(int voltage, int reps)
{
	 rightMotors(voltage, reps);
	 leftMotors(-voltage, reps);
	 pros::delay(delay);
}

void robotState::backward(int voltage, int reps)
{
 rightMotors(-voltage, reps);
 leftMotors(voltage, reps);
	 pros::delay(delay);
}

void robotState::clampUp(int voltage, int reps)
{
	if(clampIsDown == false){
		clamp.move(0);
	} else {
			for(int i = 0; i < reps; i++)
			{
				clamp.move(voltage);
				pros::delay(1);
			}
		clampIsDown = !clampIsDown;
	}
}

void robotState::clampDown(int voltage, int reps)
{
	if(clampIsDown == true)
	{
		clamp.move(0);
	} else {
		for(int i = 0; i < reps; i++)
		{
			clamp.move(-voltage);
			pros::delay(1);
		}
		clampIsDown = !clampIsDown;
  }
}

void robotState::clampStartDown(){
	clampIsDown == true;
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

robotState robot;

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
		 backLift.set_brake_mode(E_MOTOR_BRAKE_HOLD);
		 //opcontrol; (add when autonomous made)
 }

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

void competition_initialize() {}

//AUTONOMOUS

 void autonomous() {
	 frontLeft.set_brake_mode(E_MOTOR_BRAKE_HOLD);//setting brake mode
	 frontRight.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	 backLeft.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	 backRight.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	 double clampPos = 600;
	 double liftSpeed = 1200;
	 double driveSpeed = 85;
	 int step = 1;
	 while(true){
	 if(step == 1){
	 robot.raiseFront(liftSpeed, 300);
	 stop();
	 pros::delay(300);
	 step = step +1;
 } else if(step == 2){
	 robot.leftMotors(-driveSpeed, 80);
	 robot.rightMotors(driveSpeed+40, 200);
	 stop();
	 pros::delay(300);
	 step = step +1;
 } else if(step == 3){
	 for(int i = 0; i < 20; i++)
	 {
	 clamp.move(20);
  }
	 pros::delay(300);
	 step = step +1;
 } else {
	 stop();
 }
   }

 }

void opcontrol() { //DRIVING MODE
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	controllerState controls;
	double clampPos = 600;
	double clampSpeed = 500;
	double liftSpeed = 100;
	double driveSpeed = 100;

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

		if(controls.uarrow)
		{
			backLift.move(liftSpeed);
		} else if(controls.darrow) {
			backLift.move(-liftSpeed);
		} else {
			backLift.move(0);
		}

		if(controls.rightBumper1)
		{
			robot.clampUp(clampSpeed, clampPos);
		} else if (controls.rightBumper2) {
			robot.clampDown(clampSpeed, clampPos);
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
