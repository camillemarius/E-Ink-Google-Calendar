#ifndef GOOGLESCRIPTREQUEST
#define GOOGLESCRIPTREQUEST
#include "Arduino.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WifiClientSecure.h>
#include <HTTPClient.h> // Needs to be from the ESP32 platform version 3.2.0 or later, as the previous has problems with http-redirect (google script)

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include "filter/stringFilter.hpp"
 

struct calendarEntries
{
  String Month;
  String Day;
  String Time;
  String Task;
};

class googleScriptRequest
{
public:
    googleScriptRequest(int size);
    void HTTPRequest(void);
    void sortHTTPRequest(String ID_Calendar);
    bool connectToWifi(void);
    bool checkInternetConnection(void);
    bool waitForInternetConnection(int wait_s);

    String getTask(int index);
    String getDay(int index);
    String getTime(int index);
    String getMonth(int index);
    int getEntryCount();

    const String ID_Calendar1 = "#C1;;";
    const String ID_Calendar2 = "#C2;;";

private:
    WiFiManager manager; 
    const char*  ssid = "Salt_2GHz_159505";
    const char* password = "MuUY33L6SERtXG4CDT";

    String response;

    const int calEntryCount;
    struct calendarEntries calEnt[6];

    HTTPClient http;
    const String calendarServer = "script.google.com"; 

    //--------------------------------------------------------------------------------------------------------------------camillemariuschatton@gmail.com
    //const String calendarRequest = "https://script.google.com/macros/s/AKfycbyPqiILIhmpg_CP27xqAg-FXjKxnxgSB8ZtHV4Kh79C-BmsPccdP1XgMW1sXyzT3O3vdQ/exec";

    //--------------------------------------------------------------------------------------------------------------------Anna-Sophia
    //const String calendarRequest = "https://script.google.com/macros/s/AKfycbzs_3rQ9VJhPYhufw4Usoo2A7UgIbgqua-nDLkX7KQ3LO18g4160ThGnMdKXWywPL59og/exec";

    //--------------------------------------------------------------------------------------------------------------------jaya.hilfiker@gmail.com
    const String calendarRequest = "https://script.google.com/macros/s/AKfycby9f1XumMVQ46J7E3w6Cv5OaVysEP9NyyRTKoTpEqzPBotikvPOqz9Q51ZtRgvs7qJPOA/exec";
};
#endif