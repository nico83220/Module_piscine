#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

#include <Arduino.h>
#include <RTClib.h>


/*
  @class TimeManagerClass
  @brief Classe de gestion d'une console de log
*/
class TimeManagerClass
{
private:

protected:
  DS3231      m_RTC;                     // Horloge RTC
  uint32_t    m_timezoneOffsetInSecond;  // Décalage horaire
  
public:
  // Initialisation
  void Initialize();

  // Lire la date et heure actuelle
  DateTime Now();
  
  // Log de la date et l'heure de l'ARDUINO
  void LogDateTime() const;
  
  // Mise à jour de l'heure avec un service NTP
  void UpdateTimeFromNTP();

  // Changer le décalage horaire
  void SetTimezoneOffsetInSecond(uint32_t timezoneOffsetInSecond)
    { m_timezoneOffsetInSecond = timezoneOffsetInSecond; }
};

extern TimeManagerClass TimeManager;

#endif  // __TIME_MANAGER_H__
