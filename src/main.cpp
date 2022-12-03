#include "main.h"

/**
 * 常量、变量定义
 */
const double throwAngle = 58;
const double resetAngle = throwAngle - 3;
/**
 * 设备定义
 */
Controller master(E_CONTROLLER_MASTER);
// 底盘电机
Motor left_f(1);
Motor left_m(5,1);
Motor left_b(6);
Motor right_f(2,1);
Motor right_m(7);
Motor right_b(8,1);
// 投射电机
Motor Push(3);
// Intake电机
Motor Intake(4);
// 传感器
Rotation throwRotation(9);
// 三线设备
ADIDigitalOut Shoot('H');

/**
 * 自定义函数实现
 */
void diPan(double vl, double vr)
{
	// 手柄遥感的输入本就在[-127,127]，不用加pct()
	left_f.move(vl);
	left_m.move(vl);
	left_b.move(vl);
	right_f.move(vr);
	right_m.move(vr);
	right_b.move(vr);
}

void throwReset()
{
	Push.move(pct(100));
	while(throwRotation.get_angle() < throwAngle && throwRotation.get_angle() > resetAngle);
	Push.move(0);
}

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		lcd::set_text(2, "I was pressed!");
	} else {
		lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	lcd::initialize();
	lcd::set_text(1, "Hello PROS User!");

	lcd::register_btn1_cb(on_center_button);

	// 设置投射臂为自锁模式
	Push.set_brake_mode(E_MOTOR_BRAKE_HOLD);
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
void autonomous()
{
	// 自己写的自动函数
	AutoFunc();
}

/**
 * Runs the operator control code. This function will be started in its own task
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
void opcontrol() {

	while (true) {
		// lcd::print(0, "%d %d %d", (lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		//                  (lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		//                  (lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		// 双杆：获取左右遥感值
		double move = master.get_analog(ANALOG_LEFT_Y);
		double rotate = master.get_analog(ANALOG_RIGHT_X);

		// 底盘驱动
		diPan(move + rotate*0.8, move - rotate*0.8);

		// Intake及吐盘控制
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
		{
			Intake.move(pct(100));
		}
		else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
		{
			Intake.move(pct(-100));
		}
		else{
			Intake.move(0);
		}

		// 投盘
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
		{
			Push.move(pct(100));
			while(throwRotation.get_angle() < throwAngle);
			Push.move(0);
			throwReset();
		}

		// 弹射
		if(master.get_digital(pros::E_CONTROLLER_DIGITAL_Y))
		{
			Shoot.set_value(1);
		}
		// 防止主机崩溃
		delay(20);
	}
}
