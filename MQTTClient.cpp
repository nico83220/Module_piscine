#include "MQTTClient.h"
#include "LogConsole.h"
#include "Utils.h"


/*
  Constructeur
*/
MQTTClientClass::MQTTClientClass() :
  m_MQTTClientName(),
  m_ethernetClient(),
  m_MQTTClient(),
  m_connectionPeriodInMs(0),
  m_lastConnectionTryTimeInMs(0),
  m_lastLoopTimeInMs(0),
  m_subscription()
{
}


/* 
  Initialiser le client MQTT
  @param MQTTClientName Nom du client MQTT
  @param MQTTServerIP Adresse IP du serveur MQTT
  @param MQTTServerPort Port du serveur MQTT
*/
void MQTTClientClass::Initialize(const String&    MQTTClientName,
                                 const IPAddress& MQTTServerIP,
                                 uint16_t         MQTTServerPort,
                                 unsigned int     connectionPeriodInMs)
{
  m_MQTTClientName       = MQTTClientName;
  m_connectionPeriodInMs = connectionPeriodInMs;
  m_MQTTClient.setServer(MQTTServerIP, MQTTServerPort);
  m_MQTTClient.setClient(m_ethernetClient);
}


/* 
  Essayer de se connecter au serveur MQTT
  @return bool True si le client est connecté, false sinon
*/
bool MQTTClientClass::TryToConnect()
{
  // Connexion au broker MQTT
  LOG_MESSAGE(F("\nConnexion au serveur MQTT\n"));

  if ( m_MQTTClient.connect(m_MQTTClientName.c_str()) == true )
  {
    LOG_MESSAGE(F("Connecte au serveur MQTT\n"));

    // Une fois connecté, je publie une annonce
    String topic = m_MQTTClientName + Utils::GetStringFromDict(ALIVE_TOPIC);
    m_MQTTClient.publish(topic.c_str(), Utils::GetStringFromDict(ALIVE_PAYLOAD).c_str());

    // Et je souscris aux requètes voulues
    std::vector< String >::iterator it;
    for ( it=m_subscription.begin(); it!=m_subscription.end(); ++it )
    {
      String request = m_MQTTClientName + Utils::GetStringFromDict(SLASH) + *it;
      m_MQTTClient.subscribe(request.c_str());
    }
  }

  return m_MQTTClient.connected();
}


/*
  Publier une donnée MQTT
*/
bool MQTTClientClass::Publish(const String& topic,
                              const String& payload)
{
  bool res = false;

  if ( m_MQTTClient.connected() == true )
  {
    m_MQTTClient.publish(topic.c_str(), payload.c_str());
  }

  return res;
}


/*
  Changer la fonction de callback
  @param function Fonction de callback
  @param subscription Liste de souscriptions
*/
void MQTTClientClass::SetCallback(CallbackFunction             function,
                                  const std::vector< String >& subscription)
{
  m_MQTTClient.setCallback(function);
  m_subscription = subscription;
}


/* 
  Boucle de traitement à appeler dans le programme principal
  @param nowMillisec Heure actulelle en ms
*/
void MQTTClientClass::Loop(uint32_t nowMillisec)
{
  // Gestion de la connexion MQTT
  if ( m_MQTTClient.connected() == false )
  {
    // Si la période d'essai est atteinte
    if ( nowMillisec >= m_lastConnectionTryTimeInMs + m_connectionPeriodInMs )
    {
      // On mémorise l'heure de l'essai de connexion
      m_lastConnectionTryTimeInMs = nowMillisec;
      
      // On essaie de se connecter
      if ( TryToConnect() == true )
      {
        // Si on a réussi, on raz l'heure de dernier essai
        m_lastConnectionTryTimeInMs = 0;
      }
    }
  }

  // Si on est connecté, on va effectuer le reste des traitements
  if ( m_MQTTClient.connected() == true )
  {
    m_MQTTClient.loop();
  }
}


MQTTClientClass MQTTClient;
