console.log("Content script initialization started");
(function () {
  "use strict";

  console.log("Content script IIFE started");

  function detectAuthPage() {
    try {
      const authSelectors = [
        'form[action*="login"]',
        'form[action*="signin"]',
        'form[action*="signup"]',
        'form[action*="register"]',
        '#login',
        '#signup',
        '#signin',
        '#register'
      ];
  
      const authInputTypes = [
        'input[type="password"]',
        'input[name*="password"]',
        'input[name*="email"]',
        'input[name*="username"]'
      ];
  
      const authKeywords = [
        'login',
        'log in',
        'Log in',
        'sign in',
        'signup',
        'sign up',
        'register',
        'create account',
        'Forgot your password?'
      ];
  
      const hasAuthForm = authSelectors.some(selector =>
        document.querySelector(selector) !== null
      );
  
      const hasAuthInputs = authInputTypes.some(selector =>
        document.querySelector(selector) !== null
      );
  
      const pageText = document.body ? document.body.innerText.toLowerCase() : '';
      const hasAuthKeywords = authKeywords.some(keyword =>
        pageText.includes(keyword.toLowerCase())
      );
  
      const findFields = (selectors) => {
        for (const selector of selectors) {
          const fields = Array.from(document.querySelectorAll(selector));
          if (fields.length > 0) return fields;
        }
        return [];
      };
  
      const passwordFields = findFields([
        'input[type="password"]',
        'input[name*="password"]',
        'input[id*="password"]',
        'input[class*="password"]'
      ]);
      console.log("Total password fields found:", passwordFields.length);
  
      const emailFields = findFields([
        'input[type="email"]',
        'input[name*="login"]',
        'input[name*="email"]',
        'input[id*="email"]',
        'input[class*="email"]'
      ]);
      console.log("Total email fields found:", emailFields.length);
  
      const usernameFields = findFields([
        'input[autocomplete*="username"]',
        'input[name*="username"]',
        'input[id*="username"]',
        'input[class*="username"]',
        'input[name*="login"]'
      ]);
      console.log("Total username fields found:", usernameFields.length);
  
      const hasPasswordField = passwordFields.length > 0;
      const hasEmailField = emailFields.length > 0;
      const hasUsernameField = usernameFields.length > 0;
  
      const confidence = [
        { condition: hasAuthForm, weight: 0.4 },
        { condition: hasAuthInputs, weight: 0.3 },
        { condition: hasAuthKeywords, weight: 0.2 },
        { condition: hasPasswordField, weight: 0.3 },
        { condition: hasEmailField, weight: 0.2 },
        { condition: hasUsernameField, weight: 0.2 }
      ].reduce((acc, { condition, weight }) => acc + (condition ? weight : 0), 0);
  
      let pageType = 'unknown';
      if (confidence >= 0.2) {
        const url = window.location.href.toLowerCase();
        if (url.includes('login') || url.includes('signin')) {
          pageType = 'login';
        } else if (url.includes('signup') || url.includes('register')) {
          pageType = 'signup';
        } else if (pageText.includes('signup') || pageText.includes("register") || pageText.includes("sign up") || pageText.includes("create account")) {
          pageType = 'signup';
        } else {
          pageType = 'login';
        }
      }
  
      return {
        isAuthPage: confidence > 0.5,
        confidence: Math.min(confidence, 1.0),
        pageType: pageType,
        fields: {
          passwordFields: passwordFields.map(field => field.name || field.id || ''),
          emailFields: emailFields.map(field => field.name || field.id || ''),
          usernameFields: usernameFields.map(field => field.name || field.id || '')
        },
        error: null
      };
    } catch (error) {
      console.error('Error in detectAuthPage:', error);
      return {
        isAuthPage: false,
        confidence: 0,
        pageType: 'unknown',
        error: error.message
      };
    }
  }
  
  function getLoginFields() {
    console.log("Starting login field detection");
    var fieldPairs = [],
      pswd = (function () {
        var inputs = document.getElementsByTagName('input'),
          len = inputs.length,
          ret = [];
        while (len--) {
          if (inputs[len].type === 'password') {
            ret[ret.length] = inputs[len];
          }
        }
        console.log("Found", ret.length, "password fields");
        return ret;
      })(),
      pswdLength = pswd.length;

    function parentForm(elem) {
      while (elem.parentNode) {
        if (elem.parentNode.nodeName.toLowerCase() === 'form') {
          return elem.parentNode;
        }
        elem = elem.parentNode;
      }
    }

    while (pswdLength--) {
      var curPswdField = pswd[pswdLength],
        form = parentForm(curPswdField);
      if (form) {
        var inputs = form.getElementsByTagName('input');
        for (var i = 0; i < inputs.length; i++) {
          if (inputs[i] !== curPswdField && inputs[i].type === 'text') {
            fieldPairs.push([window.location.hostname, inputs[i], curPswdField]);
            console.log("Found login field pair on", window.location.hostname);
            break;
          }
        }
      }
    }
    console.log("Total field pairs found:", fieldPairs.length);
    return fieldPairs;
  }

  async function reqLoginForm() {
    try {
      console.log("Sending loginFormDetected message to background");

      // check for www. in window.location.host, if not present, add it.
      let domain = window.location.host;
      if (domain.indexOf("www.") === -1) {
        console.log("www. not found in host, adding it");
        domain = "www." + window.location.host;
      }
      chrome.runtime.sendMessage(
        {
          action: "loginFormDetected",
          type: "request",
          request: {
            type: "check-entry",
            website: domain
          }
        }
      );
      console.log("Message sent to background script");
    } catch (error) {
      console.error("Error in checkForLoginForm:", error);
    }
  }

  function fillForm(username, password, email) {
    console.log("Attempting to fill");
    const results = detectAuthPage();
    if (results.isAuthPage === true) {
      console.log("Auth page detected with confidence:", results.confidence);
      console.log("Page type:", results.pageType);
      console.log("Fields:", results.fields);
      console.log("Filling form with provided credentials");
      if (results.pageType === "login") {
        console.log("Auth page detected as login page");

        // Fill username field if available
        if (results.fields.usernameFields.length > 0) {
          const usernameField = document.querySelector(`input[name="${results.fields.usernameFields[0]}"], input[id="${results.fields.usernameFields[0]}"]`);
          if (usernameField) {
            usernameField.value = username;
          }
        }

        // Fill email field if available
        if (results.fields.emailFields.length > 0) {
          const emailField = document.querySelector(`input[name="${results.fields.emailFields[0]}"], input[id="${results.fields.emailFields[0]}"]`);
          if (emailField) {
            emailField.value = email;
          }
        }

        // Fill password field if available
        if (results.fields.passwordFields.length > 0) {
          const passwordField = document.querySelector(`input[name="${results.fields.passwordFields[0]}"], input[id="${results.fields.passwordFields[0]}"]`);
          if (passwordField) {
            passwordField.value = password;
          }
        }
        console.log("Form filled with provided credentials");
      } else if (results.pageType === "signup") {
        console.log("Auth page detected as signup page");
        console.log("Auth page detected with confidence:", results.confidence);
        console.log("Page type:", results.pageType);
        console.log("Fields:", results.fields);
        console.log("Filling form with provided credentials");
        if (results.pageType === "signup") {
          console.log("Auth page detected as signup page");
  
          // // Fill username field if available
          // if (results.fields.usernameFields.length > 0) {
          //   const usernameField = document.querySelector(`input[name="${results.fields.usernameFields[0]}"], input[id="${results.fields.usernameFields[0]}"]`);
          //   if (usernameField) {
          //     usernameField.value = username;
          //   }
          // }
  
          // Fill email field if available
          if (results.fields.emailFields.length > 0) {
            const emailField = document.querySelector(`input[name="${results.fields.emailFields[0]}"], input[id="${results.fields.emailFields[0]}"]`);
            if (emailField) {
              emailField.value = email;
            }
          }
  
          // Fill password field if available
          if (results.fields.passwordFields.length > 0) {
            const passwordField = document.querySelector(`input[name="${results.fields.passwordFields[0]}"], input[id="${results.fields.passwordFields[0]}"]`);
            if (passwordField) {
              passwordField.value = password;
            }
          }

          // Fill username field if available
          if (results.fields.usernameFields.length > 0) {
            const usernameField = document.querySelector(`input[name="${results.fields.usernameFields[0]}"], input[id="${results.fields.usernameFields[0]}"]`);
            if (usernameField) {
              usernameField.value = username;
            }
          }
          console.log("Form filled with provided credentials");
        }
      } else {
        console.log("Auth page detected as unknown page");
      }
    } else {
      console.log("No login form detected for filling");
    }
  }

  async function runTask() {
    try {
      console.log("Starting runTask");
      reqLoginForm();
    } catch (error) {
      console.error("Error in runTask:", error.message);
    }
  }

  chrome.runtime.onMessage.addListener((request, sender, sendResponse) => {
    console.log("Message received in content script:", request);
    if (request.action === "fill-form") {
      console.log("Filling form with provided credentials");
      console.log("Request data:", request.data);
      console.log(request.data.username, request.data.password, request.data.email);
      fillForm(request.data.username, request.data.password, request.data.email);
      sendResponse({ status: "success" });
    }
    return true; // Keep the message channel open
  });

  window.addEventListener('load', () => {
    console.log("Initial runTask execution");
    const results = detectAuthPage();
    console.log("Results:", results);
    if (results.isAuthPage === true) {
      if (results.pageType === "login") {
        console.log("Auth page detected as login page");
        runTask();
      } else if (results.pageType === "signup") {
        console.log("Auth page detected as signup page");
      }
      console.log("Auth page detected with confidence:", results.confidence);
      runTask();
    }
  });
})();
