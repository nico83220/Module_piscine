#include "Config.h"
#include "LogConsole.h"

#include <EEPROMex.h>
#include <EEPROMVar.h>


// Version de la configuration enregistrée dans l'EEPROM
// Valeur aléatoire. Changer cette valeur (avec n'importe quelle valeur) pour recharger les valeurs par défaut
#define  CURRENT_CONFIG_VERSION  100

// Offset d'adresse dans l'EEPROM pour le stockage de la configuration
#define  MEMORY_BASE  32


/*
  Constructeur
*/
/*
ConfigClass::CoonfigClass() :
  m_config({ CURRENT_CONFIG_VERSION,
             0, 0,
             8, 12, 20, 20, 59,
             1800, 1800,
             3600000, 7200000, 0, 0,
             7.4, 730.0, 0.5, 0.25, 10.0, 27.0, 3.0, 4.23, -2.28, -1268.78, 2718.63, 1.0, 0.0,
             1330000.0, 0.0, 0.0, 2857.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.4 }),
  m_configAdress(0)
{
}
*/


/*
  Lire la configuration depuis l'EEPROM
*/
bool ConfigClass::LoadConfig() 
{
  EEPROM.readBlock(m_configAdress, m_config);

/*  Serial<<storage.ConfigVersion<<", "<<storage.Ph_RegulationOnOff<<", "<<storage.Orp_RegulationOnOff<<'\n';
  Serial<<storage.FiltrationStart<<", "<<storage.FiltrationDuration<<", "<<storage.FiltrationStopMax<<", "<<storage.FiltrationStop<<", "<<storage.DelayPIDs<<'\n';  
  Serial<<storage.PhPumpUpTimeLimit<<", "<<storage.ChlPumpUpTimeLimit<<'\n';
//  Serial<<storage.FiltrationPumpTimeCounter<<", "<<storage.PhPumpTimeCounter<<", "<<storage.ChlPumpTimeCounter<<", "<<storage.FiltrationPumpTimeCounterStart<<", "<<storage.PhPumpTimeCounterStart<<", "<<storage.ChlPumpTimeCounterStart<<'\n';
  Serial<<storage.PhPIDWindowSize<<", "<<storage.OrpPIDWindowSize<<", "<<storage.PhPIDwindowStartTime<<", "<<storage.OrpPIDwindowStartTime<<'\n';
  Serial<<storage.Ph_SetPoint<<", "<<storage.Orp_SetPoint<<", "<<storage.PSI_HighThreshold<<", "<<storage.PSI_MedThreshold<<", "<<storage.WaterTempLowThreshold<<", "<<storage.WaterTemp_SetPoint<<", "<<storage.TempExternal<<", "<<storage.pHCalibCoeffs0<<", "<<storage.pHCalibCoeffs1<<", "<<storage.OrpCalibCoeffs0<<", "<<storage.OrpCalibCoeffs1<<", "<<storage.PSICalibCoeffs0<<", "<<storage.PSICalibCoeffs1<<'\n';
  Serial<<storage.Ph_Kp<<", "<<storage.Ph_Ki<<", "<<storage.Ph_Kd<<", "<<storage.Orp_Kp<<", "<<storage.Orp_Ki<<", "<<storage.Orp_Kd<<", "<<storage.PhPIDOutput<<", "<<storage.OrpPIDOutput<<", "<<storage.TempValue<<", "<<storage.PhValue<<", "<<storage.OrpValue<<", "<<storage.PSIValue<<'\n';
*/
  return (m_config.ConfigVersion == CURRENT_CONFIG_VERSION);
}


/*
  Enregistrer la configuration dans l'EEPROM
*/
void ConfigClass::SaveConfig() const
{
  // La fonction écrit seulement dans l'EEPROM si la valeur est différente. Cela augmente la durée de vie de l'EEPROM
  EEPROM.writeBlock(m_configAdress, m_config);
}


/*
  Initialiser l'afficheur
*/
void ConfigClass::Initialize()
{
  // Initialisation de l'EEPROM
  EEPROM.setMemPool(MEMORY_BASE, EEPROMSizeMega); 

  // Lecture de l'addresse de la configuration
  m_configAdress = EEPROM.getAddress(sizeof(ConfigData));

  // Lecture le la version deans l'EEPROM
  uint8_t versionInEEPROM = EEPROM.readByte(m_configAdress);

  // Si la version de la configuration lue dans l'EEPROM est la version actuelle
  if ( versionInEEPROM == CURRENT_CONFIG_VERSION ) 
  {
    LOG_MESSAGE(F("Lecture de la configuration courante %d dans l'EEPROM\n"),
                CURRENT_CONFIG_VERSION);

    // Restauration de la configuration depuis l'EEPROM
    LoadConfig();
  }

  // Sinon, la version dans l'EEPROM n'est pas la bonne : on utilise la configuration par défaut
  else
  {
    LOG_MESSAGE(F("La configuration présente dans l'EEPROM (%d) n'est pas la bonne, utilisation de la configuration par défaut\n"),
                CURRENT_CONFIG_VERSION);
    
    // 1ère utilisation : enregistrement de la configuration dans l'EEPROM
    SaveConfig();
  }
}


ConfigClass Coonfig;
