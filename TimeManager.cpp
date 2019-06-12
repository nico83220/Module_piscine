#include "TimeManager.h"
#include "Utils.h"
#include "LogConsole.h"

#include <NTPClient.h>


/*
  Constructeur
*/
TimeManagerClass::TimeManagerClass() :
  ISerializable("TimeManagerClass"),
  m_RTC(),
  m_timezoneOffsetInSecond(0)
{
}


/*
  Initialisation
*/
void TimeManagerClass::Initialize()
{
  // Initialisation de l'horloge RTC
  m_RTC.begin();

  // Décalage horaire de 2 heures par défaut
  m_timezoneOffsetInSecond = 2L * 3600L;
}


/*
  Lire la date et heure actuelle
*/
DateTime TimeManagerClass::Now() const
{
  DateTime res = m_RTC.now();
  return res;
}


char timeBuffer[25];

/*
  Lire la date et heure actuelle sous forme de texte
*/
String TimeManagerClass::NowStr() const
{
  DateTime now = m_RTC.now();
  memset(timeBuffer, 0, 25);
  sprintf(timeBuffer, "%02d-%02d-%d %02d:%02d:%02d", 
          now.day(), now.month(), now.year(), now.hour(), now.minute(), now.second());
  String res(timeBuffer);

  return res;
}


/* 
  Log de la date et l'heure de l'ARDUINO
*/
void TimeManagerClass::LogDateTime() const
{
  DateTime dtNow = m_RTC.now();
  LOG_MESSAGE(F("Heure RTC : %02u/%02u/%4u %02u:%02u\n"),
              (uint8_t) dtNow.day(), (uint8_t) dtNow.month(), (uint16_t) dtNow.year(), 
              (uint8_t) dtNow.hour(), (uint8_t) dtNow.minute());
}


/*
  Mise à jour de l'heure avec un service NTP
*/
void TimeManagerClass::UpdateTimeFromNTP()
{
  uint32_t NTPUpdateRequestTimeInMs = millis();

  static uint32_t s_lastNTPUpdateTimeMillisec = 0;
  if ( NTPUpdateRequestTimeInMs - s_lastNTPUpdateTimeMillisec >= 30000L )
  {
    LOG_MESSAGE(F("Demarrage de la synchro NTP\n"));

    uint8_t queryBuffer[48];
    memset(queryBuffer, 0, 48);
    queryBuffer[0] = 0x1B;
    
    EthernetUDP udp;
    static bool s_firstTime = true;
    if ( s_firstTime == true )
    {
      udp.begin(123);
      s_firstTime = false;
    }
    while ( udp.beginPacket(Utils::GetStringFromDict(NTP_SERVER).c_str(), 123) == 0 );
    udp.write(queryBuffer, 48);
    udp.endPacket();

    memset(queryBuffer, 0, 48);
    bool timedOut = false;
    uint32_t waitStartTimeInMs = millis();
    while ( udp.parsePacket() == 0 )
    {
      uint32_t ellapsedTimeInMs = millis() - waitStartTimeInMs;
      if ( ellapsedTimeInMs > 2000L )
      {
        timedOut = true;
        LOG_MESSAGE(F("Reponse UDP non recue apres %lu ms !\n"), ellapsedTimeInMs);
        break;
      }
    }

    // Si on a reçu la réponse
    if ( timedOut == false )
    {
      udp.read(queryBuffer, 48);
  
      byte byte0 = queryBuffer[40];
      byte byte1 = queryBuffer[41];
      byte byte2 = queryBuffer[42];
      byte byte3 = queryBuffer[43];
   
      uint32_t uint32_0 = byte0;
      uint32_t uint32_1 = byte1;
      uint32_t uint32_2 = byte2;
      uint32_t uint32_3 = byte3;
      uint32_0 = uint32_0 << 24;
      uint32_1 = uint32_1 << 16;
      uint32_2 = uint32_2 << 8;
      uint32_3 = uint32_3 << 0;
      
      // On combine les 4 octets pour former un entier de 32 bits : c'est l'heure NTP en secondes depuis le 01/01/1900
      uint32_t secsSince1900 = uint32_0 | uint32_1 | uint32_2 | uint32_3;

      // Puis en secondes depuis 1970
      static const uint32_t TIME_1970 = 2208988800L;
      uint32_t secsSince1970 = secsSince1900 - TIME_1970;
  
      // Décalage horaire
      secsSince1970 = secsSince1970 + m_timezoneOffsetInSecond;
      
      // Heure NTP
      DateTime dtNow(secsSince1970);
      LOG_MESSAGE(F("Heure NTP : %02u/%02u/%4u %02u:%02u (%lu)\n"),
                  (uint8_t) dtNow.day(), (uint8_t) dtNow.month(), (uint16_t) dtNow.year(), 
                  (uint8_t) dtNow.hour(), (uint8_t) dtNow.minute(), secsSince1970);

      // Mise à jour de l'horloge RTC
      m_RTC.adjust(dtNow);

      LogDateTime();
    
      s_lastNTPUpdateTimeMillisec = NTPUpdateRequestTimeInMs;
    }
  }

  else
  {
    LOG_MESSAGE(F("\nMinimum de 30s entre 2 mises a jour NTP\n"));
  }
}


/*
  Sérialiser l'objet vers ou depuis l'EEPROM
*/
void TimeManagerClass::Serialize(const SerializeMode& serializeMode,
                                 int&                 EEPROMAddress)
{
  // Ecriture
  if ( serializeMode == SERIALIZE_MODE_WRITE )
  {
    EEPROM.updateLong(EEPROMAddress, m_timezoneOffsetInSecond);
    EEPROMAddress += sizeof(uint32_t);
  }

  // Lecture
  else
  {
    m_timezoneOffsetInSecond = EEPROM.readLong(EEPROMAddress);
    EEPROMAddress += sizeof(uint32_t);

    LOG_MESSAGE(F("TimeManagerClass : lecture configuration (m_timezoneOffsetInSecond = %u)"),
                m_timezoneOffsetInSecond);
  }
}


TimeManagerClass TimeManager;
