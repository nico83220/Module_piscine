#include "Scheduler.h"
#include "TimeManager.h"
#include "Utils.h"
#include "LogConsole.h"


/*
  Constructeur
*/
ISchedulable::ISchedulable(const String& objectName)
{
  Scheduler.RegisterSchedulableObject(objectName, this);
}


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


/*
  Opérateur d'égalité
*/
bool ScheduledTask::operator==(const ScheduledTask& task) const
{
  bool res = false;

  if ( ( m_objectName  == task.m_objectName ) &&
       ( m_startHour   == task.m_startHour ) &&
       ( m_startMinute == task.m_startMinute ) &&
       ( m_stopHour    == task.m_stopHour ) &&
       ( m_stopMinute  == task.m_stopMinute ) )
  {
    res = true;
  }

  return res;
}


// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------


/*
  Constructeur
*/
SchedulerClass::SchedulerClass() :
  ISerializable("SchedulerClass")
{
}


/*
  Ajouter/modifier une programmation
*/
void SchedulerClass::AddOrModifyScheduledTask(const String& objectName,
                                              uint8_t       startHour,
                                              uint8_t       startMinute,
                                              uint8_t       stopHour,
                                              uint8_t       stopMinute)
{
  // On vérifie qu'un objet de ce nom est programmable
  std::map< String, ISchedulable* >::iterator itObj = m_schedulableObjects.find(objectName);
  if ( itObj != m_schedulableObjects.end() )
  {
    // On vérifie la cohérence des heures
    DateTime taskStartTime = TimeManager.Now();
    taskStartTime.sethour(startHour);
    taskStartTime.setminute(startMinute);
    DateTime taskStopTime = taskStartTime;
    taskStopTime.sethour(stopHour);
    taskStopTime.setminute(stopMinute);

    // L'heure de début doit précéder l'heure de fin
    if ( taskStartTime < taskStopTime )
    {
      bool canAdd = true;

      // On va rechercher si des taches pour cet objet existent
      std::vector< ScheduledTask* >::iterator itTask;
      for ( itTask=m_scheduledTasks.begin(); itTask!=m_scheduledTasks.end(); ++itTask )
      {
        ScheduledTask* task = *itTask;

        // Si j'ai trouvé une tache existante pour cet objet
        if ( task->m_objectName == objectName )
        {
          DateTime existingTaskStartTime = taskStartTime;
          existingTaskStartTime.sethour(task->m_startHour);
          existingTaskStartTime.setminute(task->m_startMinute);
          DateTime existingTaskStopTime = existingTaskStartTime;
          existingTaskStopTime.sethour(task->m_stopHour);
          existingTaskStopTime.setminute(task->m_stopMinute);

          // La nouvelle tache ne doit pas démarrer ni s'arrêter dans l'intervalle de celle-ci,
          // ni démarrer avant et s'arrêter après (englober)
          if ( ( ( taskStartTime >= existingTaskStartTime ) &&
                 ( taskStartTime <= existingTaskStopTime ) ) ||
               ( ( taskStopTime >= existingTaskStartTime ) &&
                 ( taskStopTime <= existingTaskStopTime ) ) ||
               ( ( taskStartTime < existingTaskStartTime ) &&
                 ( taskStopTime  > existingTaskStopTime ) ) )
          {
            canAdd = false;
            break;
          }
        }
      }
      
      // Si je peux ajouter la nouvelle tache
      if ( canAdd == true )
      {
        ScheduledTask* task = new ScheduledTask();
        task->m_objectName  = objectName;
        task->m_startHour   = startHour;
        task->m_startMinute = startMinute;
        task->m_stopHour    = stopHour;
        task->m_stopMinute  = stopMinute;
        m_scheduledTasks.push_back(task);
      }
    }
  }
}


/*
  Supprimer toutes les programmations d'un objet
*/
void SchedulerClass::RemoveAllScheduledTasks(const String& objectName)
{
  std::vector< ScheduledTask* >::iterator itTask = m_scheduledTasks.begin();
  while ( itTask != m_scheduledTasks.end() )
  {
    ScheduledTask* task = *itTask;

    // Si on a trouvé une tache affecté à cet objet
    if ( task->m_objectName == objectName )
    {
      // On la détruit et on l'enlève du vector
      delete(task);
      m_scheduledTasks.erase(itTask);

      // On repart au début car on a modifié le vecteur
      itTask = m_scheduledTasks.begin();
    }

    // Sinon, cela ne concerne pas l'objet, on passe au suivant
    else
    {
      ++itTask;
    }
    
  }
}


/*
  Supprimer toutes les programmations pour un objet
*/
void SchedulerClass::RemoveAllScheduledTasks()
{
  std::vector< ScheduledTask* >::iterator itTask;
  for ( itTask=m_scheduledTasks.begin(); itTask!=m_scheduledTasks.end(); ++itTask )
  {
    delete(*itTask);
  }
  m_scheduledTasks.clear();
}


