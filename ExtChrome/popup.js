document.addEventListener('DOMContentLoaded', () => {
  const sendButton = document.getElementById('sendButton');
  const messageInput = document.getElementById('messageInput');
  const responseArea = document.getElementById('responseArea');

  sendButton.addEventListener('click', () => {
    const message = messageInput.value;
    chrome.runtime.sendMessage(
      {
        type: 'SEND_TO_NATIVE_APP',
        payload: { text: message },
      },
      (response) => {
        console.log(response.status);
      }
    );
  });

  chrome.runtime.onMessage.addListener((request, sender, sendResponse) => {
    if (request.type === 'MESSAGE_FROM_NATIVE_APP') {
      responseArea.textContent = 'Received: ' + JSON.stringify(request.payload);
      sendResponse({ status: 'Message received in popup' });
    }
  });
});

