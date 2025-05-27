#include <WiFi.h>             // Biblioteca para conexão Wi-Fi
#include <PubSubClient.h>     // Biblioteca para comunicação MQTT
#include <DHT.h>              // Biblioteca para sensor de temperatura/umidade

// Informações da rede Wi-Fi (simulação Wokwi)
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Configurações do servidor MQTT público
const char* mqtt_server = "test.mosquitto.org";
const char* topic = "cicera/vazamento"; // Tópico onde será publicado o alerta

// Objetos para conexão Wi-Fi e MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Definições dos pinos conectados
#define DHTPIN 15          // Pino onde o sensor DHT está ligado
#define BUZZER_PIN 2       // Pino onde o buzzer está ligado
#define DHTTYPE DHT22      // Tipo do sensor (DHT11 ou DHT22)

// Criação do objeto do sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Limiar de umidade para considerar como vazamento
int umidade_limite = 70;

void setup_wifi() {
  delay(10);
  Serial.begin(115200);  // Inicializa o monitor serial
  Serial.println();
  Serial.print("Conectando ao WiFi ");
  Serial.println(ssid);

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP()); // Mostra IP atribuído
}

// Reconecta ao servidor MQTT caso a conexão caia
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
  pinMode(BUZZER_PIN, OUTPUT);       // Define o pino do buzzer como saída
  digitalWrite(BUZZER_PIN, LOW);     // Inicia o buzzer desligado
  dht.begin();                       // Inicializa o sensor DHT
  setup_wifi();                      // Conecta ao Wi-Fi
  client.setServer(mqtt_server, 1883); // Define o servidor MQTT e porta
}

void loop() {
  // Verifica conexão com o servidor MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Lê a umidade do ar
  float h = dht.readHumidity();

  // Verifica se a leitura foi bem-sucedida
  if (isnan(h)) {
    Serial.println("Falha na leitura do sensor DHT!");
    return;
  }

  // Mostra o valor da umidade no monitor serial
  Serial.print("Umidade: ");
  Serial.print(h);
  Serial.println(" %");

  // Se a umidade passar do limite, aciona o alerta
  if (h > umidade_limite) {
    Serial.println("Vazamento detectado!");
    digitalWrite(BUZZER_PIN, HIGH); // Liga o buzzer
    client.publish(topic, "Alerta: Vazamento detectado!"); // Publica no MQTT
  } else {
    digitalWrite(BUZZER_PIN, LOW); // Desliga o buzzer
  }

  delay(2000); // Aguarda 2 segundos para nova leitura
}







Adicionando código do projeto com ESP32
