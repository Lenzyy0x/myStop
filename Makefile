# Kurze Befehle für das ESP-IDF-Projekt unter ./firmware.
# Von einem anderen Verzeichnis aus: make -C "/pfad/zu/myStop" <target>.

.DEFAULT_GOAL := help

# Make-Variable CURDIR funktioniert auch mit `make -C`.
ROOT_DIR := $(CURDIR)

# Diese Werte können bei Bedarf auf der Kommandozeile überschrieben werden:
IDF_PY ?= idf.py
FIRMWARE_DIR ?= $(ROOT_DIR)/firmware
BUILD_DIR ?= $(FIRMWARE_DIR)/build
TEST_BUILD_DIR ?= $(FIRMWARE_DIR)/build-tests
TARGET ?= esp32c3
PORT ?=

# Gemeinsame IDF-Argumente. Beide Build-Ordner verwenden firmware/sdkconfig,
# außer SDKCONFIG wird in der Umgebung oder auf der Kommandozeile überschrieben.
IDF_PROJECT_ARGS = -C "$(FIRMWARE_DIR)"
IDF_PORT_ARGS = $(if $(strip $(PORT)),-p "$(PORT)",)
IDF_EXTRA_ARGS ?=

# Das Test-Flag muss bei jedem Test-Befehl übergeben werden, nicht nur bei
# test-build, da CMake es im Test-Build-Ordner separat zwischenspeichert.
TEST_CMAKE_ARGS = -D MYSTOP_RUN_STORAGE_TESTS=ON

.PHONY: help check-idf \
	build flash monitor flash-monitor erase-flash size size-components \
	test-build test-flash test-monitor test-flash-monitor test \
	set-target menuconfig clean test-clean cleanall

# Zeigt verfügbare Ziele an.
help:
	@printf '%s\n' \
		'MyStop-ESP-IDF-Befehle (außerhalb des Projekts: make -C /pfad/zu/myStop <ziel>):' \
		'' \
		'  make build                  Firmware normal bauen' \
		'  make flash                  Normale Firmware bauen und flashen' \
		'  make monitor                Seriellen Monitor der normalen Firmware öffnen' \
		'  make flash-monitor          Normale Firmware flashen und Monitor öffnen' \
		'  make test-build             Unity-Test-Firmware bauen' \
		'  make test-flash             Unity-Test-Firmware bauen und flashen' \
		'  make test-monitor           Seriellen Monitor der Test-Firmware öffnen' \
		'  make test                   Tests bauen, flashen und im Monitor anzeigen' \
		'  make erase-flash            Flash des verbundenen Geräts löschen' \
		'  make set-target             TARGET einstellen (Standard: esp32c3)' \
		'  make menuconfig             Gemeinsame Projekt-sdconfig konfigurieren' \
		'  make clean                  Normalen Build-Ordner bereinigen' \
		'  make test-clean             Test-Build-Ordner bereinigen' \
		'  make cleanall               Beide Build-Ordner bereinigen' \
		'' \
		'Anpassbare Variablen:' \
		'  PORT=/dev/ttyUSB0            Seriellen Port wählen; leer lassen für automatische Erkennung' \
		'  TARGET=esp32c3               ESP-IDF-Zielchip auswählen' \
		'  IDF_PY=idf.py                ESP-IDF-Befehl festlegen' \
		'  BUILD_DIR=/pfad/zum/build    Normalen Build-Ordner überschreiben' \
		'  TEST_BUILD_DIR=/pfad/zum/test Test-Build-Ordner überschreiben' \
		'  IDF_EXTRA_ARGS="..."          Zusätzliche Argumente an idf.py übergeben'

# Bricht mit einem Hinweis ab, wenn ESP-IDF nicht installiert oder initialisiert ist.
check-idf:
	@command -v "$(IDF_PY)" >/dev/null 2>&1 || { \
		printf '%s\n' "'$(IDF_PY)' wurde nicht gefunden. ESP-IDF initialisieren oder IDF_PY setzen." >&2; \
		exit 127; \
	}

# Ziele für die normale Firmware:
build: check-idf
	$(IDF_PY) $(IDF_PROJECT_ARGS) -B "$(BUILD_DIR)" $(IDF_EXTRA_ARGS) build

