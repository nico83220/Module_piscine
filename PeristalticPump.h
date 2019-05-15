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
  ContactSensor*  m_tankLevelSensor;             // Capteur de niveau du réservoir de produit

public:
  // Constructeur
  PeristalticPump(const String&  name,
                  Relay*         commandRelay,
                  ContactSensor* flowSensor,
                  ContactSensor* tankLevelSensor);

  // Destructeur
  virtual ~PeristalticPump();

  // Initialisation de la pompe
  void Initialize();

};

#endif  // __PERISTALTIC_PUMP_H__
