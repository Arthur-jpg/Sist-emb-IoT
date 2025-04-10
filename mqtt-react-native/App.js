import { StatusBar } from 'expo-status-bar';
import { useEffect, useState } from 'react';
import { StyleSheet, Text, TextInput, TouchableOpacity, View, ScrollView } from 'react-native';
import { connect, setOnMessageCallback } from './src/service/mqttservice';

export default function App() {
  const [message, setMessage] = useState('');
  const [topic, setTopic] = useState('');
  const [receivedMessagesMensagem, setReceivedMessagesMensagem] = useState([]);
  const [receivedMessagesWarning, setReceivedMessagesWaring] = useState([]);
  const [receivedMessagesPercentage, setReceivedMessagesPercentage] = useState([]);
  const [percentualFloat, setPercentualFloat] = useState(0);

  const gerarIdUnico = () => `${Date.now()}-${Math.random().toString(36).substr(2, 9)}`;



  useEffect(() => {
    connect();

    setOnMessageCallback((topico, mensagem) => {
      if (topico === "esp32/hcsr04/status") {

        setReceivedMessagesMensagem((prev) => [
          { topico, mensagem, id: gerarIdUnico() },
          ...prev,
        ]);
      }

      if (topico === "esp32/hcsr04/dados") {
        setReceivedMessagesWaring((prev) => [
          { topico, mensagem, id: gerarIdUnico() },
          ...prev,
        ]);
      }

      if (topico === "esp32/hcsr04/teste") {
        const valor = parseFloat(mensagem);
        setPercentualFloat(valor);
      
        setReceivedMessagesPercentage((prev) => [
          { topico, mensagem, id: gerarIdUnico() },
          ...prev,
        ]);
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
      transition: 'width 0.3s ease',
    }} />
  </View>
</View>


      <Text style={{ marginTop: 20, fontWeight: 'bold' }}>Mensagens Recebidas:</Text>
      <View style={styles.containerMensagens}>
        <View style={styles.coluna}>
          <ScrollView style={{ marginTop: 10, maxHeight: 200 }}>
            {receivedMessagesMensagem.map((msg) => (
              <Text key={msg.id}> {msg.mensagem}</Text>
            ))}
          </ScrollView>
        </View>
        <View style={styles.coluna}>
          <ScrollView style={{ marginTop: 10, maxHeight: 200 }}>
          {receivedMessagesWarning.map((msg) => (
            <Text key={msg.id}> {msg.mensagem}</Text>
          ))}
          </ScrollView>
        
        </View>
        <View style={styles.coluna}>
          <ScrollView style={{ marginTop: 10, maxHeight: 200 }}>
          {receivedMessagesPercentage.map((msg) => (
            <Text key={msg.id}> {msg.mensagem}</Text>
          ))}
          </ScrollView>
        
        </View>
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
  input: {
    borderWidth: 1,
    borderColor: '#aaa',
    borderRadius: 5,
    width: '100%',
    padding: 10,
    marginVertical: 5,
  },
  button: {
    backgroundColor: '#007aff',
    paddingVertical: 10,
    paddingHorizontal: 20,
    borderRadius: 8,
    marginTop: 10,
  },
  buttonText: {
    color: '#fff',
    fontWeight: 'bold',
  },
  containerMensagens: {
    flexDirection: 'row',
    justifyContent: 'space-between',
    gap: 20,
    marginTop: 20,
  },
  coluna: {
    flex: 1,
    maxHeight: 200,
    borderWidth: 1,
    borderColor: '#ccc',
    borderRadius: 10,
    padding: 10,
  }
});
