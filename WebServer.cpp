#include "WebServer.h"
#include "TimeManager.h"
#include "Utils.h"
#include "Globals.h"

#include <EthernetClient.h>


/*
  Constructeur
*/
WebServerClass::WebServerClass() :
  m_webServer(80)
{
}


/*
  Initialisation
*/
void WebServerClass::Initialize()
{
  // On démarre le serveur
  m_webServer.begin();
}


/*
  Lire le fichier XML contenant les données sytème
  appel avec http://<addresse_IP>/infos pour obtenir les infos système
*/
void WebServerClass::XML_response(const EthernetClient& client)
{
  client.println(F("<?xml version = \"1.0\" encoding=\"UTF-8\"?>"));
  client.println(F("<root>"));
  
    client.println(F("<device>"));
      client.println(F("<Date>"));//day, weekday, month, year, hour, minute, sec
        client.println(TimeManager.NowStr());
      client.println(F("</Date>"));
      client.println(F("<Firmware>"));
        client.println(Utils::GetStringFromDict(FIRMWARE_VERSION));
      client.println(F("</Firmware>"));
      client.println(F("<FreeRam>"));
        client.println(Utils::GetFreeRAM());
      client.println(F("</FreeRam>"));
      client.println(F("<IP>"));
        client.println(Ethernet.localIP());
      client.println(F("</IP>"));
      client.println(F("<Mac>"));
        client.println(myMACStr);
      client.println(F("</Mac>"));
      client.println(F("<DS18b20_0>"));
        client.println(waterTemperatureSensor.GetTemperatureSensorAddress());
        client.println(waterTemperatureSensor.GetTemperature());
      client.println(F("</DS18b20_0>"));
      client.println(F("<PhPumpEr>"));
        //client.println(PhPump.UpTimeError);
      client.println(F("</PhPumpEr>"));
      client.println(F("<ChlPumpEr>"));
        //client.println(ChlPump.UpTimeError);
      client.println(F("</ChlPumpEr>"));
    client.println(F("</device>"));

    client.println(F("<Filtration>"));
      client.println(F("<Pump>"));
        //client.println(FiltrationPump.IsRunning());
      client.println(F("</Pump>")); 
      client.println(F("<Duration>"));
        //client.println(storage.FiltrationDuration);
      client.println(F("</Duration>")); 
      client.println(F("<Start>"));
        //client.println(storage.FiltrationStart);
      client.println(F("</Start>")); 
      client.println(F("<Stop>"));
        //client.println(storage.FiltrationStop);
      client.println(F("</Stop>"));
      client.println(F("<StopMax>"));
        //client.println(storage.FiltrationStopMax);
      client.println(F("</StopMax>"));
    client.println(F("</Filtration>"));
                
    client.println(F("<pH>"));
      client.println(F("<Value>"));
        //client.println(storage.PhValue);
      client.println(F("</Value>"));           
      client.println(F("<Pump>"));
        //client.println(PhPump.IsRunning());
      client.println(F("</Pump>"));
      client.println(F("<UpT>"));
        //client.println(PhPump.UpTime/1000);
      client.println(F("</UpT>"));
      client.println(F("<TankLevel>"));
        //client.println(PhPump.TankLevel());
      client.println(F("</TankLevel>"));
      client.println(F("<PIDMode>"));
        //client.println(storage.Ph_RegulationOnOff);
      client.println(F("</PIDMode>"));
      client.println(F("<Kp>"));
        //client.println(storage.Ph_Kp);
      client.println(F("</Kp>"));
      client.println(F("<Ki>"));
        //client.println(storage.Ph_Ki);
      client.println(F("</Ki>"));
      client.println(F("<Kd>"));
        //client.println(storage.Ph_Kd);
      client.println(F("</Kd>"));
      client.println(F("<SetPoint>"));
        //client.println(storage.Ph_SetPoint);
      client.println(F("</SetPoint>"));
      client.println(F("<CalibCoeff0>"));
        //client.println(storage.pHCalibCoeffs0);
      client.println(F("</CalibCoeff0>"));
      client.println(F("<CalibCoeff1>"));
        //client.println(storage.pHCalibCoeffs1);
      client.println(F("</CalibCoeff1>"));       
    client.println(F("</pH>"));

    client.println(F("<ORP>"));
      client.println(F("<Value>"));
        //client.println(storage.OrpValue);
      client.println(F("</Value>"));           
      client.println(F("<Pump>"));
        //client.println(ChlPump.IsRunning());
      client.println(F("</Pump>"));
      client.println(F("<UpT>"));
        //client.println(ChlPump.UpTime/1000);
      client.println(F("</UpT>"));
      client.println(F("<TankLevel>"));
        //client.println(ChlPump.TankLevel());
      client.println(F("</TankLevel>"));
      client.println(F("<PIDMode>"));
        //client.println(storage.Orp_RegulationOnOff);
      client.println(F("</PIDMode>"));
      client.println(F("<Kp>"));
        //client.println(storage.Orp_Kp);
      client.println(F("</Kp>"));
      client.println(F("<Ki>"));
        //client.println(storage.Orp_Ki);
      client.println(F("</Ki>"));
      client.println(F("<Kd>"));
        //client.println(storage.Orp_Kd);
      client.println(F("</Kd>"));
      client.println(F("<SetPoint>"));
        //client.println(storage.Orp_SetPoint);
      client.println(F("</SetPoint>"));
      client.println(F("<CalibCoeff0>"));
        //client.println(storage.OrpCalibCoeffs0);
      client.println(F("</CalibCoeff0>"));
      client.println(F("<CalibCoeff1>"));
        //client.println(storage.OrpCalibCoeffs1);
      client.println(F("</CalibCoeff1>"));
    client.println(F("</ORP>"));
      
  client.println(F("</root>"));
}


