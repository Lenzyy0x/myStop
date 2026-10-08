void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(2000);

  Serial.println("ESP32-C3 Mini gestartet!");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Halöle");
  delay(1000);
}
