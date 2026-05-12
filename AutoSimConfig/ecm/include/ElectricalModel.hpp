#ifndef EKS_MODAPTO_ENERGY_CONSUMPTION_ELECTRICAL_MODEL_HPP
#define EKS_MODAPTO_ENERGY_CONSUMPTION_ELECTRICAL_MODEL_HPP

#include <vector>

#include <fmi2FunctionTypes.h>
#include <FMI2Model.hpp>

namespace EKS::Modapto::EnergyConsumption {

class ElectricalModel : public FMI2Model {
  public:
    ElectricalModel(
      fmi2String instanceName,
      fmi2CallbackLogger loggerCallback,
      fmi2CallbackAllocateMemory alloc,
      fmi2CallbackFreeMemory free,
      fmi2ComponentEnvironment environment,
      fmi2StepFinished stepFinished,
      fmi2Boolean loggingOn
    );

    void increaseTime(fmi2Real t, fmi2Real h);

    void prepareSim();

    void setMaxPower(size_t iMotor, fmi2Real current);
    fmi2Real getMaxPower(size_t iMotor) const;

    void setPowerFactor(size_t iMotor, fmi2Real factor);
    fmi2Real getPowerFactor(size_t iMotor) const;

    fmi2Real getPower() const;

    // Inputs and outputs as defined
    // current power (in Watt)
    fmi2Real power;
    // total energy (in Joule)
    fmi2Real totalEnergy;

  protected:

    // resets all values
    void resetValues() override;

  private:

    void calculatePower();

    fmi2Real getAxisPower(fmi2Real currentFactor,fmi2Real maxCurrent) const;

    void integrationStep(fmi2Real h, fmi2Real dx, fmi2Real& x) const;

    // current factor (0-100)
    std::vector<fmi2Real> powerFactor;
    // maximum current (in Watt)
    std::vector<fmi2Real> maxPower;
  };

}
#endif
