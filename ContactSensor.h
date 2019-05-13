#ifndef __CONTACT_SENSOR_H__
#define __CONTACT_SENSOR_H__

#include "Sensor.h"


/*
  @enum Contact
  @brief Définition des valeurs de contact du capteur
*/
enum Contact
{
  CONTACT_OPENED,
  CONTACT_CLOSED
};


/*
  @class ContactSensor
  @brief Classe de gestion de sondes de contact
*/
class ContactSensor : public Sensor
{
private:

protected:
  uint8_t  m_pinContact;                    // Broche sur laquelle est connecté le capteur de contact

public:
  // Constructeur
  ContactSensor(const String& name,
                uint8_t       pinOut,
                uint8_t       pinContact);

  // Destructeur
  virtual ~ContactSensor();

  // Initialisation du capteur
  virtual void Initialize();

  // Lire la valeur du contact
  Contact GetContact() const;

  // Mise à jour de la valeur du capteur
  virtual void Update(bool logValue);
};

#endif  // __CONTACT_SENSOR_H__
