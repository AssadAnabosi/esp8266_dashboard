function getE(name) {
  return document.getElementById(name);
}

function esc(str) {
  if (str) {
    return str
      .toString()
      .replace(/&/g, "&amp;")
      .replace(/</g, "&lt;")
      .replace(/>/g, "&gt;")
      .replace(/\"/g, "&quot;")
      .replace(/\'/g, "&#39;")
      .replace(/\//g, "&#x2F;");
  }
  return "";
}

function convertLineBreaks(str) {
  if (str) {
    str = str.toString();
    str = str.replace(/(?:\r\n|\r|\n)/g, "<br>");
    return str;
  }
  return "";
}

function showMessage(msg) {
  if (msg.startsWith("ERROR")) {
    getE("status").style.backgroundColor = "#d33";
    getE("status").innerHTML = "disconnected";
    console.error("disconnected (" + msg + ")");
  } else if (msg.startsWith("LOADING")) {
    getE("status").style.backgroundColor = "#fc0";
    getE("status").innerHTML = "loading...";
  } else {
    getE("status").style.backgroundColor = "#3c5";
    getE("status").innerHTML = "connected";
  }
}

function apiCall({
  route,
  callback = function () {},
  timeout = 8000,
  method = "GET",
  onTimeout = function () {
    showMessage("ERROR: request timeout for" + route);
  },
  onError = function () {
    showMessage("ERROR: request failed for" + route);
  },
  data = null,
}) {
  var request = new XMLHttpRequest();

  request.open(method, encodeURI(route), true);
  request.setRequestHeader("Content-Type", "application/json");
  request.timeout = timeout;
  request.ontimeout = onTimeout;
  request.onerror = onError;
  request.overrideMimeType("application/json");

  request.onreadystatechange = function () {
    if (this.readyState == 4) {
      if (this.status == 200) {
        showMessage("CONNECTED");
        callback(this.responseText);
      } else {
        showMessage(
          "ERROR: request failed for '" +
            route +
            "' with status: " +
            this.status
        );
        // onError();
      }
    }
  };

  showMessage("LOADING");

  /* send request */
  data ? request.send(JSON.stringify(data)) : request.send();
}

function connected() {
  if (typeof load !== "undefined") load();
}

function checkConnection() {
  apiCall({
    route: "/api/health",
    callback: connected,
    timeout: 2000,
    method: "GET",
    onTimeout: function () {
      setTimeout(checkConnection, 2000);
    },
    onError: function () {
      setTimeout(checkConnection, 2000);
    },
  });
}

window.addEventListener("load", function () {
  getE("status").style.backgroundColor = "#3c5";
  getE("status").innerHTML = "connected";
});
