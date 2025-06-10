# Stock Monitoring - Parte 2: Aprimoramento e Análise Quantitativa

## Introdução

Esta documentação detalha a segunda fase do projeto Stock Monitoring, focada no aprimoramento da interface do usuário, implementação de recursos avançados e na realização de estudos quantitativos e qualitativos para validar a eficácia do sistema.

## Melhorias na Interface do Usuário

Na segunda fase do projeto, aprimoramos significativamente a interface do usuário com as seguintes melhorias:

1. **Design Aprimorado**: A interface foi redesenhada para proporcionar uma melhor experiência de usuário e visualização mais clara dos dados.

2. **Sistema de Alertas Visuais**: Implementamos um sistema de alertas visuais com mudança de cores conforme os níveis de estoque:
   - **Verde**: Nível de estoque adequado (acima de 50%)
   - **Amarelo**: Nível de estoque médio (entre 20% e 50%)
   - **Vermelho**: Nível crítico (abaixo de 20%)

3. **Notificações em Tempo Real**: Adicionamos alertas que notificam imediatamente quando o nível do papel atinge estados críticos.

## Integração Python-MQTT-Arduino

Para melhorar a confiabilidade da transmissão de dados, implementamos um script Python (`reading_mqtt.py`) que serve como ponte entre o Arduino e o broker MQTT:

1. **Funcionamento**:
   - O Arduino envia dados do sensor ultrassônico pela porta serial
   - O script Python lê esses dados e os republica no broker MQTT (HiveMQ)
   - O aplicativo React Native se inscreve no mesmo tópico MQTT para receber os dados em tempo real

2. **Vantagens desta Abordagem**:
   - Maior estabilidade na conexão
   - Processamento dos dados antes do envio (filtragem, formatação)
   - Redução da carga de processamento do ESP32/Arduino
   - Possibilidade de implementar lógica adicional no middleware Python

3. **Configuração**:
   - Broker MQTT: broker.hivemq.com (porta 1883)
   - Tópico: arduino/data
   - Formato dos dados: valores numéricos representando a distância medida

## Estudo Quantitativo e Qualitativo

Uma das principais contribuições da segunda fase foi a realização de um estudo detalhado sobre a precisão e confiabilidade do sistema:

### Metodologia do Estudo

1. **Condições de Teste**:
   - 5 níveis de papel diferentes (2cm, 7cm, 12cm, 17cm, 22cm)
   - Múltiplas leituras para cada nível
   - Coleta de dados em formato CSV

2. **Métricas Analisadas**:
   - Média das leituras
   - Desvio padrão
   - Valores mínimos e máximos
   - Amplitude (diferença entre máximo e mínimo)
   - Taxa de erro percentual médio (MAPE)

### Resultados e Visualizações

O estudo gerou diversas visualizações para análise dos dados:

1. **Boxplot das Distâncias**: Demonstrou a distribuição das leituras para cada distância real.

2. **Desvio Padrão das Medições**: Comparou a dispersão das leituras em cada condição de teste.

3. **Séries Temporais**: Mostrou a evolução das leituras ao longo do tempo para cada distância testada.

4. **Amplitude das Medições**: Visualizou a diferença entre o maior e o menor valor registrado em cada teste.

5. **Erro Percentual**: Calculou a taxa de erro percentual médio (MAPE) do sistema em relação às distâncias reais.

### Conclusões Importantes

1. **Precisão do Sistema**: O sensor ultrassônico demonstrou boa precisão para distâncias entre 2cm e 22cm, validando seu uso para monitoramento de papel em dispensers.

2. **Limitações Identificadas**: O sensor apresenta baixa performance para distâncias muito pequenas (menores que 2cm), justificando a decisão de manter uma margem mínima para o sensor.

3. **Implementação de Melhorias**: Com base nos dados, foram implementados ajustes nos limiares de alerta para minimizar falsos positivos.

## Conclusões e Trabalhos Futuros

O aprimoramento da interface e a realização do estudo quantitativo confirmaram a viabilidade do sistema Stock Monitoring para aplicações reais. Os próximos passos incluem:

1. **Implementação de Algoritmos de Filtragem**: Para aumentar ainda mais a precisão das leituras.

2. **Expansão da Capacidade de Monitoramento**: Para múltiplos dispensers em ambientes complexos.

3. **Desenvolvimento de Forecasting**: Utilização de dados históricos para prever quando o papel precisará ser reabastecido.

4. **Integração com Sistemas de Gestão**: Conexão com sistemas existentes de gerenciamento de insumos e facilities.

---

