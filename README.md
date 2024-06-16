# Chytrá lampa z Narnie 

Táborová rekvizita - lampa z Narnie.
ESP32 a kousek neopixel led pásku.
Ovládá se přes WiFi.

Umí následující kouzla:
* hořící oheň (nastavitelný jas)
* stálé svícení (nastavitelná barva a jas)
* vysílání morseovky (nastavitelná barva, jas a rychlost)

Předpokládá napájení z lithiového článku, měří napětí akumulátoru a pokud je pod limitem, zhasne led pásek. Pokud je pod ještě nižším limitem, vypne i ESP.
(Pokud není na analogový vstup nic připojeno, běží bez problémů.)
