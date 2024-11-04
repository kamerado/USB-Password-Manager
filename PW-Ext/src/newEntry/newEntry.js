const form = document.getElementById("loginForm");

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

form.addeventlistener("submit", (event) => {
  event.preventdefault(); // prevent default form submission

  const username = document.getElementById("username").value;
  const password = document.getElementById("password").value;

  // Do something with the username and password (e.g., send to server for validation)
  console.log("Username:", username);
  console.log("Password:", password);
  chrome.runtime.sendMessage({ action: "new-entry-data", username: toString(username), password: toString(password) })
});

form.addEventListener("generate", (event) => {
  event.preventDefault(); // Prevent default form submission
  const username = chrome.runtime.sendMessage({ action: "get-default-username" }, (response) => {
    if (response.username) {
      return response.username;
    }
  })

  const password = genPass();
  document.getElementById("username").value = username;
  document.getElementById("password").value = password;

  // Do something with the username and password (e.g., send to server for validation)
  console.log("Generated username:", username);
  console.log("Generated password:", password);
  chrome.runtime.sendMessage({ action: "new-entry-data", username: toString(username), password: toString(password) })
});
