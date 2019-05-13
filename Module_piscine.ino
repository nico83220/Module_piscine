//
// Module de gestion de piscine
// Nicolas Coquelle
// coquelle.nicolas@gmail.com
//
// Modifié le 13/06/2017 Version 1.1 : suppression synchro NTP affichage T? T= pour connexion MQTT
// Modifié le 21/05/2018 Version 1.2 : refonte
// Modifié le 22/04/2019 Version 1.3 : changement écran, ajout machine état pour capteur 1-wire, ajout SoftTimer, ajout des pompes ...
//

//
// Bibliothèques nécessaires pour la compilation :
//
// ArduinoSTL
// OneWire
// DallasTemperature
// DHT
// Adafruit_Unified_Sensor
// PubSubClient
// NetEEPROM
// SoftTimer
// PciManager
// LiquidCrystal I2C
// RTCLib
// EmonLib
// NTPClientLib
//


#include <avr/wdt.h>

#include "LogConsole.h"
#include "TimeManager.h"
#include "OneWireDallasTemperatureSensor.h"
#include "DHTTemperatureAndHumiditySensor.h"
#include "PHSensor.h"
#include "ORPSensor.h"
#include "PressureSensor.h"
#include "MQTTClient.h"
#include "Relay.h"
#include "CurrentSensor.h"
#include "ContactSensor.h"
#include "WaterLevelSensor.h"
#include "Utils.h"
#include "LCDDisplay.h"
#include "PeristalticPump.h"
#include "Config.h"

#include <SoftTimer.h>


// -------------------------------------------------------------------------------------
// CREATION DES CAPTEURS
// -------------------------------------------------------------------------------------

// Capteur de température de l'eau sur la broche 46
OneWireDallasTemperatureSensor waterTemperatureSensor(Utils::GetStringFromDict(WATER_TEMPERATURE_SENSOR), 46);

// Capteur de température et d'humidité du local sur la broche A0
DHTTemperatureAndHumiditySensor roomTemperatureAndHumiditySensor(Utils::GetStringFromDict(ROOM_TEMPERATURE_AND_HUMIDITY_SENSOR), A0, DHT21);

// Capteur de pH sur la broche A8 avec 10 mesures de moyenne glissante
PHSensor phSensor(Utils::GetStringFromDict(PH_SENSOR), A8, 10);
//PHSensor phSensor(Utils::GetStringFromDict(PH_SENSOR), A8, 10, &waterTemperatureSensor);

// Capteur d'ORP (REDOX) sur la broche A10 avec 10 mesures de moyenne glissante
ORPSensor orpSensor(Utils::GetStringFromDict(ORP_SENSOR), A10, 10);

// Capteur de pression du filtre à sable sur la broche A12 avec 10 mesures de moyenne glissante
PressureSensor filterPressureSensor(Utils::GetStringFromDict(FILTER_PRESSURE_SENSOR), A12, 10);

// Capteur de consommation pompe 1
CurrentSensor pump1CurrentSensor(Utils::GetStringFromDict(POOL_PUMP_1_CURRENT_SENSOR), A1);

// Capteur de consommation pompe 2
CurrentSensor pump2CurrentSensor(Utils::GetStringFromDict(POOL_PUMP_2_CURRENT_SENSOR), A2);

// Capteur de niveau d'eau
WaterLevelSensor waterLevelSensor(Utils::GetStringFromDict(WATER_LEVEL_SENSOR), 47, 48, 49, 50, 51);

// Capteur de débit
ContactSensor flowSensor(Utils::GetStringFromDict(FLOW_SENSOR), 4, 5);

// Capteur de niveau de chlore
ContactSensor chlorineLevelSensor(Utils::GetStringFromDict(CHLORINE_LEVEL_SENSOR), 6, 7);

// Capteur de niveau de pH-
ContactSensor phMinusLevelSensor(Utils::GetStringFromDict(PH_MINUS_LEVEL_SENSOR), 8, 9);


// -------------------------------------------------------------------------------------
// CREATION DES RELAIS
// -------------------------------------------------------------------------------------

// Relais pompe piscine 1 sur la broche 23
Relay poolPump1Relay(Utils::GetStringFromDict(POOL_PUMP_1_RELAY), 25);

// Relais pompe piscine 2 sur la broche 25
Relay poolPump2Relay(Utils::GetStringFromDict(POOL_PUMP_2_RELAY), 27);

