#include <Wire.h>
#include <DS3231.h>

DS3231 rtc; // Biblioteca DS3231 by Andrey — sem parâmetros

// Pinos do display
int segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13};
// Pinos dos dígitos
int digitPins[] = {2, 3, 4, 5};

// Segmentos para números 0–9
byte numbers[10][8] = {
  {1,1,1,1,1,1,0,0}, // 0
  {0,1,1,0,0,0,0,0}, // 1
  {1,1,0,1,1,0,1,0}, // 2
  {1,1,1,1,0,0,1,0}, // 3
  {0,1,1,0,0,1,1,0}, // 4
  {1,0,1,1,0,1,1,0}, // 5
  {1,0,1,1,1,1,1,0}, // 6
  {1,1,1,0,0,0,0,0}, // 7
  {1,1,1,1,1,1,1,0}, // 8
  {1,1,1,1,0,1,1,0}  // 9
};

int tempo_restante = 60;  // 1 minuto
unsigned long ultimo_tempo = 0;

void setup() {
  Wire.begin();  // Inicializa I2C
  Serial.begin(9600);

  for (int i = 0; i < 8; i++) pinMode(segmentPins[i], OUTPUT);
  for (int i = 0; i < 4; i++) pinMode(digitPins[i], OUTPUT);

  Serial.println("Cronômetro regressivo iniciado (01:00)");
}

void loop() {
  if (millis() - ultimo_tempo >= 1000 && tempo_restante >= 0) {
    ultimo_tempo = millis();

    int minutos = tempo_restante / 60;
    int segundos = tempo_restante % 60;

    Serial.print("Tempo restante: ");
    Serial.print(minutos);
    Serial.print(":");
    if (segundos < 10) Serial.print("0");
    Serial.println(segundos);

    int numero = minutos * 100 + segundos;
    mostrarNumero(numero);

    tempo_restante--;

    if (tempo_restante < 0) {
      Serial.println("⏰ Tempo finalizado!");
      while (true) {
        piscarDisplay();
      }
    }
  }

  int minutos = tempo_restante / 60;
  int segundos = tempo_restante % 60;
  int numero = minutos * 100 + segundos;
  mostrarNumero(numero);
}

void mostrarNumero(int num) {
  int d[4];
  d[0] = (num / 1000) % 10;
  d[1] = (num / 100) % 10;
  d[2] = (num / 10) % 10;
  d[3] = num % 10;

  for (int i = 0; i < 4; i++) {
    acenderDigito(i);
    mostrarDigito(d[i]);
    delay(3);
    apagarDigito(i);
  }
}

void acenderDigito(int i) {
  for (int j = 0; j < 4; j++)
    digitalWrite(digitPins[j], HIGH);
  digitalWrite(digitPins[i], LOW);
}

void apagarDigito(int i) {
  digitalWrite(digitPins[i], HIGH);
}

void mostrarDigito(int n) {
  for (int i = 0; i < 8; i++)
    digitalWrite(segmentPins[i], numbers[n][i] ? HIGH : LOW);
}

void piscarDisplay() {
  for (int k = 0; k < 2; k++) {
    for (int i = 0; i < 4; i++) digitalWrite(digitPins[i], HIGH);
    delay(300);
    for (int i = 0; i < 4; i++) digitalWrite(digitPins[i], LOW);
    delay(300);
  }
}
