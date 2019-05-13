#include "LogConsole.h"
#include "Utils.h"
#include "TimeManager.h"


#include <TimeLib.h>


/*
  Initialisation
  @param logUDPSendIP Adresse IP d'émission UDP
  @param logUDPSendPort Port d'émission UDP
  @param logUDPListenPort Port d'écoute UDP
*/
void LogConsoleClass::Initialize(const IPAddress& logUDPSendIP,
                                 uint16_t         logUDPSendPort,
                                 uint16_t         logUDPListenPort)
{
  // Configuration de la socket UDP de log et communication
  m_logUDPSocket.begin(logUDPListenPort);

  m_logUDPSendIP   = logUDPSendIP;
  m_logUDPSendPort = logUDPSendPort;
}


#define  BUFFER_SIZE  256

/* 
  Afficher un log
  @param format Chaine de format type printf
*/
void LogConsoleClass::LogMessage(const __FlashStringHelper* format, ...)
{
  // Pour récupérer le texte dans la mémoire PROGMEM
  char formatBuffer[BUFFER_SIZE];
  memset(formatBuffer, 0, BUFFER_SIZE);
  strcpy_P(formatBuffer, (char*) format);

  char textBuffer[BUFFER_SIZE];
  memset(textBuffer, 0, BUFFER_SIZE);
  va_list argList;
  va_start(argList, format);
  vsnprintf(textBuffer, BUFFER_SIZE, formatBuffer, argList);
  va_end(argList);

  Utils::UTF8ToASCII(textBuffer);

  while ( m_logUDPSocket.beginPacket(m_logUDPSendIP, m_logUDPSendPort) == 0 );

  // Emission UDP
  size_t len  = strlen(textBuffer);
  m_logUDPSocket.write(textBuffer, len);
  m_logUDPSocket.endPacket();
  
  // Ecriture sur la liaison série
  Serial.print(textBuffer);

//  delay(10);
}


// Buffer de réception
char UDPPackerBuffer[UDP_TX_PACKET_MAX_SIZE];

/*
  Traiter la réception de commandes
*/
void LogConsoleClass::ProcessUDPCommand()
{
  // Y a-t-il une donnée sur la socket UDP ?
  int packetSize = m_logUDPSocket.parsePacket();
  if ( packetSize > 0 ) 
  {
    // Lecture de la donnée sur la socket UDP
    memset(UDPPackerBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
    m_logUDPSocket.read(UDPPackerBuffer, UDP_TX_PACKET_MAX_SIZE);

    // Si c'est la commande de RESET
    if ( ( strncmp(UDPPackerBuffer, "reset", 5) == 0 ) ||
         ( strncmp(UDPPackerBuffer, "RESET", 5) == 0 ) )
    {
      LogMessage(F("\nModule_piscine -> SoftwareReset ...\n\n"));
      Utils::SoftwareReset();
    }

    else if ( ( strncmp(UDPPackerBuffer, "ntp", 3) == 0 ) ||
              ( strncmp(UDPPackerBuffer, "NTP", 3) == 0 ) )
    {
      LogMessage(F("\nModule_piscine -> SynchroNTP ...\n\n"));
      TimeManager.UpdateTimeFromNTP();
    }

    else if ( ( strncmp(UDPPackerBuffer, "date", 4) == 0 ) ||
              ( strncmp(UDPPackerBuffer, "DATE", 4) == 0 ) )
    {
      TimeManager.LogDateTime();
    }
    
    else if ( ( strncmp(UDPPackerBuffer, "decal1", 6) == 0 ) ||
              ( strncmp(UDPPackerBuffer, "DECAL1", 6) == 0 ) )
    {
      // Décalage horaire d'une heure
      LOG_MESSAGE(F("Decalage horaire d'une heure\n"));
      TimeManager.SetTimezoneOffsetInSecond(1L * 3600L);
    }
    
    else if ( ( strncmp(UDPPackerBuffer, "decal2", 6) == 0 ) ||
              ( strncmp(UDPPackerBuffer, "DECAL2", 6) == 0 ) )
    {
      // Décalage horaire d'une heure
      LOG_MESSAGE(F("Decalage horaire de 2 heures\n"));
      TimeManager.SetTimezoneOffsetInSecond(2L * 3600L);
    }
    
    else
    {
      LogMessage(F("Module_piscine -> commande '%s' inconnue\n"),
                 UDPPackerBuffer);
    }
  }
}


LogConsoleClass LogConsole;