// Relais pompe pH- sur la broche 23
Relay phMinusPumpRelay(Utils::GetStringFromDict(PH_MINUS_PUMP_RELAY), 29);

// Relais pompe chlore sur la broche 25
Relay chlorinePumpRelay(Utils::GetStringFromDict(CHLORINE_PUMP_RELAY), 31);

// Relais lumière piscine sur la broche 33
Relay poolLightRelay(Utils::GetStringFromDict(POOL_LIGHT_RELAY), 33);

// Relais remplissage sur la broche 35
Relay poolFillingRelay(Utils::GetStringFromDict(POOL_FILLING_RELAY), 35);


// -------------------------------------------------------------------------------------
// CREATION DES POMPES
// -------------------------------------------------------------------------------------

// Pompe de régulation de pH
PeristalticPump phMinusPump(Utils::GetStringFromDict(PH_MINUS_PUMP), &phMinusPumpRelay, &flowSensor, &phMinusLevelSensor);

// Pompe de régulation de chlore
PeristalticPump chlorinePump(Utils::GetStringFromDict(CHLORINE_PUMP), &chlorinePumpRelay, &flowSensor, &chlorineLevelSensor);


// -------------------------------------------------------------------------------------
// GESTION ECRAN
// -------------------------------------------------------------------------------------

#define  TEMPERATURE_PH_AND_ORP_VALUES  0


// Fonction d'affichage 1 :
// pH ORP, température eau, air, huidité air, pression filtre, temps de filtration
void DisplayFunction1(bool displayMask)
{
  // S'il faut afficher le masque
  if ( displayMask == true )
  {
    LCDDisplay.Print(0, 0, "pH:            (   )");
    LCDDisplay.Print(0, 1, "Rx:   mV       (   )");
    LCDDisplay.Print(0, 2, "E:    C  A:    C   %");
    LCDDisplay.Print(0, 3, "P:    b             ");
  }

  // Affichage des valeur

  float  value    = phSensor.GetPH();
  String strValue = Utils::Float2String(value, 1);
  LCDDisplay.Print(3, 0, strValue.c_str());

  value    = orpSensor.GetORP();
  strValue = Utils::Float2String(value, 0);
  LCDDisplay.Print(3, 1, strValue.c_str());

  value    = waterTemperatureSensor.GetTemperature();
  strValue = Utils::Float2String(value, 1);
  LCDDisplay.Print(2, 2, strValue.c_str());

  value    = roomTemperatureAndHumiditySensor.GetTemperature();
  strValue = Utils::Float2String(value, 0);
  LCDDisplay.Print(11, 2, strValue.c_str());

  value    = roomTemperatureAndHumiditySensor.GetHumidity();
  strValue = Utils::Float2String(value, 0);
  LCDDisplay.Print(17, 2, strValue.c_str());

  value    = filterPressureSensor.GetPressure();
  strValue = Utils::Float2String(value, 1);
  LCDDisplay.Print(3, 3, strValue.c_str());
}


// -------------------------------------------------------------------------------------
// GESTION MQTT
// -------------------------------------------------------------------------------------

