#include <EEPROMInterface.h>
#include <SerialLog.h>
#include <AZ3166WiFi.h>

void setup() {

    EEPROMInterface eeprom;

    // enable the security chip
    eeprom.enableHostSecureChannel();

    // write data to the wifi ssid and password zones
    // Zone constants are defined in EEPROMInterface.h
    char ssid[] = "<Your WiFI SSID>";
    char password[] = "<Your WiFi password>";
    
    int ret = eeprom.write((uint8_t*)ssid, 
                           strlen(ssid), 
                           WIFI_SSID_ZONE_IDX);

    if (ret < 0)
    {
        Screen.print(1, "Can't write SSID");
        return;
    }
    
    ret = eeprom.write((uint8_t*)password, 
                       strlen(password), 
                       WIFI_PWD_ZONE_IDX);
    if (ret < 0)
    {
        Screen.print(1, "Can't write Password");
        return;
    }

    // Check the WiFi connects to show it's written correctly
    if (WiFi.begin() != WL_CONNECTED)
    {
        Screen.print(1, "No WiFi");
    }
    
    // Write to Zone 7 - one of the two safe zones.
    // Zones 7 and 8 are the only two not reserved
    // Each zone has a maximum size defined by STSAFE_ZONE_7_SIZE and 
    // STSAFE_ZONE_8_SIZE, and this the maximum amount of bytes you can store
    // in that zone
    char stuff[] = "Hello world!";

    ret = eeprom.write((uint8_t*)stuff, 
                       strlen(stuff), 
                       STSAFE_ZONE_7_IDX);

    // The write returns 0 on success, -1 on error.
    // One reason for an error is writing more bytes than the buffer can handle
    if (ret < 0)
    {
        Screen.print(1, "Can't write stuff");
        return;
    }

    // Read back from zone 7 - read back the entire buffer into a
    // buffer one character longer so there is always a null terminator
    uint8_t read_stuff[STSAFE_ZONE_7_SIZE + 1] = {'\0'};
    ret = eeprom.read(read_stuff, STSAFE_ZONE_7_SIZE, 0x00, STSAFE_ZONE_7_IDX);

    // ret is the count of bytes read.
    // A negative number means an error, zero means nothing read
    if (ret < 0)
    {
        Screen.print(1, "Can't read stuff");
        return;
    }
    else if (ret == 0)
    {
        Screen.print(1, "stuff should not be empty");
        return;
    }
    else
    {
        Screen.print(1, (char*)read_stuff);
    }
    
}

void loop() {
  // put your main code here, to run repeatedly:

}
