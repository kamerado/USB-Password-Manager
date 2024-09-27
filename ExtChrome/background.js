let port = null;

function connectToNativeHost() {
  port = chrome.runtime.connectNative('com.kamerado.native_app');

  port.onMessage.addListener((message) => {
    console.log('Received from native app:', message);
    chrome.runtime.sendMessage({
      type: 'MESSAGE_FROM_NATIVE_APP',
      payload: message,
    });
  });

  port.onDisconnect.addListener(() => {
    console.log('Disconnected from native app');
    port = null;
  });
}

connectToNativeHost();

function sendMessageToNativeApp(message) {
  if (port) {
    port.postMessage(message);
  } else {
    console.error('Port is not connected');
  }
}

chrome.runtime.onMessage.addListener((request, sender, sendResponse) => {
  if (request.type === 'SEND_TO_NATIVE_APP') {
    sendMessageToNativeApp(request.payload);
    sendResponse({ status: 'Message sent to native app' });
  }
});

