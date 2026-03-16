#include <fmi2Functions.h>
#include <helper.h>

fmi2Status fmi2EnterContinuousTimeMode(fmi2Component instance) {

    UNUSED(instance)
    return fmi2Error;
}

fmi2Status fmi2CompletedIntegratorStep(fmi2Component instance,
    fmi2Boolean  noSetFMUStatePriorToCurrentPoint,
    fmi2Boolean* enterEventMode,
    fmi2Boolean* terminateSimulation) {

    UNUSED(instance)
    UNUSED(noSetFMUStatePriorToCurrentPoint)
    UNUSED(enterEventMode)
    UNUSED(terminateSimulation)

    return fmi2Error;
}

/* Providing independent variables and re-initialization of caching */
fmi2Status fmi2SetTime(fmi2Component instance, fmi2Real time) {

    UNUSED(instance)
    UNUSED(time)

    return fmi2Error;
}

fmi2Status fmi2SetContinuousStates(fmi2Component instance,
    const fmi2Real continuousStates[],
    size_t nContinuousStates) {

    UNUSED(instance)
    UNUSED(continuousStates)
    UNUSED(nContinuousStates)

    return fmi2Error;
}

/* Evaluation of the model equations */
fmi2Status fmi2GetContinuousStateDerivatives(fmi2Component instance,
    fmi2Real derivatives[],
    size_t nContinuousStates) {

    UNUSED(instance)
    UNUSED(derivatives)
    UNUSED(nContinuousStates)

    return fmi2Error;
}

fmi2Status fmi2GetEventIndicators(fmi2Component instance,
    fmi2Real eventIndicators[],
    size_t nEventIndicators) {

    UNUSED(instance)
    UNUSED(eventIndicators)
    UNUSED(nEventIndicators)

    return fmi2Error;
}

fmi2Status fmi2GetContinuousStates(fmi2Component instance,
    fmi2Real continuousStates[],
    size_t nContinuousStates) {

    UNUSED(instance)
    UNUSED(continuousStates)
    UNUSED(nContinuousStates)

    return fmi2Error;
}

fmi2Status fmi2GetNominalsOfContinuousStates(fmi2Component instance,
    fmi2Real nominals[],
    size_t nContinuousStates) {

    UNUSED(instance)
    UNUSED(nominals)
    UNUSED(nContinuousStates)

    return fmi2Error;
}

fmi2Status fmi2GetNumberOfEventIndicators(fmi2Component instance,
    size_t* nEventIndicators) {

    UNUSED(instance)
    UNUSED(nEventIndicators)

    return fmi2Error;
}

fmi2Status fmi2GetNumberOfContinuousStates(fmi2Component instance,
    size_t* nContinuousStates) {

    UNUSED(instance)
    UNUSED(nContinuousStates)

    return fmi2Error;
}

fmi2Status fmi2GetOutputDerivatives(fmi2Component c,
    const fmi2ValueReference valueReferences[],
    size_t nValueReferences,
    const fmi2Integer orders[],
    fmi2Real values[],
    size_t nValues) {

    UNUSED(c)
    UNUSED(valueReferences)
    UNUSED(nValueReferences)
    UNUSED(orders)
    UNUSED(values)
    UNUSED(nValues)

    return fmi2Error;
}
