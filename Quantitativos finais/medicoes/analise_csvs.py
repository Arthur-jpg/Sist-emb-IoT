import glob
import pandas as pd
import matplotlib.pyplot as plt

# Altere o padrão abaixo para corresponder aos seus arquivos CSV
csv_files = glob.glob('distancias*.csv')

resultados = []

# Mapeamento dos arquivos para as distâncias reais
arquivo_para_distancia = {
    'distancias20-400.csv': '2 cm',
    'distancias20-401.csv': '7 cm',
    'distancias20-402.csv': '12 cm',
    'distancias20-403.csv': '17 cm',
    'distancias20-404.csv': '22 cm',
}

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

# Atualiza a lista de arquivos para garantir a ordem correta
arquivos = [
    'distancias20-400.csv',
    'distancias20-401.csv',
    'distancias20-402.csv',
    'distancias20-403.csv',
    'distancias20-404.csv',
]

# Mostra resultados individuais
for i, r in enumerate(resultados):
    distancia = arquivo_para_distancia.get(r['arquivo'], r['arquivo'])
    print(f"Distância real: {distancia}")
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

# Gráficos com rótulos de distância real
rotulos_distancia = [arquivo_para_distancia.get(f, f) for f in arquivos]

# 1. Boxplot das distâncias de cada medição
fig1, ax1 = plt.subplots(figsize=(8, 6))
ax1.boxplot(dados, labels=rotulos_distancia)
ax1.set_title('Distribuição das Distâncias por Medição')
ax1.set_ylabel('Distância (cm)')
ax1.set_xlabel('Distância Real do Papel ao Sensor')
fig1.tight_layout()
fig1.savefig('boxplot_distancias.png')
plt.close(fig1)

# 2. Gráfico de desvio padrão de cada medição
fig2, ax2 = plt.subplots(figsize=(12, 8))
ax2.bar(range(len(arquivos)), desvios, color='royalblue', alpha=0.7)
ax2.set_xticks(range(len(arquivos)))
ax2.set_xticklabels(rotulos_distancia)
ax2.set_ylabel('Desvio padrão (cm)')
ax2.set_title('Desvio Padrão das Medições')
media_desvio = sum(desvios)/len(desvios)
ax2.axhline(media_desvio, color='red', linestyle='--', label=f'Média dos desvios: {media_desvio:.2f} cm')
for i, v in enumerate(desvios):
    ax2.text(i, v + 0.01, f"{v:.2f}", ha='center', va='bottom', fontsize=12)
ax2.legend()
fig2.tight_layout()
fig2.savefig('desvios_padrao.png')
plt.close(fig2)

# 3. Todas as séries de medições (linha)
fig3, ax3 = plt.subplots(figsize=(8, 6))
for i, fname in enumerate(arquivos):
    df = pd.read_csv(fname)
    df['distancia_cm'] = df['distancia_cm'].astype(float)
    ax3.plot(df['distancia_cm'].values, label=f"{rotulos_distancia[i]}")
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
ax4.set_xticklabels(rotulos_distancia)
ax4.set_ylabel('Amplitude (cm)')
ax4.set_title('Amplitude (Máximo - Mínimo) de Cada Medição')
media_amplitude = sum(amplitudes)/len(amplitudes)
ax4.axhline(media_amplitude, color='red', linestyle='--', label=f'Média das amplitudes: {media_amplitude:.2f} cm')
ax4.legend()
fig4.tight_layout()
fig4.savefig('amplitudes.png')
plt.close(fig4)

# Cálculo da taxa de erro percentual médio (MAPE)
distancias_reais = [2, 7, 12, 17, 22]
medias_medidas = [r['media'] for r in resultados]

mape = sum(abs(m - d)/d for m, d in zip(medias_medidas, distancias_reais)) / len(distancias_reais) * 100
print(f"\nTaxa de erro percentual médio (MAPE): {mape:.2f}%")

# Gráfico da taxa de erro percentual de cada medição
percentuais_erro = [abs(m - d)/d * 100 for m, d in zip(medias_medidas, distancias_reais)]
fig5, ax5 = plt.subplots(figsize=(8, 6))
ax5.bar(rotulos_distancia, percentuais_erro, color='purple', alpha=0.7)
ax5.axhline(mape, color='red', linestyle='--', label=f'MAPE: {mape:.2f}%')
ax5.set_ylabel('Erro Percentual (%)')
ax5.set_xlabel('Distância Real do Papel ao Sensor')
ax5.set_title('Erro Percentual de Cada Medição')
for i, v in enumerate(percentuais_erro):
    ax5.text(i, v + 0.1, f"{v:.2f}%", ha='center', va='bottom', fontsize=12)
ax5.legend()
fig5.tight_layout()
fig5.savefig('erro_percentual.png')
plt.close(fig5)
