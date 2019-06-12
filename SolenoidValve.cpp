#include "SolenoidValve.h"


/* 
  Constructeur
  @param name Nom du capteur
  @param commandRelay Relais de commande de l'électrovanne
*/
SolenoidValve::SolenoidValve(const String& name,
                             Relay*        commandRelay) : 
  Actuator(name, commandRelay)
{
}


/*
  Destructeur
 */
SolenoidValve::~SolenoidValve()
{
}


/*
  Lire l'état de l'électrovanne
*/
ValveState SolenoidValve::GetState() const
{
  ValveState res = VALVE_CLOSED;

  if ( IsRunning() == true )
  {
    res = VALVE_OPENED;
  }

  return res; 
}
