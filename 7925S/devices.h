#ifndef DEVICES_H
#define DEVICES_H

#include "api.h"

using namespace pros;

/**
 * 设备定义
 */
// 底盘电机
extern Motor left_f;
extern Motor left_m;
extern Motor left_b;
extern Motor right_f;
extern Motor right_m;
extern Motor right_b;
// 投射电机
extern Motor Push;
// Intake电机
extern Motor Intake;
// 传感器
extern Rotation throwRotation;
// 三线设备
extern ADIDigitalOut Shoot;

#endif