#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

#include "Config.h"

#include <Arduino.h>
#include <RTClib.h>


/*
  @class TimeManagerClass
  @brief Classe de définition d'un gestionnaire d'heure avec horloge RTC
*/
class TimeManagerClass : public ISerializable
{
private:

protected:
  DS3231      m_RTC;                     // Horloge RTC
  uint32_t    m_timezoneOffsetInSecond;  // Décalage horaire
  
public:
  // Constructeur
  TimeManagerClass();

  // Destructeur
  virtual ~TimeManagerClass() {}

  // Initialisation
  void Initialize();

  // Lire la date et heure actuelle
  DateTime Now() const;

  // Lire la date et heure actuelle sous forme de texte
  String NowStr() const;
  
  // Log de la date et l'heure de l'ARDUINO
  void LogDateTime() const;
  
  // Mise à jour de l'heure avec un service NTP
  void UpdateTimeFromNTP();

  // Changer le décalage horaire
  void SetTimezoneOffsetInSecond(uint32_t timezoneOffsetInSecond)
    { m_timezoneOffsetInSecond = timezoneOffsetInSecond; }

  // Sérialiser l'objet vers ou depuis l'EEPROM
  virtual void Serialize(const SerializeMode& serializeMode,
                         int&                 EEPROMAddress);
};

extern TimeManagerClass TimeManager;

#endif  // __TIME_MANAGER_H__
