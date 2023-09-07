#include <ESP8266WiFi.h>

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
const char* aternosServer = "Gino980304.aternos.me";
const int aternosPort = 34137; // Aternos服务器的端口

const int ledPin = 2; // 控制板载LED的引脚

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // 连接到WiFi网络
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // 创建TCP客户端
  WiFiClient client;

  if (client.connect(aternosServer, aternosPort)) {
    Serial.println("Connected to Aternos server");

    // 发送HTTP请求
    client.println("GET /status HTTP/1.1");
    client.print("Host: ");
    client.println(aternosServer);
    client.println("Connection: close");
    client.println();

    // 等待服务器响应
    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        // 检查服务器响应以确定服务器状态
        if (line.indexOf("Server is online") != -1) {
          // 服务器在线，点亮LED
          digitalWrite(ledPin, HIGH);
          Serial.println("Server is online");
        } else {
          // 服务器不在线，关闭LED
          digitalWrite(ledPin, LOW);
          Serial.println("Server is offline");
        }
      }
    }
    client.stop();
    Serial.println("Connection closed");
  } else {
    Serial.println("Connection to Aternos server failed");
  }

  delay(10000); // 每10秒检查一次服务器状态
}

