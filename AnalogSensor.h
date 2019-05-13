#ifndef __ANALOG_SENSOR_H__
#define __ANALOG_SENSOR_H__

#include "Sensor.h"

#include <RunningMedian.h>


/*
  @class AnalogSensor
  @brief Classe abstraite de gestion de sondes analogiques avec lecture moyenne du voltage et calibration

  Les senseurs analogiques ne gère qu'une valeur
  La calibration consiste en 2 paires de valeurs (voltage, valeur) qui définissent une équation de droite pour la conversion de voltage en valeur
*/
class AnalogSensor : public Sensor
{
private:

protected:
  String         m_valueName;                     // Nom de la valeur
  String         m_valueUnit;                     // Unité de la valeur
  uint8_t        m_readsCountPerValue;            // Nombre le lecture pour chaque mise à jour de la valeur
  bool           m_calibrated;                    // Flag indiquant si le senseur est calibré
  float          m_a;                             // Valeur calculée pour la droite de transfert d'équation y = a . x + b
  float          m_b;                             // Valeur calculée pour la droite de transfert d'équation y = a . x + b
  float          m_minVoltage;                    // Limite de voltage inférieure
  float          m_maxVoltage;                    // Limite de voltage supérieure
  float          m_minValue;                      // Limite de valeur inférieure
  float          m_maxValue;                      // Limite de valeur supérieure
  Sensor*        m_compensationSensor;            // Capteur éventuel pouvant jouer sur les valeurs
  uint8_t        m_compensationSensorWhichValue;  // Quelle valeur du capteur éventuel utiliser pour modifier la droite de transfert ?
  RunningMedian  m_runningMedian;                 // Objet de gestion d'une valeur médiane

  // Lire la valeur moyenne du votage sur le port analogique du senseur
  float ReadAverageVoltageValue();
 
  // Convertir le voltage en valeur
  float ConvertVoltageToValue(float voltage);

  // Mettre à jour la droite de transfert en fonction de la valeur lue
  virtual void UpdateTransferParameters(float value) {}
  
public:
  // Constructeur
  AnalogSensor(const String& name,
               const String& valueName,
               const String& valueUnit,
               uint8_t       pin,
               uint8_t       readsCountPerValue,
               Sensor*       compensationSensor           = NULL,
               uint8_t       compensationSensorWhichValue = 0);

  // Destructeur
  virtual ~AnalogSensor();

  // Calibrer la sonde avec 2 mesures
  void CalibrateWith2Measures(float voltage1,
                              float value1,
                              float voltage2,
                              float value2);

  // Calibrer la sonde avec 1 mesure
  void CalibrateWithMeasure(float voltage,
                            float value);

  // Changer les bornes de validité du voltage pour le capteur
  void SetMinAndMaxVoltages(float minVoltage,
                            float maxVoltage)
    { m_minVoltage = minVoltage; m_maxVoltage = maxVoltage; }
                                 

  // Changer les bornes de validité du voltage pour le capteur
  void SetMinAndMaxValues(float minValue,
                          float maxValue)
    { m_minValue = minValue; m_maxValue = maxValue; }

  // Initialisation du capteur
  virtual void Initialize() = 0;

  // Mise à jour de la valeur du capteur
  virtual void Update(bool logValue);
};

#endif  // __ANALOG_SENSOR_H__
