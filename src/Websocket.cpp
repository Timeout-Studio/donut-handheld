#include <Websocket.h>

// Global pointer to the current instance (needed for the static callback)
static Dn_Websocket *clientInstance = nullptr;

// Task function that runs WebSocket loop in a separate thread
void webSocketTaskFunction(void *parameter)
{
    Dn_Websocket *client = (Dn_Websocket *)parameter;

    while (client->isRunning)
    {
        // Process WebSocket events (with timeout to prevent blocking)
        client->webSocket.loop();

        // Check message queue for outgoing messages
        WSMessage msg;
        if (xQueueReceive(client->messageQueue, &msg, 0) == pdTRUE)
        {
            if (msg.valid && client->isConnected)
            {
                // Create JSON message with limited scope to free memory after sending
                {
                    JsonDocument doc; // Reduced size for efficiency
                    doc["type"] = "message";
                    doc["from"] = client->username;
                    doc["to"] = msg.to;
                    doc["message"] = msg.content;

                    // Use a fixed buffer instead of dynamic String
                    char jsonBuffer[384]; // Make sure this is large enough
                    size_t len = serializeJson(doc, jsonBuffer, sizeof(jsonBuffer));

                    // Send message
                    client->webSocket.sendTXT(jsonBuffer, len);

                    // Brief delay to allow TCP stack to process
                    vTaskDelay(5 / portTICK_PERIOD_MS);
                }

                // Force garbage collection
                ESP.getFreeHeap();
            }
        }

        // Small delay to prevent CPU hogging
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }

    client->webSocketTaskHandle = NULL;
    vTaskDelete(NULL);
}

// Static callback function for WebSocket events
void Dn_Websocket::webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    if (clientInstance == nullptr)
    {
        return;
    }

    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.println("[WSc] Disconnected!");
        clientInstance->isConnected = false;
        break;
    case WStype_CONNECTED:
        Serial.println("[WSc] Connected!");
        clientInstance->isConnected = true;
        clientInstance->lastHeartbeat = millis();

        // Register username when connected
        clientInstance->registerUsername();
        break;
    case WStype_TEXT:
        // Protect against buffer overflow
        if (length > 0)
        {
            // Create a null-terminated copy of the payload on the heap
            char *payloadCopy = (char *)malloc(length + 1);
            if (payloadCopy)
            {
                memcpy(payloadCopy, payload, length);
                payloadCopy[length] = '\0';

                // Process the message
                clientInstance->handleMessage(payloadCopy);

                // Free the copy
                free(payloadCopy);
            }
        }
        break;
    case WStype_ERROR:
        Serial.println("[WSc] Connection error!");
        break;
    }
}

Dn_Websocket::Dn_Websocket()
{
    // Store instance pointer for static callback
    clientInstance = this;

    // Initialize username as empty string
    username[0] = '\0';

    // Create message queue
    messageQueue = xQueueCreate(5, sizeof(WSMessage)); // Reduced queue size
    if (messageQueue == NULL)
    {
        Serial.println("[WSc] Error creating message queue");
    }
}

Dn_Websocket::~Dn_Websocket()
{
    stop();

    // Delete message queue
    if (messageQueue != NULL)
    {
        vQueueDelete(messageQueue);
        messageQueue = NULL;
    }
}

void Dn_Websocket::begin(const char *serverAddress, uint16_t port, const char *url, const char *usernameStr)
{
    // Copy username to fixed buffer (with length check)
    strncpy(username, usernameStr, sizeof(username) - 1);
    username[sizeof(username) - 1] = '\0'; // Ensure null termination

    // Configure WebSocket client
    webSocket.begin(serverAddress, port, url);
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000); // Attempt to reconnect every 5 seconds

    // Enable ping/pong for connection keep-alive
    webSocket.enableHeartbeat(20000, 3000, 2);

    Serial.println("[WSc] WebSocket client initialized");

    // Start WebSocket task on core 0 (Arduino loop runs on core 1)
    isRunning = true;
    xTaskCreatePinnedToCore(
        webSocketTaskFunction, // Task function
        "WebSocketTask",       // Task name
        10240,                 // Stack size (bytes) - increased
        this,                  // Parameter to pass
        1,                     // Task priority
        &webSocketTaskHandle,  // Task handle
        0                      // Run on core 0
    );

    Serial.println("[WSc] WebSocket task started on core 0");
}

void Dn_Websocket::stop()
{
    isRunning = false;

    // Wait for task to terminate
    int timeout = 10;
    while (webSocketTaskHandle != NULL && timeout-- > 0)
    {
        delay(100);
    }

    // If task did not terminate, force kill it
    if (webSocketTaskHandle != NULL)
    {
        vTaskDelete(webSocketTaskHandle);
        webSocketTaskHandle = NULL;
    }

    // Disconnect WebSocket
    webSocket.disconnect();

    Serial.println("WebSocket client stopped");
}

bool Dn_Websocket::sendMessage(const char *to, const char *message)
{
    if (!isRunning)
    {
        Serial.println("Cannot send message: client not running");
        return false;
    }

    if (!isConnected)
    {
        Serial.println("Cannot send message: not connected");
        return false;
    }

    // Create message struct
    WSMessage msg;
    msg.valid = true;

    // Copy strings to fixed buffers (with length checks)
    strncpy(msg.to, to, sizeof(msg.to) - 1);
    msg.to[sizeof(msg.to) - 1] = '\0'; // Ensure null termination

    strncpy(msg.content, message, sizeof(msg.content) - 1);
    msg.content[sizeof(msg.content) - 1] = '\0'; // Ensure null termination

    // Send to queue with timeout
    if (xQueueSend(messageQueue, &msg, pdMS_TO_TICKS(100)) != pdTRUE)
    {
        Serial.println("Message queue full, message dropped");
        return false;
    }

    return true;
}

bool Dn_Websocket::isWebSocketConnected()
{
    return isConnected;
}

void Dn_Websocket::loop()
{
    // Check connection status
    if (!isConnected)
    {
        Serial.println("WebSocket disconnected, checking task...");

        // If task died, attempt to restart it
        if (webSocketTaskHandle == NULL && isRunning)
        {
            Serial.println("WebSocket task died, attempting restart...");
            stop();
            delay(1000);
            // Restart would need to be handled at the application level
        }
    }
}

void Dn_Websocket::handleMessage(char *payload)
{
    // Process incoming messages here
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }

    // Example: Check if message is a response to username registration
    if (doc.containsKey("type") && doc["type"] == "registration")
    {
        if (doc["status"] == "success")
        {
            Serial.println("Username registration successful!");
        }
        else
        {
            Serial.println("Username registration failed");
        }
    }

    // Handle incoming chat messages
    if (doc.containsKey("type") && doc["type"] == "message")
    {
        // Use strcmp for string comparison since we're using char arrays
        const char *from = doc["from"];
        const char *message = doc["message"];

        lastMessage = message;
    }

    // Force garbage collection after JSON processing
    ESP.getFreeHeap();
}

void Dn_Websocket::registerUsername()
{
    if (username[0] != '\0')
    {
        // Create JSON message for registration
        JsonDocument doc;
        doc["type"] = "register";
        doc["username"] = username;

        char jsonBuffer[128];
        size_t len = serializeJson(doc, jsonBuffer, sizeof(jsonBuffer));

        // Send registration message
        webSocket.sendTXT(jsonBuffer, len);
        Serial.println("Username registration sent");
    }
}