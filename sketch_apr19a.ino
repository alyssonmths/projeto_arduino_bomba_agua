#include <SoftwareSerial.h>

#define sensorBoia 2
#define sensorNivel A5
#define buzzer 6

#define rele 8

#define ledVermelho 11
#define ledAzul 12
#define ledVerde 13

bool releLigado = false;
bool bloqueado = false;

unsigned long inicioBuzzer = 0;

bool notificouCheio = false;
bool notificouBaixo = false;

// controle de envio (EVITA TRAVAMENTO)
bool enviando = false;

SoftwareSerial esp(9, 10);

// -------------------- SETUP --------------------
void setup() {

  setPinModes();

  Serial.begin(9600);
  esp.begin(9600);

  Serial.println("Iniciando ESP8266...");
  conectarWifi();
}

// -------------------- LOOP --------------------
void loop() {

  lerESP();

  int valorBoia = digitalRead(sensorBoia);
  int valorNivel = analogRead(sensorNivel);

  Serial.print("Boia: ");
  Serial.print(valorBoia);
  Serial.print(" | Nivel: ");
  Serial.println(valorNivel);

  // nível cheio
  if (valorBoia == LOW && valorNivel >= 500) {

    releLigado = false;
    bloqueado = true;

    tocarBuzzer();

    if (!notificouCheio && !enviando) {
      sendMessage(
        "+5581995985705",
        "✅🚿 Nível cheio de água! Bomba desligada.",
        15);

      notificouCheio = true;
      notificouBaixo = false;
    }
  } else {
    noTone(buzzer);
    inicioBuzzer = 0;
  }

  if (valorBoia == HIGH) {
    bloqueado = false;
  }

  // nível baixo
  if (!bloqueado) {

    if ((valorBoia == HIGH && valorNivel < 100) || (valorBoia == LOW && valorNivel < 500)) {

      releLigado = true;

      if (!notificouBaixo && !enviando) {
        sendMessage(
          "+5581995985705",
          "⚠️🚿 Nível baixo de água! Bomba ligada.",
          15);

        notificouBaixo = true;
        notificouCheio = false;
      }
    }
  }

  atualizarLeds(valorBoia, valorNivel);

  digitalWrite(rele, releLigado ? LOW : HIGH);

  delay(200);
}

void lerESP() {
  while (esp.available()) {
    Serial.write(esp.read());
  }
}

// -------------------- BUZZER --------------------
void tocarBuzzer() {
  if (inicioBuzzer == 0) {
    inicioBuzzer = millis();
    tone(buzzer, 1000);
  }

  if (millis() - inicioBuzzer >= 5000) {
    noTone(buzzer);
  }
}

// -------------------- LEDS --------------------
void atualizarLeds(int valorBoia, int valorNivel) {

  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAzul, LOW);
  digitalWrite(ledVermelho, LOW);

  if (valorBoia == LOW && valorNivel >= 400) {
    digitalWrite(ledVerde, HIGH);
  } else if (valorBoia == LOW && valorNivel < 400) {
    digitalWrite(ledAzul, HIGH);
  } else if (valorBoia == HIGH && valorNivel < 100) {
    digitalWrite(ledVermelho, HIGH);
  }
}

// -------------------- PINOS --------------------
void setPinModes() {

  pinMode(sensorBoia, INPUT);
  pinMode(sensorNivel, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(rele, OUTPUT);

  pinMode(ledVermelho, OUTPUT);
  pinMode(ledAzul, OUTPUT);
  pinMode(ledVerde, OUTPUT);
}

// -------------------- WIFI --------------------
void conectarWifi() {

  sendCommand("AT");
  delay(200);

  sendCommand("AT+CWMODE=1");
  delay(200);

  sendCommand("AT+CWJAP=\"Maria_Alyce\",\"03111980\"");
  delay(5000);

  sendCommand("AT+CIFSR");
}

void sendCommand(String cmd) {
  esp.println(cmd);
}

// -------------------- ENVIO DE MENSAGEM (CONTROLADO) --------------------
void sendMessage(String phone, String message, int delayMessage) {

  if (enviando) return;
  enviando = true;

  String host = "192.168.0.103";
  int port = 3000;

  String body = "{";
  body += "\"phone\":\"" + phone + "\",";
  body += "\"message\":\"" + message + "\",";
  body += "\"delayMessage\":" + String(delayMessage);
  body += "}";

  // conecta TCP
  esp.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + String(port));
  delay(1000);

  String post = "";
  post += "POST /send HTTP/1.1\r\n";
  post += "Host: " + host + "\r\n";
  post += "Content-Type: application/json\r\n";
  post += "Content-Length: " + String(body.length()) + "\r\n";
  post += "Connection: close\r\n\r\n";
  post += body;

  esp.println("AT+CIPSEND=" + String(post.length()));
  delay(500);

  esp.print(post);

  delay(1000);

  enviando = false;

  Serial.println("✔ Requisição enviada");
}