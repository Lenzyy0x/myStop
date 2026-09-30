Dieses Projekt nutzt einen ESP32 um Wartezeiten für Transportmittel einer beliebigen Haltestelle vom ÖPNV auf einem eInk-Display darzustellen für den Nutzen von zu Hause.


Das Projekt kann mit esp-idf.py im myStop/firmware Verzeichnis ausgeführt werden (idf.py build flash monitor erase-flash ...)

Später soll es aber mit OTA - (Over the air) Updates realisiert werden. Zum Beispiel mithilfe von GitHub Actions oder Polling vom ESP.

## Storage-Tests

Die Storage-Tests laufen auf dem ESP32. Im Firmware-Verzeichnis einen separaten Test-Build erstellen und flashen:

```sh
cd firmware
idf.py -B build-tests -D MYSTOP_RUN_STORAGE_TESTS=ON build
idf.py -B build-tests -p /dev/ttyUSB0 flash monitor
```

`/dev/ttyUSB0` durch den seriellen Port des Boards ersetzen (oder einfach weglassen falls man nur einen Mikrocontroller angeschlossen hat). Die Tests starten automatisch und das Ergebnis erscheint im Monitor. Die normale Firmware wird weiterhin mit `idf.py build flash monitor` gebaut.
