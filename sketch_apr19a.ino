#define sensorBoia 4
#define sensorNivel A4
#define rele 8

#define verde 13
#define azul 12
#define vermelho 11

bool releLigado = false;
bool bloqueado = false;

void setup() {
  pinMode(sensorBoia, INPUT);
  pinMode(sensorNivel, INPUT);
  pinMode(rele, OUTPUT);

  pinMode(verde, OUTPUT);
  pinMode(azul, OUTPUT);
  pinMode(vermelho, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  int valorBoia = digitalRead(sensorBoia);
  int valorNivel = analogRead(sensorNivel);

  Serial.print("Boia: ");
  Serial.print(valorBoia);
  Serial.print(" | Nivel: ");
  Serial.println(valorNivel);

  // 🔴 REGRA 1: nível cheio → desliga e BLOQUEIA
  if (valorBoia == LOW && valorNivel >= 400) {
    releLigado = false;
    bloqueado = true;
  }

  // 🟢 REGRA 2: desbloqueia somente quando boia HIGH
  if (valorBoia == HIGH) {
    bloqueado = false;
  }

  // 🟡 REGRA 3: só liga se NÃO estiver bloqueado
  if (!bloqueado) {

    if (valorBoia == HIGH && valorNivel < 50) {
      releLigado = true;
    } else if (valorBoia == LOW && valorNivel < 400) {
      releLigado = true;
    }
  }

  // chama função dos LEDs
  atualizarLeds(valorBoia, valorNivel);

  digitalWrite(rele, releLigado ? LOW : HIGH);

  delay(500);
}

// =========================
// 🎨 FUNÇÃO DOS LEDs
// =========================
void atualizarLeds(int valorBoia, int valorNivel) {

  // desliga todos
  digitalWrite(verde, LOW);
  digitalWrite(azul, LOW);
  digitalWrite(vermelho, LOW);

  // define estado
  if (valorBoia == LOW && valorNivel >= 400) {
    digitalWrite(verde, HIGH);  // cheio
  } else if (valorBoia == LOW && valorNivel < 400) {
    digitalWrite(azul, HIGH);  // intermediário
  } else if (valorBoia == HIGH && valorNivel < 50) {
    digitalWrite(vermelho, HIGH);  // baixo
  }
}