/*
  Boucle de traitement à appeler dans le programme principal
*/
void SchedulerClass::Loop(uint32_t nowMillisec)
{
  // Heure actuelle
  DateTime now = TimeManager.Now();

  // On parcourt la liste des programmations
  std::vector< ScheduledTask* >::iterator itTask;
  for ( itTask=m_scheduledTasks.begin(); itTask!=m_scheduledTasks.end(); ++itTask )
  {
    // Tache
    ScheduledTask* task = *itTask;
    DateTime taskStartTime = now;
    taskStartTime.sethour(task->m_startHour);
    taskStartTime.setminute(task->m_startMinute);
    DateTime taskStopTime = now;
    taskStopTime.sethour(task->m_stopHour);
    taskStopTime.setminute(task->m_stopMinute);

    // Objet programmable
    std::map< String, ISchedulable* >::iterator itObj = m_schedulableObjects.find(task->m_objectName);
    if ( itObj != m_schedulableObjects.end() )
    {
      ISchedulable* obj = itObj->second;
      
      // Si l'objet n'est pas démarré 
      if ( obj->ScheduledIsRunning() == false )
      {
        // Si on est dans le créneau de démarrage
        if ( ( now >= taskStartTime ) &&
             ( now <= taskStopTime ) )
        {
          // On démarre l'objet
          obj->ScheduledStart();
        }
      }

      // Sinon, l'objet est démarré
      else
      {
        // Si on est en dehors du créneau de démarrage
        if ( ( now < taskStartTime ) ||
             ( now > taskStopTime ) )
        {
          // On arrête l'objet
          obj->ScheduledStop();
        }
      }
    }
  }
}


/*
  Sérialiser l'objet vers ou depuis l'EEPROM
*/
void SchedulerClass::Serialize(const SerializeMode& serializeMode,
                                int&                 EEPROMAddress)
{
  // Ecriture
  if ( serializeMode == SERIALIZE_MODE_WRITE )
  {
    // Nombre de taches
    EEPROM.updateLong(EEPROMAddress, (uint32_t) m_scheduledTasks.size());
    EEPROMAddress += sizeof(uint32_t);

    // Sérialisation de chaque tache
    std::vector< ScheduledTask* >::iterator itTask;
    for ( itTask=m_scheduledTasks.begin(); itTask!=m_scheduledTasks.end(); ++itTask )
    {
      // Tache
      ScheduledTask* task = *itTask;
      Utils::UpdateStringInEEPROM(EEPROMAddress, task->m_objectName);
      EEPROM.updateByte(EEPROMAddress, task->m_startHour);
      EEPROMAddress += sizeof(uint8_t);
      EEPROM.updateByte(EEPROMAddress, task->m_startMinute);
      EEPROMAddress += sizeof(uint8_t);
      EEPROM.updateByte(EEPROMAddress, task->m_stopHour);
      EEPROMAddress += sizeof(uint8_t);
      EEPROM.updateByte(EEPROMAddress, task->m_stopMinute);
      EEPROMAddress += sizeof(uint8_t);
    }
  }

  // Lecture
  else
  {
    // Avant de lire depuis la mémoire, on vide les taches existantes
    RemoveAllScheduledTasks();

    // Nombre de taches
    uint32_t tasksCount = EEPROM.readLong(EEPROMAddress);
    EEPROMAddress += sizeof(uint32_t);

    LOG_MESSAGE(F("SchedulerClass : lecture configuration (tasksCount = %u)"),
                tasksCount);

    // Sérialisation de chaque tache
    for ( uint32_t i=0; i<tasksCount; ++i )
    {
      String  objectName  = Utils::ReadStringInEEPROM(EEPROMAddress);
      uint8_t startHour   = EEPROM.readByte(EEPROMAddress);
      EEPROMAddress += sizeof(uint8_t);
      uint8_t startMinute = EEPROM.readByte(EEPROMAddress);
      EEPROMAddress += sizeof(uint8_t);
      uint8_t stopHour    = EEPROM.readByte(EEPROMAddress);
      EEPROMAddress += sizeof(uint8_t);
      uint8_t stopMinute  = EEPROM.readByte(EEPROMAddress);
      EEPROMAddress += sizeof(uint8_t);

      LOG_MESSAGE(F("SchedulerClass : lecture configuration (objectName = %s - startHour = %u - startMinute = %u - stopHour = %u - stopMinute = %u)"),
                  objectName.c_str(),
                  startHour,
                  startMinute,
                  stopHour,
                  stopMinute);

      // Ajout de la tache
      AddOrModifyScheduledTask(objectName, startHour, startMinute, stopHour, stopMinute);
    }
  }
}


/*
  Enregistrement d'un objet programmable
*/
void SchedulerClass::RegisterSchedulableObject(const String& objectName,
                                               ISchedulable* schedulableObject)
{
  m_schedulableObjects[objectName] = schedulableObject;
}


SchedulerClass Scheduler;
