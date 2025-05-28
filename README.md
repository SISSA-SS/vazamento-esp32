# Projeto: Monitoramento de Vazamento com ESP32

Este projeto utiliza um ESP32, sensor de umidade DHT22 e um buzzer para detectar vazamentos e enviar alertas via protocolo MQTT.

## Funcionalidades

- Leitura de umidade com o sensor DHT22
- Alerta sonoro com buzzer quando há vazamento (umidade > 70%)
- Publicação de mensagem no tópico MQTT cicera/vazamento
- Conexão com broker público: test.mosquitto.org

## Componentes Utilizados

- ESP32
- Sensor DHT22
- Buzzer
- Conexão Wi-Fi (simulação Wokwi)

## Como Usar

1. Carregue o código para o ESP32.
2. Verifique no monitor serial as leituras.
3. Use uma ferramenta como o MQTT Explorer para ver as mensagens no tópico cicera/vazamento.

## Broker MQTT

- Broker: test.mosquitto.org
- Porta: 1883
- Tópico: cicera/vazamento

## Demonstração

[Link para o vídeo de demonstração](coloque aqui quando gravar de novo mostrando o MQTT funcionando)
