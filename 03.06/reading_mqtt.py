import serial
import time
import paho.mqtt.client as mqtt
import sys

# Configuração MQTT
MQTT_BROKER = "broker.hivemq.com"
MQTT_PORT = 1883
MQTT_TOPIC = "arduino/data"  # Você pode alterar para um tópico específico

# Configuração Serial
# Observe que você precisa alterar a PORTA_COM para a porta COM do seu Arduino
# Exemplo: 'COM3' no Windows ou '/dev/ttyUSB0' no Linux
PORTA_COM = 'COM3'  # Altere conforme necessário
BAUD_RATE = 115200

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conectado ao broker MQTT")
        print(f"Publicando para o tópico: {MQTT_TOPIC}")
    else:
        print(f"Falha na conexão com o broker MQTT, código de retorno: {rc}")

def main():
    # Configuração do cliente MQTT
    client = mqtt.Client()
    client.on_connect = on_connect
    
    try:
        # Conectar ao broker MQTT
        print(f"Conectando ao broker MQTT: {MQTT_BROKER}")
        client.connect(MQTT_BROKER, MQTT_PORT, 60)
        client.loop_start()
        
        # Conectar à porta serial
        try:
            print(f"Abrindo porta serial {PORTA_COM} a {BAUD_RATE} bps...")
            ser = serial.Serial(PORTA_COM, BAUD_RATE, timeout=1)
            print("Porta serial aberta com sucesso!")
        except serial.SerialException as e:
            print(f"Erro ao abrir a porta serial: {e}")
            client.loop_stop()
            client.disconnect()
            sys.exit(1)
            
        # Loop principal - ler serial e publicar no MQTT
        print("Iniciando leitura da porta serial. Pressione CTRL+C para encerrar.")
        while True:
            if ser.in_waiting > 0:
                # Ler linha da porta serial
                linha = ser.readline().decode('utf-8').strip()
                if linha:
                    print(f"Dados recebidos: {linha}")
                    # Publicar no MQTT
                    client.publish(MQTT_TOPIC, linha)
                    print(f"Dados enviados para o tópico: {MQTT_TOPIC}")
            time.sleep(0.1)  # Pequena pausa para não sobrecarregar a CPU
                
    except KeyboardInterrupt:
        print("\nPrograma encerrado pelo usuário")
    except Exception as e:
        print(f"Erro: {e}")
    finally:
        # Limpeza ao sair
        print("Fechando conexões...")
        try:
            ser.close()
            print("Porta serial fechada")
        except:
            pass
        
        try:
            client.loop_stop()
            client.disconnect()
            print("Desconectado do broker MQTT")
        except:
            pass

if __name__ == "__main__":
    main()