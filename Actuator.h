#ifndef __ACTUATOR_H__
#define __ACTUATOR_H__

#include "ObjectBase.h"
#include "Scheduler.h"
#include "Config.h"
#include "Relay.h"

#include <ArduinoSTL.h>
#include <vector>


/*
  @class Actuator
  @brief Classe abstraite definissant une classe générique de gestion d'actionneurs
*/
class Actuator : public ObjectBase,
                 public ISchedulable,
                 public ISerializable
{
private:

protected:
  Relay*    m_commandRelay;                      // Relais de commande de l'actionneur
  uint32_t  m_currentRunningTimeInMs;            // Durée actuelle de fonctionnement de l'actionneur 
  uint32_t  m_requestedRunningTimeInMs;          // Durée demandée de fonctionnement de l'actionneur
  uint32_t  m_maximumRunningTimeInMs;            // Durée maximum de fonctionnement de l'actionneur

  static std::vector< Actuator* >  m_actuators;  // Liste des actionneurs du système

  // Action programmée de démarrage
  virtual void ScheduledStart();

  // Action programmée d'arrêt
  virtual void ScheduledStop();

  // Lire si l'objet est démarré
  virtual bool ScheduledIsRunning() const;

public:
  // Constructeur
  Actuator(const String& name,
           Relay*        commandRelay);

  // Destructeur
  virtual ~Actuator();

  // Lire la durée actuelle de fonctionnement de l'actionneur
  uint32_t GetCurrentRunningTimeInMs() const
    { return m_currentRunningTimeInMs; }

  // Lire la durée demandée de fonctionnement de l'actionneur
  uint32_t GetRequestedRunningTimeInMs() const
    { return m_requestedRunningTimeInMs; }

  // Lire la durée maximum de fonctionnement de l'actionneur
  uint32_t GetMaximumRunningTimeInMs() const
    { return m_maximumRunningTimeInMs; }

  // Changer la durée max de fonctionnement de l'actionneur
  void SetMaximumRunningTimeInMs(uint32_t maximumRunningTimeInMs)
    { m_maximumRunningTimeInMs = maximumRunningTimeInMs; }

  // Initialisation de l'actionneur
  virtual void Initialize();

  // Démarrer l'actionneur
  virtual void Start(uint32_t requestedRunningTimeInMs);

  // Arrêter l'actionneur
  virtual void Stop();

  // Lire si l'actionneur est en route
  virtual bool IsRunning() const;

  // Boucle de traitement à appeler dans le programme principal
  virtual void Loop(uint32_t nowMillisec);

  // Sérialiser l'objet vers ou depuis l'EEPROM
  virtual void Serialize(const SerializeMode& serializeMode,
                         int&                 EEPROMAddress);

  // Initialiser tous les actionneurs
  static void InitializeAllActuators();

  // Effectuer le traitement cyclique de tous les actionneurs
  static void LoopAllActuators(uint32_t nowMillisec);
};

#endif  // __ACTUATOR_H__
