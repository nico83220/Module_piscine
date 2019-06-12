#include "LogConsole.h"
#include "Utils.h"
#include "TimeManager.h"

#include <TimeLib.h>


/*
  Constructeur
*/
LogConsoleClass::LogConsoleClass() :
  m_logTCPServer(NULL),
  m_logTCPClient(NULL),
  m_logUDPSocket(NULL)
{
}


/*
  Initialisation
  @param logTCPListenPort Port d'écoute pour les connexion TCP
*/
void LogConsoleClass::Initialize(uint16_t logTCPListenPort)
{
  // On démarre le serveur
  m_logTCPServer = new EthernetServer(logTCPListenPort);
  m_logTCPServer->begin();

  // On attend une connexion
  m_logTCPClient = m_logTCPServer->available();
}


/*
  Initialisation
*/
void LogConsoleClass::Initialize(const IPAddress& logUDPSendIP,
                                 uint16_t         logUDPSendPort,
                                 uint16_t         logUDPListenPort)
{
  // Configuration de la socket UDP de log et communication
  m_logUDPSocket = new EthernetUDP();
  m_logUDPSocket->begin(logUDPListenPort);

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

  if ( m_logUDPSocket != NULL )
  {
    while ( m_logUDPSocket->beginPacket(m_logUDPSendIP, m_logUDPSendPort) == 0 );

    // Emission UDP
    size_t len  = strlen(textBuffer);
    m_logUDPSocket->write(textBuffer, len);
    m_logUDPSocket->endPacket();
  }

  else if ( m_logTCPClient == true )
  {
    //size_t len  = strlen(textBuffer);
    //m_logClient.write(textBuffer, len);
    m_logTCPClient.print(textBuffer);
  }

  // Ecriture sur la liaison série
  Serial.print(textBuffer);
}


// Buffer de réception
#define  PACKET_BUFFER_SIZE  30
char packerBuffer[PACKET_BUFFER_SIZE];

/*
  Traiter la réception de commandes
*/
void LogConsoleClass::ProcessCommand()
{
  uint8_t nbChars = 0;

  if ( m_logUDPSocket != NULL )
  {
    // Y a-t-il une donnée sur la socket UDP ?
    int packetSize = m_logUDPSocket->parsePacket();
    if ( packetSize > 0 ) 
    {
      // Lecture de la donnée sur la socket UDP
      memset(packerBuffer, 0, PACKET_BUFFER_SIZE);
      nbChars = m_logUDPSocket->read(packerBuffer, PACKET_BUFFER_SIZE);
    }
  }

  else if ( m_logTCPClient == true )
  {
    // Y a-t-il une donnée sur la socket client ?
    if ( m_logTCPClient.available() == true ) 
    {
      // Lecture de la donnée sur la socket client
      memset(packerBuffer, 0, PACKET_BUFFER_SIZE);
      nbChars = m_logTCPClient.read(packerBuffer, PACKET_BUFFER_SIZE);
    }
  }

  if ( nbChars > 0 )
  {
    // Si c'est la commande de RESET
    if ( strncmp(packerBuffer, "reset", 5) == 0 )
    {
      LogMessage(F("\nModule_piscine -> SoftwareReset ...\n\n"));
      Utils::SoftwareReset();
    }

    else if ( strncmp(packerBuffer, "ntp", 3) == 0 )
    {
      LogMessage(F("\nModule_piscine -> SynchroNTP ...\n\n"));
      TimeManager.UpdateTimeFromNTP();
    }

    else if ( strncmp(packerBuffer, "date", 4) == 0 )
    {
      TimeManager.LogDateTime();
    }
    
    else if ( strncmp(packerBuffer, "decal1", 6) == 0 )
    {
      // Décalage horaire d'une heure
      LogMessage(F("Decalage horaire d'une heure\n"));
      TimeManager.SetTimezoneOffsetInSecond(1L * 3600L);
    }
    
    else if ( strncmp(packerBuffer, "decal2", 6) == 0 )
    {
      // Décalage horaire d'une heure
      LogMessage(F("Decalage horaire de 2 heures\n"));
      TimeManager.SetTimezoneOffsetInSecond(2L * 3600L);
    }
    
    else
    {
      LogMessage(F("Module_piscine -> commande '%s' inconnue\n"),
                packerBuffer);
    }
  }
}


LogConsoleClass LogConsole;
