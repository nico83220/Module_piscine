#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "OneWireDallasTemperatureSensor.h"
#include "DHTTemperatureAndHumiditySensor.h"
#include "PHSensor.h"
#include "ORPSensor.h"
#include "PressureSensor.h"
#include "CurrentSensor.h"
#include "ContactSensor.h"
#include "LiquidLevelSensor.h"
#include "PoolLevelSensor.h"
#include "Relay.h"
#include "PeristalticPump.h"
#include "SolenoidValve.h"
#include "Light.h"


// Adresse mac
extern String myMACStr;

// Capteurs
extern OneWireDallasTemperatureSensor waterTemperatureSensor;
extern DHTTemperatureAndHumiditySensor roomTemperatureAndHumiditySensor;
extern PHSensor phSensor;
extern ORPSensor orpSensor;
extern PressureSensor filterPressureSensor;
extern CurrentSensor pump1CurrentSensor;
extern CurrentSensor pump2CurrentSensor;
extern PoolLevelSensor waterLevelSensor;
extern ContactSensor flowSensor;
extern LiquidLevelSensor chlorineLevelSensor;
extern LiquidLevelSensor phMinusLevelSensor;

// Relais
extern Relay poolPump1Relay;
extern Relay poolPump2Relay;
extern Relay phMinusPumpRelay;
extern Relay chlorinePumpRelay;
extern Relay poolLightRelay;
extern Relay poolFillingRelay;

// Pompes
extern Pump poolPump1;
extern Pump poolPump2;
extern PeristalticPump phMinusPump;
extern PeristalticPump chlorinePump;

// Electrovannes
extern SolenoidValve poolFillingValve;

// Lumières
extern Light poolLight;

// Autres


#endif  // __GLOBALS_H__
