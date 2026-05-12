#include <fmi2Functions.h>
#include <helper.h>

fmi2Status fmi2EnterEventMode(fmi2Component instance) {
    UNUSED(instance)
    return fmi2Error;
}

// fmi2Status fmi2EvaluateDiscreteStates(fmi2Component instance) {
//     UNUSED(instance)
//     return fmi2Error;
// }

// fmi2Status fmi2UpdateDiscreteStates(fmi2Component instance,
//     fmi2Boolean* discreteStatesNeedUpdate,
//     fmi2Boolean* terminateSimulation,
//     fmi2Boolean* nominalsOfContinuousStatesChanged,
//     fmi2Boolean* valuesOfContinuousStatesChanged,
//     fmi2Boolean* nextEventTimeDefined,
//     fmi2Float64* nextEventTime) {

//     UNUSED(instance)
//     UNUSED(discreteStatesNeedUpdate)
//     UNUSED(terminateSimulation)
//     UNUSED(nominalsOfContinuousStatesChanged)
//     UNUSED(valuesOfContinuousStatesChanged)
//     UNUSED(nextEventTimeDefined)
//     UNUSED(nextEventTime)

//     return fmi2Error;
// }

// fmi2Status fmi2EnterStepMode(fmi2Component instance) {

//     UNUSED(instance)
//     return fmi2Error;
// }
