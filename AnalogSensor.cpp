#include "AnalogSensor.h"
#include "LogConsole.h"
#include "Utils.h"


/* 
  Constructeur
  @param name Nom du capteur
  @param valueName Nom de la valeur (i.e. pH)
  àparam valueUnit Unité de la valeur (i.e. mV)
  @param pin Broche sur laquelle est connecté le capteur
  @param pinreadsCountPerValue Nombre de lecture pour chaque mise à jour de valeur
  @param compensationSensor Capteur éventuel pouvant jouer sur les valeurs
  @param compensationSensorWhichValue Quelle valeur du capteur éventuel utiliser pour modifier la droite de transfert ?
*/
AnalogSensor::AnalogSensor(const String& name,
                           const String& valueName,
                           const String& valueUnit,
                           uint8_t       pin,
                           uint8_t       readsCountPerValue,
                           Sensor*       compensationSensor           /* = NULL */,
                           uint8_t       compensationSensorWhichValue /* = 0 */) :
  Sensor(name, pin, 1),
  m_valueName(valueName),
  m_valueUnit(valueUnit),
  m_readsCountPerValue(readsCountPerValue),
  m_calibrated(false),
  m_a(1.0),
  m_b(0.0),
  m_minVoltage(0.0),
  m_maxVoltage(5.0),
  m_minValue(0.0),
  m_maxValue(5.0),
  m_compensationSensor(compensationSensor),
  m_compensationSensorWhichValue(compensationSensorWhichValue),
  m_runningMedian(m_readsCountPerValue)
{
}


/*
  Destructeur
 */
AnalogSensor::~AnalogSensor()
{
}


/* 
  Lire la valeur moyenne du votage sur le port analogique du senseur
  @return float Valeur moyenne du votage sur le port analogique du senseur
*/
float AnalogSensor::ReadAverageVoltageValue()
{
  float res = 0.0;

  // On lit la valeur brute sur le port analogique (valeur entre 0 et 1023)
  int rawValue = analogRead(m_pin);

  // Calcul du voltage
  float voltageValue = (5.0 * rawValue) / 1023.0;

  // Ajout de la valeur dans l'objet RunningMedian
  m_runningMedian.add(voltageValue);

  // Demande de la valeur moyenne
  res = m_runningMedian.getAverage(m_readsCountPerValue);

  return res;
}


/*
  Convertir le voltage en valeur
  @param voltage Voltage présent en entrée du capteur
  @return float Valeur correspondant au voltage en entrée
*/
float AnalogSensor::ConvertVoltageToValue(float voltage) 
{
  float res = voltage;

  // Si le senseur est calibré
  if ( m_calibrated == true )
  {
    if ( voltage < m_minVoltage )
    {
      res = m_minValue;

      LOG_MESSAGE(F("ERREUR : mesure sur la sonde %s en dessous du voltage minimal (%.2f < %.2f)\n"),
                  m_name.c_str(),
                  voltage,
                  m_minVoltage);
    }
    
    else if ( res > m_maxVoltage )
    {
      res = m_maxValue;

      LOG_MESSAGE(F("ERREUR : mesure sur la sonde %s au dessus du voltage maximal (%.2f > %.2f)\n"),
                  m_name.c_str(),
                  voltage,
                  m_maxVoltage);
    }

    else
    {
      // Si un capteur de compensation est défini
      if ( m_compensationSensor != NULL )
      {
        // On met à jour les paramètres m_a et m_b avec sa valeur
        UpdateTransferParameters(m_compensationSensor->GetValue(m_compensationSensorWhichValue));
      }

      // On calcule la valeur
      res = m_a * voltage + m_b;

      // Limitation aux valeurs min et max
      if ( res < m_minValue )
      {
        res = m_minValue;
      }
      else if ( res > m_maxValue )
      {
        res = m_maxValue;
      }
    }
  }

  else
  {
    LOG_MESSAGE(F("ERREUR : sonde %s non calibrée pour donner la valeur correspondant au voltage %.2f\n"),
                m_name.c_str(),
                voltage);
  }

  return res;
}


/* 
  Calibrer la sonde avec 2 mesures
  @param voltage1 Valeur en volt du 1er point
  @param value1 Valeur correspondant au voltage donné du 1er point
  @param voltage2 Valeur en volt du 2nd point
  @param value2 Valeur correspondant au voltage donné du 2nd point
*/
void AnalogSensor::CalibrateWith2Measures(float voltage1,
                                          float value1,
                                          float voltage2,
                                          float value2)
{
  if ( ( voltage1 >= m_minVoltage ) &&
       ( voltage1 <= m_maxVoltage ) &&
       ( voltage2 >= m_minVoltage ) &&
       ( voltage2 <= m_maxVoltage ) &&
       ( voltage1 != voltage2 ) )
  {
    // Calcul des paramètres de la droite de transfert
    m_a = (value2 - value1) / (voltage2 - voltage1);
    m_b = value1 - m_a * voltage1;
    
    m_calibrated = true;
  }

  else
  {
    LOG_MESSAGE(F("ERREUR : La calibration du capteur %s a échoué avec les paramètres (%f, %f, %f, %f)\n"),
                m_name.c_str(),
                voltage1,
                value1,
                voltage2,
                value2);

    m_calibrated = false;
  }
}


/* 
  Calibrer la sonde avec 1 mesure
  @param voltage Valeur en volt
  @param value Valeur correspondant au voltage donné
*/
void AnalogSensor::CalibrateWithMeasure(float voltage,
                                        float value)
{
  // On ne peut le faire que si le capteur est déja calibré car cet méthode ne change que le paramètre m_b
  if ( m_calibrated == true )
  {
    m_b = value - m_a * voltage;
  }

  else
  {
    LOG_MESSAGE(F("ERREUR : La calibration du capteur %s a échoué avec les paramètres (%f, %f) il n'est pas encore calibré\n"),
                m_name.c_str(),
                voltage,
                value);
  }
}


/*
  Mise à jour de la valeur du capteur
  @param logValue True pour afficher un log en même temps que la mise à jour
 */
void AnalogSensor::Update(bool logValue)
{
  if ( m_sensorOK == true )
  {
    // Lecture de la valeur moyenne
    float voltage = ReadAverageVoltageValue();
  
    // Calcul de la valeur correspondante
    float value = ConvertVoltageToValue(voltage);

    m_values[0] = value;

    if ( logValue == true )
    {
      if ( m_valueUnit == "" )
      {
        LOG_MESSAGE(F("Capteur %s - %s : %s (voltage : %s)\n"),
                    m_name.c_str(),
                    m_valueName.c_str(),
                    Utils::Float2String(m_values[0], 2).c_str(),
                    Utils::Float2String(voltage, 2).c_str());
      }

      else
      {
        LOG_MESSAGE(F("Capteur %s - %s : %s %s (voltage : %s)\n"),
                    m_name.c_str(),
                    m_valueName.c_str(),
                    m_valueUnit.c_str(),
                    Utils::Float2String(m_values[0], 2).c_str(),
                    Utils::Float2String(voltage, 2).c_str());
      }
    }
  }
}
