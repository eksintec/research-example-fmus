#include <fmi2Functions.h>
#include <variables.hpp>
#include <ElectricalModel.hpp>
#include <helper.h>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <string>

extern "C" {

#define LOG_CATEGORY_EVENTS "logEvents"

using namespace EKS;
using namespace EKS::Modapto::EnergyConsumption;

const char* fmi2GetVersion() {
    return fmi2Version;
}

const char* fmi2GetTypesPlatform(){
    return fmi2TypesPlatform;
}

fmi2Component fmi2Instantiate(  fmi2String instanceName,
                                    fmi2Type fmuType,
                                    fmi2String fmuGUID,
                                    fmi2String fmuResourceLocation,
                                    const fmi2CallbackFunctions* functions,
                                    fmi2Boolean visible,
                                    fmi2Boolean loggingOn)
{
    UNUSED(fmuResourceLocation)
    UNUSED(visible)

    if (!fmuGUID || strlen(fmuGUID) == 0) {
        return NULL; // no GUID
    }

    if (strcmp(fmuGUID, modelToken)) {
        return NULL; // wrong GUID
    }

    if(fmuType != fmi2CoSimulation){
        return NULL; // not a co-simulation
    }

    try {
        ElectricalModel *m = new ElectricalModel(
            instanceName,
            functions->logger,
            functions->allocateMemory,
            functions->freeMemory,
            functions->componentEnvironment,
            functions->stepFinished,
            loggingOn
        );

        return m;
    } catch (const std::runtime_error& error) {
        if (functions->logger) {
            functions->logger(functions->componentEnvironment, instanceName, fmi2Status::fmi2Fatal, LOG_CATEGORY_EVENTS, error.what());
        }
    }

    return NULL; // failed 
}

void fmi2FreeInstance(fmi2Component c) {
    delete (ElectricalModel*)c;
}

fmi2Status fmi2Reset(fmi2Component c) {
    ElectricalModel* model = (ElectricalModel*)c;
    model->log(LOG_CATEGORY_EVENTS, "fmi2Reset");
    model->resetModel();
    return fmi2OK;
}

fmi2Status fmi2Terminate(fmi2Component c)
{
    ElectricalModel* model = (ElectricalModel*)c;
    model->log(LOG_CATEGORY_EVENTS, "fmi2Terminate");
    return fmi2OK;
}

fmi2Status fmi2SetDebugLogging(fmi2Component c, fmi2Boolean loggingOn, size_t nCat, const fmi2String categories[])
{
    fmi2Status ret = fmi2OK;ElectricalModel* model = (ElectricalModel*)c;
    if(nCat == 0) {
        model->setLoggingCategories(loggingOn);
    } else {
        for(size_t i=0; i<nCat; i++){
            if(strcmp(categories[i], "logEvents") == 0) {
                model->setLoggingCategories(loggingOn, LoggingCategories::Events);
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
    UNUSED(startTime)
    UNUSED(stopTimeDefined)
    UNUSED(stopTime)

    ElectricalModel* model = (ElectricalModel*)c;
    model->log(LOG_CATEGORY_EVENTS, "fmi2SetupExperiment");
    return fmi2OK;
}

fmi2Status fmi2EnterInitializationMode(fmi2Component c)
{
    ElectricalModel* model = (ElectricalModel*)c;
    model->log(LOG_CATEGORY_EVENTS, "fmi2EnterInitializationMode");
    return fmi2OK;
}

fmi2Status fmi2ExitInitializationMode(fmi2Component c)
{
    ElectricalModel* model = (ElectricalModel*)c;
    model->log(LOG_CATEGORY_EVENTS, "fmi2ExitInitializationMode");

    try {
        model->prepareSim();
    } catch (const std::runtime_error& error) {
        model->log(LOG_CATEGORY_EVENTS, error.what(), fmi2Status::fmi2Fatal);
        return fmi2Fatal;
    }
    return fmi2OK;
}

fmi2Status fmi2GetReal(fmi2Component c,
                            const fmi2ValueReference valueReferences[],
                            size_t nValueReferences,
                            fmi2Real values[])
{
    ElectricalModel* model = (ElectricalModel*)c;
    std::stringstream ss;
    ss << "fmi2GetReal called!";

    for(size_t i=0; i<nValueReferences; i++) {
        ss << std::endl << "Fetching value reference " << valueReferences[i] << " (#r" << valueReferences[i] << "#)";
        switch(valueReferences[i]) {
            case robot_used_power:
                values[i] = model->power;
                break;
            case robot_used_energy:
                values[i] = model->totalEnergy;
                break;
            case timestamp_start:
                values[i] = model->timestampStart;
                break;
            case timestamp_stop:
                values[i] = model->timestampStop;
                break;

            case max_power_0:
                values[i] = model->getMaxPower(0);
                break;
            case max_power_1:
                values[i] = model->getMaxPower(1);
                break;
            case max_power_2:
                values[i] = model->getMaxPower(2);
                break;
            case max_power_3:
                values[i] = model->getMaxPower(3);
                break;
            case max_power_4:
                values[i] = model->getMaxPower(4);
                break;
            case max_power_5:
                values[i] = model->getMaxPower(5);
                break;

            case power_0:
                values[i] = model->getPowerFactor(0);
                break;
            case power_1:
                values[i] = model->getPowerFactor(1);
                break;
            case power_2:
                values[i] = model->getPowerFactor(2);
                break;
            case power_3:
                values[i] = model->getPowerFactor(3);
                break;
            case power_4:
                values[i] = model->getPowerFactor(4);
                break;
            case power_5:
                values[i] = model->getPowerFactor(5);
                break;
                
            default:
                ss << std::endl << "Unknown value reference given.";
                model->log(LOG_CATEGORY_EVENTS, ss.str().c_str(), fmi2Error);
                return fmi2Error;
        }
        ss << " = " << values[i];
    }
    model->log(LOG_CATEGORY_EVENTS, ss.str().c_str());
    return fmi2OK;
}


fmi2Status fmi2SetReal(fmi2Component c,
                                      const fmi2ValueReference valueReferences[],
                                      size_t nValueReferences,
                                      const fmi2Real values[])
{
    ElectricalModel* model = (ElectricalModel*)c;
    std::stringstream ss;
    ss << "fmi2SetReal called!";

    for(size_t i=0; i<nValueReferences; i++) {
        ss << std::endl << "Setting value reference " << valueReferences[i] << " (#r" << valueReferences[i] << "#) to " << values[i];
        switch(valueReferences[i]) {
            // Robot used power
            // Robot used energy
            // timestamp_start
            // timestamp_stop/reset

            case max_power_0:
                model->setMaxPower(0, values[i]);
                break;
            case max_power_1:
                model->setMaxPower(1, values[i]);
                break;
            case max_power_2:
                model->setMaxPower(2, values[i]);
                break;
            case max_power_3:
                model->setMaxPower(3, values[i]);
                break;
            case max_power_4:
                model->setMaxPower(4, values[i]);
                break;
            case max_power_5:
                model->setMaxPower(5, values[i]);
                break;

            case power_0:
                model->setPowerFactor(0, values[i]);
                break;
            case power_1:
                model->setPowerFactor(1, values[i]);
                break;
            case power_2:
                model->setPowerFactor(2, values[i]);
                break;
            case power_3:
                model->setPowerFactor(3, values[i]);
                break;
            case power_4:
                model->setPowerFactor(4, values[i]);
                break;
            case power_5:
                model->setPowerFactor(5, values[i]);
                break;
                
            default:
                ss << std::endl << "Unknown value reference found. Aborting.";
                model->log(LOG_CATEGORY_EVENTS, ss.str().c_str(), fmi2Error);
                return fmi2Error;
        }
    }
    model->log(LOG_CATEGORY_EVENTS, ss.str().c_str());
    return fmi2OK;
}

fmi2Status fmi2GetInteger(fmi2Component c,
                                      const fmi2ValueReference valueReferences[],
                                      size_t nValueReferences,
                                      fmi2Integer values[])
{
    ElectricalModel* model = (ElectricalModel*)c;
    std::stringstream ss;
    ss << "fmi2GetInteger called.";

    for(size_t i=0; i<nValueReferences; i++) {
        ss << std::endl << "Fetching value reference " << valueReferences[i] << " (#i" << valueReferences[i] << "#)";
        switch(valueReferences[i]) {
            case measurement_state:
                values[i] = (fmi2Integer)model->measurementState;
                break;
            default:
                ss << std::endl << "Unknown value reference given.";
                model->log(LOG_CATEGORY_EVENTS, ss.str().c_str(), fmi2Error);
                return fmi2Error;
        }
    }
    model->log(LOG_CATEGORY_EVENTS, ss.str().c_str());
    return fmi2OK;
}

fmi2Status fmi2SetInteger(fmi2Component c,
                                      const fmi2ValueReference valueReferences[],
                                      size_t nValueReferences,
                                      const fmi2Integer values[])
{
    ElectricalModel* model = (ElectricalModel*)c;
    std::stringstream ss;
    ss << "fmi2SetInteger called.";

    for(size_t i=0; i<nValueReferences; i++) {
        ss << std::endl << "Setting value reference " << valueReferences[i] << " (#i" << valueReferences[i] << ") to " << values[i];
        switch(valueReferences[i]) {
            // case measurement_state:
            //     model->measurementState = values[i];
                // break;
            default:
                ss << std::endl << "Unknown value reference given.";
                model->log(LOG_CATEGORY_EVENTS, ss.str().c_str(), fmi2Error);
                return fmi2Error;
        }
    }
    model->log(LOG_CATEGORY_EVENTS, ss.str().c_str());
    return fmi2OK;
}

fmi2Status fmi2GetBoolean(fmi2Component c,
                                      const fmi2ValueReference valueReferences[],
                                      size_t nValueReferences,
                                      fmi2Boolean values[])
{
    ElectricalModel* model = (ElectricalModel*)c;
    std::stringstream ss;
    ss << "fmi2GetBoolean called.";

    for(size_t i=0; i<nValueReferences; i++) {
        ss << std::endl << "Fetching value reference " << valueReferences[i] << " (#b" << valueReferences[i] << "#)";
        switch(valueReferences[i]) {
            case start_measurement:
                values[i] = model->start;
                break;
            case stop_measurement:
                values[i] = model->stop;
                break;
            case reset_measurement:
                values[i] = model->reset;
                break;
            default:
                ss << std::endl << "Unknown value reference given.";
                model->log(LOG_CATEGORY_EVENTS, ss.str().c_str(), fmi2Error);
                return fmi2Error;
        }
    }
    model->log(LOG_CATEGORY_EVENTS, ss.str().c_str());
    return fmi2OK;
}

fmi2Status fmi2SetBoolean(fmi2Component c,
                                      const fmi2ValueReference valueReferences[],
                                      size_t nValueReferences,
                                      const fmi2Boolean values[])
{
    ElectricalModel* model = (ElectricalModel*)c;
    std::stringstream ss;
    ss << "fmi2SetBoolean called.";

    for(size_t i=0; i<nValueReferences; i++) {
        ss << std::endl << "Setting value reference " << valueReferences[i] << " (#b" << valueReferences[i] << "#) to " << values[i];
        switch(valueReferences[i]) {
            case start_measurement:
                model->start = values[i];
                break;
            case stop_measurement:
                model->stop = values[i];
                break;
            case reset_measurement:
                model->reset = values[i];
                break;
            default:
                ss << std::endl << "Unknown value reference given.";
                model->log(LOG_CATEGORY_EVENTS, ss.str().c_str(), fmi2Error);
                return fmi2Error;
        }
    }
    model->log(LOG_CATEGORY_EVENTS, ss.str().c_str());
    return fmi2OK;
}


fmi2Status fmi2GetString(fmi2Component c,
                                      const fmi2ValueReference valueReferences[],
                                      size_t nValueReferences,
                                      fmi2String values[])
{
    UNUSED(values);

    ElectricalModel *model = (ElectricalModel*) c;
    std::stringstream ss;
    ss << "fmi2GetString called. No variables available.";
    for(size_t i=0; i<nValueReferences; i++){
        ss << std::endl << "Requesting value reference " << valueReferences[i];
    }
    model->log(LOG_CATEGORY_EVENTS, ss.str().c_str(), fmi2Error);
    return fmi2Error;
}


fmi2Status fmi2SetString(fmi2Component c,
                                      const fmi2ValueReference valueReferences[],
                                      size_t nValueReferences,
                                      const fmi2String values[])
{
    ElectricalModel *model = (ElectricalModel*) c;
    std::stringstream ss;
    ss << "fmi2SetString called. No variables available.";
    for(size_t i=0; i<nValueReferences; i++){
        ss << std::endl << "Requesting value reference " << valueReferences[i] << " to \"" << values[i] << "\"";
    }
    model->log(LOG_CATEGORY_EVENTS, ss.str().c_str(), fmi2Error);

    return fmi2Error;
}

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

    ElectricalModel* model = (ElectricalModel*)c;
    std::stringstream ss;

    ss << "Starting fmi2DoStep.";
    // Optionally output internal state of FMU for debugging
    model->log(LOG_CATEGORY_EVENTS, ss.str().c_str());

    model->increaseTime(currentCommunicationPoint, communicationStepSize);

    // Reset stringbuffer
    ss.str(std::string());

    ss << "Finished fmi2DoStep.";
    // Optionally output internal state of FMU for debugging
    model->log(LOG_CATEGORY_EVENTS, ss.str().c_str());

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


}
