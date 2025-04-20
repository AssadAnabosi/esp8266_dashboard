var selectedSSID = "";
var selectedEnc = "";
var modalOpen = false;

var access_points = [];

function drawScan() {
  var html;
  var width;
  var color;
  var ssid;
  var channel;
  var rssi;
  var enc;
  var mac;
  var connected;

  // Access Points
  getE("apNum").innerHTML = access_points.length;
  html =
    "<tr>" +
    "<th class='ssid'>SSID</th>" +
    "<th class='ch'>Ch</th>" +
    "<th class='rssi'>RSSI</th>" +
    "<th class='enc'></th>" +
    "<th class='lock'></th>" +
    "<th class='mac'>MAC</th>" +
    "<th class='connect'></th>" +
    "</tr>";

  for (var i = 0; i < access_points.length; i++) {
    ssid = access_points[i].ssid;
    channel = access_points[i].channel;
    rssi = access_points[i].rssi;
    enc = access_points[i].enc;
    mac = access_points[i].mac;
    connected = access_points[i].connected;
    width = parseInt(rssi) + 130;

    if (width < 50) color = "meter_red";
    else if (width < 70) color = "meter_orange";
    else color = "meter_green";
    html +=
      "<tr>" +
      "<td class='ssid'>" +
      esc(ssid) +
      "</td>" + // SSID
      "<td class='ch'>" +
      esc(channel) +
      "</td>" + // Ch
      // RSSI
      "<td class='rssi'><div class='meter_background'> <div class='meter_foreground " +
      color +
      "' style='width: " +
      width +
      "%;'><div class='meter_value'>" +
      rssi +
      "</div></div> </div></td>" +
      "<td class='enc'>" +
      esc(enc) +
      "</td>" + // ENC
      "<td class='lock'>" +
      (enc == "OPEN" ? "&#x1F513;" : "&#x1f512;") +
      "</td>" + // Lock Emoji
      "<td class='mac'>" +
      esc(mac) +
      "</td>"; // MAC
    // action button
    html += connected
      ? "<td class='connect'><button class='red' onclick='disconnect()'>Disconnect</button></td>"
      : "<td class='connect'><button class='red' onclick='connect(\"" +
        ssid +
        "\")'>Connect</button></td>";
    html += "</tr>";
  }

  getE("apTable").innerHTML = html;
}

function scan() {
  getE("scan_ap").disabled = true;
  apiCall({
    route: "/api/scan",
    callback: function (res) {
      access_points = JSON.parse(res);
      showMessage("connected");
      drawScan();
      getE("scan_ap").disabled = false;
    },
    timeout: 8000,
    onTimeout: function () {
      showMessage("ERROR: timeout scanning for wifi networks");
      getE("scan_ap").disabled = false;
    },
    onError: function () {
      showMessage("ERROR: failed to scan for wifi networks");
      getE("scan_ap").disabled = false;
    },
  });
}

function connect(ssid) {
  var ap = access_points.find((ap) => ap.ssid === ssid);
  if (!ap) {
    alert("Network not found");
    return;
  }

  selectedSSID = ssid;
  selectedEnc = ap.enc;

  if (ap.enc === "NONE" || ap.enc === "OPEN") {
    // No password needed
    sendConnectRequest(ssid, "");
  } else {
    // Show password modal
    handleModal({
      title: "Enter password for " + ssid,
      input: true,
      buttonText: "Connect",
    });

    getE("modal-button").onclick = function () {
      var password = getE("modal-input").value;
      getE("network-modal").style.display = "none";
      sendConnectRequest(selectedSSID, password);
    };
  }
}

function sendConnectRequest(ssid, password) {
  apiCall({
    route: "/api/connect",
    method: "POST",
    data: { ssid: ssid, password: password },
    callback: function (res) {
      showMessage("Connected to " + ssid);
      scan();
    },
    timeout: 8000,
    onTimeout: function () {
      showMessage("ERROR: timeout connecting to wifi network");
    },
    onError: function () {
      showMessage("ERROR: failed to connect to wifi network");
    },
  });
}

function sendDisconnectRequest() {
  apiCall({
    route: "/api/disconnect",
    method: "POST",
    callback: function () {
      scan();
    },
    timeout: 8000,
    onTimeout: function () {
      showMessage("ERROR: timeout disconnecting from wifi network");
    },
    onError: function () {
      showMessage("ERROR: failed to disconnect from wifi network");
    },
  });
}

function handleModal({ title, input, buttonText }) {
  getE("modal-title").innerText = title;
  getE("modal-button").innerText = buttonText;
  if (input) {
    getE("modal-input").style.display = "block";
    getE("modal-input").value = "";
    getE("modal-input").focus();

    getE("modal-input").addEventListener("keydown", function (event) {
      if (event.key === "Enter") {
        getE("modal-button").click();
      }
    });

    getE("modal-input").onkeyup = function (event) {
      if (event.key === "Enter") {
        getE("modal-button").click();
      }
    };

    getE("modal-input").oninput = function () {
      if (getE("modal-input").value.length >= 8) {
        getE("modal-button").disabled = false;
      } else {
        getE("modal-button").disabled = true;
      }
    };

    getE("modal-button").disabled = true;
  } else {
    getE("modal-input").style.display = "none";
    getE("modal-button").disabled = false;
  }

  // Display the modal
  getE("network-modal").style.display = "block";
  getE("close-modal").onclick = function () {
    getE("network-modal").style.display = "none";
  };
  document.addEventListener("keydown", function (event) {
    if (event.key === "Escape") {
      getE("network-modal").style.display = "none";
    }
  });
  window.onclick = function (event) {
    if (event.target == getE("network-modal")) {
      getE("network-modal").style.display = "none";
    }
  };
}

function disconnect() {
  handleModal({
    title: "Are you sure you want to disconnect?",
    input: false,
    buttonText: "Disconnect",
  });

  getE("modal-button").onclick = function () {
    getE("network-modal").style.display = "none";
    sendDisconnectRequest();
  };
}

function buttonFunc() {
  getE("scan_ap").disabled = false;
}

function load() {
  scan();
}
