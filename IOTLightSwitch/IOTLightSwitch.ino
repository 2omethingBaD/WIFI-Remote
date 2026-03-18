// Load Wi-Fi library
#include <WiFi.h>
#include <WebServer.h>

// Wifi creds, change to what you want!
#define ssid "My_Wifi_Remote"
#define password "12345678"

// object pin
#define relay_pin 6

IPAddress local_ip(192,168,1,1); // enter 192.168.1.1 to get to webpage once connected
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

// Set web server port number to 80
WebServer server(80);

// status checks
bool momentary_mode = false;  // false = latching, true = momentary
bool relay_on = false;


void setup()
{
  Serial.begin(115200);

  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin, LOW);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(1000);

  server.on("/", handle_OnConnect);
  // Latching toggle endpoint
  server.on("/handTurnOnOff", hand_TurnOnOff);
  // Momentary endpoints
  server.on("/on", handle_RelayOn);
  server.on("/off", handle_RelayOff);
  // Mode toggle endpoint
  server.on("/ModeChangeSelected", handle_ModeSwitch);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP Server Started");
}


void loop()
{
  server.handleClient();
}


String getHTML()
{
  String htmlCode = "<!DOCTYPE html><html lang=\"en\">\n";
  htmlCode += "<head>\n";

  // Mobile-optimized viewport: lock scale to prevent zoom/tilt resizing
  htmlCode += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no, viewport-fit=cover\">\n";
  htmlCode += "<meta name=\"apple-mobile-web-app-capable\" content=\"yes\">\n";
  htmlCode += "<meta name=\"apple-mobile-web-app-status-bar-style\" content=\"black-translucent\">\n";
  htmlCode += "<meta charset=\"UTF-8\">\n";
  htmlCode += "<title>My Wifi Remote</title>\n";
  htmlCode += "<link rel=\"icon\" href=\"data:,\">\n";

  // CSS (dark mode + pastel yellow theme, mobile-first)
  htmlCode += "<style>\n";
  htmlCode += ":root{\n";
  htmlCode += "  --bg:#0b0c10;\n";
  htmlCode += "  --card:#12141b;\n";
  htmlCode += "  --card2:#0f1117;\n";
  htmlCode += "  --text:#e8eaf1;\n";
  htmlCode += "  --muted:#a7acbd;\n";
  htmlCode += "  --border:rgba(255,255,255,.08);\n";
  htmlCode += "  --shadow:rgba(0,0,0,.55);\n";
  htmlCode += "  --yellow:#f7e38b;      /* pastel yellow */\n";
  htmlCode += "  --yellow2:#ffe9a8;\n";
  htmlCode += "  --yellowGlow:rgba(247,227,139,.28);\n";
  htmlCode += "  --danger:#ff6b6b;\n";
  htmlCode += "}\n";

  // Reset + mobile gesture/tap friendliness
  htmlCode += "*{box-sizing:border-box;-webkit-tap-highlight-color:transparent;}\n";
  htmlCode += "html,body{height:100%;}\n";
  htmlCode += "body{\n";
  htmlCode += "  margin:0;\n";
  htmlCode += "  font-family: ui-sans-serif, system-ui, -apple-system, Segoe UI, Roboto, Arial;\n";
  htmlCode += "  color:var(--text);\n";
  htmlCode += "  background: radial-gradient(900px 700px at 50% -10%, rgba(247,227,139,.12), transparent 60%),\n";
  htmlCode += "             radial-gradient(700px 550px at 10% 20%, rgba(255,255,255,.05), transparent 55%),\n";
  htmlCode += "             var(--bg);\n";
  htmlCode += "  display:flex;\n";
  htmlCode += "  align-items:center;\n";
  htmlCode += "  justify-content:center;\n";
  htmlCode += "  padding: max(16px, env(safe-area-inset-top)) 16px max(16px, env(safe-area-inset-bottom));\n";
  htmlCode += "  overscroll-behavior: none;\n";
  htmlCode += "  -webkit-user-select:none; user-select:none;\n";
  htmlCode += "}\n";

  // Container card
  htmlCode += ".wrap{width:min(520px, 100%);}\n";
  htmlCode += ".card{\n";
  htmlCode += "  background: linear-gradient(180deg, rgba(255,255,255,.04), transparent 22%),\n";
  htmlCode += "              linear-gradient(180deg, var(--card), var(--card2));\n";
  htmlCode += "  border:1px solid var(--border);\n";
  htmlCode += "  border-radius:24px;\n";
  htmlCode += "  padding:18px;\n";
  htmlCode += "  box-shadow: 0 18px 50px var(--shadow);\n";
  htmlCode += "}\n";

  // Header
  htmlCode += ".top{display:flex;align-items:center;justify-content:space-between;gap:12px;margin-bottom:14px;}\n";
  htmlCode += ".title{display:flex;flex-direction:column;gap:4px;}\n";
  htmlCode += ".title h1{margin:0;font-size:18px;letter-spacing:.3px;font-weight:800;}\n";
  htmlCode += ".title p{margin:0;color:var(--muted);font-size:13px;}\n";

  // Mode pill
  htmlCode += ".pill{\n";
  htmlCode += "  display:inline-flex;align-items:center;gap:8px;\n";
  htmlCode += "  padding:9px 12px;\n";
  htmlCode += "  border-radius:999px;\n";
  htmlCode += "  border:1px solid rgba(247,227,139,.25);\n";
  htmlCode += "  background: rgba(247,227,139,.10);\n";
  htmlCode += "  color: var(--yellow2);\n";
  htmlCode += "  font-size:12px;\n";
  htmlCode += "  white-space:nowrap;\n";
  htmlCode += "}\n";
  htmlCode += ".dot{width:9px;height:9px;border-radius:50%;background:var(--yellow);box-shadow:0 0 14px var(--yellowGlow);}\n";

  // Buttons area
  htmlCode += ".btns{display:grid;grid-template-columns:1fr;gap:12px;margin-top:12px;}\n";

  // Primary control button (big, thumb-friendly)
  htmlCode += ".button{\n";
  htmlCode += "  width:100%;\n";
  htmlCode += "  border:none;\n";
  htmlCode += "  border-radius:18px;\n";
  htmlCode += "  padding:18px 16px;\n";
  htmlCode += "  font-size:22px;\n";
  htmlCode += "  font-weight:900;\n";
  htmlCode += "  letter-spacing:.6px;\n";
  htmlCode += "  color:#131416;\n";
  htmlCode += "  background: linear-gradient(180deg, var(--yellow2), var(--yellow));\n";
  htmlCode += "  box-shadow: 0 10px 24px rgba(0,0,0,.45), 0 0 0 1px rgba(247,227,139,.25) inset;\n";
  htmlCode += "  cursor:pointer;\n";
  htmlCode += "  touch-action: manipulation;\n";
  htmlCode += "}\n";
  htmlCode += ".button:active{transform: translateY(1px); filter: brightness(.98);}\n";

  // Secondary mode button
  htmlCode += ".mode-button{\n";
  htmlCode += "  width:100%;\n";
  htmlCode += "  border-radius:16px;\n";
  htmlCode += "  padding:14px 14px;\n";
  htmlCode += "  font-size:15px;\n";
  htmlCode += "  font-weight:800;\n";
  htmlCode += "  letter-spacing:.2px;\n";
  htmlCode += "  border:1px solid rgba(255,255,255,.10);\n";
  htmlCode += "  color: var(--text);\n";
  htmlCode += "  background: rgba(255,255,255,.06);\n";
  htmlCode += "  box-shadow: 0 10px 22px rgba(0,0,0,.35);\n";
  htmlCode += "  cursor:pointer;\n";
  htmlCode += "  touch-action: manipulation;\n";
  htmlCode += "}\n";
  htmlCode += ".mode-button:active{transform: translateY(1px); filter: brightness(1.05);}\n";

  // Footer helper text
  htmlCode += ".hint{margin-top:14px;color:var(--muted);font-size:12px;line-height:1.35;}\n";
  htmlCode += ".kbd{display:inline-block;padding:2px 8px;border-radius:999px;border:1px solid rgba(255,255,255,.10);background:rgba(255,255,255,.05);color:var(--yellow2);}\n";

  // Slightly larger spacing on very tall screens
  htmlCode += "@media (min-height: 800px){ .card{padding:22px;} .button{padding:20px 16px;} }\n";

  htmlCode += "</style>\n";
  htmlCode += "</head>\n";

  htmlCode += "<body>\n";
  htmlCode += "<div class=\"wrap\">\n";
  htmlCode += "  <div class=\"card\">\n";

  // Header row
  htmlCode += "    <div class=\"top\">\n";
  htmlCode += "      <div class=\"title\">\n";
  htmlCode += "        <h1>My Wi-Fi Remote</h1>\n";
  htmlCode += "        <p>Tap to control • Optimized for mobile</p>\n";
  htmlCode += "      </div>\n";

  // Mode pill (dynamic)
  htmlCode += "      <div class=\"pill\"><span class=\"dot\"></span> ";
  htmlCode += (momentary_mode ? "Momentary" : "Latching");
  htmlCode += "</div>\n";
  htmlCode += "    </div>\n";

  // Buttons
  htmlCode += "    <div class=\"btns\">\n";

  if (momentary_mode)
  {
    htmlCode += "      <button class=\"button\" "
              "onmousedown=\"setRelay('on');\" ontouchstart=\"setRelay('on');\" "
              "onmouseup=\"setRelay('off');\" ontouchend=\"setRelay('off');\" "
              "ontouchcancel=\"setRelay('off');\" "
              "onmouseleave=\"setRelay('off');\">HOLD</button>\n";
  }
  else
  {
    htmlCode += "      <button class=\"button\" onclick=\"handTurnOnOff()\">ON / OFF</button>\n";
  }

  htmlCode += "      <button id=\"modeButton\" class=\"mode-button\" onclick=\"handleModeSwitch()\">Switch Button Mode</button>\n";
  htmlCode += "    </div>\n";

  // Helpful hint
  htmlCode += "    <div class=\"hint\">";
  htmlCode += momentary_mode
              ? "In <span class=\"kbd\">Momentary</span> mode, press and hold to keep the relay on."
              : "In <span class=\"kbd\">Latching</span> mode, tap to toggle the relay on/off.";
  htmlCode += "</div>\n";

  htmlCode += "  </div>\n";
  htmlCode += "</div>\n";

  // JS (same behavior + prevent double-tap zoom on iOS)
  htmlCode += "<script>\n";

  // Prevent iOS double-tap to zoom (common “tilt/zoom” annoyance)
  htmlCode += "let lastTouchEnd = 0;\n";
  htmlCode += "document.addEventListener('touchend', function (e) {\n";
  htmlCode += "  const now = (new Date()).getTime();\n";
  htmlCode += "  if (now - lastTouchEnd <= 300) { e.preventDefault(); }\n";
  htmlCode += "  lastTouchEnd = now;\n";
  htmlCode += "}, { passive: false });\n";

  // Momentary on/off
  htmlCode += "function setRelay(state) {\n";
  htmlCode += "  var xhr = new XMLHttpRequest();\n";
  htmlCode += "  xhr.open('GET', '/' + state, true);\n";
  htmlCode += "  xhr.send();\n";
  htmlCode += "}\n";

  // Latching toggle
  htmlCode += "function handTurnOnOff() {\n";
  htmlCode += "  var xhr = new XMLHttpRequest();\n";
  htmlCode += "  xhr.open('GET', '/handTurnOnOff', true);\n";
  htmlCode += "  xhr.onload = function(){ location.reload(); };\n";
  htmlCode += "  xhr.send();\n";
  htmlCode += "}\n";

  // Toggle mode and reload
  htmlCode += "function handleModeSwitch() {\n";
  htmlCode += "  var xhr = new XMLHttpRequest();\n";
  htmlCode += "  xhr.open('GET', '/ModeChangeSelected', true);\n";
  htmlCode += "  xhr.onload = function(){ location.reload(); };\n";
  htmlCode += "  xhr.send();\n";
  htmlCode += "}\n";

  htmlCode += "</script>\n";

  htmlCode += "</body></html>\n";
  return htmlCode;
}


// Handlers
void hand_TurnOnOff()
{
  relay_on = !relay_on;
  digitalWrite(relay_pin, relay_on ? HIGH : LOW);
  Serial.println(relay_on ? "Relay is on" : "Relay is off");
  server.send(200, "text/plain", "OK");
}


void handle_RelayOn()
{
  relay_on = true;
  digitalWrite(relay_pin, HIGH);
  Serial.println("Relay ON (momentary press)");
  server.send(200, "text/plain", "OK");
}


void handle_RelayOff()
{
  relay_on = false;
  digitalWrite(relay_pin, LOW);
  Serial.println("Relay OFF (momentary release)");
  server.send(200, "text/plain", "OK");
}


void handle_ModeSwitch()
{
  momentary_mode = !momentary_mode;
  Serial.println(momentary_mode ? "Momentary button mode active" : "Latching button mode active");
  relay_on = false;
  digitalWrite(relay_pin, LOW);
  server.send(200, "text/plain", "OK");
}


void handle_OnConnect()
{
  server.send(200, "text/html", getHTML());
}


void handle_NotFound()
{
  server.send(404, "text/plain", "Not Found");
}