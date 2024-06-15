#include "platform.h"

#if defined(ESP32)

  #include <ESP.h>
  #include "WiFi.h" 
  #include "driver/adc.h"
  #include <esp_wifi.h>
  #ifdef X_ESP32_BT_PRESENT
    #include <esp_bt.h>
  #endif
  #include <Arduino.h>

  // https://www.savjee.be/2019/12/esp32-tips-to-increase-battery-life/
  void ra__DeepSleep( long usec )
  {
      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
  #ifdef X_ESP32_BT_PRESENT
      btStop();
  #endif
    
      #if ESP_ARDUINO_VERSION_MAJOR == 1 
        adc_power_off();
      #endif
      
  #ifdef X_ESP32_BT_PRESENT
      esp_bt_controller_disable();
  #endif
      
      esp_sleep_enable_timer_wakeup( usec );
      esp_deep_sleep_start();
  }
  
  // https://rweather.github.io/arduinolibs/RNG_8cpp_source.html
  int trng() {
    return esp_random();
  }

#endif


#if defined(ESP8266)

  #include <ESP8266WiFi.h>
  
  void ra__DeepSleep( long usec )
  {
      ESP.deepSleep( usec );
  }
  
  /**
      https://github.com/CSSHL/ESP8266-Arduino-cryptolibs
      
      Simple access to ESP8266's TRNG as documented on
      http://esp8266-re.foogod.com/wiki/Random_Number_Generator
      
      To use, just call trng() from your code: you'll get 4 completely random bytes.    
  */ 
  int trng() {
    return *((volatile int*)0x3FF20E44);
  }
  
#endif

