#include "Relay.h"
#include "LogConsole.h"

#include <Arduino.h>


/* 
  Constructeur
  @param name Nom du capteur
  @param pin Broche sur laquelle est connecté le relais
*/
Relay::Relay(const String& name,
             uint8_t       pin) :
  ObjectBase(name),
  m_pin(pin)
{
}


/*
  Destructeur
 */
Relay::~Relay()
{
}


/* 
  Initialisation du relais
*/
void Relay::Initialize()
{
  pinMode(m_pin, OUTPUT);  

  // Etat ouvert
  digitalWrite(m_pin, HIGH);

  LOG_MESSAGE(F("Initialisation du relais %s sur la broche %d OK\n"),
              m_name.c_str(),
              m_pin);
}


/*
  Lire l'état du relais
*/
RelayState Relay::GetState() const
{
  RelayState res = RELAY_OPENED;

  if ( digitalRead(m_pin) == LOW )
  {
    res = RELAY_CLOSED;
  }

  return res;
}


/* 
  Changer l'état du relais
  @param state Nouvel état
*/
void Relay::SetState(const RelayState& state)
{
  if ( state == RELAY_OPENED )
  {
    Open();
  }
  
  else if ( state == RELAY_CLOSED )
  {
    Close();
  }
}


/*
  Ouvrir le relais
*/
void Relay::Open()
{
  digitalWrite(m_pin, HIGH);
}


/*
  Fermer le relais
*/
void Relay::Close()
{
  digitalWrite(m_pin, LOW);
}
