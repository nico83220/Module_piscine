#ifndef __PERISTALTIC_PUMP_H__
#define __PERISTALTIC_PUMP_H__

#include "ObjectBase.h"

#include <ArduinoSTL.h>
#include <map>

class Relay;
class ContactSensor;


/*
  @class PeristalticPump
  @brief Classe de gestion de pompes péristaltiques  
*/
class PeristalticPump : public ObjectBase
{
private:

protected:
  Relay*          m_commandRelay;                // Relais de commande la pompe
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

  // Initialisation du relais
  void Initialize();

};

#endif  // __PERISTALTIC_PUMP_H__
