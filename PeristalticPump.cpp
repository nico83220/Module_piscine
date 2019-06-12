#include "PeristalticPump.h"
#include "Relay.h"
#include "ContactSensor.h"
#include "AlarmsManager.h"


/* 
  Constructeur
  @param name Nom du capteur
  @param pin Broche sur laquelle est connecté le relais
*/
PeristalticPump::PeristalticPump(const String&      name,
                                 Relay*             commandRelay,
                                 LiquidLevelSensor* liquidLevelSensor,
                                 ContactSensor*     flowSensor) : 
  Pump(name, commandRelay, liquidLevelSensor),
  m_flowSensor(flowSensor)
{
}


/*
  Destructeur
 */
PeristalticPump::~PeristalticPump()
{
}


/*
  Démarrer la pompe
  @param requestedRunningTimeInMs Durée de fonctionnement souhaitée
*/
void PeristalticPump::Start(uint32_t requestedRunningTimeInMs)
{
  // Si la pompe est arrêtée
  if ( IsRunning() == false )
  {
    // On va contrôler qu'une alarme de débit n'existe pas
    if ( ( m_flowSensor == NULL ) ||
         ( AlarmsManager.ExistsAlarm(m_flowSensor->GetName()) == false ) )
    {
      // On appelle le traitement du parent
      Pump::Start(requestedRunningTimeInMs);
    }
  }
}


/*
  Boucle de traitement à appeler dans le programme principal
*/
void PeristalticPump::Loop(uint32_t nowMillisec)
{
  // On effectue d'abord le traitement du parent
  Pump::Loop(nowMillisec);

  // Puis si la pompe est démarrée
  if ( IsRunning() == true )
  {
    // On va contrôler le capteur de débit
    if ( ( m_flowSensor != NULL ) &&
          ( m_flowSensor->GetContact() == CONTACT_OPENED ) )
    {
      // On arrête la pompe
      Stop();

      // On remonte une alarme de débit
      AlarmsManager.AddAlarm(m_flowSensor->GetName(), m_flowSensor, NO_FLOW_ALARM, NULL);
    }
  }
}