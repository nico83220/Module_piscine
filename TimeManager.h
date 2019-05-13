#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

#include <RTClib.h>


/*
  @class TimeManagerClass
  @brief Classe de gestion d'une console de log
*/
class TimeManagerClass
{
private:

protected:
  RTC_DS3231  m_RTC;                     // Horloge RTC
  bool        m_RTC_OK;                  // Flag indiquant que l'horloge RTC est OK
  uint32_t    m_timezoneOffsetInSecond;  // Décalage horaire
  
public:
  // Initialisation
  void Initialize();

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
