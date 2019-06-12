#include "Sensor.h"
#include "LogConsole.h"
#include "Utils.h"

using namespace std;


/* static */
vector< Sensor* >  Sensor::m_sensors;


/* 
  Constructeur
  @param name Nom du capteur
  @param pin Broche sur laquelle est connecté le capteur
  @param valuesNumber Nombre de valeurs gérées par le capteur
*/
Sensor::Sensor(const String& name,
               uint8_t       pin,
               uint8_t       valuesNumber) :
  ObjectBase(name),
  m_values(),
  m_pin(pin),
  m_sensorOK(false)
{
  // On crée le nombre de valeurs nécessaires
  for ( uint8_t i=0;i<valuesNumber; ++i )
  {
    m_values.push_back(0.0);
  }

  Sensor::m_sensors.push_back(this);
}


/*
  Destructeur
 */
Sensor::~Sensor()
{
}


/* 
  Lecture de la valeur du capteur
  @param whichValue Index de la valeur à lire
*/
float Sensor::GetValue(uint8_t whichValue /* = 0 */) const
{
  float res = 0.0;
  
  if ( ( m_values.size() > 0 ) &&
       ( whichValue < m_values.size() ) )
  {
    res = m_values[whichValue];
    String strValue = Utils::Float2String(res, 1);
  }
  
  return res; 
}


/* 
  Initialiser tous les capteurs
*/
/* static */
void Sensor::InitializeAllSensors()
{
  vector< Sensor* >::iterator it;
  for ( it=m_sensors.begin(); it!=m_sensors.end(); ++it )
  {
    LOG_MESSAGE(F("%s ...\n"), (*it)->m_name.c_str());
    (*it)->Initialize();
  }
}


/*
  Mettre à jour tous les capteurs
  @param logValue True pour afficher un log en même temps que la mise à jour
*/
/* static */
void Sensor::UpdateAllSensors(bool logValue)
{
  if ( logValue == true )
  {
    LOG_MESSAGE(F("\nMise a jour des capteurs\n"));
  }

  vector< Sensor* >::iterator it;
  for ( it=m_sensors.begin(); it!=m_sensors.end(); ++it )
  {
    (*it)->Update(logValue);
  }
}


/* 
  Lire la valeur d'un capteur
  @param name Nom du capteur
  @param whichValue Index de la valeur à lire
  @return 
/* static */
float Sensor::GetValue(const String& name,
                       uint8_t       whichValue /* = 0 */)
{
  float res = 0.0;
  
  vector< Sensor* >::iterator it;
  for ( it=m_sensors.begin(); it!=m_sensors.end(); ++it )
  {
    res = (*it)->GetValue(whichValue);
  }

  return res;
}
