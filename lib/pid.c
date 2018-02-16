/**
 * pid.c - Super basic PID Fundamentals Library
 */

#pragma systemFile

typedef struct {

    // Configuration Variables
    float Kp;
    float Ki;
    float Kd;

    float maxI; // The maximum allowed integral
    float errorThreshold; // Where error is smaller enough that it should be 0

    // State Variables
    float target; // The target of the PID
    float error;  // How far we currently are from the target
    float integral; // Sum of error
    float derivative; // Most recent error

} pidConfiguration;

float pidCalculate(pidConfiguration & config, int current) {
    config.derivative = config.error;

    config.error = config.target - current;
    config.integral += config.error;

    if (abs(config.error) < config.errorThreshold) {
        config.error = 0;
    }

    // Cap the integral to the specified max value
    if (abs(config.integral) > config.maxI) {
        config.integral = sgn(config.integral) * config.maxI;
    }

    // If the error is zero, integral is unneeded
    if (config.error == 0) {
        config.integral = 0;
    }

    return clamp(
        (config.Kp * config.error) +
        (config.Ki * config.integral) +
        (config.Kd * config.derivative),
    -127, 127);

}

void pidReset(pidConfiguration & config) {
    config.target = 0;
    config.error = 0;
    config.integral = 0;
    config.derivative = 0;
}

void pidConfigure(pidConfiguration & config, int Kp, int Ki, int Kd) {
    config.Kp = Kp;
    config.Ki = Ki;
    config.Kd = Kd;
    return;
}
