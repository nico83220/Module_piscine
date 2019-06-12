#include "Actuator.h"
#include "LogConsole.h"
#include "Utils.h"
#include "AlarmsManager.h"

using namespace std;


/* static */
vector< Actuator* >  Actuator::m_actuators;


/* 
  Constructeur
  @param name Nom de l'actionneur
  @param commandRelay Relais de commande de la lumière
*/
Actuator::Actuator(const String& name,
                   Relay*        commandRelay) :
  ObjectBase(name),
  ISchedulable(name),
  ISerializable(name),
  m_commandRelay(commandRelay),
  m_currentRunningTimeInMs(0),
  m_requestedRunningTimeInMs(0),
  m_maximumRunningTimeInMs(0)
{
  Actuator::m_actuators.push_back(this);
}


/*
  Destructeur
 */
Actuator::~Actuator()
{
}


/*
  Action programmée de démarrage
*/
void Actuator::ScheduledStart()
{
  // On démarre sans durée demandée
  Start(0);
}


/*
  Action programmée d'arrêt
*/
void Actuator::ScheduledStop()
{
  // Si l'actionneur est démarré pour une durée prédéfinie, on ne fait rien
  // car il s'arrêtera à la fin de cette durée
  if ( m_requestedRunningTimeInMs == 0 )
  {
    Stop();
  }
}


/*
  Lire si l'objet est démarré
*/
bool Actuator::ScheduledIsRunning() const
{
  bool res = IsRunning();

  return res;
}


/* 
  Initialisation de l'actionneur
*/
void Actuator::Initialize()
{
  Config.RegisterSerializableObject(this);

  if ( m_commandRelay != NULL )
  {
    m_commandRelay->Initialize();
  }
}


/* 
  Démarrer l'actionneur
  @param requestedRunningTimeInMs Durée de fonctionnement de l'actionneur
*/
void Actuator::Start(uint32_t requestedRunningTimeInMs)
{
  // Si l'actionneur est arrêté
  if ( IsRunning() == false )
  {
    // On va contrôler qu'une alarme de durée max n'existe pas
    if ( AlarmsManager.ExistsAlarm(m_name) == false )
    {
      if ( m_commandRelay != NULL )
      {
        // On démarre l'actionneur en fermant le relais
        m_commandRelay->Close();

        // On remet la durée courante à 0
        m_currentRunningTimeInMs = 0;

        // On remet la durée demandée à 0
        m_requestedRunningTimeInMs = 0;

        // On positionne la durée demandée
        if ( requestedRunningTimeInMs > 0 )
        {
          // Si une durée max est définie et si la durée demandée est supérieure à la durée max
          // alors on limite à la durée max
          if ( ( m_maximumRunningTimeInMs > 0 ) &&
              ( requestedRunningTimeInMs > m_maximumRunningTimeInMs ) )
          {
            m_requestedRunningTimeInMs = m_maximumRunningTimeInMs;
          }

          // Sinon, on mémorise la durée demandée
          else
          {
            m_requestedRunningTimeInMs = requestedRunningTimeInMs;
          }
        }
      }
    }
  }
}


/* 
  Arrêter l'actionneur
*/
void Actuator::Stop()
{
  // Si l'actionneur est démarré
  if ( IsRunning() == true )
  {
    if ( m_commandRelay != NULL )
    {
      // On arrête l'actionneur en ouvrant le relais
      m_commandRelay->Open();

      // On remet la durée courante à 0
      m_currentRunningTimeInMs = 0;

      // On remet la durée demandée à 0
      m_requestedRunningTimeInMs = 0;
    }
  }
}


/*
  Lire si l'actionneur est en route
*/
bool Actuator::IsRunning() const
{ 
  bool res = false;

  if ( ( m_commandRelay != NULL ) &&
       ( m_commandRelay->GetState() == RELAY_CLOSED ) )
  {
    res = true;
  }

  return res; 
}


/*
  Boucle de traitement à appeler dans le programme principal
*/
void Actuator::Loop(uint32_t nowMillisec)
{
  // Si l'actionneur est démarré
  if ( IsRunning() == true )
  {
    // On met à jour la durée de fermeture actuel du relais 
    m_currentRunningTimeInMs += nowMillisec;

    // Si une durée de fonctionnement est définie
    if ( m_requestedRunningTimeInMs > 0 )
    {
      // Si on a atteint la durée de fermeture demandée
      if ( m_currentRunningTimeInMs >= m_requestedRunningTimeInMs )
      {
        // On arrête l'actionneur
        Stop();
      }
    }

    // Si une durée max est définie
    if ( m_maximumRunningTimeInMs > 0 )
    {
      // Si on a atteint la limite de durée de fermeture
      if ( m_currentRunningTimeInMs >= m_maximumRunningTimeInMs )
      {
        // On arrête l'actionneur
        Stop();

        // On remonte une alarme de durée max atteinte
        uint32_t* param = new uint32_t;
        *param = nowMillisec;
        AlarmsManager.AddAlarm(m_name, NULL, MAXIMUM_TIME_ALARM, param);
      }
    }
  }
}


/*
  Sérialiser l'objet vers ou depuis l'EEPROM
*/
void Actuator::Serialize(const SerializeMode& serializeMode,
                         int&                 EEPROMAddress)
{
  /*
  // Ecriture
  if ( serializeMode == SERIALIZE_MODE_WRITE )
  {
    bool running = IsRunning();
    EEPROM.updateByte(EEPROMAddress, running);
    EEPROMAddress += sizeof(uint8_t);
    EEPROM.updateLong(EEPROMAddress, m_currentRunningTimeInMs);
    EEPROMAddress += sizeof(uint32_t);
    EEPROM.updateLong(EEPROMAddress, m_requestedRunningTimeInMs);
    EEPROMAddress += sizeof(uint32_t);
    EEPROM.updateLong(EEPROMAddress, m_maximumRunningTimeInMs);
    EEPROMAddress += sizeof(uint32_t);
  }

  // Lecture
  else
  {
    bool running = EEPROM.readByte(EEPROMAddress);
    EEPROMAddress += sizeof(uint8_t);
    // On force l'état si besoin
    if ( running != IsRunning() )
    {
      if ( running == true )
      {
        Start(0);
      }
      else
      {
        Stop();
      }
    }
    // Et on écrase les valeurs par celles sauvegardées sauvegardées
    m_currentRunningTimeInMs = EEPROM.readLong(EEPROMAddress);
    EEPROMAddress += sizeof(uint32_t);
    m_requestedRunningTimeInMs = EEPROM.readLong(EEPROMAddress);
    EEPROMAddress += sizeof(uint32_t);
    m_maximumRunningTimeInMs = EEPROM.readLong(EEPROMAddress);
    EEPROMAddress += sizeof(uint32_t);
  }
  */
}


/* 
  Initialiser tous les actionneurs
*/
/* static */
void Actuator::InitializeAllActuators()
{
  vector< Actuator* >::iterator it;
  for ( it=m_actuators.begin(); it!=m_actuators.end(); ++it )
  {
    LOG_MESSAGE(F("%s ...\n"), (*it)->m_name.c_str());
    (*it)->Initialize();
  }
}


/*
  Effectuer le traitement cyclique de tous les actionneurs
*/
/* static */
void Actuator::LoopAllActuators(uint32_t nowMillisec)
{
  vector< Actuator* >::iterator it;
  for ( it=m_actuators.begin(); it!=m_actuators.end(); ++it )
  {
    (*it)->Loop(nowMillisec);
  }
}
