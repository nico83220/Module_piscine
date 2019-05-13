#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <Arduino.h>


/*
  @class ConfigClass
  @brief Classe de conteneur de la configuration du système  
*/
class ConfigClass
{
  struct ConfigData 
  {
    uint8_t ConfigVersion;   // Pour tester si c'est la 1ère fois qu'i=on lit dans l'EEPROM
    bool Ph_RegulationOnOff, Orp_RegulationOnOff;
    uint8_t FiltrationStart, FiltrationDuration, FiltrationStopMax, FiltrationStop, DelayPIDs;  
    uint32_t PhPumpUpTimeLimit, ChlPumpUpTimeLimit;
    uint32_t PhPIDWindowSize, OrpPIDWindowSize, PhPIDwindowStartTime, OrpPIDwindowStartTime;
    double Ph_SetPoint, Orp_SetPoint, PSI_HighThreshold, PSI_MedThreshold, WaterTempLowThreshold, WaterTemp_SetPoint, TempExternal, pHCalibCoeffs0, pHCalibCoeffs1, OrpCalibCoeffs0, OrpCalibCoeffs1, PSICalibCoeffs0, PSICalibCoeffs1;
    double Ph_Kp, Ph_Ki, Ph_Kd, Orp_Kp, Orp_Ki, Orp_Kd, PhPIDOutput, OrpPIDOutput, TempValue, PhValue, OrpValue, PSIValue;
  };
  
private:

protected:
  ConfigData  m_config;
  int         m_configAdress;

  // Lire la configuration depuis l'EEPROM
  bool LoadConfig();

  // Enregistrer la configuration dans l'EEPROM
  void SaveConfig() const;

public:
  // Initialisation
  void Initialize();
};

extern ConfigClass Config;

#endif  // __CONFIG_H__
