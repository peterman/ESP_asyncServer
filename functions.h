// Saves the configuration to a file
void saveConfiguration(const char *filename, const Settings &config) {
  SPIFFS.remove(filename);      // Delete existing file, otherwise the configuration is appended to the file

  File file = SPIFFS.open(filename, "w");
  if (!file) { Serial.println(F("Failed to create file"));  return; }

  StaticJsonDocument<512> doc;

  // Set the values in the document
  doc["hostname"] = config.host;
  doc["ssid"] = config.ssid;
  doc["wifikey"] = config.pass;
  doc["edituser"] = config.edit_user;
  doc["editpass"] = config.edit_pass;
  
  // Serialize JSON to file
  if (serializeJsonPretty(doc, file) == 0) { Serial.println(F("Failed to write to file")); }

  // Close the file
  file.close();
}

void readsettings() {
  File file = SPIFFS.open(settingsfile, "r");

  if (!file)
    {
      Serial.println("There was an error opening the file");
      Serial.println("create new File");
      saveConfiguration(settingsfile, settings);
      return;
    }

    else
    {
      Serial.println("File opened!");

      StaticJsonDocument<512> doc;

      DeserializationError error = deserializeJson(doc, file);
      if (error)
      {
        Serial.println("error...");
      }
      else
      {
        Serial.println("Another winner!");
        
        strcpy(settings.ssid, doc["ssid"]);
        strcpy(settings.pass, doc["wifikey"]);
        strcpy(settings.host, doc["hostname"]);
        strcpy(settings.edit_user, doc["edituser"]);
        strcpy(settings.edit_pass, doc["editpass"]);
      }
      Serial.println("");
    }

    file.close();
  }

// Prints the content of a file to the Serial
void printFile(const char *filename) {
  // Open file for reading
  File file = SPIFFS.open(filename, "r");
  if (!file) {
    Serial.println(F("Failed to read file"));
    return;
  }

  // Extract each characters by one by one
  while (file.available()) {
    Serial.print((char)file.read());
  }
  Serial.println();

  // Close the file
  file.close();
}

int dBmtoPercentage(int dBm)
{
  int quality;
  const int RSSI_MIN = -100;
  const int RSSI_MAX = -50;
    if(dBm <= RSSI_MIN) { quality = 0; }
    else if(dBm >= RSSI_MAX) {  quality = 100; }
    else { quality = 2 * (dBm + 100); }
return quality;
}

String encryptionTypes(int which) {
  switch (which) {
    case ENC_TYPE_WEP: return "WEP"; break;
    case ENC_TYPE_TKIP: return "WPA/TKIP"; break;
    case ENC_TYPE_CCMP: return "WPA2/CCMP"; break;
    case ENC_TYPE_NONE: return "None"; break;
    case ENC_TYPE_AUTO: return "Auto"; break;
    default: return "Unknown"; break;
  }
}
     
void Save_Wifiscan_Result(){
  WiFi.scanNetworks(true);
  int count = 0;
  while ((WiFi.scanComplete() < 0) && (count < 10)) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("end scanning");
  int n = WiFi.scanComplete();
  Serial.printf("%d Networks found", n);
  Serial.println();
  String json = "[";
  if(n){
    for (int i = 0; i < n; ++i){
      if(i) json += ",";
      json += "{";
      json += "\"rssi\":\""+String(WiFi.RSSI(i))+"\"";
      json += ",\"quality\":\""+String(dBmtoPercentage(WiFi.RSSI(i)))+"\"";
      json += ",\"ssid\":\""+WiFi.SSID(i)+"\"";
      json += ",\"bssid\":\""+WiFi.BSSIDstr(i)+"\"";
      json += ",\"channel\":\""+String(WiFi.channel(i))+"\"";
      json += ",\"secure\":\""+String(encryptionTypes(WiFi.encryptionType(i)))+"\"";
      json += ",\"hidden\":\""+String(WiFi.isHidden(i)?"true":"false")+"\"";
      json += "}";
    }
  }
  json += "]";
  SPIFFS.remove(wifinets);      // Delete existing file, otherwise the configuration is appended to the file
  File file = SPIFFS.open(wifinets, "w");
  if (!file) { Serial.println(F("Failed to create file"));  return; }
  file.println(json); 
  file.close();      
  json = String();
}
