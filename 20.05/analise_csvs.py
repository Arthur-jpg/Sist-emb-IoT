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

dados = []
for fname in arquivos:
    df = pd.read_csv(fname)
    df['distancia_cm'] = df['distancia_cm'].astype(float)
    dados.append(df['distancia_cm'])

# 1. Boxplot das distâncias de cada medição
fig1, ax1 = plt.subplots(figsize=(8, 6))
ax1.boxplot(dados, labels=[f"Medição {i+1}" for i in range(len(arquivos))])
ax1.set_title('Distribuição das Distâncias por Medição')
ax1.set_ylabel('Distância (cm)')
ax1.set_xlabel('Arquivo de Medição')
fig1.tight_layout()
fig1.savefig('boxplot_distancias.png')
plt.close(fig1)

# 2. Médias e desvios padrão (barras com erro)
fig2, ax2 = plt.subplots(figsize=(8, 6))
ax2.bar(range(len(arquivos)), medias, yerr=desvios, capsize=8, color='royalblue', alpha=0.7)
ax2.set_xticks(range(len(arquivos)))
ax2.set_xticklabels([f"Medição {i+1}" for i in range(len(arquivos))])
ax2.set_ylabel('Distância média (cm)')
ax2.set_title('Média e Desvio Padrão das Medições')
media_geral = sum(medias)/len(medias)
ax2.axhline(media_geral, color='red', linestyle='--', label=f'Média geral: {media_geral:.2f} cm')
ax2.legend()
fig2.tight_layout()
fig2.savefig('medias_desvios.png')
plt.close(fig2)

# 3. Todas as séries de medições (linha)
fig3, ax3 = plt.subplots(figsize=(8, 6))
for i, fname in enumerate(arquivos):
    df = pd.read_csv(fname)
    df['distancia_cm'] = df['distancia_cm'].astype(float)
    ax3.plot(df['distancia_cm'].values, label=f"Medição {i+1}")
ax3.set_title('Evolução das Distâncias em Cada Medição')
ax3.set_xlabel('Amostra')
ax3.set_ylabel('Distância (cm)')
ax3.legend()
fig3.tight_layout()
fig3.savefig('series_temporais.png')
plt.close(fig3)

# 4. Amplitude de cada medição (barra)
fig4, ax4 = plt.subplots(figsize=(8, 6))
ax4.bar(range(len(arquivos)), amplitudes, color='orange', alpha=0.7)
ax4.set_xticks(range(len(arquivos)))
ax4.set_xticklabels([f"Medição {i+1}" for i in range(len(arquivos))])
ax4.set_ylabel('Amplitude (cm)')
ax4.set_title('Amplitude (Máximo - Mínimo) de Cada Medição')
media_amplitude = sum(amplitudes)/len(amplitudes)
ax4.axhline(media_amplitude, color='red', linestyle='--', label=f'Média das amplitudes: {media_amplitude:.2f} cm')
ax4.legend()
fig4.tight_layout()
fig4.savefig('amplitudes.png')
plt.close(fig4)
