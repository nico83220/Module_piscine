#ifndef __MQTT_CLIENT_H__
#define __MQTT_CLIENT_H__

#include "ObjectBase.h"

#define MQTT_SOCKET_TIMEOUT 3

#include <EthernetClient.h>
#include <PubSubClient.h>
#include <ArduinoSTL.h>
#include <map>


/*
  @class MQTTClientClass
  @brief Classe de gestion du client MQTT
*/
class MQTTClientClass
{
  typedef void (*CallbackFunction)(char*, uint8_t*, unsigned int);
  
private:

protected:
  String                 m_MQTTClientName;             // Nom du client MQTT
  EthernetClient         m_ethernetClient;             // Client ethernet pour le client MQTT
  PubSubClient           m_MQTTClient;                 // Client MQTT
  
  uint32_t               m_connectionPeriodInMs;       // Période de connexion
  uint32_t               m_lastConnectionTryTimeInMs;  // Heure de dernière temptative de connexion
  
  uint32_t               m_lastLoopTimeInMs;           // Heure de dernier traitement

  std::vector< String >  m_subscription;               // Liste de souscriptions

public:
  // Constructeur
  MQTTClientClass();

  // Destructeur
  virtual ~MQTTClientClass() {}

  // Initialiser le client MQTT
  void Initialize(const String&    MQTTClientName,
                  const IPAddress& MQTTServerIP,
                  uint16_t         MQTTServerPort,
                  unsigned int     connectionPeriodInMs);

  // Lire si le client MQTT est connecté
  bool IsConnected() const
    { return m_MQTTClient.connected(); }

  // Essayer de se connecter au serveur MQTT
  bool TryToConnect();

  // Publier une donnée MQTT
  bool Publish(const String& topic,
               const String& payload);

  // Changer la fonction de callback
  void SetCallback(CallbackFunction             function,
                   const std::vector< String >& subscription);

  // Boucle de traitement à appeler dans le programme principal
  void Loop(uint32_t nowMillisec);
};

extern MQTTClientClass MQTTClient;

#endif  // __MQTT_CLIENT_H__
