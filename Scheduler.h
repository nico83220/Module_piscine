#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "Config.h"

#include <ArduinoSTL.h>
#include <map>
#include <vector>


/*
  @class ISchedulable
  @brief Interface pour la programmation des objets  
*/
class ISchedulable
{
  friend class SchedulerClass;

private:

protected:
  // Action programmée de démarrage
  virtual void ScheduledStart() = 0;

  // Action programmée d'arrêt
  virtual void ScheduledStop() = 0;

  // Lire si l'objet est démarré
  virtual bool ScheduledIsRunning() const = 0;

public:
  // Constructeur
  ISchedulable(const String& objectName);
};


/*
  @struct ScheduledTask
  @brief Classe représentant une tache programmée
*/
struct ScheduledTask
{
  String   m_objectName;
  uint8_t  m_startHour;
  uint8_t  m_startMinute;
  uint8_t  m_stopHour;
  uint8_t  m_stopMinute;

  // Opérateur d'égalité
  bool operator==(const ScheduledTask& task) const;
};


/*
  @class SchedulerClass
  @brief Classe de gestion d'un programmateur
*/
class SchedulerClass : public ISerializable
{
private:

protected:
  // Liste des objets programmables du système
  std::map< String, ISchedulable* >  m_schedulableObjects;

  // Liste des programmations
  std::vector< ScheduledTask* >      m_scheduledTasks;

public:
  // Constructeur
  SchedulerClass();

  // Destructeur
  virtual ~SchedulerClass() {}

  // Ajouter/modifier une programmation
  void AddOrModifyScheduledTask(const String& objectName,
                                uint8_t       startHour,
                                uint8_t       startMinute,
                                uint8_t       stopHour,
                                uint8_t       stopMinute);

  // Supprimer toutes les programmations d'un objet
  void RemoveAllScheduledTasks(const String& objectName);

  // Supprimer toutes les programmations 
  void RemoveAllScheduledTasks();

  // Boucle de traitement à appeler dans le programme principal
  void Loop(uint32_t nowMillisec);

  // Sérialiser l'objet vers ou depuis l'EEPROM
  virtual void Serialize(const SerializeMode& serializeMode,
                         int&                 EEPROMAddress);

  // Enregistrement d'un objet programmable
  void RegisterSchedulableObject(const String& objectName,
                                 ISchedulable* schedulableObject);
};

extern SchedulerClass Scheduler;

#endif  // __SCHEDULER_H__
