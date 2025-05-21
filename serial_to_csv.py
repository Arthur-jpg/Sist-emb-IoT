import serial
import csv

# Configure a porta e baudrate conforme seu ambiente
SERIAL_PORT = 'COM6'  # Altere para sua porta (ex: 'COM4', '/dev/ttyUSB0')
BAUDRATE = 115200
CSV_FILE = 'distancias20-400.csv'

# Dica para erro de acesso negado:
# - Feche o Monitor Serial da IDE Arduino ou qualquer outro programa que esteja usando a porta COM3.
# - Só um programa pode acessar a porta serial por vez.

# Dica para erro "FileNotFoundError":
# - Verifique se o ESP32 está conectado ao computador.
# - Confira se o SERIAL_PORT ('COM3') está correto.
#   - No Windows, veja a porta em: Gerenciador de Dispositivos > Portas (COM e LPT).
#   - No Linux/Mac, use '/dev/ttyUSB0', '/dev/ttyACM0', etc.
# - Altere SERIAL_PORT para a porta correta se necessário.

ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1)

with open(CSV_FILE, 'w', newline='') as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(['distancia_cm'])
    print("Gravando... Pressione Ctrl+C para parar.")
    try:
        while True:
            line = ser.readline().decode('utf-8').strip()
            if line:
                try:
                    valor = float(line)
                    writer.writerow([f"{valor:.3f}"])
                    print(f"{valor:.3f}")
                except ValueError:
                    pass  # Ignora linhas que não são números
    except KeyboardInterrupt:
        print("\nFinalizado.")
ser.close()