flash: check-idf
	$(IDF_PY) $(IDF_PROJECT_ARGS) -B "$(BUILD_DIR)" $(IDF_EXTRA_ARGS) $(IDF_PORT_ARGS) flash

monitor: check-idf
	$(IDF_PY) $(IDF_PROJECT_ARGS) -B "$(BUILD_DIR)" $(IDF_EXTRA_ARGS) $(IDF_PORT_ARGS) monitor

flash-monitor: check-idf
	$(IDF_PY) $(IDF_PROJECT_ARGS) -B "$(BUILD_DIR)" $(IDF_EXTRA_ARGS) $(IDF_PORT_ARGS) flash monitor
erase-flash: check-idf
	$(IDF_PY) $(IDF_PROJECT_ARGS) -B "$(BUILD_DIR)" $(IDF_EXTRA_ARGS) $(IDF_PORT_ARGS) erase-flash

size: check-idf
	$(IDF_PY) $(IDF_PROJECT_ARGS) -B "$(BUILD_DIR)" $(IDF_EXTRA_ARGS) size

size-components: check-idf
	$(IDF_PY) $(IDF_PROJECT_ARGS) -B "$(BUILD_DIR)" $(IDF_EXTRA_ARGS) size-components

# Unity-Tests laufen auf dem ESP32. CMake-Zwischendateien und generierte Dateien
# liegen in einem eigenen Build-Ordner; das Test-Flag gilt für jeden Test-Befehl.
test-build: check-idf
	$(IDF_PY) $(IDF_PROJECT_ARGS) -B "$(TEST_BUILD_DIR)" $(TEST_CMAKE_ARGS) $(IDF_EXTRA_ARGS) build

test-flash: check-idf
	$(IDF_PY) $(IDF_PROJECT_ARGS) -B "$(TEST_BUILD_DIR)" $(TEST_CMAKE_ARGS) $(IDF_EXTRA_ARGS) $(IDF_PORT_ARGS) flash

test-monitor: check-idf
	$(IDF_PY) $(IDF_PROJECT_ARGS) -B "$(TEST_BUILD_DIR)" $(TEST_CMAKE_ARGS) $(IDF_EXTRA_ARGS) $(IDF_PORT_ARGS) monitor

test-flash-monitor: check-idf
	$(IDF_PY) $(IDF_PROJECT_ARGS) -B "$(TEST_BUILD_DIR)" $(TEST_CMAKE_ARGS) $(IDF_EXTRA_ARGS) $(IDF_PORT_ARGS) flash monitor

# Die Tests starten nach dem Boot automatisch. Dieses Ziel baut und flasht sie
# und zeigt anschließend das Unity-Ergebnis im seriellen Monitor.
test: test-flash-monitor

# Ein Zielwechsel kann sdkconfig neu erzeugen. ESP-IDF sichert die vorherige
# Konfiguration gegebenenfalls als sdkconfig.old; danach Board und Flash prüfen.
set-target: check-idf
	$(IDF_PY) $(IDF_PROJECT_ARGS) -B "$(BUILD_DIR)" $(IDF_EXTRA_ARGS) set-target "$(TARGET)"

# menuconfig bearbeitet firmware/sdkconfig, die von beiden Build-Ordnern genutzt wird.
menuconfig: check-idf
	$(IDF_PY) $(IDF_PROJECT_ARGS) -B "$(BUILD_DIR)" $(IDF_EXTRA_ARGS) menuconfig

# Diese Bereinigungsziele entfernen nur generierte Build-Dateien. Sie löschen
# weder den Flash des Boards noch absichtlich die Projektdatei sdkconfig.
clean: check-idf
	$(IDF_PY) $(IDF_PROJECT_ARGS) -B "$(BUILD_DIR)" $(IDF_EXTRA_ARGS) clean

test-clean: check-idf
	$(IDF_PY) $(IDF_PROJECT_ARGS) -B "$(TEST_BUILD_DIR)" $(TEST_CMAKE_ARGS) $(IDF_EXTRA_ARGS) clean

# Bereinigt zuerst den normalen und danach den Test-Build-Ordner.
# Die Aufrufe laufen nacheinander und nicht parallel.
cleanall:
	+$(MAKE) --no-print-directory clean
	+$(MAKE) --no-print-directory test-clean