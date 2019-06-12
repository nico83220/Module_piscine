#include "ContactSensor.h"
#include "LogConsole.h"
#include "Utils.h"

#include <Arduino.h>


/*
  Constructeur
  @param name Nom du capteur
  @param pinOut Broche en sortie sur laquelle est connectée le capteur
  @param pinContact Broche en entrée sur laquelle est connecté le capteur 
 */
ContactSensor::ContactSensor(const String& name,
                             uint8_t       pinOut,
                             uint8_t       pinContact) :
  Sensor(name, pinOut, 1),
  m_pinContact(pinContact)
{
}


/*
  Destructeur
 */
ContactSensor::~ContactSensor()
{
}


/* 
  Initialisation du capteur
 */
void ContactSensor::Initialize()
{
  // On met la sortie à un état haut
  pinMode(m_pin, OUTPUT);
  digitalWrite(m_pin, HIGH);

  pinMode(m_pin, INPUT_PULLUP);  

  LOG_MESSAGE(F("Initialisation du capteur de contact %s sur les broches %d %d OK\n"),
              m_name.c_str(),
              m_pin,
              m_pinContact);

  m_sensorOK = true;
}



/*
  Lire la valeur du contact
*/
Contact ContactSensor::GetContact() const
{
  Contact res = CONTACT_CLOSED;

  // Un niveau bas correspond à un contact ouvert
  if ( m_values[0] == LOW )
  {
    res = CONTACT_OPENED;
  }

  return res;
}


/** 
  Mise à jour de la valeur du capteur
  @param logValue True pour afficher un log en même temps que la mise à jour
*/
void ContactSensor::Update(bool logValue)
{
  if ( m_sensorOK == true )
  {
    m_values[0] = digitalRead(m_pinContact);

    if ( logValue == true )
    {
      static String s_opened = Utils::GetStringFromDict(OPENED);
      static String s_closed = Utils::GetStringFromDict(CLOSED);
      LOG_MESSAGE(F("Capteur %s - contact : %s\n"),
                  m_name.c_str(),
                  ( m_values[0] == HIGH ) ? s_closed.c_str() : s_opened.c_str());
    }
  }
}
