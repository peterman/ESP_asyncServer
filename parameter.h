struct Settings {
  char ssid[15] = "devwifi";
  char pass[15] = "devwifi1";
  char host[15] = "asm-connect";
  char edit_user[10] = "admin";
  char edit_pass[10] = "admin";
};

const char *settingsfile = "/settings.json";
const char *wifinets = "/wifinets.json";

Settings settings;
