#include "Pump.h"
#include "Relay.h"

#include <Arduino.h>


/* 
  Constructeur
  @param name Nom du capteur
  @param pin Broche sur laquelle est connecté le relais
*/
Pump::Pump(const String&  name,
           Relay*         commandRelay) : 
  ObjectBase(name),
  m_commandRelay(commandRelay),
  m_running(false)
{
}


/*
  Destructeur
 */
Pump::~Pump()
{
}


/* 
  Initialisation de la pompe
*/
void Pump::Initialize()
{
}


/* 
  Démarrer la pompe
*/
void Pump::Start()
{
  m_running = true;
}


/* 
  Arrêter la pompe
*/
void Pump::Stop()
{
  m_running = false;
}
