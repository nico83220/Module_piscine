#ifndef __LOG_CONSOLE_H__
#define __LOG_CONSOLE_H__

#include <EthernetUdp.h>

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
  EthernetUDP  m_logUDPSocket;      // Socket de log UDP
  IPAddress    m_logUDPSendIP;      // Adresse IP d'émission UDP
  uint16_t     m_logUDPSendPort;    // Port d'émission UDP

public:
  // Initialisation
  void Initialize(const IPAddress& logUDPSendIP,
                  uint16_t         logUDPSendPort,
                  uint16_t         logUDPListenPort);
  
  // Afficher un log
  void LogMessage(const __FlashStringHelper* format, ...);

  // Traiter la réception de commandes
  void ProcessUDPCommand();
};

extern LogConsoleClass LogConsole;

#endif  // __LOG_CONSOLE_H__
