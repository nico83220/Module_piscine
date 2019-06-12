#ifndef __SOLENOID_VALVE_H__
#define __SOLENOID_VALVE_H__

#include "Actuator.h"


/*
  @enum ValveState
  @brief Définition des états des vannes
*/
enum ValveState
{
  VALVE_CLOSED,
  VALVE_OPENED,
};


/*
  @class SolenoidValve
  @brief Classe de gestion d'électrovanne
*/
class SolenoidValve : public Actuator
{
private:

protected:

public:
  // Constructeur
  SolenoidValve(const String& name,
                Relay*        commandRelay);

  // Destructeur
  virtual ~SolenoidValve();

  // Ouvrir l'électrovanne
  void Open(uint32_t requestedRunningTimeInMs)
    { Start(requestedRunningTimeInMs); }

  // Fermer l'électrovanne
  void Close()
    { Stop(); }

  // Lire l'état de l'électrovanne
  ValveState GetState() const;
};

#endif  // __SOLENOID_VALVE_H__
