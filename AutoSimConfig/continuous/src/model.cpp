#include <fmi2Functions.h>
#include <modapto-cfp.hpp>
#include <helper.h>
#include <cstring>
#include <cstdio>
#include <variables.hpp>

#ifdef __cplusplus
extern "C" {
#endif

using namespace EKS::Modapto::CarbonFootprint;

#define INST ((ModaptoCarbonFootprint*) c)

const char* fmi2GetVersion() {
    return fmi2Version;
}

const char* fmi2GetTypesPlatform(){
    return fmi2TypesPlatform;
}

static void logInInstantiation(fmi2String instanceName, fmi2Status status, const fmi2CallbackFunctions* functions, const char* msg){
    if(functions->logger) {
        functions->logger(functions->componentEnvironment, instanceName, status, "NULL", "%s", msg);
    }
}

static void logErrorInInstantiation(fmi2String instanceName, const fmi2CallbackFunctions* functions, const char* msg){
    logInInstantiation(instanceName, fmi2Fatal, functions, msg);
}

fmi2Component fmi2Instantiate(  fmi2String instanceName,
                                    fmi2Type fmuType,
                                    fmi2String fmuGUID,
                                    fmi2String fmuResourceLocation,
                                    const fmi2CallbackFunctions* functions,
                                    fmi2Boolean visible,
                                    fmi2Boolean loggingOn)
{
    // UNUSED(instanceName)
    UNUSED(fmuResourceLocation)
    UNUSED(visible)
    // UNUSED(requiredIntermediateVariables)
    // UNUSED(nRequiredIntermediateVariables)
    // UNUSED(intermediateUpdate)

    if (!fmuGUID || strlen(fmuGUID) == 0) {
        logErrorInInstantiation(instanceName, functions, "No GUID was provided by simulation environment.");
        return NULL;
    }

    if (strcmp(fmuGUID, modelToken)) {
        logErrorInInstantiation(instanceName, functions, "The GUID does not match between implementation and manifest.");
        return NULL;
    }

    if(fmuType != fmi2CoSimulation){
        logErrorInInstantiation(instanceName, functions, "Only Co-Simulation is provided.");
        return NULL;
    }

    return new ModaptoCarbonFootprint(
        instanceName,
        functions->logger,
        functions->allocateMemory,
        functions->freeMemory,
        functions->componentEnvironment,
        functions->stepFinished,
        loggingOn
    );
}

void fmi2FreeInstance(fmi2Component c) {
    delete INST;
}

fmi2Status fmi2Reset(fmi2Component c) {
    INST->log(LOG_CATEGORY_EVENTS, "fmi2Reset");
    INST->resetValues();
    return fmi2OK;
}

fmi2Status fmi2Terminate(fmi2Component c)
{
    UNUSED(c)
    INST->log(LOG_CATEGORY_EVENTS, "fmi2Terminate");
    return fmi2OK;
}

fmi2Status fmi2SetDebugLogging(fmi2Component c, fmi2Boolean loggingOn, size_t nCat, const fmi2String categories[])
{
    fmi2Status ret = fmi2OK;
    if(nCat == 0) {
        INST->loggingCategories.events = loggingOn;
    } else {
        for(size_t i=0; i<nCat; i++){
            if(strcmp(categories[i], "logEvents") == 0) {
                INST->loggingCategories.events = loggingOn;
            } else {
                ret = fmi2Warning;
            }
        }
    }
    return ret;
}


fmi2Status fmi2SetupExperiment(fmi2Component c,
                                fmi2Boolean toleranceDefined,
                                fmi2Real tolerance,
                                fmi2Real startTime,
                                fmi2Boolean stopTimeDefined,
                                fmi2Real stopTime)
{
    UNUSED(toleranceDefined)
    UNUSED(tolerance)
    UNUSED(stopTimeDefined)
    UNUSED(stopTime)

    INST->log(LOG_CATEGORY_EVENTS, "fmi2SetupExperiment");
    INST->resetTime(startTime);
    return fmi2OK;
}

fmi2Status fmi2EnterInitializationMode(fmi2Component c)
{
    INST->log(LOG_CATEGORY_EVENTS, "fmi2EnterInitializationMode");
    return fmi2OK;
}

fmi2Status fmi2ExitInitializationMode(fmi2Component c)
{
    INST->log(LOG_CATEGORY_EVENTS, "fmi2ExitInitializationMode");
    return fmi2OK;
}






fmi2Status fmi2GetReal(fmi2Component c,
                            const fmi2ValueReference valueReferences[],
                            size_t nValueReferences,
                            fmi2Real values[])
{
    INST->log(LOG_CATEGORY_EVENTS, "fmi2GetReal");

    for(size_t i=0; i<nValueReferences; i++) {
        switch(valueReferences[i]) {
            case measurement_t_start:
                values[i] = INST->tStart;
                break;
            case measurement_t_stop:
                values[i] = INST->tStop;
                break;
            case p:
                values[i] = INST->power;
                break;
            case cMix:
                values[i] = INST->cMix;
                break;
            case dm_CO2:
                values[i] = INST->dMass;
                break;
            case m_Total:
            case m_Meas:
                values[i] = INST->cummMass;
                break;
            case E_Tot:
                values[i] = INST->eTot;
                break;
            default:
                return fmi2Error;
        }
    }
    return fmi2OK;
}

fmi2Status fmi2SetReal(fmi2Component c,
                                      const fmi2ValueReference valueReferences[],
                                      size_t nValueReferences,
                                      const fmi2Real values[])
{
    INST->log(LOG_CATEGORY_EVENTS, "fmi3SetFloat32");

    for(size_t i=0; i<nValueReferences; i++) {
        char buf[20];
        snprintf(buf, 20, "valueRef = %d", valueReferences[i]);
        INST->log(LOG_CATEGORY_EVENTS, buf);

        switch(valueReferences[i]) {
            case measurement_t_start:
                INST->tStart = values[i];
                break;
            case measurement_t_stop:
                INST->tStop = values[i];
                break;
            case p:
                INST->power = values[i];
                break;
            case cMix:
                INST->cMix = values[i];
                break;
            case dm_CO2:
                INST->dMass = values[i];
                break;
            case m_Meas:
                INST->cummMass = values[i];
                break;
            case E_Tot:
                INST->eTot = values[i];
                break;
            default:
                return fmi2Error;
        }
    }
    return fmi2OK;
}

fmi2Status fmi2GetBoolean(fmi2Component c,
                            const fmi2ValueReference valueReferences[],
                            size_t nValueReferences,
                            fmi2Boolean values[])
{
    INST->log(LOG_CATEGORY_EVENTS, "fmi2GetReal");

    for(size_t i=0; i<nValueReferences; i++) {
        switch(valueReferences[i]) {
            case meas_start:
                values[i] = INST->start;
                break;
            case meas_stop:
                values[i] = INST->stop;
                break;
            case meas_reset:
                values[i] = INST->reset;
                break;
            case measurement_started:
                values[i] = INST->started;
                break;
            case measurement_stopped:
                values[i] = INST->stopped;
                break;
        }
    }
    return fmi2OK;
}

fmi2Status fmi2SetBoolean(fmi2Component c,
                            const fmi2ValueReference valueReferences[],
                            size_t nValueReferences,
                            const fmi2Boolean values[])
{
    INST->log(LOG_CATEGORY_EVENTS, "fmi2GetReal");

    for(size_t i=0; i<nValueReferences; i++) {
        switch(valueReferences[i]) {
            case meas_start:
                INST->start = values[i];
                break;
            case meas_stop:
                INST->stop = values[i];
                break;
            case meas_reset:
                INST->reset = values[i];
                break;
            case measurement_started:
                INST->started = values[i];
                break;
            case measurement_stopped:
                INST->stopped = values[i];
                break;
        }
    }
    return fmi2OK;
}

EMPTY_GETTER_SETTER(Integer)
EMPTY_GETTER_SETTER(String)


bool ModaptoCarbonFootprint::isPositiveEdge(fmi2Boolean lastValue, fmi2Boolean newValue) const {
    return newValue && !lastValue;
}

void ModaptoCarbonFootprint::integrationStep(fmi2Real h, fmi2Real& x, const fmi2Real& dx) const {
    // Euler forward as simplest integration step
    x += h * dx;
}



// fmi2Status fmi3GetNumberOfVariableDependencies(fmi2Component c,
//                                                fmi2ValueReference valueReference,
//                                                size_t* nDependencies) {
//     UNUSED(c)
//     UNUSED(valueReference);
//     UNUSED(nDependencies);

//     return fmi3Error;
// }

// fmi2Status fmi3GetVariableDependencies(fmi2Component c,
//     fmi2ValueReference dependent,
//     size_t elementIndicesOfDependent[],
//     fmi2ValueReference independents[],
//     size_t elementIndicesOfIndependents[],
//     fmi3DependencyKind dependencyKinds[],
//     size_t nDependencies) {

//     UNUSED(c)
//     UNUSED(dependent);
//     UNUSED(elementIndicesOfDependent);
//     UNUSED(independents);
//     UNUSED(elementIndicesOfIndependents);
//     UNUSED(dependencyKinds);
//     UNUSED(nDependencies);

//     return fmi3Error;
// }


fmi2Status fmi2GetDirectionalDerivative(fmi2Component c,
    const fmi2ValueReference unknowns[],
    size_t nUnknowns,
    const fmi2ValueReference knowns[],
    size_t nKnowns,
    const fmi2Real dvKnown[],
    fmi2Real dvUnknown[]) {

    UNUSED(c)
    UNUSED(unknowns)
    UNUSED(nUnknowns)
    UNUSED(knowns)
    UNUSED(nKnowns)
    UNUSED(dvKnown)
    UNUSED(dvUnknown)

    return fmi2Error;
}

fmi2Status fmi3GetAdjointDerivative(fmi2Component c,
    const fmi2ValueReference unknowns[],
    size_t nUnknowns,
    const fmi2ValueReference knowns[],
    size_t nKnowns,
    const fmi2Real dvKnown[],
    fmi2Real dvUnknown[]) {

    UNUSED(c)
    UNUSED(unknowns)
    UNUSED(nUnknowns)
    UNUSED(knowns)
    UNUSED(nKnowns)
    UNUSED(dvKnown)
    UNUSED(dvUnknown)

    return fmi2Error;
}


/***************************************************
 Functions for Co-Simulation
 ****************************************************/

fmi2Status fmi2DoStep(fmi2Component c,
                        fmi2Real      currentCommunicationPoint,
                        fmi2Real      communicationStepSize,
                        fmi2Boolean   noSetFMUStatePriorToCurrentPoint)
{
    UNUSED(noSetFMUStatePriorToCurrentPoint)

    INST->time = currentCommunicationPoint;
    INST->increaseTime(communicationStepSize);

    return fmi2OK;
}

fmi2Status fmi2SetRealInputDerivatives(fmi2Component c,
                                            const fmi2ValueReference vr[], size_t nvr,
                                            const fmi2Integer order[],
                                            const fmi2Real value[])
{
    UNUSED(c)
    UNUSED(vr)
    UNUSED(nvr)
    UNUSED(order)
    UNUSED(value)

    return fmi2Error;
}

fmi2Status fmi2GetRealOutputDerivatives(fmi2Component c,
                                            const fmi2ValueReference vr[], size_t nvr,
                                            const fmi2Integer order[],
                                            fmi2Real value[])
{
    UNUSED(c)
    UNUSED(vr)
    UNUSED(nvr)
    UNUSED(order)
    UNUSED(value)

    return fmi2Error;
}

fmi2Status fmi2CancelStep(fmi2Component c)
{
    UNUSED(c)
    return fmi2Error;
}

/* Inquire slave status */
fmi2Status fmi2GetStatus(fmi2Component c, const fmi2StatusKind s, fmi2Status*  value)
{
    UNUSED(c)
    UNUSED(s)
    UNUSED(value)

    return fmi2Error;
}

fmi2Status fmi2GetRealStatus(fmi2Component c, const fmi2StatusKind s, fmi2Real*    value)
{
    UNUSED(c)
    UNUSED(s)
    UNUSED(value)

    return fmi2Error;
}

fmi2Status fmi2GetIntegerStatus(fmi2Component c, const fmi2StatusKind s, fmi2Integer* value)
{
    UNUSED(c)
    UNUSED(s)
    UNUSED(value)

    return fmi2Error;
}

fmi2Status fmi2GetBooleanStatus(fmi2Component c, const fmi2StatusKind s, fmi2Boolean* value)
{
    UNUSED(c)
    UNUSED(s)
    UNUSED(value)

    return fmi2Error;
}

fmi2Status fmi2GetStringStatus(fmi2Component c, const fmi2StatusKind s, fmi2String*  value)
{
    UNUSED(c)
    UNUSED(s)
    UNUSED(value)

    return fmi2Error;
}


#ifdef __cplusplus
}
#endif