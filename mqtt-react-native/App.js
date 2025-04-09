import { StatusBar } from 'expo-status-bar';
import { useEffect, useState } from 'react';
import { StyleSheet, Text, TextInput, TouchableOpacity, View, ScrollView } from 'react-native';
import { connect, setOnMessageCallback } from './src/service/mqttservice';

export default function App() {
  const [message, setMessage] = useState('');
  const [topic, setTopic] = useState('');
  const [receivedMessages, setReceivedMessages] = useState([]);

  useEffect(() => {
    connect();

    setOnMessageCallback((topico, mensagem) => {
      setReceivedMessages((prev) => [
        { topico, mensagem, id: Date.now() },
        ...prev,
      ]);
    });
  }, []);

  const handleSend = () => {
    sendMessage(topic, message);
  };

  return (
    <View style={styles.container}>
      <Text style={styles.title}>MQTT NO TELEFONE</Text>

      <Text style={{ marginTop: 20, fontWeight: 'bold' }}>Mensagens Recebidas:</Text>
      <ScrollView style={{ marginTop: 10, maxHeight: 200 }}>
        {receivedMessages.map((msg) => (
          <Text key={msg.id}> {msg.mensagem}</Text>
        ))}
      </ScrollView>

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
});
