#ifndef MODAPTO_CFP_HPP
#define MODAPTO_CFP_HPP

#include <fmi2FunctionTypes.h>

#define LOG_CATEGORY_EVENTS "logEvents"

class LoggingCategories {
public:
    fmi2Boolean events;
};


class ModaptoCarbonFootprint {
public:
    ModaptoCarbonFootprint(
        fmi2String instanceName,
        fmi2CallbackLogger loggerCallback,
        fmi2CallbackAllocateMemory alloc,
        fmi2CallbackFreeMemory free,
        fmi2ComponentEnvironment environment,
        fmi2StepFinished stepFinished,
        fmi2Boolean loggingOn
    );
    void resetTime(fmi2Real startTime);
    void resetValues();
    void increaseTime(fmi2Real h);

    void log(const char* category, const char* msg, fmi2Status status) const;
    void log(const char* category, const char* msg) const;
    
    LoggingCategories loggingCategories;

    // Some environment pointers
    fmi2String instanceName;
    fmi2CallbackLogger loggerCallback;
    fmi2CallbackAllocateMemory memAlloc;
    fmi2CallbackFreeMemory memFree;
    fmi2ComponentEnvironment environment;
    fmi2StepFinished stepFinished;
    fmi2Boolean loggingOn;

    // Inputs and outputs as defined
    fmi2Real time, tStart, tStop;
    fmi2Real power, cMix, dMass, cummMass, eTot;
    fmi2Boolean start, stop, started, stopped;

private:
    // Some internal memory
    fmi2Boolean lastStart, lastStop;

    bool isPositiveEdge(fmi2Boolean lastValue, fmi2Boolean newValue) const;
    void integrationStep(fmi2Real h, fmi2Real& x, const fmi2Real& dx) const;
};

#endif
