#include "Config.h"
#include "LogConsole.h"

#include <EEPROMVar.h>

using namespace std;


/*
  Constructeur
*/
ISerializable::ISerializable(const String& name) :
  m_serializableName(name)
{
}


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


// Version de la configuration enregistrée dans l'EEPROM
// Valeur aléatoire. Changer cette valeur (avec n'importe quelle valeur) pour recharger les valeurs par défaut
#define  CURRENT_CONFIG_VERSION  1

// Offset d'adresse dans l'EEPROM pour le stockage de la configuration
#define  MEMORY_BASE  32


/*
  Constructeur
*/
ConfigClass::ConfigClass() :
/*  m_config({ CURRENT_CONFIG_VERSION,
             0, 0,
             8, 12, 20, 20, 59,
             1800, 1800,
             3600000, 7200000, 0, 0,
             7.4, 730.0, 0.5, 0.25, 10.0, 27.0, 3.0, 4.23, -2.28, -1268.78, 2718.63, 1.0, 0.0,
             1330000.0, 0.0, 0.0, 2857.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.4 }),*/
  m_configAdress(MEMORY_BASE),
  m_configVersion(CURRENT_CONFIG_VERSION)
{
}


/*
  Lire la configuration depuis l'EEPROM
*/
bool ConfigClass::LoadConfig() 
{
  bool res = false;

  // Lecture le la version dans l'EEPROM
  uint8_t versionInEEPROM = EEPROM.readByte(m_configAdress);

  LOG_MESSAGE(F("ConfigClass (%u elements) : lecture configuration (versionInEEPROM = %u / m_configVersion = %u)\n"),
              m_serializableObjects.size(),
              versionInEEPROM,
              m_configVersion);

  // Si c'est la bonne, on continue la lecture
  if ( versionInEEPROM == m_configVersion )
  {
    int address = m_configAdress + sizeof(uint8_t);

    // On demande à chaque objet de se sérialiser en lecture
    vector< ISerializable* >::iterator it;
    for ( it=m_serializableObjects.begin(); it!=m_serializableObjects.end(); ++it )
    {
      (*it)->Serialize(SERIALIZE_MODE_READ, address);
    }

    res = true;
  }

  return res;
}


/*
  Enregistrer la configuration dans l'EEPROM
*/
void ConfigClass::SaveConfig() const
{
  // Ecriture le la version dans l'EEPROM si elle est différente
  EEPROM.updateByte(m_configAdress, m_configVersion);
  int address = m_configAdress + sizeof(uint8_t);

  // On demande à chaque objet de se sérialiser en écriture
  vector< ISerializable* >::iterator it;
  for ( it=m_serializableObjects.begin(); it!=m_serializableObjects.end(); ++it )
  {
    (*it)->Serialize(SERIALIZE_MODE_WRITE, address);
  }
}


/*
  Initialisation
*/
void ConfigClass::Initialize()
{
  // Initialisation de l'EEPROM
  EEPROM.setMemPool(MEMORY_BASE, EEPROMSizeMega); 

  // Tentative de restauration de la configuration depuis l'EEPROM
  if ( LoadConfig() == true ) 
  {
    LOG_MESSAGE(F("Lecture de la configuration courante %d depuis l'EEPROM\n"),
                CURRENT_CONFIG_VERSION);
  }

  // Sinon, la version dans l'EEPROM n'est pas la bonne : on utilise la configuration par défaut
  else
  {
    LOG_MESSAGE(F("La configuration présente dans l'EEPROM (%d) n'est pas la bonne, utilisation de la configuration par défaut\n"),
                CURRENT_CONFIG_VERSION);
    
    // 1ère utilisation : enregistrement de la configuration par défaut dans l'EEPROM
    SaveConfig();
  }
}


/*
  Enregistrement d'un objet sérialisable
*/
void ConfigClass::RegisterSerializableObject(ISerializable* serializableObject)
{
  if ( serializableObject != NULL )
  {
    m_serializableObjects.push_back(serializableObject);
  }
}


ConfigClass Config;
