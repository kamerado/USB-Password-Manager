function connectToNativeApp() {
    const nativeAppName = "com.kamerado.native_app";  // This should match your native app registration
    const port = chrome.runtime.connectNative(nativeAppName);
    
    port.onMessage.addListener((msg) => {
      console.log("Received message from native app: ", msg);
    });
  
    port.onDisconnect.addListener(() => {
      console.log("Disconnected from native app");
    });
  
    return port;
  }
  
  // Sending a message to the native app
  function sendMessageToNativeApp(message) {
    const port = connectToNativeApp();
    port.postMessage(message);
  }
  
  // Example usage
  chrome.action.onClicked.addListener(() => {
    sendMessageToNativeApp({ text: "Hello from the extension" });
  });