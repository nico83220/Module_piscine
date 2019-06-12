#ifndef __RELAY_H__
#define __RELAY_H__

#include "ObjectBase.h"


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

public:
  // Constructeur
  Relay(const String& name,
        uint8_t       pin);

  // Destructeur
  virtual ~Relay();

  // Initialisation du relais
  void Initialize();

  // Lire l'état du relais
  RelayState GetState() const;

  // Changer l'état du relais
  void SetState(const RelayState& state);

  // Ouvrir le relais
  void Open();

  // Fermer le relais
  void Close();
};

#endif  // __RELAY_H__
