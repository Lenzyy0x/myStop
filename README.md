Dieses Projekt nutzt einen ESP32 um Wartezeiten für Transportmittel einer beliebigen Haltestelle vom ÖPNV auf einem eInk-Display darzustellen für den Nutzen von zu Hause.


Das Projekt kann mit esp-idf.py im myStop/firmware Verzeichnis ausgeführt werden (idf.py build flash monitor erase-flash ...)

Später soll es aber mit OTA - (Over the air) Updates realisiert werden. Zum Beispiel mithilfe von GitHub Actions oder Polling vom ESP.

## Storage-Tests

Die Storage-Tests laufen auf dem ESP32. Das kommentierte Root-`Makefile` bietet kurze Befehle. Führe sie im Projekt-Root aus; von einem anderen Verzeichnis kannst du Make mit `-C` auf das Projekt zeigen lassen. Voraussetzung ist eine initialisierte ESP-IDF-Umgebung, sodass `idf.py` im `PATH` liegt, sowie GNU Make.

```sh
make help
make test-build
make test PORT=/dev/ttyUSB0
```

Von außerhalb des Projektverzeichnisses zum Beispiel:

```sh
make -C "/pfad/zu/myStop" test PORT=/dev/ttyUSB0
```

`make test` baut und flasht die Test-Firmware und öffnet danach den seriellen Monitor. Die Tests starten automatisch. `PORT` ist optional; ohne Angabe versucht ESP-IDF den angeschlossenen Port selbst zu erkennen. Alternativ lassen sich Flashen und Monitor getrennt aufrufen: `make test-flash PORT=/dev/ttyUSB0` und `make test-monitor PORT=/dev/ttyUSB0`.

Die normale Firmware lässt sich mit `make build` bauen oder mit `make flash-monitor PORT=/dev/ttyUSB0` flashen und überwachen. `make erase-flash PORT=...` löscht den Flash des Boards einschließlich NVS-Daten und sollte nur bewusst verwendet werden.

### Firmware-Konfiguration

`firmware/sdkconfig.defaults` enthält die Projekt-Defaults: 4 MB Flash, die ESP-IDF-Partitionstabelle mit zwei OTA-Slots und den Unity-Test-Runner. ESP-IDF liest diese Datei automatisch ein, weil sie neben der Firmware-`CMakeLists.txt` liegt. Eine neu erzeugte `sdkconfig` wird daraus initialisiert; eine bereits vorhandene `sdkconfig` bleibt maßgeblich und wird durch Änderungen an den Defaults nicht automatisch überschrieben. Auch die getrennten Build-Ordner `build` und `build-tests` verwenden standardmäßig dieselbe `firmware/sdkconfig`; sie trennen nur generierte Build-Dateien. Das Ziel-Board kann über das Makefile gesetzt werden:

```sh
make set-target TARGET=esp32c3
make build
```

`make menuconfig` bearbeitet dieselbe `sdkconfig`. Ein Wechsel des Ziels kann die Konfiguration neu erzeugen; danach sollten Board- und Flash-Einstellungen geprüft werden. Die lokale `sdkconfig` und Build-Ordner sind generiert und nicht versioniert. Darum liegen die gemeinsam gewünschten Ausgangswerte in `firmware/sdkconfig.defaults`.

## TODO / Roadmap

### Stabilität und Grundlage

- Storage-Tests für Schreiben, Lesen, Löschen, fehlende Schlüssel, zu kleine Puffer und Neustart/Persistenz ergänzen; Fehler beim Test-Setup und -Cleanup prüfen.
- [ ] ESP-IDF-Version und Hardware-Flashgröße dokumentieren sowie sicherstellen, dass die OTA-Partitionierung zum tatsächlich verwendeten Board passt. Die Projekt-Defaults für 4 MB Flash und zwei OTA-Slots liegen in `firmware/sdkconfig.defaults`.

### Produktfunktion

- Anforderungen und Datenquelle für Abfahrtszeiten festlegen; Netzwerkverbindung, API-Aufruf, JSON-Auswertung, Zeitüberschreitungen und Fehlerzustände implementieren.
- Netzwerk-Provisionierung integrieren und testen; die deklarierte `network_provisioning`-Abhängigkeit ist derzeit noch nicht Teil des Anwendungspfads.
- eInk-Display und Hardwaretreiber anbinden, inklusive Aktualisierungsintervall und Darstellung bei fehlenden oder veralteten Daten.
- Simulator für dieselben Daten- und Darstellungszustände wie die Firmware aufbauen.
