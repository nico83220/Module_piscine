#ifndef __WEB_SERVER_H__
#define __WEB_SERVER_H__

#include <Arduino.h>
#include <EthernetServer.h>


/*
  @class WebServerClass
  @brief Classe de gestion d'un serveur web
*/
class WebServerClass
{
private:

protected:
  EthernetServer  m_webServer;      // Serveur TCP d'écoute

  // Lire le fichier XML contenant les données sytème
  void XML_response(const EthernetClient& client);

public:
  // Constructeur
  WebServerClass();

  // Destructeur
  virtual ~WebServerClass() {}

  // Initialisation
  void Initialize();

  // Boucle de traitement à appeler dans le programme principal
  void Loop(uint32_t nowMillisec);
};

extern WebServerClass WebServer;

#endif  // __WEB_SERVER_H__
