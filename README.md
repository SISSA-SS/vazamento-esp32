# 💧 Projeto: Monitoramento de Vazamento com ESP32

Este projeto utiliza um ESP32 com sensor DHT22 e buzzer para detectar aumento de umidade (possível vazamento) e enviar alertas via protocolo MQTT. A simulação é feita no Wokwi.

## 🚀 Funcionalidades
- Leitura periódica da umidade do ar com sensor DHT22
- Alerta sonoro com buzzer se umidade > 70%
- Publicação de mensagens no tópico MQTT `cicera/vazamento`
  - Inclui a umidade atual nas mensagens
- Conexão com broker MQTT público

## 🧩 Componentes Utilizados
- ESP32
- Sensor DHT22
- Buzzer
- Conexão Wi-Fi (via simulação no Wokwi)

## 🔧 Como Usar
1. Carregue o código no ESP32 ou rode no Wokwi.
2. Abra o monitor serial para ver as leituras.
3. Use um cliente MQTT (ex: MQTT Explorer) para visualizar as mensagens.

## 🛰️ Broker MQTT
- **Broker:** `test.mosquitto.org`
- **Porta:** `1883`
- **Tópico:** `cicera/vazamento`

## 🧪 Demonstração
🔗 [Link para o vídeo de demonstração](coloque aqui quando gravar novamente mostrando o MQTT funcionando)

## 👩‍💻 Autor
Projeto acadêmico para a disciplina de Sistemas Embarcados.


## Demonstração

[[Link para o vídeo de demonstração]([coloque aqui quando gravar de novo mostrando o MQTT funcionando)](https://youtu.be/N20J7lxckE8](https://youtu.be/N20J7lxckE8))
