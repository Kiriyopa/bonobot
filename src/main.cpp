#include "main.h"
#include "cmath"
using namespace pros;

Controller master (E_CONTROLLER_MASTER); //Connects controller, Motors to port#
Motor frontLeft(8);
Motor frontRight(11); //correct
Motor backLeft(4);
Motor backRight(2); //correct
Motor liftLeft(13);
Motor liftRight(1); //correct
Motor clamp(14); //correct
Motor backLift(7); //correct
IMU gyro(19);

class robotState  //naming functions for autonomous, all removed and will return when autonomous is programmed
{
public:
	void leftMotors(int voltage, int reps);
	void rightMotors(int voltage, int reps);

	void setClampUp();
	void setClampDown();
public:
	void turnClockwise(int voltage, int reps);
	void turnCounterclockwise(int voltage, int reps);
	void turn(); // clockwise positive, counterclockwise negative;
	void forward(int voltage, int reps);
	void backward(int voltage, int reps);

	void raiseFront(int voltage, int reps);
	void lowerFront(int voltage, int reps);

	void raiseBack(int voltage, int reps);
	void lowerBack(int voltage, int reps);

	void clampUp(int voltage, int reps);
	void clampDown(int voltage, int reps);

	void turnTo(int voltage, double direction);
public:
	int delay = 1; //initialize delay time
	bool clampIsDown = false; //FIGURE OUT A WAY TO CHANGE IT DEPENDING ON AUTON OR DRIVE START
	double orientation = gyro.get_yaw()*(358.0/360.0);
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
		backLift.move(0);
}

robotState robot;

void robotState::setClampUp(){
	clampIsDown = false;
}

void robotState::setClampDown(){
	clampIsDown = true;
}

void robotState::rightMotors(int voltage, int reps)
{
 for(int i = 0; i < reps; i++)
 {
	 frontRight.move(voltage);
	 backRight.move(voltage);
	 pros::delay(delay);
 }
}

void robotState::leftMotors(int voltage, int reps)
{
 for(int i = 0; i < reps; i++)
 {
 frontLeft.move(-voltage);
 backLeft.move(-voltage);
 pros::delay(delay);
 }
}

void robotState::forward(int voltage, int reps)
{
	for(int i = 0; i < reps; i++)
	{
	frontLeft.move(-voltage);
	frontRight.move(voltage);
	backLeft.move(-voltage);
	backRight.move(voltage);
	pros::delay(delay);
	}
}

void robotState::backward(int voltage, int reps)
{
	for(int i = 0; i < reps; i++)
  {
  frontLeft.move(voltage);
  frontRight.move(-voltage);
  backLeft.move(voltage);
  backRight.move(-voltage);
  pros::delay(delay);
  }
}

void robotState::turnTo(int voltage, double direction){
	//while(robot.orientation > (direction + 10.0) || (robot.orientation < (direction - 10.0))){
	while(true){
		if(abs(gyro.get_yaw()*(358.0/360.0)-direction) > 5.0){ //upload this and make sure it works!
			break;
   	}else if(gyro.get_yaw()*(358.0/360.0) > direction){
			robot.turnCounterclockwise(voltage, 1);
		}else if(gyro.get_yaw()*(358.0/360.0) < direction){
			robot.turnClockwise(voltage, 1);
		}
	}
}

void robotState::turnClockwise(int voltage, int reps)
{
	for(int i = 0; i < reps; i++){
	frontLeft.move(-voltage);
	backLeft.move(-voltage);
	frontRight.move(-voltage);
	backRight.move(-voltage);
	pros::delay(delay);
	}
}

void robotState::turnCounterclockwise(int voltage, int reps){
	for(int i = 0; i < reps; i++)
	{
	frontLeft.move(voltage);
	backLeft.move(voltage);
	frontRight.move(voltage);
	backRight.move(voltage);
	pros::delay(delay);
}
}

void robotState::lowerFront(int voltage, int reps){
 for(int i = 0; i < reps; i++){
	 liftLeft.move(voltage);
	 liftRight.move(-voltage);
	 pros::delay(delay);
 }
}

void robotState::raiseFront(int voltage, int reps){
 for(int i = 0; i < reps; i++){
	 liftLeft.move(-voltage);
	 liftRight.move(voltage);
	 pros::delay(delay);
 }
}

void robotState::lowerBack(int voltage, int reps){
 for(int i = 0; i < reps; i++)
 {
   backLift.move(-voltage);
	 pros::delay(delay);
 }
}

void robotState::raiseBack(int voltage, int reps){
 for(int i = 0; i < reps; i++){
	 backLift.move(voltage);
	 pros::delay(delay);
 }
}

void robotState::clampUp(int voltage, int reps){
	if(clampIsDown == false){
		clamp.move(0);
	} else {
			for(int i = 0; i < reps; i++){
				clamp.move(voltage);
				pros::delay(1);
			}
		clamp.move(0);
		clampIsDown = !clampIsDown;
	}
}

void robotState::clampDown(int voltage, int reps){
	if(clampIsDown == true){
		clamp.move(0);
	} else {
		for(int i = 0; i < reps; i++){
			clamp.move(-voltage);
			pros::delay(1);
		}
		clamp.move(0);
		clampIsDown = !clampIsDown;
  }
}

void on_center_button() { // funny
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
     // monke time
     pros::lcd::register_btn1_cb(on_center_button);
     frontLeft.set_brake_mode(E_MOTOR_BRAKE_COAST);//setting brake mode
     frontRight.set_brake_mode(E_MOTOR_BRAKE_COAST);
     backLeft.set_brake_mode(E_MOTOR_BRAKE_COAST);
     backRight.set_brake_mode(E_MOTOR_BRAKE_COAST);
		 liftLeft.set_brake_mode(E_MOTOR_BRAKE_HOLD);
		 liftRight.set_brake_mode(E_MOTOR_BRAKE_HOLD);
		 clamp.set_brake_mode(E_MOTOR_BRAKE_BRAKE);
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
 int clampPos = 200;
 int liftSpeed = 127;
 int driveSpeed = 85;
 int turnSpeed = 20;
 int clampSpeed = 110;
 delay(3000);
 robot.turnTo(turnSpeed, 90.0);

 //robot.raiseFront(liftSpeed, 250);
// robot.turnClockwise(driveSpeed, 50);
 //robot.forward(driveSpeed, 230);
 //stop();
 //pros::delay(300);
 //robot.setClampDown();
 //robot.clampUp(clampSpeed, clampPos+400);
// pros::delay(300);
// robot.backward(driveSpeed, 200);
 //stop();
 //robot.lowerFront(liftSpeed, 500);
 //stop();
 //robot.turnClockwise(driveSpeed, 390);
 //stop();
 //robot.lowerBack(liftSpeed, 940);
 //stop();
// robot.backward(driveSpeed, 1400);
// stop();
 //robot.raiseBack(liftSpeed, 870);
 //stop();
 //robot.forward(driveSpeed, 1200);
 //stop();
}

void opcontrol() { //DRIVING MODE
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	controllerState controls;
	int clampPos = 500;
	int clampSpeed = 127;
	int liftSpeed = 127;
	int driveSpeed = 100;
	robot.setClampUp();

	while (true) {
		pros::lcd::set_text(1, std::to_string(gyro.get_yaw()));
    //pros::lcd::set_text(2, std::to_string(frontLeft.FUNCTION TO READ EncoderData()));
		clamp.set_brake_mode(E_MOTOR_BRAKE_COAST);
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
