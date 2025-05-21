import glob
import pandas as pd
import matplotlib.pyplot as plt

# Altere o padrão abaixo para corresponder aos seus arquivos CSV
csv_files = glob.glob('distancias*.csv')

resultados = []

for fname in csv_files:
    df = pd.read_csv(fname)
    # Converte para float caso esteja como string
    df['distancia_cm'] = df['distancia_cm'].astype(float)
    media = df['distancia_cm'].mean()
    desvio = df['distancia_cm'].std()
    minimo = df['distancia_cm'].min()
    maximo = df['distancia_cm'].max()
    amplitude = maximo - minimo
    resultados.append({
        'arquivo': fname,
        'media': media,
        'desvio_padrao': desvio,
        'min': minimo,
        'max': maximo,
        'amplitude': amplitude,
        'n amostras': len(df)
    })

# Mostra resultados individuais
for r in resultados:
    print(f"Arquivo: {r['arquivo']}")
    print(f"  Média: {r['media']:.4f} cm")
    print(f"  Desvio padrão: {r['desvio_padrao']:.4f} cm")
    print(f"  Mínimo: {r['min']:.4f} cm")
    print(f"  Máximo: {r['max']:.4f} cm")
    print(f"  Amplitude: {r['amplitude']:.4f} cm")
    print(f"  Nº de amostras: {r['n amostras']}")
    print("")

# Comparação entre medições
medias = [r['media'] for r in resultados]
desvios = [r['desvio_padrao'] for r in resultados]
amplitudes = [r['amplitude'] for r in resultados]
arquivos = [r['arquivo'] for r in resultados]

print("Comparação entre medições:")
print(f"  Média das médias: {sum(medias)/len(medias):.4f} cm")
print(f"  Média dos desvios padrão: {sum(desvios)/len(desvios):.4f} cm")
print(f"  Média das amplitudes: {sum(amplitudes)/len(amplitudes):.4f} cm")
print(f"  Diferença máxima entre médias: {max(medias) - min(medias):.4f} cm")

# --------- GRÁFICOS ---------
plt.style.use('seaborn-v0_8-darkgrid')

fig, axs = plt.subplots(2, 2, figsize=(14, 10))

# 1. Boxplot das distâncias de cada medição
dados = []
for fname in arquivos:
    df = pd.read_csv(fname)
    df['distancia_cm'] = df['distancia_cm'].astype(float)
    dados.append(df['distancia_cm'])
axs[0, 0].boxplot(dados, labels=[f"Medição {i+1}" for i in range(len(arquivos))])
axs[0, 0].set_title('Distribuição das Distâncias por Medição')
axs[0, 0].set_ylabel('Distância (cm)')
axs[0, 0].set_xlabel('Arquivo de Medição')

# 2. Médias e desvios padrão (barras com erro)
axs[0, 1].bar(range(len(arquivos)), medias, yerr=desvios, capsize=8, color='royalblue', alpha=0.7)
axs[0, 1].set_xticks(range(len(arquivos)))
axs[0, 1].set_xticklabels([f"Medição {i+1}" for i in range(len(arquivos))])
axs[0, 1].set_ylabel('Distância média (cm)')
axs[0, 1].set_title('Média e Desvio Padrão das Medições')

# 3. Todas as séries de medições (linha)
for i, fname in enumerate(arquivos):
    df = pd.read_csv(fname)
    df['distancia_cm'] = df['distancia_cm'].astype(float)
    axs[1, 0].plot(df['distancia_cm'].values, label=f"Medição {i+1}")
axs[1, 0].set_title('Evolução das Distâncias em Cada Medição')
axs[1, 0].set_xlabel('Amostra')
axs[1, 0].set_ylabel('Distância (cm)')
axs[1, 0].legend()

# 4. Amplitude de cada medição (barra)
axs[1, 1].bar(range(len(arquivos)), amplitudes, color='orange', alpha=0.7)
axs[1, 1].set_xticks(range(len(arquivos)))
axs[1, 1].set_xticklabels([f"Medição {i+1}" for i in range(len(arquivos))])
axs[1, 1].set_ylabel('Amplitude (cm)')
axs[1, 1].set_title('Amplitude (Máximo - Mínimo) de Cada Medição')

plt.tight_layout()
plt.show()
