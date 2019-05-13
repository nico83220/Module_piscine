#include "PHSensor.h"
#include "LogConsole.h"
#include "Utils.h"


/*
  Constructeur
  @param name Nom du capteur
  @param pin Broche sur laquelle est connecté le capteur
  @param readsCountPerValue Nombre de lecture analogique à chaque mise à jour de la valeur
  @param waterTemperatureSensor Pointeur sur une sonde de température de l'eau si on veut appliquer une correction 
*/
PHSensor::PHSensor(const String& name,
                   uint8_t       pin,
                   uint8_t       readsCountPerValue     /* = 10 */,
                   Sensor*       waterTemperatureSensor /* = NULL */) :
  AnalogSensor(name, Utils::GetStringFromDict(PH), Utils::GetStringFromDict(PH_UNIT), pin, readsCountPerValue, waterTemperatureSensor, 0)
{
  SetMinAndMaxValues(0.0, 14.0);
  if ( ( waterTemperatureSensor == NULL ) ||
       ( waterTemperatureSensor->IsOK() == false ) )
  {
    m_a =  3.56;
    m_b = -1.889;
  }
  else
  {
    // Pas besoin d'initialiser les paramètres m_a et m_b car ils seront mis à jour à chaque mesure
    // par l'appel à UpdateTransfertParameters
  }
  m_calibrated = true;
}


/*
  Destructeur
 */
PHSensor::~PHSensor()
{
}


/*
  Mettre à jour la droite de transfert en fonction de la valeur lue
  @param value Valeur du capteur de compensation
*/
void PHSensor::UpdateTransferParameters(float value)
{
  m_a = 1 / (0.257179 + 0.000941468 * value);
  m_b = 7 - 2.5 / (0.257179 + 0.000941468 * value);
}


/* 
  Initialisation du capteur
 */
void PHSensor::Initialize()
{
  LOG_MESSAGE(F("Initialisation du capteur de pH %s sur la broche %d OK\n"),
              m_name.c_str(),
              m_pin);

  // Rien à faire pour l'initialisation
  m_sensorOK = true;
}
