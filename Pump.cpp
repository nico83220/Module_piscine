#include "Pump.h"
#include "Relay.h"
#include "LiquidLevelSensor.h"
#include "AlarmsManager.h"


/* 
  Constructeur
  @param name Nom du capteur
  @param commandRelay Relais de commande de la pompe
  @param liquidLevelSensor Capteur de niveau de liquide à pomper
*/
Pump::Pump(const String&      name,
           Relay*             commandRelay,
           LiquidLevelSensor* liquidLevelSensor) : 
  Actuator(name, commandRelay),
  m_commandRelay(commandRelay),
  m_liquidLevelSensor(liquidLevelSensor)
{
}


/*
  Destructeur
 */
Pump::~Pump()
{
}


/* 
  Démarrer la pompe
  @param requestedRunningTimeInMs Durée de fonctionnement souhaitée
*/
void Pump::Start(uint32_t requestedRunningTimeInMs)
{
  // Si la pompe est arrêtée
  if ( IsRunning() == false )
  {
    // On va contrôler qu'une alarme de niveau de liquide n'existe pas
    if ( ( m_liquidLevelSensor == NULL ) ||
         ( AlarmsManager.ExistsAlarm(m_liquidLevelSensor->GetName()) == false ) )
    {
      // On appelle le traitement du parent
      Actuator::Start(requestedRunningTimeInMs);
    }
  }
}


/*
  Boucle de traitement à appeler dans le programme principal
*/
void Pump::Loop(uint32_t nowMillisec)
{
  // Si la pompe fonctionne
  if ( IsRunning() == true )
  {
    // Si on a atteint le fond du réservoir
    if ( ( m_liquidLevelSensor != NULL ) &&
         ( m_liquidLevelSensor->IsTooLow() == true ) )
    {
      // On arrête la pompe
      Stop();

      // On remonte une alarme de réservoir vide
      AlarmsManager.AddAlarm(m_liquidLevelSensor->GetName(), m_liquidLevelSensor, LOW_LEVEL_ALARM, NULL);
    }

    // Sinon, réservoir OK ou pas de contrôle de réservoir
    else
    {
      // On appelle le traitement du parent
      Actuator::Loop(nowMillisec);
    }
  }
}

