#include <modapto-cfp.hpp>
#include <math.h>

ModaptoCarbonFootprint::ModaptoCarbonFootprint(
    fmi2String instanceName,
    fmi2CallbackLogger loggerCallback,
    fmi2CallbackAllocateMemory alloc,
    fmi2CallbackFreeMemory free,
    fmi2ComponentEnvironment environment,
    fmi2StepFinished stepFinished,
    fmi2Boolean loggingOn) : instanceName(instanceName),
                             loggerCallback(loggerCallback),
                             memAlloc(alloc),
                             memFree(free),
                             environment(environment),
                             stepFinished(stepFinished),
                             loggingOn(loggingOn)
{
    loggingCategories.events = loggingOn;
    resetValues();
}

void ModaptoCarbonFootprint::resetValues()
{
    tStart = tStop = 0;
    power = cMix = dMass = cummMass = eTot = 0;
    start = stop = started = stopped = lastStart = lastStop = false;
}

void ModaptoCarbonFootprint::resetTime(fmi2Real startTime)
{
    time = startTime;
}

void ModaptoCarbonFootprint::increaseTime(fmi2Real h)
{
    dMass = power * cMix;

    if (isPositiveEdge(lastStart, start))
    {
        log(LOG_CATEGORY_EVENTS, "Positive edge on start input detected.");

        tStart = tStop = time;
        started = true;
        stopped = false;
        cummMass = 0;
        eTot = 0;
    }
    if (isPositiveEdge(lastStop, stop))
    {
        log(LOG_CATEGORY_EVENTS, "Positive edge on stop input detected.");

        tStop = time;
        stopped = true;
    }

    if (started && !stopped)
    {
        integrationStep(h, cummMass, dMass);
        integrationStep(h, eTot, power);
    }

    lastStart = start;
    lastStop = stop;
}

void ModaptoCarbonFootprint::log(const char *category, const char *msg, fmi2Status status) const
{
    (*loggerCallback)(environment, instanceName, status, category, msg);
}

void ModaptoCarbonFootprint::log(const char *category, const char *msg) const
{
    log(category, msg, fmi2OK);
}
