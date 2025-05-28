import { StatusBar } from 'expo-status-bar';
import { useEffect, useState } from 'react';
import { StyleSheet, Text, View, ScrollView } from 'react-native';
import { connect, setOnMessageCallback } from './src/service/mqttservice';

export default function App() {
  const [receivedMessages, setReceivedMessages] = useState([]);
  const [percentualFloat, setPercentualFloat] = useState(0);

  const gerarIdUnico = () => `${Date.now()}-${Math.random().toString(36).substr(2, 9)}`;

  useEffect(() => {
    connect();

    setOnMessageCallback((topico, mensagem) => {
      if (topico === "sensor/distancia/teste5") {
        setReceivedMessages((prev) => [
          { topico, mensagem, id: gerarIdUnico() },
          ...prev,
        ]);
        const valor = parseFloat(mensagem);
        setPercentualFloat(valor);
      }
    });
  }, []);

  return (
    <View style={styles.container}>
      <Text style={styles.title}>MQTT NO TELEFONE</Text>
      <View style={{ marginTop: 40, width: '100%', alignItems: 'center' }}>
        <Text style={{ marginBottom: 10 }}>Percentual: {percentualFloat.toFixed(2)}%</Text>
        <View style={{
          width: '100%',
          height: 25,
          backgroundColor: '#eee',
          borderRadius: 10,
          overflow: 'hidden',
        }}>
          <View style={{
            width: `${percentualFloat}%`,
            height: '100%',
            backgroundColor: '#4caf50',
          }} />
        </View>
      </View>

      <Text style={{ marginTop: 20, fontWeight: 'bold' }}>Mensagens Recebidas:</Text>
      <View style={styles.containerMensagens}>
        <ScrollView style={{ marginTop: 10, maxHeight: 200, width: '100%' }}>
          {receivedMessages.map((msg) => (
            <Text key={msg.id}> {msg.mensagem}</Text>
          ))}
        </ScrollView>
      </View>

      <StatusBar style="auto" />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    paddingTop: 60,
    alignItems: 'center',
    paddingHorizontal: 20,
  },
  title: {
    fontSize: 24,
    fontWeight: 'bold',
    color: '#000',
    marginBottom: 20,
  },
  containerMensagens: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    gap: 20,
    marginTop: 20,
  },
});
