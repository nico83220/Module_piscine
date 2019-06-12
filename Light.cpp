#include "Light.h"

#include <Arduino.h>


/* 
  Constructeur
  @param name Nom du capteur
  @param commandRelay Relais de commande de la lumière
*/
Light::Light(const String& name,
             Relay*        commandRelay) : 
  Actuator(name, commandRelay)
{
}


/*
  Destructeur
 */
Light::~Light()
{
}


/*
  Lire l'état de la lumière
*/
LightState Light::GetState() const
{
  LightState res = LIGHT_OFF;

  if ( IsRunning() == true )
  {
    res = LIGHT_ON;
  }

  return res; 
}
