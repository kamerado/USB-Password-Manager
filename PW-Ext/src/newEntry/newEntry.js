const form = document.getElementById("loginForm");

form.addEventListener("submit", (event) => {
  event.preventDefault(); // Prevent default form submission

  const username = document.getElementById("username").value;
  const password = document.getElementById("password").value;

  // Do something with the username and password (e.g., send to server for validation)
  console.log("Username:", username);
  console.log("Password:", password);
  chrome.runtime.sendMessage({ action: "new-entry-data", username: toString(username), password: toString(password) })
});
