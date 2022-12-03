#include "7925S/auto.h"

void AutoFunc()
{
    diPan(pct(100), pct(100));
    delay(sec(1));
    diPan(0, 0);
}