/*
  Fonction de traitement des requètes MQTT
*/
void MQTTCallback(char*        topic,
                  uint8_t*     payload,
                  unsigned int length)
{
  String topicString(topic);

  // On remplit le buffer avec les caractères reçus plus le caractère null de fin de chaine
  char messageBuff[length + 1];
  strncpy(messageBuff, (char*) payload, length);
  messageBuff[length] = '\0';
  String payloadString(messageBuff);

  LOG_MESSAGE(F("Requete MQTT recue : topic '%s' -> payload '%s'\n"),
              topicString.c_str(),
              payloadString.c_str());

  // Traitement des commandes

  static String s_slashSlash = Utils::GetStringFromDict(SLASH_SLASH);
  static String s_ON         = Utils::GetStringFromDict(ON_PAYLOAD);
  static String s_OFF        = Utils::GetStringFromDict(OFF_PAYLOAD);
  static String s_pump1Topic = Utils::GetStringFromDict(POOL_PUMP_1_RELAY);
  static String s_pump2Topic = Utils::GetStringFromDict(POOL_PUMP_2_RELAY);
  static String s_poolLight  = Utils::GetStringFromDict(POOL_LIGHT_RELAY);

  // On cherche si le topic commence par le nom du client
  static String s_clientName = Utils::GetStringFromDict(MQTT_CLIENT);
  if ( topicString.indexOf(s_clientName) != -1 )
  {
    // On enlève le début et les '/' du début
    topicString.replace(s_clientName, "");
    topicString.replace(s_slashSlash, "");

    // Traitement du topic 'POMPE_1'
    if ( topicString.indexOf(s_pump1Topic) != -1 )
    {
      // On enlève le début -> topicString contient le nom du relais
      topicString.replace(s_pump1Topic, "");
      topicString.replace(s_slashSlash, "");

      // payloadString doit contenir "ON" ou "OFF"
      if ( payloadString.equalsIgnoreCase(s_ON) == true )
      {
        Relay::SetState(s_pump1Topic, RELAY_CLOSED);
      }
      else if ( payloadString.equalsIgnoreCase(s_OFF) == true )
      {
        Relay::SetState(s_pump1Topic, RELAY_OPENED);
      }
    }

    // Traitement du topic 'POMPE_2'
    else if ( topicString.indexOf(s_pump2Topic) != -1 )
    {
      // On enlève le début -> topicString contient le nom du relais
      topicString.replace(s_pump2Topic, "");
      topicString.replace(s_slashSlash, "");

      // payloadString doit contenir "ON" ou "OFF"
      if ( payloadString.equalsIgnoreCase(s_ON) == true )
      {
        Relay::SetState(s_pump2Topic, RELAY_CLOSED);
      }
      else if ( payloadString.equalsIgnoreCase(s_OFF) == true )
      {
        Relay::SetState(s_pump2Topic, RELAY_OPENED);
      }
    }

    // Traitement du topic 'LUMIERE_PISCINE'
    else if ( topicString.indexOf(s_poolLight) != -1 )
    {
      // On enlève le début -> topicString contient le nom du relais
      topicString.replace(s_poolLight, "");
      topicString.replace(s_slashSlash, "");

      // payloadString doit contenir "ON" ou "OFF"
      if ( payloadString.equalsIgnoreCase(s_ON) == true )
      {
        Relay::SetState(s_poolLight, RELAY_CLOSED);
      }
      else if ( payloadString.equalsIgnoreCase(s_OFF) == true )
      {
        Relay::SetState(s_poolLight, RELAY_OPENED);
      }
    }
  }
}


// -------------------------------------------------------------------------------------
// TACHES CYCLIQUES (avec SoftTimer)
// -------------------------------------------------------------------------------------

//void EthernetClientLoop(Task* me);
void UpdateSensorsLoop(Task* me);
//void ORPRegulationLoop(Task* me);
//void PHRegulationLoop(Task* me);
void MQTTPublishDataLoop(Task* me);
void MainLoop(Task* me);


// -------------------------------------------------------------------------------------
// FONCTION D'INITIALISATION
// -------------------------------------------------------------------------------------

