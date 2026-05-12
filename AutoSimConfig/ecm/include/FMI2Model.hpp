#ifndef EKS_FMI2_MODEL_HPP
#define EKS_FMI2_MODEL_HPP

#include <fmi2FunctionTypes.h>

namespace EKS {

// bit field on enabled logging categories
enum class LoggingCategories {
  None = 0,
  Events = 1 << 0
};

enum class MeasurementState {
  Inited = 0,
  Started = 1,
  Stopped = 2,
  Resetted = 3
};

// base class for FMI 2models
class FMI2Model {
  public:
    FMI2Model(
      fmi2String instanceName,
      fmi2CallbackLogger loggerCallback,
      fmi2CallbackAllocateMemory alloc,
      fmi2CallbackFreeMemory free,
      fmi2ComponentEnvironment environment,
      fmi2StepFinished stepFinished,
      fmi2Boolean loggingOn
    );

    virtual ~FMI2Model() = default;

    void resetModel();

    void setLoggingCategories(
      bool enable,
      LoggingCategories categories = defaultLoggingCategories);

    void log(
      const char* category,
      const char* msg,
      fmi2Status status = fmi2OK) const;

    // @todo make private?
    fmi2Boolean start, stop, reset;
    fmi2Real timestampStart, timestampStop;
    MeasurementState measurementState;

  protected:

    void resetState();

    virtual void resetValues() = 0;

    void updateState(fmi2Real t);

  private:
    bool isPositiveEdge(fmi2Boolean lastValue, fmi2Boolean newValue) const;

    fmi2Boolean lastStart, lastStop, lastReset;

    LoggingCategories loggingCategories;

    // Some environment pointers
    fmi2String instanceName;
    fmi2CallbackLogger loggerCallback;
    fmi2CallbackAllocateMemory memAlloc;
    fmi2CallbackFreeMemory memFree;
    fmi2ComponentEnvironment environment;
    fmi2StepFinished stepFinished;
    fmi2Boolean loggingOn;

    const static LoggingCategories defaultLoggingCategories
      = LoggingCategories::Events;
  };

}
#endif
