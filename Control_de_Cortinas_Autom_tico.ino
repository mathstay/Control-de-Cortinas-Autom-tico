#include <Wire.h>
#include <RTClib.h>  // Librería para el módulo RTC

RTC_DS3231 rtc;  // Creamos el objeto para el reloj

// Pines
const int motorPin1 = 8; // Motor control pin 1
const int motorPin2 = 9; // Motor control pin 2
const int ldrPin = A0;   // Sensor de luz

// Configuración
const int lightThreshold = 500; // Umbral de luz (ajustar según pruebas)
const int openHour = 7;         // Hora para abrir (7:00 AM)
const int closeHour = 20;       // Hora para cerrar (8:00 PM)

void setup() {
  Serial.begin(9600);
  Wire.begin();
  rtc.begin();
  
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(ldrPin, INPUT);

  // Verifica si el RTC está corriendo
  if (!rtc.isrunning()) {
    Serial.println("RTC no está funcionando, configurando hora...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Ajusta al momento de compilación
  }
}

void loop() {
  DateTime now = rtc.now(); // Obtenemos la hora actual
  int lightLevel = analogRead(ldrPin); // Leemos el sensor de luz
  
  Serial.print("Hora actual: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.println(now.minute());
  
  Serial.print("Nivel de luz: ");
  Serial.println(lightLevel);
  
  // Control basado en hora
  if (now.hour() >= openHour && now.hour() < closeHour) {
    // Durante el día
    if (lightLevel > lightThreshold) {
      abrirCortinas();
    } else {
      cerrarCortinas();
    }
  } else {
    // De noche, cerramos cortinas
    cerrarCortinas();
  }

  delay(5000); // Espera 5 segundos antes de la próxima medición
}

// Funciones para controlar las cortinas
void abrirCortinas() {
  Serial.println("Abriendo cortinas...");
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  delay(3000); // Ajusta el tiempo que necesite el motor para abrir
  detenerMotor();
}

void cerrarCortinas() {
  Serial.println("Cerrando cortinas...");
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  delay(3000); // Ajusta el tiempo que necesite el motor para cerrar
  detenerMotor();
}

void detenerMotor() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
}
