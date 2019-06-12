#ifndef __ALARMS_MANAGER_H__
#define __ALARMS_MANAGER_H__

#include <Arduino.h>
#include <ArduinoSTL.h>
#include <map>

class Sensor;


enum AlarmType
{
  HIGH_PRESSURE_ALARM,
  LOW_PRESSURE_ALARM,
  NO_FLOW_ALARM,
  LOW_LEVEL_ALARM,
  MAXIMUM_TIME_ALARM,
};


/*
  @struct Alarm
  @brief Classe représentant une alarmes
*/
struct Alarm
{
  Sensor*    m_sensor;
  AlarmType  m_type;
  uint32_t   m_timeInMs;
  void*      m_param;
};


/*
  @class AlarmsManagerClass
  @brief Classe de définition d'un gestionnaire d'alarmes
*/
class AlarmsManagerClass
{
private:

protected:
  std::map< String, Alarm >  m_alarms;   // Alarmes en cours
  
public:
  // Constructeur
  AlarmsManagerClass();

  // Destructeur
  virtual ~AlarmsManagerClass() {}

  // Ajouter une alarme
  void AddAlarm(const String&    sensorName,
                Sensor*          sensor,
                const AlarmType& type,
                void*            param);

  // Supprimer une alarme
  void RemoveAlarm(const String& sensorName);

  // Lire si une alarme existe
  bool ExistsAlarm(const String& sensorName);

  // Boucle de traitement à appeler dans le programme principal
  void Loop(uint32_t nowMillisec);
};

extern AlarmsManagerClass AlarmsManager;

#endif  // __ALARMS_MANAGER_H__
