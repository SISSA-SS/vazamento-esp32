#include <WiFi.h>             // Biblioteca para conexão Wi-Fi
#include <PubSubClient.h>     // Biblioteca para comunicação MQTT
#include <DHT.h>              // Biblioteca para sensor DHT

// Credenciais da rede Wi-Fi (simulada no Wokwi)
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Configuração do servidor MQTT público
const char* mqtt_server = "test.mosquitto.org";
const char* topic = "cicera/vazamento";  // Tópico para envio das mensagens

// Objetos de conexão
WiFiClient espClient;
PubSubClient client(espClient);

// Pinos do sensor e buzzer
#define DHTPIN 15            // Pino do sensor DHT
#define BUZZER_PIN 2         // Pino do buzzer
#define DHTTYPE DHT22        // Tipo de sensor (DHT11 ou DHT22)

// Criação do objeto do sensor
DHT dht(DHTPIN, DHTTYPE);

// Limiar de umidade para acionar o alerta
int umidade_limite = 70;

void setup_wifi() {
  delay(10);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Conectando ao WiFi ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado ao servidor MQTT!");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);           // Define o buzzer como saída
  digitalWrite(BUZZER_PIN, LOW);         // Inicia com buzzer desligado
  dht.begin();                           // Inicializa o sensor DHT
  setup_wifi();                          // Conecta ao Wi-Fi
  client.setServer(mqtt_server, 1883);   // Define o servidor MQTT
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float h = dht.readHumidity();  // Lê a umidade

  if (isnan(h)) {
    Serial.println("Falha na leitura do sensor DHT!");
    return;
  }

  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.println(" %");

  char mensagem[50];  // Buffer para mensagem MQTT

  if (h > umidade_limite) {
    Serial.println("Vazamento detectado!");
    digitalWrite(BUZZER_PIN, HIGH);
    snprintf(mensagem, sizeof(mensagem), "Alerta: Vazamento! Umidade: %.2f %%", h);
    client.publish(topic, mensagem);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    snprintf(mensagem, sizeof(mensagem), "Umidade normal: %.2f %%", h);
    client.publish(topic, mensagem);
  }

  delay(2000);  // Aguarda 2 segundos para nova leitura
}




