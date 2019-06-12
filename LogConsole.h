#ifndef __LOG_CONSOLE_H__
#define __LOG_CONSOLE_H__

#include <EthernetServer.h>
#include <EthernetClient.h>

#define  LOG_MESSAGE   LogConsole.LogMessage
#define  LOG_NEW_LINE  LogConsole.LogMessage(F("\n"));
#define  LOG_MEM       LogConsole.LogMessage(F("\nRAM dispo : %d\n"), Utils::GetFreeRAM());
#define  LOG_SEP       LogConsole.LogMessage(F("------------------------------------------------------------\n"));
#define  LOG_SEP2      LogConsole.LogMessage(F("************************************************************\n"));


/*
  @class LogConsoleClass
  @brief Classe de gestion d'une console de log
*/
class LogConsoleClass
{
private:

protected:
  EthernetServer*  m_logTCPServer;      // Serveur TCP d'écoute
  EthernetClient   m_logTCPClient;      // Client TCP de log
  EthernetUDP*     m_logUDPSocket;      // Socket de log UDP
  IPAddress        m_logUDPSendIP;      // Adresse IP d'émission UDP
  uint16_t         m_logUDPSendPort;    // Port d'émission UDP

public:
  // Constructeur
  LogConsoleClass();

  // Destructeur
  virtual ~LogConsoleClass() {}

  // Initialisation
  void Initialize(uint16_t logTCPListenPort);

  // Initialisation
  void Initialize(const IPAddress& logUDPSendIP,
                  uint16_t         logUDPSendPort,
                  uint16_t         logUDPListenPort);
  
  // Afficher un log
  void LogMessage(const __FlashStringHelper* format, ...);

  // Traiter la réception de commandes
  void ProcessCommand();
};

extern LogConsoleClass LogConsole;

#endif  // __LOG_CONSOLE_H__
