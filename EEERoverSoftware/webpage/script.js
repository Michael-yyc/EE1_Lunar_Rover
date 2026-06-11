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
async function updateMagnetic() {
try {
const response = await fetch('/magnetic', { cache: 'no-store' });
const text = await response.text();
document.getElementById('magnetic').textContent = text;} 
catch (error) {document.getElementById('magnetic').textContent = 'Error';}}