/*
  Boucle de traitement à appeler dans le programme principal
  @param nowMillisec Heure actulelle en ms
*/
void WebServerClass::Loop(uint32_t nowMillisec)
{
  // Y a-t-il un client connecté ?
  EthernetClient client = m_webServer.available();

  // Si oui
  if ( client  == true )
  {
    boolean currentLineIsBlank = true;
    String  readString;

    while ( client.connected() == true )
    {
      // Y a-t-il des données ?
      if ( client.available() == true )
      {
        // Lecture d'un octet
        char c = client.read();

        // On met en buffer la 1ère partie de la requète HTTP en lisant caractère par caractère
        if ( readString.length() < 100 )
        {
          // Mémorisation du caractère dans la chaine
          readString += c;
        }
        
        // La requète se termine par une ligne vide avec un '\n'
        // On répond au client seulement après la réception complète de la requète
        if ( ( c == '\n' ) && 
             ( currentLineIsBlank == true ) ) 
        {
          // On envoie un header de réponse HTTP standard
          client.println(F("HTTP/1.1 200 OK"));

          // La suite de la réponse dépend si on demande la page web ou le fichier XML
            
          // Si la requète contient "infos" : fichier XML
          if ( readString.indexOf("infos") > 0 )
          {
            // On envoie le reste du header HTTP
            client.println(F("Content-Type: text/xml"));
            client.println(F("Connection: keep-alive"));
            client.println();

            // On envoie le fichier XML
            XML_response(client);
          }

          // Sinon, c'est la page web qui est demandée
          else
          {
            // On envoie le reste du header HTTP
            client.println(F("Content-Type: text/xml"));
            client.println(F("Connection: keep-alive"));
            client.println("");

            client.println(F("<!DOCTYPE html>"));
            client.println(F("<html>"));
            client.println(F("<head>")); 
                    
            client.println(F("<style>"));
            client.println(F("table.blueTable {"));
            client.println(F("border: 1px solid #1C6EA4;"));
            client.println(F("background-color: #EEEEEE;"));
            client.println(F("width: 70\%;"));
            client.println(F("text-align: left;"));
            client.println(F("border-collapse: collapse;"));
            client.println(F("}"));
            client.println(F("table.blueTable td, table.blueTable th {"));
            client.println(F("border: 1px solid #AAAAAA;"));
            client.println(F("padding: 3px 2px;"));
            client.println(F("}"));
            client.println(F("table.blueTable tbody td {"));
            client.println(F("font-size: 14px;"));
            client.println(F("}"));
            client.println(F("table.blueTable tr:nth-child(even) {"));
            client.println(F("background: #D0E4F5;"));
            client.println(F("}"));
            client.println(F("table.blueTable tfoot td {"));
            client.println(F("font-size: 14px;"));
            client.println(F("}"));
            client.println(F("table.blueTable tfoot .links {"));
            client.println(F("text-align: right;"));
            client.println(F("}"));
            client.println(F("table.blueTable tfoot .links a{"));
            client.println(F("display: inline-block;"));
            client.println(F("background: #1C6EA4;"));
            client.println(F("color: #FFFFFF;"));
            client.println(F("padding: 2px 8px;"));
            client.println(F("border-radius: 5px;"));
            client.println(F("}"));
            client.println(F("</style>"));

            client.println(F("<title></title>"));

            // Function AJAX qui demande le fichier XML d'infos à l'Arduino
            // et remplit la page web avec les données reçues, toutes les 2 secondes
            client.println(F("<script>"));
            client.println(F("function GetData()"));
            client.println(F("{"));
              client.println(F("nocache = \"&nocache=\" + Math.random() * 1000000;"));
              client.println(F("var request = new XMLHttpRequest();"));
              client.println(F("request.onreadystatechange = function()"));
              client.println(F("{"));
                client.println(F("if ( this.readyState == 4 ) {"));
                  client.println(F("if ( this.status == 200 ) {"));
                    client.println(F("if ( this.responseXML != null ) {"));
                      
                      // Extraction des données depuis le fichier XML
                      client.println(F("document.getElementById(\"Date\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('Date')[0].childNodes[0].nodeValue;"));

                      client.println(F("document.getElementById(\"WaterTemp\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('DS18b20_0')[0].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"pH\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('Value')[0].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"ORP\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('Value')[1].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"Filtration\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('Pump')[0].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"FiltStart\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('Start')[0].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"FiltStop\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('Stop')[0].childNodes[0].nodeValue;"));

                      client.println(F("document.getElementById(\"pH2\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('Value')[0].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"ORP2\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('Value')[1].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"pHPump\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('Pump')[1].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"ORPPump\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('Pump')[2].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"pHTank\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('TankLevel')[0].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"ORPTank\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('TankLevel')[1].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"pHPID\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('PIDMode')[0].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"ORPPID\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('PIDMode')[1].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"pHSetPoint\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('SetPoint')[0].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"ORPSetPoint\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('SetPoint')[1].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"pHCal0\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('CalibCoeff0')[0].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"pHCal1\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('CalibCoeff1')[0].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"ORPCal0\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('CalibCoeff0')[1].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"ORPCal1\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('CalibCoeff1')[1].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"pHKp\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('Kp')[0].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"ORPKp\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('Kp')[1].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"pHKi\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('Ki')[0].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"ORPKi\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('Ki')[1].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"pHKd\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('Kd')[0].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"ORPKd\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('Kd')[1].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"PhPumpEr\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('PhPumpEr')[0].childNodes[0].nodeValue;"));
                      client.println(F("document.getElementById(\"ChlPumpEr\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('ChlPumpEr')[0].childNodes[0].nodeValue;")); 
                      client.println(F("document.getElementById(\"pHPT\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('UpT')[0].childNodes[0].nodeValue;")); 
                      client.println(F("document.getElementById(\"ChlPT\").innerHTML ="));
                      client.println(F("this.responseXML.getElementsByTagName('UpT')[1].childNodes[0].nodeValue;"));
                      
                    client.println(F("}"));
                  client.println(F("}"));
                client.println(F("}"));
              client.println(F("}"));
              client.println(F("request.open(\"GET\", \"infos\" + nocache, true);"));
              client.println(F("request.send(null);"));
              client.println(F("setTimeout('GetData()', 4000);"));
            client.println(F("}"));
            client.println(F("</script>"));
    
            client.println(F("</head>"));
            client.println(F("<body onload=\"GetData()\">"));
            client.println(F("<h1>Module Piscine</h1>")); 
            client.println(F("<h3><span id=\"Date\">...</span></h3>\n\n")) ; 
  
            // Preemière table : Water temp, pH and ORP values
            client.println(F("<table class=\"blueTable\">"));
            client.println(F("<tbody>"));
            client.println(F("<tr>"));
            client.println(F("<td>Water temp. (deg): <span id=\"WaterTemp\">...</span></td>"));
            client.println(F("<td>pH: <span id=\"pH\">...</span></td>"));
            client.println(F("<td>ORP (mV): <span id=\"ORP\">...</span></td>"));
            client.println(F("</tr>"));
            client.println(F("<tr>"));
            client.println(F("<td>Filtration: <span id=\"Filtration\">...</span></td>"));
            client.println(F("<td>Start: <span id=\"FiltStart\">...</span></td>"));
            client.println(F("<td>Stop: <span id=\"FiltStop\">...</span></td>"));
            client.println(F("</tr>"));                        
            client.println(F("</tbody>"));
            client.println(F("</table>"));
            client.println(F("<br>"));

            // Seconde table : pH and ORP parameters
            client.println(F("<table class=\"blueTable\">"));
            client.println(F("<tbody>"));
            client.println(F("<tr>"));
            client.println(F("<td></td>"));
            client.println(F("<td>pH</td>"));
            client.println(F("<td>ORP/Chl</td>"));
            client.println(F("</tr>"));
            client.println(F("<tr>"));
            client.println(F("<td>Value</td>"));
            client.println(F("<td><span id=\"pH2\">...</span></td>"));
            client.println(F("<td><span id=\"ORP2\">...</span></td>"));
            client.println(F("</tr>"));
            client.println(F("<td>SetPoint</td>"));
            client.println(F("<td><span id=\"pHSetPoint\">...</span></td>"));
            client.println(F("<td><span id=\"ORPSetPoint\">...</span></td>"));
            client.println(F("</tr>"));
            client.println(F("<tr>"));
            client.println(F("<tr>"));
            client.println(F("<td>Pump</td>"));
            client.println(F("<td><span id=\"pHPump\">...</span></td>"));
            client.println(F("<td><span id=\"ORPPump\">...</span></td>"));
            client.println(F("</tr>"));    
            client.println(F("<tr>"));
            client.println(F("<td>Tank level</td>"));
            client.println(F("<td><span id=\"pHTank\">...</span></td>"));
            client.println(F("<td><span id=\"ORPTank\">...</span></td>"));
            client.println(F("</tr>"));               
            client.println(F("<tr>"));
            client.println(F("<td>PID</td>"));
            client.println(F("<td><span id=\"pHPID\">...</span></td>"));
            client.println(F("<td><span id=\"ORPPID\">...</span></td>"));
            client.println(F("</tr>"));  
            client.println(F("<tr>"));
            client.println(F("<td>Calib. coeff0</td>"));
            client.println(F("<td><span id=\"pHCal0\">...</span></td>"));
            client.println(F("<td><span id=\"ORPCal0\">...</span></td>"));
            client.println(F("</tr>"));                         
            client.println(F("<tr>"));
            client.println(F("<td>Calib. coeff1</td>"));
            client.println(F("<td><span id=\"pHCal1\">...</span></td>"));
            client.println(F("<td><span id=\"ORPCal1\">...</span></td>"));
            client.println(F("</tr>"));
            client.println(F("<tr>"));
            client.println(F("<td>Kp</td>"));
            client.println(F("<td><span id=\"pHKp\">...</span></td>"));
            client.println(F("<td><span id=\"ORPKp\">...</span></td>"));
            client.println(F("</tr>"));                       
            client.println(F("<tr>"));
            client.println(F("<td>Ki</td>"));
            client.println(F("<td><span id=\"pHKi\">...</span></td>"));
            client.println(F("<td><span id=\"ORPKi\">...</span></td>"));
            client.println(F("</tr>"));                          
            client.println(F("<tr>"));
            client.println(F("<td>Kd</td>"));
            client.println(F("<td><span id=\"pHKd\">...</span></td>"));
            client.println(F("<td><span id=\"ORPKd\">...</span></td>"));
            client.println(F("</tr>")); 
            client.println(F("<tr>"));
            client.println(F("<td>Pumps Uptime (sec):</td>"));
            client.println(F("<td><span id=\"pHPT\">...</span></td>"));
            client.println(F("<td><span id=\"ChlPT\">...</span></td>"));
            client.println(F("</tr>"));                               
            client.println(F("</tbody>"));
            client.println(F("</table>"));
            client.println(F("<br>"));

            // Troisième table : messages
            client.println(F("<table class=\"blueTable\">"));
            client.println(F("<tbody>"));
            client.println(F("<tr>"));
            client.println(F("<td>Errors: <span id=\"Errors\">...</span></td>"));
            client.println(F("<td>Acid Pump: <span id=\"PhPumpEr\">...</span></td>"));
            client.println(F("<td>Chl Pump: <span id=\"ChlPumpEr\">...</span></td>"));
            client.println(F("</tr>"));
            client.println(F("</tbody>"));
            client.println(F("</table>"));
            client.println(F("<br>"));

            client.println(F("</body>"));
            client.println(F("</html>"));              
          }

          // Remise à 0 du buffer
          readString = F("");

          // On sort de la boucle while ( client.connected() == true )
          break;
        }
          
        // Chaque ligne de texte se termine par '\r\n'
        if ( c == '\n' ) 
        {
          // last character on line of received text
          // starting new line with next character read
          currentLineIsBlank = true;
        } 

        else if ( c != '\r' ) 
        {
          // a text character was received from client
          currentLineIsBlank = false;
        }
      } // if ( client.available() == true )
    } // while ( client.connected() == true )
    
    // On donne du temps au client pour recevoir les données
    delay(1);
    
    // On ferme la connexion
    client.stop(); 
  } // if ( client == true )  
}

  
WebServerClass WebServer;
