#include "Relay.h"
#include "LogConsole.h"

#include <Arduino.h>


/* static */
std::vector< Relay* >  Relay::m_relays;


/* 
  Constructeur
  @param name Nom du capteur
  @param pin Broche sur laquelle est connecté le relais
*/
Relay::Relay(const String& name,
             uint8_t       pin) :
  ObjectBase(name),
  m_pin(pin),
  m_state(RELAY_OPENED)
{
  Relay::m_relays.push_back(this);
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
  digitalWrite(m_pin, HIGH);

  LOG_MESSAGE(F("Initialisation du relais %s sur la broche %d OK\n"),
              m_name.c_str(),
              m_pin);
}


/* 
  Changer l'état du relais
  @param state Nouvel état
*/
void Relay::SetState(const RelayState& state)
{
  m_state = state;

  if ( m_state == RELAY_OPENED )
  {
    digitalWrite(m_pin, HIGH);  
  }
  
  else if ( m_state == RELAY_CLOSED )
  {
    digitalWrite(m_pin, LOW);  
  }
}


/* 
  Initialiser tous les relais
*/
/* static */
void Relay::InitializeAllRelays()
{
  std::vector< Relay* >::iterator it;
  for ( it=m_relays.begin(); it!=m_relays.end(); ++it )
  {
    LOG_MESSAGE(F("%s ...\n"), (*it)->m_name.c_str());
    (*it)->Initialize();
  }
}


/*
  Lire l'état d'un relais
  @param name Nom du relais
  @return RelayState Etat trouvé
*/
/* static */
RelayState Relay::GetState(const String& name)
{
  RelayState res = RELAY_OPENED;
  
  std::vector< Relay* >::iterator it;
  for ( it=m_relays.begin(); it!=m_relays.end(); ++it )
  {
    if ( (*it)->m_name == name )
    {
      res = (*it)->GetState();
      break;
    }
  }

  return res;
}


/* 
  Changer l'état d'un relais
  @param name Nom du relais
  @param state Nouvel état
*/
/* static */
void Relay::SetState(const String&     name,
                     const RelayState& state)
{
  std::vector< Relay* >::iterator it;
  for ( it=m_relays.begin(); it!=m_relays.end(); ++it )
  {
    if ( (*it)->m_name == name )
    {
      (*it)->SetState(state);
      break;
    }
  }
}
