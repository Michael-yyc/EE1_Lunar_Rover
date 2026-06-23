const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ZAMBAM Mission Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">

  <style>
    * {
      box-sizing: border-box;
      margin: 0;
      padding: 0;
    }

    :root {
      --bg: #10051a;
      --panel: #211036;
      --panel2: #2b1745;
      --line: rgba(255, 192, 203, 0.25);
      --text: #faf5fb;
      --dim: #cdbed6;
      --green: #83d68a;
      --red: #b94b63;
      --violet: #665EA6;
      --pink: #FFC0CB;
    }

    body {
      min-height: 100vh;
      font-family: Arial, sans-serif;
      color: var(--text);
      background:
        radial-gradient(circle at top left, #3a1d5c, transparent 35%),
        linear-gradient(135deg, #08030f, var(--bg));
      padding: 18px;
    }

    .shell {
      max-width: 1100px;
      margin: 0 auto;
    }

    .top-bar {
      display: flex;
      justify-content: space-between;
      align-items: center;
      gap: 16px;
      padding: 18px;
      margin-bottom: 18px;
      border: 1px solid var(--line);
      border-radius: 16px;
      background: linear-gradient(180deg, var(--panel), #160b25);
    }

    .brand h1 {
      font-size: 32px;
      letter-spacing: 6px;
    }

    .brand p {
      margin-top: 5px;
      color: var(--dim);
      font-size: 12px;
      letter-spacing: 2px;
    }

    .status {
      display: flex;
      align-items: center;
      gap: 10px;
      padding: 10px 12px;
      border: 1px solid var(--line);
      border-radius: 12px;
      background: rgba(255,255,255,0.05);
    }

    .dot {
      width: 11px;
      height: 11px;
      border-radius: 50%;
      background: var(--red);
      display: inline-block;
    }

    .dot.live {
      background: var(--green);
      box-shadow: 0 0 9px rgba(131, 214, 138, 0.7);
    }

    .status small {
      display: block;
      color: var(--dim);
      font-size: 11px;
      letter-spacing: 1px;
    }

    .status strong {
      font-size: 14px;
    }

    .grid {
      display: grid;
      grid-template-columns: 1.1fr 0.9fr;
      gap: 18px;
    }

    .panel {
      padding: 18px;
      border: 1px solid var(--line);
      border-radius: 16px;
      background: linear-gradient(180deg, var(--panel), #160b25);
      box-shadow: 0 0 18px rgba(102, 94, 166, 0.14);
    }

    .panel-header {
      display: flex;
      justify-content: space-between;
      align-items: center;
      margin-bottom: 14px;
    }

    .panel-header h2 {
      font-size: 17px;
      letter-spacing: 3px;
    }

    .badge {
      padding: 6px 10px;
      border-radius: 8px;
      border: 1px solid var(--line);
      background: rgba(102, 94, 166, 0.28);
      font-size: 12px;
      font-weight: bold;
    }

    .badge.live {
      color: #e7ffe8;
      border-color: rgba(131, 214, 138, 0.45);
    }

    .badge.dim {
      color: var(--dim);
    }

    .classification {
      text-align: center;
      padding: 18px;
      margin-bottom: 18px;
      border: 1px solid var(--line);
      border-radius: 14px;
      background: var(--panel2);
    }

    .classification p {
      color: var(--dim);
      margin-bottom: 8px;
      letter-spacing: 1px;
    }

    .classification h3 {
      font-size: 38px;
      letter-spacing: 3px;
      margin-bottom: 8px;
    }

    .sensor {
      margin-top: 14px;
      padding: 14px;
      border: 1px solid var(--line);
      border-radius: 12px;
      background: rgba(255,255,255,0.04);
    }

    .sensor-top {
      display: flex;
      justify-content: space-between;
      margin-bottom: 8px;
    }

    .sensor h4 {
      font-size: 13px;
      letter-spacing: 2px;
      color: var(--dim);
    }

    .sensor-value {
      font-size: 28px;
      font-weight: bold;
    }

    .age-value {
      font-size: 34px;
      font-weight: bold;
      margin-bottom: 10px;
    }

    .age-meta {
      color: var(--dim);
      line-height: 1.7;
    }

    #serialLog {
      min-height: 230px;
      margin-top: 10px;
      padding: 12px;
      border: 1px solid var(--line);
      border-radius: 12px;
      background: rgba(0,0,0,0.28);
      color: #eee5ef;
      white-space: pre-wrap;
      font-size: 13px;
      line-height: 1.5;
      overflow-y: auto;
    }

    .footer {
      margin-top: 14px;
      text-align: right;
      color: var(--dim);
      font-size: 14px;
    }

    @media (max-width: 800px) {
      .top-bar {
        flex-direction: column;
        align-items: flex-start;
      }

      .grid {
        grid-template-columns: 1fr;
      }

      .brand h1 {
        font-size: 26px;
      }
    }
  </style>
</head>

<body>
  <main class="shell">

    <header class="top-bar">
      <div class="brand">
        <h1>ZAMBAM</h1>
        <p>ROCK GEOLOGICAL CLASSIFIER — MISSION CONTROL</p>
      </div>

      <div class="status">
        <span id="telemetryDot" class="dot"></span>
        <div>
          <small>TELEMETRY</small>
          <strong id="telemetryText">OFFLINE</strong>
        </div>
      </div>
    </header>

    <section class="grid">

      <section class="panel">
        <div class="panel-header">
          <h2>TYPE</h2>
          <span id="typeCount" class="badge">0 ACTIVE</span>
        </div>

        <div class="classification">
          <p>Current Classification</p>
          <h3 id="rockType">---</h3>
          <p>Confidence: <span id="confidence">---</span>%</p>
        </div>

        <div class="sensor">
          <div class="sensor-top">
            <h4>INFRARED</h4>
            <span id="irDot" class="dot"></span>
          </div>
          <div id="irValue" class="sensor-value">---</div>
        </div>

        <div class="sensor">
          <div class="sensor-top">
            <h4>ULTRASONIC</h4>
            <span id="ultrasoundDot" class="dot"></span>
          </div>
          <div id="ultrasoundValue" class="sensor-value">---</div>
        </div>

        <div class="sensor">
          <div class="sensor-top">
            <h4>MAGNETIC FIELD</h4>
            <span id="magneticDot" class="dot"></span>
          </div>
          <div id="magneticValue" class="sensor-value">---</div>
        </div>
      </section>

      <section>
        <section class="panel">
          <div class="panel-header">
            <h2>AGE</h2>
            <span id="ageBadge" class="badge dim">INACTIVE</span>
          </div>

          <div id="ageValue" class="age-value">---</div>
        </section>

        <section class="panel" style="margin-top:18px;">
          <div class="panel-header">
            <h2>SERIAL MONITOR</h2>
            <span class="badge">115200 BAUD</span>
          </div>
          <pre id="serialLog">Waiting for sensor data...</pre>
        </section>
      </section>

    </section>

    <footer class="footer">
      Last Update: <span id="lastUpdate">---</span>
    </footer>

  </main>

  <script>
    const serialLog = document.getElementById("serialLog");
    let storedRocks = JSON.parse(localStorage.getItem("storedRocks")) || [];

    setInterval(fetchData, 1000);
    fetchData();

    async function fetchData() {
      try {
        const response = await fetch("/data");
        if (!response.ok) throw new Error("No data");

        const data = await response.json();
        updateDashboard(data);
      } catch (error) {
        setTelemetry(false);
        addSerialLog("[WAITING] No sensor data received");
      }
    }

    function updateDashboard(data) {
      setTelemetry(true);

      updateAge(data.age);
      updateType(data.rock);

      document.getElementById("lastUpdate").textContent =
        new Date().toLocaleTimeString();

      if (data.rock && data.rock.id) {
        storeRock(data);
      }

      addSerialLog(formatLog(data));
    }

    function setTelemetry(isOnline) {
      const dot = document.getElementById("telemetryDot");
      const text = document.getElementById("telemetryText");

      if (isOnline) {
        dot.className = "dot live";
        text.textContent = "LIVE";
      } else {
        dot.className = "dot";
        text.textContent = "OFFLINE";
      }
    }

    function updateAge(age) {
      const badge = document.getElementById("ageBadge");

      if (!age || !age.detected) {
        badge.textContent = "INACTIVE";
        badge.className = "badge dim";
        document.getElementById("ageValue").textContent = "---";
        return;
      }

      badge.textContent = "ACTIVE";
      badge.className = "badge live";

      document.getElementById("ageValue").textContent =
        formatAgeDisplay(age.value);
    }

    function updateType(rock) {
      if (!rock) {
        document.getElementById("rockType").textContent = "---";
        document.getElementById("confidence").textContent = "---";
        document.getElementById("typeCount").textContent = "0 ACTIVE";

        updateSignal(null, "irValue", "irDot");
        updateSignal(null, "ultrasoundValue", "ultrasoundDot");
        updateSignal(null, "magneticValue", "magneticDot");
        return;
      }

      document.getElementById("rockType").textContent = rock.type ?? "---";
      document.getElementById("confidence").textContent = rock.confidence ?? "---";

      updateSignal(rock.ir, "irValue", "irDot");
      updateSignal(rock.ultrasound, "ultrasoundValue", "ultrasoundDot");
      updateSignal(rock.magnetic, "magneticValue", "magneticDot");

      let activeCount = 0;
      if (rock.ir?.detected) activeCount++;
      if (rock.ultrasound?.detected) activeCount++;
      if (rock.magnetic?.detected) activeCount++;

      document.getElementById("typeCount").textContent = activeCount + " ACTIVE";
    }

    function updateSignal(signal, valueId, dotId) {
      const valueEl = document.getElementById(valueId);
      const dotEl = document.getElementById(dotId);

      if (!signal || !signal.detected) {
        valueEl.textContent = "---";
        dotEl.className = "dot";
        return;
      }

      valueEl.textContent = signal.value ?? "---";
      dotEl.className = "dot live";
    }

    function formatAgeDisplay(value) {
      if (value === null || value === undefined || value === "") return "---";

      const text = String(value).trim();
      if (text.toLowerCase().includes("billion")) return text;

      const num = parseFloat(text);
      if (!isNaN(num)) return num + " billion years";

      return text + " billion years";
    }

    function storeRock(data) {
      const rock = data.rock;
      const age = data.age;

      const record = {
        id: rock.id,
        age: age?.detected ? formatAgeDisplay(age.value) : "---",
        type: rock.type ?? "---",
        ir: rock.ir?.detected ? rock.ir.value : "---",
        ultrasound: rock.ultrasound?.detected ? rock.ultrasound.value : "---",
        magnetic: rock.magnetic?.detected ? rock.magnetic.value : "---",
        time: new Date().toLocaleTimeString()
      };

      const existingIndex = storedRocks.findIndex(item => item.id === record.id);

      if (existingIndex >= 0) {
        storedRocks[existingIndex] = record;
      } else {
        storedRocks.push(record);
      }

      localStorage.setItem("storedRocks", JSON.stringify(storedRocks));
    }

    function addSerialLog(text) {
      if (!serialLog) return;

      if (serialLog.textContent.includes("Waiting")) {
        serialLog.textContent = "";
      }

      const time = new Date().toLocaleTimeString();
      serialLog.textContent += time + " " + text + "\\n";

      const lines = serialLog.textContent.split("\\n");
      if (lines.length > 10) {
        serialLog.textContent = lines.slice(lines.length - 10).join("\\n");
      }

      serialLog.scrollTop = serialLog.scrollHeight;
    }

    function formatLog(data) {
      return "[ROCK " + (data.rock?.id ?? "?") + "] " +
        (data.rock?.type ?? "---") +
        " | AGE=" + formatAgeDisplay(data.age?.value ?? "---") +
        " | IR=" + (data.rock?.ir?.value ?? "---") +
        " | US=" + (data.rock?.ultrasound?.value ?? "---") +
        " | MG=" + (data.rock?.magnetic?.value ?? "---");
    }
  </script>
</body>
</html>
)rawliteral";