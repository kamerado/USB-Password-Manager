// Listen for a specific event on the page (e.g., button click)
document.addEventListener('click', (event) => {
    console.log("before click.");
    if (event.target.id === 'sendMessageButton') {
        // Send a message to the background script
        console.log("clicked.");
        chrome.runtime.sendMessage({
            action: "sendToNativeApp",
            data: { text: "Message from content script" }
        }, (response) => {
            // Handle the response from the background script
            console.log("Response from background script: ", response);
        });
    }
});