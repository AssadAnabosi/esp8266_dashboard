var connections_status = {
  softAP: { enabled: false, ssid: "", ip: "" },
  station: { connected: false, ssid: "", ip: "" },
};

const wifiIcon =
  "<svg xmlns='http://www.w3.org/2000/svg' width='24' height='24' viewBox='0 0 24 24' fill='none' stroke='currentColor' stroke-width='2' stroke-linecap='round' stroke-linejoin='round' class='icon icon-true'><path d='M12 20h.01'></path><path d='M2 8.82a15 15 0 0 1 20 0'></path><path d='M5 12.859a10 10 0 0 1 14 0'></path><path d='M8.5 16.429a5 5 0 0 1 7 0'></path></svg>";
const wifiDisableIcon =
  "<svg xmlns='http://www.w3.org/2000/svg' width='24' height='24' viewBox='0 0 24 24' fill='none' stroke='currentColor' stroke-width='2' stroke-linecap='round' stroke-linejoin='round' class='icon icon-false'><path d='M12 20h.01'></path><path d='M8.5 16.429a5 5 0 0 1 7 0'></path><path d='M5 12.859a10 10 0 0 1 5.17-2.69'></path><path d='M19 12.859a10 10 0 0 0-2.007-1.523'></path><path d='M2 8.82a15 15 0 0 1 4.177-2.643'></path><path d='M22 8.82a15 15 0 0 0-11.288-3.764'></path><path d='m2 2 20 20'></path></svg>";

function drawConnectionsStatus() {
  let ap_card = "";
  let station_card = "";

  // Access Point Card
  ap_card += "<div class='card'>"; // card
  // Badge
  ap_card += connections_status.softAP.enabled
    ? "<div class='card-badge badge-green'>Active</div>"
    : "<div class='card-badge badge-red'>Inactive</div>";
  // Header
  ap_card += "<div class='card-title'>Soft Access Point Mode</div>";
  ap_card +=
    "<div class='card-description'>Access point for direct connection</div>";
  // Content
  ap_card += "<div class='card-content'>"; // card-content
  ap_card += connections_status.softAP.enabled ? wifiIcon : wifiDisableIcon;
  ap_card += "<div style='width: 80%;'>";
  if (connections_status.softAP.enabled) {
    ap_card += "<div class='connection-details'>";
    ap_card += "<div>";
    ap_card += "<p class='text-sm text-muted-foreground'>SSID</p>";
    ap_card +=
      "<p class='font-medium'>" + connections_status.softAP.ssid + "</p>";
    ap_card += "</div>";
    ap_card += "<div>";
    ap_card +=
      "<p class='text-sm text-muted-foreground' style='margin-top: 0.25rem;'>IP Address</p>";
    ap_card +=
      "<p class='font-medium'>" + connections_status.softAP.ip + "</p>";
    ap_card += "</div>";
    ap_card += "</div>"; // connection-details
  } else {
    ap_card +=
      "<p class='text-muted-foreground text-sm'>Access Point is disabled</p>";
  }
  ap_card += "</div>";
  ap_card += "</div>"; // card-content
  ap_card += "</div>"; // card

  // Station Card

  station_card += "<div class='card'>"; // card
  // Badge
  station_card += connections_status.station.connected
    ? "<div class='card-badge badge-green'>Connected</div>"
    : "<div class='card-badge badge-red'>Disconnected</div>";
  // Header
  station_card += "<div class='card-title'>Station Mode</div>";
  station_card +=
    "<div class='card-description'>Connection to existing WiFi network</div>";
  // Content
  station_card += "<div class='card-content'>"; // card-content
  station_card += connections_status.station.connected
    ? wifiIcon
    : wifiDisableIcon;
  station_card += "<div style='width: 80%;'>";
  if (connections_status.station.connected) {
    station_card += "<div class='connection-details'>";
    station_card += "<div>";
    station_card += "<p class='text-sm text-muted-foreground'>Connected to</p>";
    station_card +=
      "<p class='font-medium'>" + connections_status.station.ssid + "</p>";
    station_card += "</div>";
    station_card += "<div>";
    station_card +=
      "<p class='text-sm text-muted-foreground' style='margin-top: 0.25rem;'>IP Address</p>";
    station_card +=
      "<p class='font-medium'>" + connections_status.station.ip + "</p>";
    station_card += "</div>";
    station_card += "</div>"; // connection-details
  } else {
    station_card +=
      "<p class='text-sm text-muted-foreground'>Not connected to any network</p>";
    station_card += "<p className='text-sm'>Go to Scan page to connect</p>";
  }
  station_card += "</div>";

  getE("status-container").innerHTML = ap_card + station_card;
}

function getConnectionsStatus() {
  drawConnectionsStatus(); // draw empty status first
  apiCall({
    route: "/api/status",
    callback: function (res) {
      connections_status = JSON.parse(res);
      drawConnectionsStatus();
    },
    timeout: 8000,
    onTimeout: function () {
      showMessage("ERROR: timeout getting connections status");
    },
    onError: function () {
      showMessage("ERROR: failed getting connections status");
    },
  });
}

function load() {
  getConnectionsStatus();
}
