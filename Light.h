#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Actuator.h"


/*
  @enum LightState
  @brief Définition des états des lumières
*/
enum LightState
{
  LIGHT_OFF,
  LIGHT_ON,
};


/*
  @class Light
  @brief Classe de gestion de lumières
*/
class Light : public Actuator
{
private:

protected:

public:
  // Constructeur
  Light(const String& name,
        Relay*        commandRelay);

  // Destructeur
  virtual ~Light();

  // Allumer la lumière
  void SwitchOn(uint32_t requestedRunningTimeInMs)
    { Start(requestedRunningTimeInMs); }

  // Eteindre la lumière
  void SwitchOff()
    { Stop(); }

  // Lire l'état de la lumière
  LightState GetState() const;
};

#endif  // __LIGHT_H__
