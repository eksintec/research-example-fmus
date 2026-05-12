#include <ElectricalModel.hpp>

#include <stdexcept>
#include <string> 

#include <fmi2TypesPlatform.h>

namespace EKS::Modapto::EnergyConsumption {

  ElectricalModel::ElectricalModel(
    fmi2String instanceName,
    fmi2CallbackLogger loggerCallback,
    fmi2CallbackAllocateMemory alloc,
    fmi2CallbackFreeMemory free,
    fmi2ComponentEnvironment environment,
    fmi2StepFinished stepFinished,
    fmi2Boolean loggingOn) :
    FMI2Model(instanceName, loggerCallback, alloc, free, environment, stepFinished, loggingOn),
    power(0),
    totalEnergy(0),
    powerFactor(6,0),
    maxPower(6,0)
  {
    resetValues();
  }

  void ElectricalModel::increaseTime(fmi2Real t, fmi2Real h)
  {
    calculatePower();
    updateState(t); // @todo update state before power calculation?
    if (measurementState == MeasurementState::Started) {
      integrationStep(h, power, totalEnergy);
    } else if (measurementState == MeasurementState::Resetted) {
      totalEnergy = 0;
    }
  }

  void ElectricalModel::prepareSim()
  {
    for (int i = 0; i < 6; ++i) {
      if (maxPower[i] == 0) {
        throw std::runtime_error(
          "max current of axis " + std::to_string(i + 1) + " is 0!");
      }
    }
  }

  void ElectricalModel::setMaxPower(size_t iMotor, fmi2Real power)
  {
    maxPower[iMotor] = power;
  }

  fmi2Real ElectricalModel::getMaxPower(size_t iMotor) const
  {
    return maxPower[iMotor];
  }

  void ElectricalModel::setPowerFactor(size_t iMotor, fmi2Real factor)
  {
    powerFactor[iMotor] = factor;
  }

  fmi2Real ElectricalModel::getPowerFactor(size_t iMotor) const
  {
    return powerFactor[iMotor];
  }

  fmi2Real ElectricalModel::getPower() const
  {
    fmi2Real power = 0;
    for (int i = 0; i < 6; i++) {
      power += getAxisPower(powerFactor[i], maxPower[i]);
    }

    return power;
  }

  void ElectricalModel::resetValues()
  {
    power = 0;
    totalEnergy = 0;

    for (int i = 0; i < 6; i++) {
      powerFactor[i] = 0;
      maxPower[i] = 0.0;
    }
  }

  void ElectricalModel::calculatePower()
  {
    power = getPower();
  }

  fmi2Real ElectricalModel::getAxisPower(
    fmi2Real powerFactor, fmi2Real maxPower) const
  {
    fmi2Real power = maxPower * (fmi2Real)powerFactor / 100;

    if (power < 0) {
      // We are not recuperating any power.
      return 0;
    }
    else {
      return power;
    }
  }

  void ElectricalModel::integrationStep(fmi2Real h, fmi2Real dx, fmi2Real& x) const
  {
    x += h * dx;
  }

}
