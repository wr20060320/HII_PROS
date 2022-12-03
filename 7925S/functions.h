#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/**
 * 函数定义
 */
// 底盘集成控制
void diPan(double vl, double vr);
// 臂复位
void throwReset();

// 将-127~+127的电压单位转换为百分比单位
#define pct(x) x / 100 * 127
// 秒转换为毫秒
#define sec(x) x * 1000

#endif