void setup()
{
  // Initialisation du watchdog
  MCUSR  &= ~_BV(WDRF);             // reset bit à 0
  WDTCSR |=  _BV(WDCE) | _BV(WDE);  // disable WDT
  WDTCSR = 0;

  // Initialisation de la console série
  // ----------------------------------

  Serial.begin(57600);


  // Initialisation de la couche réseau
  // ----------------------------------

  uint8_t   myMAC[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x16 };
  IPAddress myIP(192,168,10,45);
  IPAddress gw(192,168,10,1);
  IPAddress mask(255,255,255,0);
  IPAddress bcast(192,168,10,255);

  // Initialisation de la couche ethernet
  Ethernet.begin(myMAC, myIP, gw, mask);


  // Initialisation de la console de log
  // -----------------------------------

  // Envoi sur l'adresse broadcast sur le port 2300, écoute sur le port 2301
  LogConsole.Initialize(bcast, 2300, 2301);

  // Traces d'initialisation de la couche réseau
  LOG_NEW_LINE;
  LOG_SEP;
  LOG_SEP;
  LOG_MEM;
  LOG_NEW_LINE;
  LOG_MESSAGE(F("Adresse IP : %d.%d.%d.%d\n"), myIP[0], myIP[1], myIP[2], myIP[3]);
  LOG_MESSAGE(F("Passerelle : %d.%d.%d.%d\n"), gw[0], gw[1], gw[2], gw[3]);
  LOG_MESSAGE(F("Masque     : %d.%d.%d.%d\n"), mask[0], mask[1], mask[2], mask[3]);
  LOG_MESSAGE(F("Broadcast  : %d.%d.%d.%d\n"), bcast[0], bcast[1], bcast[2], bcast[3]);
  LOG_NEW_LINE;


  // Initialisation de la configuration
  // ----------------------------------

  //Config.Initialize();


  // Initialisation de l'heure
  // -------------------------

  LOG_MESSAGE(F("Initialisation de l'heure\n"));
  TimeManager.Initialize();
  LOG_NEW_LINE;


  // Initialisation de l'afficheur LCD
  // ---------------------------------

  LOG_MESSAGE(F("Initialisation de l'afficheur LCD\n"));

  LCDDisplay.Initialize(0x27, 20, 4);  // Adresse 0x27 pour keyestudio
  LCDDisplay.Clear();
  LCDDisplay.Print(0, 0, "[Module_piscine]");
  LCDDisplay.Print(0, 1, "Version : %s", Utils::GetStringFromDict(FIRMWARE_VERSION).c_str());
  
  LOG_NEW_LINE;
  LOG_MESSAGE(F("[Module_piscine]\n"));
  LOG_MESSAGE(F("Version : %s\n"), Utils::GetStringFromDict(FIRMWARE_VERSION).c_str());
  LOG_NEW_LINE;


  // Initialisation des capteurs
  // ---------------------------
  
  LOG_MESSAGE(F("Initialisation des capteurs\n"));
  Sensor::InitializeAllSensors();
  LOG_NEW_LINE;


  // Initialisation des relais
  // -------------------------
  
  LOG_MESSAGE(F("Initialisation des relais\n"));
  Relay::InitializeAllRelays();
  LOG_NEW_LINE;


  // Initialisation du client MQTT
  // -----------------------------

  LOG_MEM;
  LOG_MESSAGE(F("Initialisation du client MQTT\n"));

  MQTTClient.Initialize(Utils::GetStringFromDict(MQTT_CLIENT), IPAddress(192,168,10,11), 1883, 10000);
  std::vector< String > subscription = 
    { Utils::GetStringFromDict(POOL_PUMP_1_RELAY), 
      Utils::GetStringFromDict(POOL_PUMP_2_RELAY), 
      Utils::GetStringFromDict(POOL_LIGHT_RELAY), };
  MQTTClient.SetCallback(&MQTTCallback, subscription);

  LOG_NEW_LINE;


  // Création des modes d'affichage
  // ------------------------------
  
  LOG_MEM;
  LOG_MESSAGE(F("Creation des modes d'affichage\n"));
  
  LCDDisplay.RegisterDisplayMode(TEMPERATURE_PH_AND_ORP_VALUES, new DisplayMode(&DisplayFunction1, 5000));
  LCDDisplay.SetCurrentDisplayMode(TEMPERATURE_PH_AND_ORP_VALUES);
  
  LOG_NEW_LINE;


  // Création des boucles de traitements
  // -----------------------------------

  LOG_MEM;
  LOG_MESSAGE(F("Creation des boucles de traitements\n"));

/*
  // Boucle de client ethernet toutes les 500 ms
  SoftTimer.add(new Task(500, EthernetClientLoop));
*/
  // Boucle de mise à jour des capteurs toutes les 10 s
  SoftTimer.add(new Task(10000, UpdateSensorsLoop));
/*
  // Boucle de régulation ORP toutes les 1 s
  SoftTimer.add(new Task(1000, ORPRegulationLoop));

  // Boucle de régulation du pH toutes les 1.1 s
  SoftTimer.add(new Task(1100, PHRegulationLoop));
*/
  // Boucle de publication MQTT toutes les 30 s
  SoftTimer.add(new Task(30000, MQTTPublishDataLoop));

  // Boucle principale toutes les 100 ms
  SoftTimer.add(new Task(100, MainLoop));

  LOG_NEW_LINE;
  LOG_SEP;
  LOG_SEP;
  LOG_NEW_LINE;

  delay(2000);
  
  // Activation d'un watchdog à 8 sec
  // --------------------------------

  wdt_enable(WDTO_8S);
}


// -------------------------------------------------------------------------------------
// BOUCLE PRINCIPALE
// -------------------------------------------------------------------------------------

