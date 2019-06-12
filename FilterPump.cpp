#include "FilterPump.h"
#include "Relay.h"
#include "PressureSensor.h"
#include "AlarmsManager.h"
#include "LogConsole.h"


/* 
  Constructeur
  @param name Nom de la pompe
*/
FilterPump::FilterPump(const String&      name,
                       Relay*             commandRelay,
                       LiquidLevelSensor* liquidLevelSensor,
                       PressureSensor*    filterPressureSensor) : 
  Pump(name, commandRelay, liquidLevelSensor),
  ISerializable(name),
  m_filterPressureSensor(filterPressureSensor)
{
}


/*
  Destructeur
 */
FilterPump::~FilterPump()
{
}


/*
  Démarrer la pompe
  @param requestedRunningTimeInMs Durée de fonctionnement souhaitée
*/
void FilterPump::Start(uint32_t requestedRunningTimeInMs)
{
  // Si la pompe est arrêtée
  if ( IsRunning() == false )
  {
    // On va contrôler qu'une alarme de pression n'existe pas
    if ( ( m_filterPressureSensor == NULL ) ||
         ( AlarmsManager.ExistsAlarm(m_filterPressureSensor->GetName()) == false ) )
    {
      // On appelle le traitement du parent
      Pump::Start(requestedRunningTimeInMs);
    }
  }
}


/*
  Boucle de traitement à appeler dans le programme principal
*/
void FilterPump::Loop(uint32_t nowMillisec)
{
  // On effectue d'abord le traitement commun
  Pump::Loop(nowMillisec);

  // Puis si la pompe est démarrée
  if ( IsRunning() == true )
  {
    // On va contrôler la pression du filtre
    if ( ( m_filterPressureSensor != NULL ) &&
          ( ( m_filterPressureSensor->GetPressure() < m_minPressure ) ||
            ( m_filterPressureSensor->GetPressure() > m_maxPressure ) ) )
    {
      // On arrête la pompe
      Stop();

      // On remonte une alarme de pression
      if ( m_filterPressureSensor->GetPressure() < m_minPressure )
      {
        float* param = new float;
        *param = m_minPressure;
        AlarmsManager.AddAlarm(m_filterPressureSensor->GetName(), m_filterPressureSensor, LOW_PRESSURE_ALARM, param);
      }
      else
      {
        float* param = new float;
        *param = m_maxPressure;
        AlarmsManager.AddAlarm(m_filterPressureSensor->GetName(), m_filterPressureSensor, HIGH_PRESSURE_ALARM, param);
      }
    }
  }
}


/*
  Sérialiser l'objet vers ou depuis l'EEPROM
*/
void FilterPump::Serialize(const SerializeMode& serializeMode,
                           int&                 EEPROMAddress)
{
  Pump::Serialize(serializeMode, EEPROMAddress);

  // Ecriture
  if ( serializeMode == SERIALIZE_MODE_WRITE )
  {
    EEPROM.updateFloat(EEPROMAddress, m_minPressure);
    EEPROMAddress += sizeof(float);
    EEPROM.updateFloat(EEPROMAddress, m_maxPressure);
    EEPROMAddress += sizeof(float);
  }

  // Lecture
  else
  {
    m_minPressure = EEPROM.readFloat(EEPROMAddress);
    EEPROMAddress += sizeof(float);
    m_maxPressure = EEPROM.readFloat(EEPROMAddress);
    EEPROMAddress += sizeof(float);

    LOG_MESSAGE(F("FilterPump %s : lecture configuration (m_minPressure = %f - m_maxPressure = %f)"),
                m_name.c_str(),
                m_minPressure,
                m_maxPressure);
  }
}
