#include "chargeutils.h"

int computeCurrent(int secs, int chargeBefore, int chargeAfter)
{
    if (secs == 0)
        return -1;
    return ((chargeBefore - chargeAfter) * 36) / (10 * secs);
}

int computeCharge(int secs, int current, int chargeBefore)
{
    return chargeBefore - (secs * current) / 3600;
}
