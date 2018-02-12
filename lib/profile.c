/**
 * profile.c - Manages motion profiling (based on encoder ticks)
 **/

/**
 * Follows a trapezoidal acceleration curve
 * @param targetTicks The total number of ticks to be achieved by the profile
 * @param int currentTicks The current tick measures
 * @param int dvPeriod The fraction of time for the acceleration and deceleration period
 * @return int multipler A value from 0 to 1 to multiply your PWM by
 **/
int profileTrapezoid(int currentTicks, int targetTicks, int dvPeriod) {
    int position = currentTicks / targetTicks;

    // Acceleration Period: For dvPeriod of the motion
    if (position < dvPeriod) {
        return position * (1 / dvPeriod);
    // Maintain Period: For (1 - 2 dvPeriod) of the motion
    } else if (position <= dvPeriod && position >= (1 - dvPeriod)) {
        return 1;
    // Deceleration Period: For dvPeriod of the motion
    } else if (position < (1 - dvPeriod)) {
        return (position - 1) * -(1 / dvPeriod);
    }

    // In the strange occurance that none of the above conditions activate, eliminate the multiplier
    return 1;
} 