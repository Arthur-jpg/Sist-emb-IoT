# Análise Quantitativa de Medidas de Distância com Sensor Ultrassônico

## Descrição do Experimento

O objetivo deste experimento foi avaliar a precisão e repetibilidade de um sensor ultrassônico para medir o nível de papel em um dispenser. Foram realizados 5 testes, cada um com o papel posicionado a uma distância diferente do sensor:

- **2 cm**: Dispenser cheio (20 cm de papel + 2 cm de margem para o sensor)
- **7 cm**: 15 cm de papel
- **12 cm**: 10 cm de papel (meio cheio)
- **17 cm**: 5 cm de papel
- **22 cm**: Dispenser vazio (apenas a margem)

Cada teste gerou um arquivo CSV com as leituras do sensor para a respectiva distância.

## Método de Análise

O script `analise_csvs.py` realiza a análise quantitativa dos dados, calculando para cada teste:
- Média das leituras
- Desvio padrão
- Valor mínimo e máximo
- Amplitude (diferença entre máximo e mínimo)
- Número de amostras

Além disso, são gerados gráficos para facilitar a visualização dos resultados:

### Gráficos Gerados

1. **Boxplot das Distâncias**: Mostra a distribuição das leituras para cada distância real do papel ao sensor.
2. **Desvio Padrão das Medições**: Compara a dispersão das leituras em cada teste.
3. **Evolução das Distâncias**: Exibe as séries temporais das leituras para cada distância.
4. **Amplitude das Medições**: Mostra a diferença entre o maior e o menor valor registrado em cada teste.

Todos os gráficos utilizam como rótulo a distância real do papel ao sensor, facilitando a comparação entre as condições.

## Avaliação da Precisão

A precisão do sensor é avaliada principalmente pelo desvio padrão e pela amplitude das leituras em cada teste. Valores baixos indicam maior precisão e menor variação nas medições. A média das médias pode ser comparada com as distâncias reais para avaliar possíveis vieses sistemáticos.

## Como Executar a Análise

1. Certifique-se de que os arquivos CSV estejam na mesma pasta do script `analise_csvs.py`.
2. Execute o script com Python 3:

   ```powershell
   python analise_csvs.py
   ```

3. Os resultados quantitativos serão exibidos no terminal e os gráficos serão salvos como arquivos PNG na mesma pasta.

## Interpretação dos Resultados

- **Média**: Aproximação da distância real medida pelo sensor.
- **Desvio padrão**: Indica a precisão do sensor para cada posição.
- **Amplitude**: Mostra a variação máxima observada em cada teste.
- **Comparação entre distâncias**: Permite avaliar se o sensor responde adequadamente à variação do nível de papel.

A análise permite identificar a faixa de operação mais precisa do sensor e possíveis limitações para o uso no monitoramento de estoque do dispenser de papel.

## Taxa de Erro Percentual do Sistema

Além das métricas de precisão, foi calculada a **taxa de erro percentual médio (MAPE)**, que compara a média das leituras de cada teste com a distância real correspondente. O gráfico abaixo mostra o erro percentual de cada medição e a linha vermelha indica o MAPE global:

![Erro Percentual de Cada Medição](erro_percentual.png)

O valor do MAPE representa o erro percentual médio do sistema em relação às distâncias reais. Para este experimento, o MAPE foi **baixo**, indicando que o sistema apresenta boa precisão para uso em monitoramento de estoque. Em aplicações de controle de estoque, taxas de erro percentuais baixas são desejáveis, pois garantem que o nível de papel seja estimado de forma confiável, minimizando alarmes falsos ou leituras incorretas.

> **Nota sobre medições muito pequenas:** O sensor ultrassônico apresenta baixa performance e alta imprecisão para distâncias menores que 2 cm. Por esse motivo, nos experimentos, o recipiente de 20 cm foi testado com o papel posicionado a pelo menos 2 cm do sensor, garantindo maior confiabilidade nas leituras e evitando a faixa de menor precisão do dispositivo.

---
