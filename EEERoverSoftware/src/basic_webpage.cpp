// Webpage to return when root is requested

#include <webpage.h>

const char basic_webpage[] =
R"html(
<html><head><style>
.btn {background-color: inherit;padding: 14px 28px;font-size: 16px;}
.btn:hover {background: #eee;}
</style></head>
<p>Magnetic field: <span id="magnetic">Scanning...</span></p>
<p>LED STATE: <span id="state">OFF</span></p>
<body>
<button class="btn" onclick="ledOn()">LED On</button>
<button class="btn" onclick="ledOff()">LED Off</button>
<button class="btn" onclick="forward()">Forward</button>
<button class="btn" onclick="left()">Left</button>
<button class="btn" onclick="right()">Right</button>
<button class="btn" onclick="backward()">Backward</button>
<button class="btn" onclick="stop()">Stop</button>
</body>
<script>
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function() {
if (this.readyState == 4 && this.status == 200) {document.getElementById("state").innerHTML = this.responseText;}};
function ledOn() {xhttp.open("GET", "/on");xhttp.send();}
function ledOff() {xhttp.open("GET", "/off");xhttp.send();}
function forward() {xhttp.open("GET", "/F");xhttp.send();}
function backward() {xhttp.open("GET", "/B");xhttp.send();}
function left() {xhttp.open("GET", "/L");xhttp.send();}
function right() {xhttp.open("GET", "/R");xhttp.send();}
function stop() {xhttp.open("GET", "/S");xhttp.send();}
async function test() {
try {
const response = await fetch('/magnetic', { cache: 'no-store' });
const text = await response.text();
if (text != "0") {
document.getElementById('magnetic').textContent = text;
}
} catch (error) {
document.getElementById('magnetic').textContent = 'Error';
}
}
setInterval(test, 5000);
</script>
</html>)html";