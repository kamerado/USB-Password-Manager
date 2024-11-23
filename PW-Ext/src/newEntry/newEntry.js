const form = document.getElementById("loginForm");
console.log("Login form detected. opening new entry form...");
var website;

chrome.runtime.onMessage.addListener((message, sender, sendResponse) => {
  if (message.type === "init") {
    // Process the data
    console.log("Received arguments:", message.data);
    website = message.data.website;
    sendResponse({ status: "success" });
  }
});
function genPass(length = 16) {
  const chars = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+-=[]{};':\"\\|,.<>/?";
  let password = "";

  const array = new Uint32Array(length);
  window.crypto.getRandomValues(array);

  for (let i = 0; i < length; i++) {
    const randomIndex = array[i] % chars.length;
    password += chars.charAt(randomIndex);
  }

  return password;
}
// When submit or generate button is clicked
function handleSubmitOrGenerate() {
  // Your existing submit/generate logic here

  // After successful submission/generation, send message to close window
  chrome.runtime.sendMessage({
    action: "closeNewEntryWindow"
  }, (response) => {
    console.log("Window close request sent");
  });
}
// Function to get query parameters from the URL
function getQueryParams() {
  const params = {};
  const queryString = window.location.search.substring(1);
  const queryArray = queryString.split("&");
  queryArray.forEach((param) => {
    const [key, value] = param.split("=");
    params[decodeURIComponent(key)] = decodeURIComponent(value);
  });
  return params;
}

// Get the query parameters
// const queryParams = getQueryParams();
// const website = queryParams.website || "";

form.addEventListener("submit", (event) => {
  event.preventDefault(); // prevent default form submission
  const username = document.getElementById("username").value;
  const password = document.getElementById("password").value;

  console.log("Username:", username);
  console.log("Password:", password);
  chrome.runtime.sendMessage({ action: "new-entry-data", website: website, username: username, password: password });
  handleSubmitOrGenerate();
});

document.getElementById("generate").addEventListener("click", async (event) => {
  event.preventDefault(); // prevent default form submission
  console.log("Generating new username and password...");

  // chrome.runtime.sendMessage({ action: "get-default-username" }, (response) => {
  //   if (response.username) {
  //     document.getElementById("username").value = response.username;
  //   }
  // })

  const password = genPass();
  document.getElementById("password").value = password;
  console.log("Generated password:", password);

  const username = "";

  // Do something with the username and password (e.g., send to server for validation)
  console.log("Generated username:", username);
  console.log("Generated password:", password);
  chrome.runtime.sendMessage({
    action: "new-entry-data",
    website: website,
    username: document.getElementById("username").value,
    password: password
  })
  handleSubmitOrGenerate()
});
