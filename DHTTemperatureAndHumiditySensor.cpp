#include "DHTTemperatureAndHumiditySensor.h"
#include "LogConsole.h"
#include "Utils.h"


/*
  Constructeur
  @param name Nom du capteur
  @param pin Broche sur laquelle est connecté le capteur
  @param type Type de capteur (DHT11, DHT22, DHT21, AM2301)
 */
DHTTemperatureAndHumiditySensor::DHTTemperatureAndHumiditySensor(const String& name,
                                                                 uint8_t       pin,
                                                                 uint8_t       type) :
  Sensor(name, pin, 2),
  m_temperatureAndHumiditySensor(pin, type)
{
}


/*
  Destructeur
 */
DHTTemperatureAndHumiditySensor::~DHTTemperatureAndHumiditySensor()
{
}


/* 
  Initialisation du capteur
 */
void DHTTemperatureAndHumiditySensor::Initialize()
{
  // Initialisation de la librairie DHT
  m_temperatureAndHumiditySensor.begin();

  LOG_MESSAGE(F("Initialisation du capteur de temperature et humidite %s sur la broche %d OK\n"),
              m_name.c_str(),
              m_pin);

  m_sensorOK = true;
}


/** 
  Mise à jour de la valeur du capteur
  @param logValue True pour afficher un log en même temps que la mise à jour
*/
void DHTTemperatureAndHumiditySensor::Update(bool logValue)
{
  if ( m_sensorOK == true )
  {
    float roomTemperature = m_temperatureAndHumiditySensor.readTemperature();
    float roomHumidity    = m_temperatureAndHumiditySensor.readHumidity();
    
    m_values[0] = roomTemperature;
    m_values[1] = roomHumidity;

    if ( logValue == true )
    {
      LOG_MESSAGE(F("Capteur %s - temperature : %s C, humidite : %s %%\n"),
                  m_name.c_str(),
                  Utils::Float2String(roomTemperature, 1).c_str(),
                  Utils::Float2String(roomHumidity, 1).c_str());
    }
  }
}
