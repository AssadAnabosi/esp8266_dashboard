var settingsJson = {};
var settingsDescJson = {
  ap_ssid: {
    label: "AP SSID",
    description:
      "SSID of access point used for the web interface (if enabled).\nThe length must be between 1 and 31 characters.",
  },
  ap_password: {
    label: "AP Password",
    description:
      "Password of access point used for the web interface (if enabled).\nThe length must be between 8 and 31 characters.",
  },
  ap_channel: {
    label: "AP Channel",
    description: "Default WiFi channel that is used when starting.",
  },
  ap_hidden: {
    label: "AP Hidden",
    description:
      "Hides the access point that is used for the web interface (if enabled).",
  },
  ap_status: {
    label: "Access Point",
    description:
      "Enables the access point that is used for the web interface (if enabled).",
  },
  hostname: {
    label: "Hostname",
    description:
      "Hostname of the device. This is used to identify the device on the network.",
  },
};

function load() {
  apiCall({
    route: "/api/settings",
    method: "GET",
    callback: function (res) {
      settingsJson = JSON.parse(res);
      showMessage("connected");
      draw();
    },
    timeout: 8000,
    onTimeout: function () {
      showMessage("ERROR: timeout loading settings");
    },
    onError: function () {
      showMessage("ERROR: failed to load settings");
    },
  });
}

function draw() {
  var html = "";
  for (var key in settingsJson) {
    key = esc(key);
    if (
      settingsJson.hasOwnProperty(key) &&
      settingsDescJson.hasOwnProperty(key)
    ) {
      html +=
        "<div class='row'>" +
        "<div class='col-6'>" +
        "<label class='settingName " +
        (typeof settingsJson[key] == "boolean" ? "labelFix" : "") +
        "' for='" +
        key +
        "'>" +
        settingsDescJson[key].label +
        ":</label>" +
        "</div>" +
        "<div class='col-6'>";

      if (typeof settingsJson[key] == "boolean") {
        html +=
          "<label class='checkBoxContainer'><input type='checkbox' name='" +
          key +
          "' id='" +
          key +
          "' " +
          (settingsJson[key] ? "checked" : "") +
          " ><span class='checkmark'></span></label>";
      } else if (typeof settingsJson[key] == "number") {
        html +=
          "<input type='number' name='" +
          key +
          "' value=" +
          settingsJson[key] +
          " id='" +
          key +
          "' " +
          " >";
      } else if (typeof settingsJson[key] == "string") {
        html +=
          "<input type='text' name='" +
          key +
          "' value='" +
          settingsJson[key].toString() +
          "' " +
          (key == "version" ? "readonly" : "") +
          " id='" +
          key +
          "' " +
          " >";
      }

      html +=
        "</div>" +
        "</div>" +
        "<div class='row'>" +
        "<div class='col-12'>" +
        "<p>" +
        convertLineBreaks(esc(settingsDescJson[key].description)) +
        "</p>" +
        "<hr>" +
        "</div>" +
        "</div>";
    }
  }
  getE("settingsList").innerHTML = html;
}

function save() {
  handleModal({
    title: "Save settings",
    buttonText: "Save",
    onclick: function () {
      let newSettings = {
        ap_ssid: getE("ap_ssid").value,
        ap_password: getE("ap_password").value,
        ap_channel: getE("ap_channel").value,
        ap_hidden: getE("ap_hidden").checked,
        ap_status: getE("ap_status").checked,
        hostname: getE("hostname").value,
      };
      apiCall({
        route: "/api/settings",
        method: "POST",
        data: newSettings,
        timeout: 8000,
      });
      showMessage("Saving settings...");
      alert("Saving... page will reload in 30 seconds");
      setTimeout(function () {
        window.location.reload();
      }, 30000);
    },
  });
}

function reset() {
  handleModal({
    title: "Reset settings",
    buttonText: "Reset",
    onclick: function () {
      apiCall({
        route: "/api/reset",
        method: "POST",
      });
      showMessage("Resetting settings...");
      alert("Resetting... page will reload in 30 seconds");
      setTimeout(function () {
        window.location.reload();
      }, 30000);
    },
  });
}

function reboot() {
  handleModal({
    title: "Reboot device",
    buttonText: "Reboot",
    onclick: function () {
      apiCall({
        route: "/api/reboot",
        method: "POST",
      });
      showMessage("Rebooting...");
      alert("Rebooting... page will reload in 30 seconds");
      setTimeout(function () {
        window.location.reload();
      }, 30000);
    },
  });
}

function handleModal({ title, buttonText, onclick }) {
  getE("modal-title").innerText = title;
  getE("modal-button").innerText = buttonText;

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

  getE("modal-button").onclick = function () {
    getE("network-modal").style.display = "none";
    if (onclick) onclick();
  };
}
