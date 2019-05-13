#ifndef __RELAY_H__
#define __RELAY_H__

#include "ObjectBase.h"

#include <ArduinoSTL.h>
#include <vector>


/*
  @enum RelayState
  @brief Définition des états des relais
*/
enum RelayState
{
  RELAY_OPENED,
  RELAY_CLOSED
};


/*
  @class Relay
  @brief Classe de gestion de relais  
*/
class Relay : public ObjectBase
{
private:

protected:
  uint8_t     m_pin;                       // Pin sur laquelle est connecté le relais
  RelayState  m_state;                     // Etat du relais

  static std::vector< Relay* >  m_relays;  // Liste des relais du système

public:
  // Constructeur
  Relay(const String& name,
        uint8_t       pin);

  // Destructeur
  virtual ~Relay();

  // Initialisation du relais
  void Initialize();

  // Lire l'état du relais
  RelayState GetState() const
    { return m_state; }

  // Changer l'état du relais
  void SetState(const RelayState& state);

  // Initialiser tous les relais
  static void InitializeAllRelays();

  // Lire l'état d'un relais
  static RelayState GetState(const String& name);

  // Changer l'état d'un relais
  static void SetState(const String& name,
                       const RelayState& state);
};

#endif  // __RELAY_H__
