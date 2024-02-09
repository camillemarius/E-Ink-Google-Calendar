#include "googleScriptRequest.hpp"

googleScriptRequest::googleScriptRequest(int size): calEntryCount(size){
    //size: number of Tasks
}

void googleScriptRequest::HTTPRequest(void)
{
    // Getting calendar from your published google script
    Serial.println("Getting calendar");
    Serial.println(calendarRequest);

    http.end();
    http.setTimeout(20000);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    if (!http.begin(calendarRequest))
    {
        Serial.println("Cannot connect to google script");
    }

    Serial.println("Connected to google script");
    int returnCode = http.GET();
    Serial.print("Returncode: ");
    Serial.println(returnCode);
    response = http.getString();
    Serial.print("Response: ");
    Serial.println(response);
}

void googleScriptRequest::sortHTTPRequest(String ID_Calendar)
{
    int indexFrom = response.length() - 1;
    int nextSemiq = 0;
    int cutDateIndex = 0;
    String strBuffer = "";
    int count = 0;
    int line = 0;

    // Extract Calendar1 and Calendar2 from common String
    int index_ID_Calendar1 = response.indexOf(ID_Calendar1, 0);
    int index_ID_Calendar2 = response.indexOf(ID_Calendar2, 0);

    if (ID_Calendar == ID_Calendar1) {
        response = response.substring(index_ID_Calendar1 + ID_Calendar1.length() , index_ID_Calendar2);
    }
    else {
        response = response.substring(index_ID_Calendar2 + ID_Calendar2.length(), indexFrom);
    }

    // Fill calendarEntries with entries from the get-request
    while (nextSemiq >= 0 && line < calEntryCount)
    {
        nextSemiq = response.indexOf(";", 0);
        strBuffer = response.substring(0, nextSemiq);
        response = response.substring(nextSemiq + 1, indexFrom);

        // Extract String
        switch (count)
        {
        case 0:
			//------------------------------------------ 8 1 15:30;Bier um vier;
            cutDateIndex = strBuffer.indexOf(" ", 0);
            calEnt[line].Day = strBuffer.substring(0, cutDateIndex); // Extract day
            Serial.print("Day: ");
            Serial.println(calEnt[line].Day);
			
			//------------------------------------------ 1 15:30;Bier um vier;
			strBuffer = strBuffer.substring(cutDateIndex + 1);
            cutDateIndex = strBuffer.indexOf(" ", 0);
            calEnt[line].Month = strBuffer.substring(0, cutDateIndex); // Extract month
            Serial.print("Month: ");
            Serial.println(calEnt[line].Month);
			
			//------------------------------------------ 15:30;Bier um vier;
			strBuffer = strBuffer.substring(cutDateIndex+1);
            cutDateIndex = strBuffer.indexOf(";", 0);
            calEnt[line].Time = strBuffer.substring(0, cutDateIndex); // Extract time
            Serial.print("Time: ");
            Serial.println(calEnt[line].Time);

            count = 1;
			
            break;

        case 1: //Filter
		
			//------------------------------------------ Bier um vier;
			strBuffer = strBuffer.substring(cutDateIndex+1);
			
            // shorten Young Boys to YB
            stringFilter::YoungBoys_to_YB(strBuffer);

            // Umlaute
            stringFilter::umlauts(strBuffer);

            // Save Event
            calEnt[line].Task = strBuffer;
            Serial.print("Task: ");
            Serial.println(calEnt[line].Task);
            count = 0;
            line++;
            break;

        default:
            count = 0;
            break;
        }
    }
}

bool googleScriptRequest::connectToWifi(void) {
    manager.setConfigPortalTimeout(3*60);
    if(manager.autoConnect("E-Paper-Calendar")) {
        return true;
    }
    Serial.println("There is no Wifi Connection, not even afer 3 Min Timeout");
    return false;
}

bool googleScriptRequest::waitForInternetConnection(int wait_s) {
    
    Serial.println("SSID: " + WiFi.SSID());
    int conn_result = WiFi.waitForConnectResult(wait_s*600000); // connect try it for 1 minute
    Serial.print("conn_result: ");
    Serial.println(conn_result, DEC);
    return true;
}

bool googleScriptRequest::checkInternetConnection(void)
{
  HTTPClient http;
  if (http.begin("script.google.com", 443))
  {
    http.end();
    return true;
  }
  else
  {
    http.end();
    return false;
  }
}

String googleScriptRequest::getTask(int index) {
    if(index<calEntryCount) {
        return calEnt[index].Task;
    }
    return "Out of Bound";
}

String googleScriptRequest::getTime(int index){
    if(index<calEntryCount) {
        return calEnt[index].Time;
    }
    return "Out of Bound";
}


String googleScriptRequest::getDay(int index){
    if(index<calEntryCount) {
        return calEnt[index].Day;
    }
    return "Out of Bound";
}

String googleScriptRequest::getMonth(int index){
    if(index<calEntryCount) {
        return calEnt[index].Month;
    }
    return "Out of Bound";
}

int googleScriptRequest::getEntryCount() {
    return calEntryCount;
}