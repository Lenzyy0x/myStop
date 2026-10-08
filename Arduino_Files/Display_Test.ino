#include <Arduino.h>
#include <SPI.h>
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold12pt7b.h>

// --------------------------------------------------
// ESP32-C3 <-> DESPI-C02
// --------------------------------------------------

#define EPD_SDI   3
#define EPD_SCK   4
#define EPD_CS    5
#define EPD_DC    6
#define EPD_RST   8
#define EPD_BUSY  10

// GDEY075T7:
// 800 x 480
// UC8179
GxEPD2_BW<GxEPD2_750_GDEY075T7,
          GxEPD2_750_GDEY075T7::HEIGHT>
display(
    GxEPD2_750_GDEY075T7(
        EPD_CS,
        EPD_DC,
        EPD_RST,
        EPD_BUSY
    )
);

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("================================");
    Serial.println("GDEY075T7 E-Paper Test");
    Serial.println("================================");

    // SPI mit deiner Pinbelegung starten
    SPI.begin(
        EPD_SCK,   // SCK
        -1,        // MISO - nicht benötigt
        EPD_SDI,   // MOSI / SDI
        EPD_CS
    );

    Serial.println("Initialisiere Display...");

    display.init(115200);

    Serial.println("Display initialisiert.");

    // --------------------------------------------------
    // Bildschirm komplett löschen
    // --------------------------------------------------

    Serial.println("Loesche Display...");

    display.setFullWindow();
    display.firstPage();

    do
    {
        display.fillScreen(GxEPD_WHITE);
    }
    while (display.nextPage());

    Serial.println("Display geloescht.");

    delay(1000);

    // --------------------------------------------------
    // Testbild
    // --------------------------------------------------

    Serial.println("Schreibe Testbild...");

    display.setFullWindow();
    display.firstPage();

    do
    {
        display.fillScreen(GxEPD_WHITE);

        // Titel
        display.setTextColor(GxEPD_BLACK);
        display.setFont(&FreeMonoBold12pt7b);

        display.setCursor(40, 60);
        display.println("myStop");

        display.setCursor(40, 95);
        display.println("E-Paper TEST");

        // Rahmen
        display.drawRect(20, 120, 760, 300, GxEPD_BLACK);

        // Linien
        display.drawLine(20, 180, 780, 180, GxEPD_BLACK);
        display.drawLine(20, 240, 780, 240, GxEPD_BLACK);
        display.drawLine(20, 300, 780, 300, GxEPD_BLACK);
        display.drawLine(20, 360, 780, 360, GxEPD_BLACK);

        // Testkreise
        display.drawCircle(700, 80, 25, GxEPD_BLACK);
        display.fillCircle(740, 80, 25, GxEPD_BLACK);

        // Text
        display.setCursor(40, 165);
        display.println("ESP32-C3");

        display.setCursor(40, 225);
        display.println("DESPI-C02");

        display.setCursor(40, 285);
        display.println("GDEY075T7");

        display.setCursor(40, 345);
        display.println("800 x 480");

        display.setCursor(40, 405);
        display.println("SPI OK");

    }
    while (display.nextPage());

    Serial.println("Testbild fertig.");

    // Display schlafen legen
    display.hibernate();

    Serial.println("Display in Sleep Mode.");
}

void loop()
{
    // nichts zu tun
}