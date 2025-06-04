import { Message, Client } from "paho-mqtt";

let isConnected = false;
let messageCallback = null;

const clientId = "mqtt_" + Math.random().toString(16).substr(2, 8);
// Usar porta 8081 para WebSocket (porta aberta publicamente)
const client = new Client("broker.hivemq.com", 8000, "/mqtt", clientId);

const connect = () => {
    if (isConnected && client.isConnected()) {
      console.log("🔄 Já conectado ao broker.");
      return;
    }
  
    // Configurar callbacks
    client.onMessageArrived = onMessageArrived;
    client.onConnectionLost = onConnectionLost;
      console.log("🔌 Tentando conectar a broker.hivemq.com na porta 8000...");
    
    try {
      client.connect({
        onSuccess: onSuccess,
        onFailure: onFail,
        useSSL: false, // Desativar SSL
        timeout: 30, // Aumentar o timeout para dar mais tempo para conexão
        keepAliveInterval: 30, // Manter a conexão viva
      });
    } catch (error) {
      console.error("❌ Erro ao iniciar conexão:", error);
      isConnected = false;
    }
};
  

function onSuccess() {
  console.log("✅ Conectado ao broker Mosquitto!");
  isConnected = true;
  
  // Lista de tópicos para inscrição
  const topics = [
    "esp32/hcsr04/status",
    "esp32/hcsr04/dados",
    "esp32/hcsr04/teste",
    "sensor/distancia/teste5",
    "arduino/data",
  ];
  
  // Inscrever em cada tópico com log
  topics.forEach(topic => {
    console.log(`📋 Inscrevendo no tópico: ${topic}`);
    client.subscribe(topic, {
      qos: 0,
      onSuccess: () => console.log(`✅ Inscrito com sucesso em: ${topic}`),
      onFailure: (err) => console.log(`❌ Falha ao inscrever em ${topic}:`, err),
    });
  });
  
  console.log("🎉 Configuração completa! Aguardando mensagens...");
}

function onFail(error) {
  console.log("❌ Falha ao conectar:", error);
  console.log("Código de erro:", error.errorCode);
  console.log("Mensagem:", error.errorMessage);
  isConnected = false;
}

function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("🔌 Conexão perdida:", responseObject.errorMessage);
    isConnected = false;
    
    // Tentar reconectar após 5 segundos
    console.log("🔄 Tentando reconectar em 5 segundos...");
    setTimeout(() => {
      try {
        connect();
      } catch (e) {
        console.error("❌ Erro ao tentar reconectar:", e);
      }
    }, 5000);
  }
}

function onMessageArrived(message) {
  try {
    const topic = message.destinationName;
    const payload = message.payloadString;
    
    console.log(`📩 Mensagem recebida no tópico [${topic}]:`, payload);
    
    if (messageCallback) {
      messageCallback(topic, payload);
    } else {
      console.warn("⚠️ Mensagem recebida, mas nenhum callback registrado!");
    }
  } catch (error) {
    console.error("❌ Erro ao processar mensagem:", error);
  }
}


const setOnMessageCallback = (callback) => {
  messageCallback = callback;
};

const getConnectionStatus = () => {
  return isConnected;
};

// Função para reconectar manualmente
const reconnect = () => {
  console.log("🔄 Tentando reconexão manual...");
  
  // Se estiver conectado, desconectar primeiro
  if (client && client.isConnected()) {
    try {
      client.disconnect();
    } catch (e) {
      console.error("❌ Erro ao desconectar:", e);
    }
    isConnected = false;
  }
  
  // Pequeno delay antes de reconectar
  setTimeout(() => {
    connect();
  }, 1000);
};

export { connect, setOnMessageCallback, getConnectionStatus as isConnected, reconnect };
