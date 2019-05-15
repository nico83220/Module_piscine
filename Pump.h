#ifndef __PUMP_H__
#define __PUMP_H__

#include "ObjectBase.h"

class Relay;


/*
  @class Pump
  @brief Classe de gestion de pompes  
*/
class Pump : public ObjectBase
{
private:

protected:
  Relay*   m_commandRelay;                // Relais de commande la pompe
  bool     m_running;                     // Flag indiquant si la pompe est en route

public:
  // Constructeur
  Pump(const String&  name,
       Relay*         commandRelay);

  // Destructeur
  virtual ~Pump();

  // Initialisation de la pompe
  void Initialize();

  // Démarrer la pompe
  void Start();

  // Arrêter la pompe
  void Stop();

  // Lire si la pompe est en route
  bool IsRunning() const
    { return m_running; }
};

#endif  // __PUMP_H__
