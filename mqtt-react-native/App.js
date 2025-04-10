import { StatusBar } from 'expo-status-bar';
import { useEffect, useState } from 'react';
import { StyleSheet, Text, TextInput, TouchableOpacity, View, ScrollView } from 'react-native';
import { connect, setOnMessageCallback } from './src/service/mqttservice';

export default function App() {
  const [message, setMessage] = useState('');
  const [topic, setTopic] = useState('');
  const [receivedMessagesMensagem, setReceivedMessagesMensagem] = useState([]);
  const [receivedMessagesWarning, setReceivedMessagesWaring] = useState([]);

  useEffect(() => {
    connect();

    setOnMessageCallback((topico, mensagem) => {
      if (topico === "teste/mosquitto/expo") {

        setReceivedMessagesMensagem((prev) => [
          { topico, mensagem, id: Date.now() },
          ...prev,
        ]);
      }

      if (topico === "teste/mosquitto/expo2") {
        setReceivedMessagesWaring((prev) => [
          { topico, mensagem, id: Date.now() },
          ...prev,
        ]);
      }
    });
  }, []);


  return (
    <View style={styles.container}>
      <Text style={styles.title}>MQTT NO TELEFONE</Text>

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
