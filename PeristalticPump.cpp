#include "PeristalticPump.h"
#include "Relay.h"
#include "ContactSensor.h"

#include <Arduino.h>


/* 
  Constructeur
  @param name Nom du capteur
  @param pin Broche sur laquelle est connecté le relais
*/
PeristalticPump::PeristalticPump(const String&  name,
                                 Relay*         commandRelay,
                                 ContactSensor* flowSensor,
                                 ContactSensor* tankLevelSensor) : 
  ObjectBase(name),
  m_commandRelay(commandRelay),
  m_flowSensor(flowSensor),
  m_tankLevelSensor(tankLevelSensor)
{
}


/*
  Destructeur
 */
PeristalticPump::~PeristalticPump()
{
}


/* 
  Initialisation du relais
*/
void PeristalticPump::Initialize()
{
}

