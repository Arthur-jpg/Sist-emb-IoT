import { StatusBar } from 'expo-status-bar';
import { useEffect, useState } from 'react';
import { StyleSheet, Text, View, ScrollView, Animated, TouchableOpacity } from 'react-native';
import { connect, setOnMessageCallback, isConnected, reconnect } from './src/service/mqttservice';

export default function App() {
  const [receivedMessages, setReceivedMessages] = useState([]);
  const [percentualFloat, setPercentualFloat] = useState(0);
  const [connectionStatus, setConnectionStatus] = useState(false);
  const [lastUpdateTime, setLastUpdateTime] = useState(null);
  const animatedWidth = useState(new Animated.Value(0))[0];

  const gerarIdUnico = () => `${Date.now()}-${Math.random().toString(36).substr(2, 9)}`;
  
  // Atualiza a animação quando o valor percentual muda
  useEffect(() => {
    Animated.timing(animatedWidth, {
      toValue: percentualFloat > 100 ? 100 : percentualFloat,
      duration: 500,
      useNativeDriver: false
    }).start();
  }, [percentualFloat]);

  // Verifica status de conexão a cada 2 segundos
  useEffect(() => {
    const intervalId = setInterval(() => {
      setConnectionStatus(isConnected());
    }, 2000);
    
    return () => clearInterval(intervalId);
  }, []);

  useEffect(() => {
    connect();

    setOnMessageCallback((topico, mensagem) => {
      console.log(`Recebido - Tópico: ${topico}, Mensagem: ${mensagem}`);
      
      const timestamp = new Date();
      setLastUpdateTime(timestamp);
      
      setReceivedMessages((prev) => {
        // Limitar a 50 mensagens para não sobrecarregar a memória
        const newMessages = [
          { topico, mensagem, id: gerarIdUnico(), timestamp },
          ...prev,
        ];
        return newMessages.slice(0, 50);
      });
      
      if (topico === "sensor/distancia/teste5" || topico === "arduino/data") {
        try {
          const valor = parseFloat(mensagem);
          if (!isNaN(valor)) {
            setPercentualFloat(valor);
          }
        } catch (error) {
          console.error("Erro ao converter mensagem para número:", error);
        }
      }
    });
  }, []);

  // Formatar horário da última atualização
  const formatTime = (date) => {
    if (!date) return 'Nunca';
    return date.toLocaleTimeString();
  };

  return (
    <View style={styles.container}>
      {/* Cabeçalho com status de conexão */}
      <View style={styles.header}>
        <Text style={styles.title}>MQTT Monitor</Text>
        <View style={styles.statusContainer}>
          <View style={[
            styles.statusIndicator, 
            {backgroundColor: connectionStatus ? '#4caf50' : '#f44336'}
          ]} />
          <TouchableOpacity 
            onPress={() => {
              reconnect();
              console.log("Tentando reconectar ao broker MQTT...");
            }}
            style={styles.reconnectButton}
          >
            <Text style={styles.statusText}>
              {connectionStatus ? 'Conectado' : 'Reconectar'}
            </Text>
          </TouchableOpacity>
        </View>
      </View>

      {/* Card com informação de sensor */}
      <View style={styles.card}>
        <View style={styles.cardHeader}>
          <Text style={styles.cardTitle}>Sensor de Distância</Text>
          <Text style={styles.cardSubtitle}>
            Última atualização: {formatTime(lastUpdateTime)}
          </Text>
        </View>

        {/* Valor percentual com animação */}
        <View style={styles.valueContainer}>
          <Text style={styles.valueText}>{percentualFloat.toFixed(1)}%</Text>
        </View>

        {/* Barra de progresso animada */}
        <View style={styles.progressBackground}>
          <Animated.View 
            style={[
              styles.progressForeground,
              {
                width: animatedWidth.interpolate({
                  inputRange: [0, 100],
                  outputRange: ['0%', '100%'],
                }),
                backgroundColor: percentualFloat > 80 ? '#f44336' : 
                               percentualFloat > 60 ? '#ff9800' : '#4caf50',
              }
            ]} 
          />
        </View>
      </View>

      {/* Seção de mensagens */}
      <View style={styles.messagesSection}>
        <Text style={styles.sectionTitle}>
          Mensagens MQTT ({receivedMessages.length})
        </Text>
        <View style={styles.messagesContainer}>
          <ScrollView style={styles.messagesList}>
            {receivedMessages.map((msg) => (
              <View key={msg.id} style={styles.messageItem}>
                <View style={styles.messageHeader}>
                  <Text style={styles.messageTopic}>{msg.topico}</Text>
                  <Text style={styles.messageTime}>
                    {msg.timestamp.toLocaleTimeString()}
                  </Text>
                </View>
                <Text style={styles.messageContent}>{msg.mensagem}</Text>
              </View>
            ))}
            {receivedMessages.length === 0 && (
              <Text style={styles.emptyMessage}>
                Aguardando mensagens...
              </Text>
            )}
          </ScrollView>
        </View>
      </View>

      {/* Informações de conexão */}      <View style={styles.connectionInfo}>
        <Text>Broker: broker.hivemq.com (porta 8000/WS)</Text>
        <Text>Tópicos: arduino/data, sensor/distancia/teste5</Text>
        <TouchableOpacity
          style={styles.debugButton}
          onPress={() => {
            console.log("Estado atual do app:", {
              conectado: connectionStatus,
              ultimaAtualizacao: lastUpdateTime,
              valorAtual: percentualFloat,
              mensagensRecebidas: receivedMessages.length,
            });
          }}
          >
          <Text style={styles.debugButtonText}>Depurar conexão</Text>
        </TouchableOpacity>
      </View>

      <StatusBar style="auto" />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#f5f5f5',
    paddingTop: 50,
    paddingHorizontal: 16,
  },
  header: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    alignItems: 'center',
    marginBottom: 20,
    paddingBottom: 10,
    borderBottomWidth: 1,
    borderBottomColor: '#e0e0e0',
  },
  title: {
    fontSize: 22,
    fontWeight: 'bold',
    color: '#333',
  },
  statusContainer: {
    flexDirection: 'row',
    alignItems: 'center',
  },
  statusIndicator: {
    width: 10,
    height: 10,
    borderRadius: 5,
    marginRight: 6,
  },
  statusText: {
    fontSize: 14,
    color: '#666',
  },
  reconnectButton: {
    paddingVertical: 4,
    paddingHorizontal: 8,
    borderRadius: 4,
    backgroundColor: '#f0f0f0',
  },
  card: {
    backgroundColor: 'white',
    borderRadius: 10,
    padding: 16,
    marginVertical: 10,
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.1,
    shadowRadius: 4,
    elevation: 3,
  },
  cardHeader: {
    marginBottom: 16,
  },
  cardTitle: {
    fontSize: 18,
    fontWeight: 'bold',
    color: '#333',
  },
  cardSubtitle: {
    fontSize: 12,
    color: '#888',
    marginTop: 2,
  },
  valueContainer: {
    alignItems: 'center',
    marginBottom: 16,
  },
  valueText: {
    fontSize: 36,
    fontWeight: 'bold',
    color: '#2196f3',
  },
  progressBackground: {
    width: '100%',
    height: 20,
    backgroundColor: '#e0e0e0',
    borderRadius: 10,
    overflow: 'hidden',
  },
  progressForeground: {
    height: '100%',
  },
  messagesSection: {
    flex: 1,
    marginTop: 20,
  },
  sectionTitle: {
    fontSize: 16,
    fontWeight: 'bold',
    color: '#333',
    marginBottom: 10,
  },
  messagesContainer: {
    flex: 1,
    backgroundColor: 'white',
    borderRadius: 10,
    overflow: 'hidden',
    shadowColor: '#000',
    shadowOffset: { width: 0, height: 2 },
    shadowOpacity: 0.1,
    shadowRadius: 3,
    elevation: 2,
  },
  messagesList: {
    padding: 10,
  },
  messageItem: {
    paddingVertical: 8,
    paddingHorizontal: 12,
    borderBottomWidth: 1,
    borderBottomColor: '#f0f0f0',
  },
  messageHeader: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    marginBottom: 4,
  },
  messageTopic: {
    fontSize: 12,
    color: '#2196f3',
    fontWeight: '500',
  },
  messageTime: {
    fontSize: 10,
    color: '#9e9e9e',
  },
  messageContent: {
    fontSize: 14,
    color: '#333',
  },
  emptyMessage: {
    textAlign: 'center',
    color: '#9e9e9e',
    padding: 20,
  },
  connectionInfo: {
    padding: 16,
    backgroundColor: '#e6f7ff',
    borderRadius: 8,
    marginVertical: 10,
  },
  debugButton: {
    marginTop: 10,
    backgroundColor: '#333',
    padding: 8,
    borderRadius: 4,
    alignItems: 'center',
  },
  debugButtonText: {
    color: 'white',
    fontWeight: '500',
  },
});
