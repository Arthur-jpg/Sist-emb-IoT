# **Stock Monitoring - Monitoramento Inteligente de Papel Higiênico**  

## **1. Descrição**  

O **Stock Monitoring** é um sistema baseado em IoT para monitoramento do estoque de papel higiênico em estabelecimentos comerciais. O sistema utiliza sensores ultrassônicos e um microcontrolador ESP32 para medir o nível de papel nos dispensers e notificar um servidor na nuvem via MQTT quando o estoque estiver baixo.  

O objetivo do projeto é otimizar a reposição de papel higiênico, garantindo maior eficiência na gestão de insumos e melhorando a experiência dos usuários em banheiros de locais públicos, como shoppings, hospitais e restaurantes.  

---  

## **2. Tecnologias Utilizadas**  

| Tecnologia | Descrição |  
|------------|-------------|  
| **ESP32** | Microcontrolador responsável pelo processamento e comunicação Wi-Fi. |  
| **HC-SR04** | Sensor ultrassônico para medir a quantidade de papel restante. |  
| **C (Arduino Framework)** | Linguagem de programação utilizada no firmware do ESP32. |  
| **MQTT (Mosquitto)** | Protocolo de comunicação para envio de alertas ao servidor. |  
| **Wi-Fi** | Conexão de rede utilizada para comunicação do ESP32 com o servidor. |  
| **Servidor na Nuvem** | Responsável por armazenar e processar os alertas recebidos. |  

---  

## **3. Arquitetura do Sistema**  

O sistema opera da seguinte forma:  

1. O ESP32 coleta dados do sensor ultrassônico posicionado no dispenser.  
2. Quando o nível de papel atinge menos de 20% da capacidade do dispenser, o ESP32 envia um alerta via MQTT.  
3. O broker MQTT (Mosquitto) recebe e repassa a mensagem para o sistema na nuvem.  
4. O sistema receptor notifica os responsáveis via painel de controle e alerta móvel.  

A tabela a seguir apresenta um resumo dos componentes e suas funções:  

| Componente | Função |  
|------------|-------------|  
| **ESP32** | Processamento e envio de dados via Wi-Fi. |  
| **HC-SR04** | Medição do nível de papel no dispenser. |  
| **Mosquitto (Broker MQTT)** | Gerenciamento das mensagens entre ESP32 e servidor. |  
| **Servidor na Nuvem** | Armazena os dados e emite alertas. |  
| **Painel de Controle** | Interface para monitoramento dos dispensers. |  

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

## **6. Melhorias Futuras**  

O sistema está em fase inicial de desenvolvimento e poderá ser aprimorado com as seguintes funcionalidades:  

| Melhorias | Descrição |  
|------------|-------------|  
| **Otimização de Energia** | Melhor gerenciamento do consumo do ESP32 para maior eficiência. |  
| **Compatibilidade Ampliada** | Adaptação para diferentes modelos de dispensers. |  
| **Interface Gráfica Avançada** | Desenvolvimento de um painel web para visualização dos níveis de papel. |  
| **Integração com Outros Sistemas** | Conexão com sistemas de gestão de estoque já existentes. |  
| **Correção de inconsistências** | Correção na inconsistência da medida de desitância e aumento da precisão. |  

---  

## **7. Testes e Validação**  

A fase de testes será realizada para validar a precisão das medições do sensor, a confiabilidade do envio de alertas via MQTT e a eficiência do sistema na prevenção da falta de papel. Os testes incluirão:  

- Validação da medição de nível de papel em diferentes dispensers.  
- Testes de conectividade e latência do envio de alertas via MQTT.  
- Testes de usabilidade do painel de controle (futuramente).  

---  

## **8. Equipe**  

| Nome | Função |  
|------------|-------------|  
| **Arthur Peixoto Schiller** | Desenvolvimento |  
| **Franc Wang** | Desenvolvimento |  
| **Juliana de Oliveira** | Desenvolvimento |  
| **Prof. Rigel Fernandes** | Orientação acadêmica e supervisão técnica. |  

