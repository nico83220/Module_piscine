#include "OneWireDallasTemperatureSensor.h"
#include "LogConsole.h"
#include "Utils.h"


// 12bits (0,06°C) temperature sensor resolution
#define  TEMPERATURE_RESOLUTION  12


/* static */ 
OneWireDallasTemperatureSensor* OneWireDallasTemperatureSensor::m_currentSensor = NULL;


/*
  Constructeur
  @param name Nom du capteur
  @param pin Broche sur laquelle est connecté le capteur
 */
OneWireDallasTemperatureSensor::OneWireDallasTemperatureSensor(const String& name,
                                                               uint8_t       pin) :
  Sensor(name, pin, 1),
  m_oneWireSensor(pin),
  m_temperatureSensor(&m_oneWireSensor),
  m_temperatureSensorAddress({0, 0, 0, 0, 0, 0, 0, 0}),
  m_stateMachine()
{
}


/*
  Destructeur
 */
OneWireDallasTemperatureSensor::~OneWireDallasTemperatureSensor()
{
}


/*
  Etat "Demande de valeur"
*/
/* static */ 
void OneWireDallasTemperatureSensor::RequestValueState()
{
  if ( OneWireDallasTemperatureSensor::m_currentSensor != NULL )
  {
    // Demande de température à la sonde
    OneWireDallasTemperatureSensor::m_currentSensor->m_temperatureSensor.requestTemperaturesByAddress(
      OneWireDallasTemperatureSensor::m_currentSensor->m_temperatureSensorAddress);

    // Passage à l'état suivant d'attente de la fin de lecture
    OneWireDallasTemperatureSensor::m_currentSensor->m_stateMachine.next(OneWireDallasTemperatureSensor::WaitValueState);
  }
}


/*
  Etat "Attente de valeur"
*/
/* static */ 
void OneWireDallasTemperatureSensor::WaitValueState()
{
  if ( OneWireDallasTemperatureSensor::m_currentSensor != NULL )
  {
    // Si on a attendu assez pour la conversion
    if ( OneWireDallasTemperatureSensor::m_currentSensor->m_stateMachine.elapsed(1000 / (1 << (12 - TEMPERATURE_RESOLUTION))) )
    {
      // Passage à l'état suivant dans lequel la valeur est disponible
      OneWireDallasTemperatureSensor::m_currentSensor->m_stateMachine.next(OneWireDallasTemperatureSensor::ValueAvailableState);
    }
  }
}


/*
  Etat "Valeur disponible"
*/
/* static */ 
void OneWireDallasTemperatureSensor::ValueAvailableState()
{
  if ( OneWireDallasTemperatureSensor::m_currentSensor != NULL )
  {
    // La valeur est disponible, on la mémorise
    float value = OneWireDallasTemperatureSensor::m_currentSensor->m_temperatureSensor.getTempC(
      OneWireDallasTemperatureSensor::m_currentSensor->m_temperatureSensorAddress);
    OneWireDallasTemperatureSensor::m_currentSensor->m_values[0] = value;

    // Retour à l'état initial pour la prochaine demande
    OneWireDallasTemperatureSensor::m_currentSensor->m_stateMachine.next(OneWireDallasTemperatureSensor::RequestValueState);
  }
} 


/* 
  Initialisation du capteur
 */
void OneWireDallasTemperatureSensor::Initialize()
{
  // Initialisation de la librairie DallasTemperature
  m_temperatureSensor.begin();

  // Lecture de l'adresse
  if ( m_temperatureSensor.getDS18Count() > 0 )
  {
    m_temperatureSensor.getAddress(m_temperatureSensorAddress, 0);
      
    // Changer la résolution de la lecture de température
    m_temperatureSensor.setResolution(m_temperatureSensorAddress, TEMPERATURE_RESOLUTION);

    // On se met en mode asynchrone
    m_temperatureSensor.setWaitForConversion(false); 

    // Initialisation de la machine à état sur l'état de démarrage de lecture
    m_stateMachine.next(OneWireDallasTemperatureSensor::RequestValueState);

    LOG_MESSAGE(F("Initialisation du capteur de temperature %s sur la broche %d OK\n"),
                m_name.c_str(),
                m_pin);

    m_sensorOK = true;
  }
  
  else
  {
    LOG_MESSAGE(F("Aucun capteur de temperature 1-wire trouve sur la broche %d !\n"),
                m_pin);
  }
}


/** 
  Mise à jour de la valeur du capteur
  @param logValue True pour afficher un log en même temps que la mise à jour
*/
void OneWireDallasTemperatureSensor::Update(bool logValue)
{
  if ( m_sensorOK == true )
  {
    // A chaque appel, on fait travailler la machine d'état car c'est elle qui fait tout le travail
    // mais juste avant, il faut mémoriser le senseur courant
    OneWireDallasTemperatureSensor::m_currentSensor = this;
    m_stateMachine.run();

    if ( logValue == true )
    {
      LOG_MESSAGE(F("Capteur %s - temperature : %s C\n"),
                  m_name.c_str(),
                  Utils::Float2String(m_values[0], 1).c_str());
    }
  }
}
