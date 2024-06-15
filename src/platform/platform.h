#ifndef RA_PLATFORM_H
#define RA_PLATFORM_H

#include <Arduino.h>

void ra__DeepSleep( long usec );
int trng();

#ifdef ESP8266
    #define ESP_ISR_RAM ICACHE_RAM_ATTR
#endif
#ifdef ESP32    
    #define ESP_ISR_RAM IRAM_ATTR
#endif    

#ifdef ESP32

    #ifdef CONFIG_IDF_TARGET_ESP32S2
	#define X_ESP32_BT_NOT_PRESENT
    #else
	#define X_ESP32_BT_PRESENT
    #endif

#endif

#endif