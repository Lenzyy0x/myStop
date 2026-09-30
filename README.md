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

`-B` legt den Build-Ordner fest, damit Test-Builds vom normalen Build getrennt sind
`-D` übergibt CMake Variable zum Aktivieren der Tests

`/dev/ttyUSB0` durch den seriellen Port des Boards ersetzen (oder einfach weglassen falls man nur einen Mikrocontroller angeschlossen hat). Die Tests starten automatisch und das Ergebnis erscheint im Monitor. Die normale Firmware wird weiterhin mit `idf.py build flash monitor` gebaut.

### Firmware-Konfiguration

`firmware/sdkconfig.defaults` enthält die Projekt-Defaults: 4 MB Flash, die ESP-IDF-Partitionstabelle mit zwei OTA-Slots und den Unity-Test-Runner. ESP-IDF liest diese Datei automatisch ein, weil sie neben der Firmware-`CMakeLists.txt` liegt. Eine neu erzeugte `sdkconfig` wird daraus initialisiert; eine bereits vorhandene `sdkconfig` bleibt maßgeblich und wird durch Änderungen an den Defaults nicht automatisch überschrieben. Auch mit verschiedenen `-B`-Build-Ordnern wird standardmäßig dieselbe `firmware/sdkconfig` verwendet; `-B` trennt nur generierte Build-Dateien. Für einen frischen Build zuerst im Verzeichnis `firmware` das Ziel setzen und dann bauen:

```sh
cd firmware
idf.py -B build-defaults set-target esp32c3
idf.py -B build-defaults build
```

Die lokale `sdkconfig` und Build-Ordner sind generiert und nicht versioniert. Darum liegen die gemeinsam gewünschten Ausgangswerte in `sdkconfig.defaults`.

## TODO / Roadmap

### Stabilität und Grundlage

- Storage-Tests für Schreiben, Lesen, Löschen, fehlende Schlüssel, zu kleine Puffer und Neustart/Persistenz ergänzen; Fehler beim Test-Setup und -Cleanup prüfen.
- [ ] ESP-IDF-Version und Hardware-Flashgröße dokumentieren sowie sicherstellen, dass die OTA-Partitionierung zum tatsächlich verwendeten Board passt. Die Projekt-Defaults für 4 MB Flash und zwei OTA-Slots liegen in `firmware/sdkconfig.defaults`.

### Produktfunktion

- Anforderungen und Datenquelle für Abfahrtszeiten festlegen; Netzwerkverbindung, API-Aufruf, JSON-Auswertung, Zeitüberschreitungen und Fehlerzustände implementieren.
- Netzwerk-Provisionierung integrieren und testen; die deklarierte `network_provisioning`-Abhängigkeit ist derzeit noch nicht Teil des Anwendungspfads.
- eInk-Display und Hardwaretreiber anbinden, inklusive Aktualisierungsintervall und Darstellung bei fehlenden oder veralteten Daten.
- Simulator für dieselben Daten- und Darstellungszustände wie die Firmware aufbauen.
