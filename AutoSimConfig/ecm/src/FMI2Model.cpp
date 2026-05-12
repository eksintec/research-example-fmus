#include <FMI2Model.hpp>

namespace EKS {

FMI2Model::FMI2Model(
  fmi2String instanceName,
  fmi2CallbackLogger loggerCallback,
  fmi2CallbackAllocateMemory alloc,
  fmi2CallbackFreeMemory free,
  fmi2ComponentEnvironment environment,
  fmi2StepFinished stepFinished,
  fmi2Boolean loggingOn) :
  instanceName(instanceName),
  loggerCallback(loggerCallback),
  memAlloc(alloc),
  memFree(free),
  environment(environment),
  stepFinished(stepFinished),
  loggingOn(loggingOn)
{
  resetState();
}

void FMI2Model::resetModel()
{
  resetState();
  resetValues();
}

void FMI2Model::setLoggingCategories(bool enable, LoggingCategories categories)
{
  loggingCategories = enable
    ? (LoggingCategories)((int)loggingCategories | (int)categories)
    : (LoggingCategories)((int)loggingCategories & ~(int)categories);
}

void FMI2Model::log(
  const char* category, const char* msg, fmi2Status status) const
{
  (*loggerCallback)(environment, instanceName, status, category, msg);
}

void FMI2Model::resetState()
{
  start = stop = reset = false;
  lastStart = lastStop = lastReset = false;
  timestampStart = timestampStop = 0;
  measurementState = MeasurementState::Inited;
  loggingCategories = loggingOn
    ? defaultLoggingCategories : LoggingCategories::None;
}

void FMI2Model::updateState(fmi2Real t)
{
  switch (measurementState) {
  case MeasurementState::Inited:
  case MeasurementState::Resetted:
    if (isPositiveEdge(lastStart, start)) {
      measurementState = MeasurementState::Started;
      timestampStart = t;
    }
    break;
  case MeasurementState::Started:
    if (isPositiveEdge(lastStop, stop)) {
      measurementState = MeasurementState::Stopped;
      timestampStop = t;
    }
    break;
  case MeasurementState::Stopped:
    if (isPositiveEdge(lastReset, reset)) {
      measurementState = MeasurementState::Resetted;
    }
    break;
  }

  lastStart = start;
  lastStop = stop;
  lastReset = reset;
}

bool FMI2Model::isPositiveEdge(
  fmi2Boolean lastValue, fmi2Boolean newValue) const
{
  return !lastValue && newValue;
}

}
