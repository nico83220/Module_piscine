#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <ArduinoSTL.h>
#include <vector>
#include <EEPROMex.h>


/*
  @enum SerializeMode
  @brief Définition des modes de sérialisation en EEPROM
*/
enum SerializeMode
{
  SERIALIZE_MODE_WRITE,
  SERIALIZE_MODE_READ,
};


/*
  @class ISerializable
  @brief Interface pour la sérialisation en EEPROM
*/
class ISerializable
{
  friend class ConfigClass;

private:

protected:
  String  m_serializableName;

public:
  // Constructeur
  ISerializable(const String& name);

  

  // Sérialiser l'objet vers ou depuis l'EEPROM
  virtual void Serialize(const SerializeMode& serializeMode,
                         int&                 EEPROMAddress) = 0;
};


/*
  @class ConfigClass
  @brief Classe de gestion de la configuration du système  
*/
class ConfigClass
{
  /*
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
  */
  
private:

protected:
  // Adresse mémoire de la config
  int                            m_configAdress;

  // Version de la config
  uint8_t                        m_configVersion;

  // Liste des objets sérialisables du système
  std::vector< ISerializable* >  m_serializableObjects;  

  // Lire la configuration depuis l'EEPROM
  bool LoadConfig();

  // Enregistrer la configuration dans l'EEPROM
  void SaveConfig() const;

public:
  // Constructeur
  ConfigClass();

  // Destructeur
  virtual ~ConfigClass() {}

  // Initialisation
  void Initialize();

  // Enregistrement d'un objet sérialisable
  void RegisterSerializableObject(ISerializable* serializableObject);
};

extern ConfigClass Config;

#endif  // __CONFIG_H__