void MainLoop(Task* me)
{
  // Reset du watchdog pour qu'il ne se déclenche pas
  wdt_reset();

  // Traitement des possibles ordres sur la socket UDP
  LogConsole.ProcessUDPCommand();

  uint32_t nowMillisec = millis();

  static uint32_t s_lastLogMemTimeMillisec = 0;
  if ( nowMillisec - s_lastLogMemTimeMillisec >= 30000 )
  {
    s_lastLogMemTimeMillisec = nowMillisec;
    LOG_MEM;
  }

  // Traitements du client MQTT (sauf publication)
  MQTTClient.Loop(nowMillisec);

  // Traitements de l'affichage
  LCDDisplay.Loop(nowMillisec);
}

/*
// -------------------------------------------------------------------------------------
// Boucle de client ethernet
// -------------------------------------------------------------------------------------

void EthernetClientLoop(Task* me)
{
}
*/

// -------------------------------------------------------------------------------------
// Boucle mise à jour des capteurs
// -------------------------------------------------------------------------------------

void UpdateSensorsLoop(Task* me)
{
  uint32_t beforeMillisec = millis();
  
  // Mise à jour de tous les capteurs
  Sensor::UpdateAllSensors(true);

  uint32_t afterMillisec = millis();

  LOG_MESSAGE(F("MAJ capteurs -> %d ms\n"), afterMillisec - beforeMillisec);
}


/*
// -------------------------------------------------------------------------------------
// Boucle de régulation ORP
// -------------------------------------------------------------------------------------

void ORPRegulationLoop(Task* me)
{
}


// -------------------------------------------------------------------------------------
// Boucle de régulation pH
// -------------------------------------------------------------------------------------

void PHRegulationLoop(Task* me)
{
}
*/

// -------------------------------------------------------------------------------------
// Boucle de publication MQTT
// -------------------------------------------------------------------------------------

void MQTTPublishDataLoop(Task* me)
{
  if ( MQTTClient.IsConnected() == true )
  {
    static String s_slash                 = Utils::GetStringFromDict(SLASH);
    static String s_waterTemperatureTopic = Utils::GetStringFromDict(MQTT_CLIENT) + s_slash + Utils::GetStringFromDict(WATER_TEMPERATURE_TOPIC);
    static String s_airTemperatureTopic   = Utils::GetStringFromDict(MQTT_CLIENT) + s_slash + Utils::GetStringFromDict(AIR_TEMPERATURE_TOPIC);
    static String s_airHumidityTopic      = Utils::GetStringFromDict(MQTT_CLIENT) + s_slash + Utils::GetStringFromDict(AIR_HUMIDITY_TOPIC);
    static String s_pHTopic               = Utils::GetStringFromDict(MQTT_CLIENT) + s_slash + Utils::GetStringFromDict(PH_TOPIC);
    static String s_ORPTopic              = Utils::GetStringFromDict(MQTT_CLIENT) + s_slash + Utils::GetStringFromDict(ORP_TOPIC);
    static String s_filterPressureTopic   = Utils::GetStringFromDict(MQTT_CLIENT) + s_slash + Utils::GetStringFromDict(FILTER_PRESSURE_TOPIC);

    // Envoi de la température de l'eau
    float  value    = waterTemperatureSensor.GetTemperature();
    String strValue = Utils::Float2String(value, 1);
    MQTTClient.Publish(s_waterTemperatureTopic, strValue);

    // Envoi de la température de l'air
    value    = roomTemperatureAndHumiditySensor.GetTemperature();
    strValue = Utils::Float2String(value, 0);
    MQTTClient.Publish(s_airTemperatureTopic, strValue);

    // Envoi de l'humidité de l'air
    value    = roomTemperatureAndHumiditySensor.GetHumidity();
    strValue = Utils::Float2String(value, 1);
    MQTTClient.Publish(s_airHumidityTopic, strValue);

    // Envoi du pH
    value    = phSensor.GetPH();
    strValue = Utils::Float2String(value, 1);
    MQTTClient.Publish(s_pHTopic, strValue);

    // Envoi de l'ORP
    value    = orpSensor.GetORP();
    strValue = Utils::Float2String(value, 1);
    MQTTClient.Publish(s_ORPTopic, strValue);

    // Envoi de la pression du filtre
    value    = filterPressureSensor.GetPressure();
    strValue = Utils::Float2String(value, 1);
    MQTTClient.Publish(s_filterPressureTopic, strValue);

    LOG_MESSAGE(F("\nPublication MQTT\n"));
  }
}
