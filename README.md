# **Stock Monitoring - Monitoramento Inteligente de Papel Higiênico**  

## **1. Descrição**  

O **Stock Monitoring** é um sistema baseado em IoT para monitoramento do estoque de papel higiênico em estabelecimentos comerciais. O sistema utiliza sensores ultrassônicos e um microcontrolador ESP32 para medir o nível de papel nos dispensers e notificar um servidor na nuvem via MQTT quando o estoque estiver baixo.  

O objetivo do projeto é otimizar a reposição de papel higiênico, garantindo maior eficiência na gestão de insumos e melhorando a experiência dos usuários em banheiros de locais públicos, como shoppings, hospitais e restaurantes.  

> **ATUALIZAÇÃO (2ª FASE)**: O projeto agora conta com uma interface aprimorada e um estudo quantitativo completo sobre a precisão do sistema. Detalhes completos disponíveis no arquivo [PARTE2.md](PARTE2.md).

---  

## **2. Tecnologias Utilizadas**  

| Tecnologia | Descrição |  
|------------|-------------|  
| **ESP32** | Microcontrolador responsável pelo processamento e comunicação Wi-Fi. |  
| **HC-SR04** | Sensor ultrassônico para medir a quantidade de papel restante. |  
| **C (Arduino Framework)** | Linguagem de programação utilizada no firmware do ESP32. |  
| **MQTT (HiveMQ)** | Protocolo de comunicação para envio de alertas ao servidor. |  
| **Wi-Fi** | Conexão de rede utilizada para comunicação do ESP32 com o servidor. |  
| **Python** | Middleware para leitura serial e publicação no broker MQTT. |  
| **React Native** | Desenvolvimento da interface mobile com alertas visuais inteligentes. |  
| **Servidor na Nuvem** | Responsável por armazenar e processar os alertas recebidos. |  

---  

## **3. Arquitetura do Sistema**  

O sistema opera da seguinte forma:  

1. O ESP32 coleta dados do sensor ultrassônico posicionado no dispenser.  
2. Os dados são enviados pela porta serial e processados por um middleware Python.
3. O script Python publica os dados no broker MQTT (HiveMQ).
4. O aplicativo React Native recebe os dados e exibe visualmente o status com alertas:
   - Verde: Nível adequado (>50%)
   - Amarelo: Nível médio (20-50%)
   - Vermelho: Nível crítico (<20%)
5. O sistema notifica os responsáveis via painel de controle e alerta móvel quando necessário.

A tabela a seguir apresenta um resumo dos componentes e suas funções:  

| Componente | Função |  
|------------|-------------|  
| **ESP32** | Processamento e envio de dados via porta serial. |  
| **HC-SR04** | Medição do nível de papel no dispenser. |  
| **Python Middleware** | Leitura serial e publicação no broker MQTT. |
| **HiveMQ (Broker MQTT)** | Gerenciamento das mensagens entre middleware e aplicativo. |  
| **React Native App** | Interface para monitoramento dos dispensers com alertas visuais. |
| **Servidor na Nuvem** | Armazena os dados e emite alertas. |  

---  

## **4. Contribuição da Equipe**  

Embora o projeto utilize diversas tecnologias e metodologias já estabelecidas, a principal contribuição da equipe foi a **definição das regras e parâmetros do sistema**. O time foi responsável por criar as regras de alerta e aviso, ajustando os **parâmetros conforme o tamanho do dispenser**. Além disso, toda a **ligação entre as tecnologias** (sensor, ESP32, MQTT e sistema na nuvem) foi desenvolvida pela equipe para garantir um funcionamento eficiente e integrado.  

---  

## **5. Benefícios do Sistema**  

| Benefício | Descrição |  
|------------|-------------|  
| **Monitoramento Automatizado** | Substitui a necessidade de inspeções manuais nos banheiros. |  
| **Redução de Reclamações** | Garante que o papel seja reposto antes de acabar. |  
| **Baixo Custo** | Implementação estimada em R$40 por unidade. |  
| **Gestão Eficiente** | Pode ser integrado a sistemas maiores de gerenciamento de insumos. |  
| **Escalabilidade** | Possibilidade de expansão para outros tipos de estoque. |  

---  

## **6. Melhorias Implementadas na 2ª Fase**

Na segunda fase do projeto, diversas melhorias foram implementadas:

| Melhorias | Descrição |  
|------------|-------------|  
| **Interface Aprimorada** | Nova interface com alertas visuais e mudança de cores para níveis críticos e médios. |  
| **Middleware Python** | Implementação de script Python para comunicação entre Arduino e MQTT. |  
| **Estudo Quantitativo** | Análise detalhada da precisão e confiabilidade do sistema. |  
| **Visualização de Dados** | Geração de gráficos para análise da performance do sensor. |  
| **Algoritmos de Alerta** | Refinamento dos algoritmos de detecção de níveis críticos. |  

## **7. Melhorias Futuras**  

O sistema continua em desenvolvimento e poderá ser aprimorado com as seguintes funcionalidades:  

| Melhorias | Descrição |  
|------------|-------------|  
| **Otimização de Energia** | Melhor gerenciamento do consumo do ESP32 para maior eficiência. |  
| **Compatibilidade Ampliada** | Adaptação para diferentes modelos de dispensers. |  
| **Sistemas de Previsão** | Implementação de algoritmos de forecasting para prever necessidades de reabastecimento. |  
| **Expansão de Monitoramento** | Suporte para múltiplos dispensers em ambientes complexos. |  

---  

## **7. Testes e Validação**  

A fase de testes foi realizada para validar a precisão das medições do sensor, a confiabilidade do envio de alertas via MQTT e a eficiência do sistema na prevenção da falta de papel. Os testes incluíram:  

- Validação da medição de nível de papel em diferentes dispensers.  
- Testes de conectividade e latência do envio de alertas via MQTT.  
- Testes de usabilidade do painel de controle com interface aprimorada.  

### **7.1. Estudo Quantitativo**  

Foi realizado um estudo quantitativo detalhado para avaliar a precisão e repetibilidade do sistema:

- **5 posições de teste**: 2cm, 7cm, 12cm, 17cm, 22cm
- **Métricas analisadas**: Média, desvio padrão, valores mínimos/máximos, amplitude
- **Taxa de erro percentual médio (MAPE)**: Calculada para validar a precisão geral do sistema
- **Visualizações geradas**: Boxplot, séries temporais, desvio padrão, amplitude, erro percentual

Os resultados completos do estudo estão disponíveis no diretório `Quantitativos finais`.

---  

## **8. Equipe**  

| Nome | Função |  
|------------|-------------|  
| **Arthur Peixoto Schiller** | Desenvolvimento |  
| **Franc Wang** | Desenvolvimento |  
| **Juliana de Oliveira** | Desenvolvimento |  
| **Prof. Rigel Fernandes** | Orientação acadêmica e supervisão técnica. |  

