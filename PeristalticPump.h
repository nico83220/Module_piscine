#ifndef __PERISTALTIC_PUMP_H__
#define __PERISTALTIC_PUMP_H__

#include "Pump.h"

class ContactSensor;


/*
  @class PeristalticPump
  @brief Classe de gestion de pompes péristaltiques  
*/
class PeristalticPump : public Pump
{
private:

protected:
  ContactSensor*  m_flowSensor;                  // Capteur de débit de circulation

public:
  // Constructeur
  PeristalticPump(const String&      name,
                  Relay*             commandRelay,
                  LiquidLevelSensor* liquidLevelSensor,
                  ContactSensor*     flowSensor);

  // Destructeur
  virtual ~PeristalticPump();

  // Démarrer la pompe
  virtual void Start(uint32_t requestedRunningTimeInMs);

  // Boucle de traitement à appeler dans le programme principal
  virtual void Loop(uint32_t nowMillisec);
};

#endif  // __PERISTALTIC_PUMP_H__
