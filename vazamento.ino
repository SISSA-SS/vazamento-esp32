#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

#define DHTPIN 15
#define DHTTYPE DHT22
#define BUZZER_PIN 2

DHT dht(DHTPIN, DHTTYPE);

const int umidade_limite = 70;

void setup_wifi() {
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.print("Conectando ao WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado com sucesso! ");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao servidor MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falha (rc=");
      Serial.print(client.state());
      Serial.println("). Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Erro ao ler o sensor DHT!");
    return;
  }

  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.print(" % | Temperatura: ");
  Serial.print(t);
  Serial.println(" °C");

  // Converte valores para string e publica em tópicos separados
  char tempString[8];
  char humString[8];

  dtostrf(t, 1, 2, tempString);
  dtostrf(h, 1, 2, humString);

  client.publish("cicera/temperatura", tempString);
  client.publish("cicera/umidade", humString);

  if (h > umidade_limite) {
    Serial.println(">> ALERTA: Vazamento detectado!");
    digitalWrite(BUZZER_PIN, HIGH);
    client.publish("cicera/vazamento", "Alerta: Vazamento detectado!");
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(2000);
}
