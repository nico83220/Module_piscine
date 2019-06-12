#include "AlarmsManager.h"
#include "Utils.h"
#include "LogConsole.h"
#include "PressureSensor.h"
#include "ContactSensor.h"
#include "LiquidLevelSensor.h"


/*
  Constructeur
*/
AlarmsManagerClass::AlarmsManagerClass()
{
}


/*
  Ajouter une alarme
*/
void AlarmsManagerClass::AddAlarm(const String&    sensorName,
                                  Sensor*          sensor,
                                  const AlarmType& type,
                                  void*            param)
{
  Alarm alarm;
  alarm.m_sensor = sensor;
  alarm.m_type   = type;
  alarm.m_param  = param;
  m_alarms[sensorName] = alarm;
}


/*
  Supprimer une alarme
*/
void AlarmsManagerClass::RemoveAlarm(const String& sensorName)
{
  m_alarms.erase(sensorName);
}


/*
  Lire si une alarme existe
*/
bool AlarmsManagerClass::ExistsAlarm(const String& sensorName)
{
  bool res = false;

  if ( m_alarms.find(sensorName) != m_alarms.end() )
  {
    res = true;
  }

  return res;
}


/*
  Boucle de traitement à appeler dans le programme principal
  @param nowMillisec Heure actulelle en ms
*/
void AlarmsManagerClass::Loop(uint32_t nowMillisec)
{
  // Dans cette boucle, on va essayer de remettre les alarmes à 0
  std::map< String, Alarm >::iterator it = m_alarms.begin();
  while ( it != m_alarms.end() )
  {
    bool removeAlarm = false;

    // En fonction du type d'alarme
    switch ( it->second.m_type )
    {
    case HIGH_PRESSURE_ALARM:
      if ( ( it->second.m_sensor != NULL ) &&
           ( it->second.m_param != NULL ) )
      {
        // Le paramètre est la valeur maximum de pression
        float* maximumPressure = (float*) (it->second.m_param);

        // Si la pression est en dessous de la pression maximum
        if ( ((PressureSensor*) (it->second.m_sensor))->GetPressure() <= *maximumPressure )
        {
          // On détruit ici le paramètre car un delete d'un void* n'est pas possible
          // alors qu'ici on connait le type
          delete(maximumPressure);

          // On demande de supprimer l'alarme
          removeAlarm = true;
        }
      }
      break;

    case LOW_PRESSURE_ALARM:
      if ( ( it->second.m_sensor != NULL ) &&
           ( it->second.m_param != NULL ) )
      {
        // Le paramètre est la valeur minimum de pression
        float* minimumPressure = (float*) (it->second.m_param);

        // Si la pression est au dessus de la pression minimum
        if ( ((PressureSensor*) (it->second.m_sensor))->GetPressure() >= *minimumPressure )
        {
          // On détruit ici le paramètre car un delete d'un void* n'est pas possible
          // alors qu'ici on connait le type
          delete(minimumPressure);

          // On demande de supprimer l'alarme
          removeAlarm = true;
        }
      }
      break;

    case NO_FLOW_ALARM:
      if ( it->second.m_sensor != NULL )
      {
        // Si le contact est fermé
        if ( ((ContactSensor*) (it->second.m_sensor))->GetContact() == CONTACT_CLOSED )
        {
          // On demande de supprimer l'alarme
          removeAlarm = true;
        }
      }
      break;

    case LOW_LEVEL_ALARM:
      if ( it->second.m_sensor != NULL )
      {
        // Si le niveau est correct
        if ( ((LiquidLevelSensor*) (it->second.m_sensor))->IsTooLow() == false )
        {
          // On demande de supprimer l'alarme
          removeAlarm = true;
        }
      }
      break;

    case MAXIMUM_TIME_ALARM:
      if ( it->second.m_param != NULL )
      {
        // Le paramètre est l'heure de déclenchement
        uint32_t* alarmTime = (uint32_t*) (it->second.m_param);

        // Après 5 minutes
        if ( ( alarmTime != NULL ) &&
            ( nowMillisec - *alarmTime > 300000L ) )
        {
          // On détruit ici le paramètre car un delete d'un void* n'est pas possible
          // alors qu'ici on connait le type
          delete(alarmTime);

          // On demande de supprimer l'alarme
          removeAlarm = true;
        }
      }
      break;

    default:
      break;
    }

    // Si on doit supprimer l'alarme
    if ( removeAlarm == true )
    {
      // On supprime l'item dans le map
      m_alarms.erase(it);

      // Et on repart au début du map car on a fait un erase
      it = m_alarms.begin();
    }

    // Sinon, on ne la supprime pas et on passe à la suivante
    else
    {
      ++it;
    }
  }
}

AlarmsManagerClass AlarmsManager;
