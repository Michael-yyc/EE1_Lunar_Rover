// Webpage to return when root is requested

#include <webpage.h>

const char plan_z[] =
R"html(
<html><body>
<p> Age: <span id="age"> Scanning...</span></p>
<p> Magnetic Direction: <span id="magnetic"> Scanning...</span></p>
<p> Infrared Pulse Rate: <span id="IR"> Scanning...</span></p>
<p> Ultrasound: <span id="US"> Scanning...</span></p>
<script>
async function update_magnetic() {
try {
const response = await fetch('/magnetic');
const text = await response.text();
document.getElementById('magnetic').textContent = text;
} catch (error) {document.getElementById('magnetic').textContent = 'Error';}}
async function update_age() {
try {
const response = await fetch('/age');
const text = await response.text();
document.getElementById('age').textContent = text;
} catch (error) {document.getElementById('age').textContent = 'Error';}}
async function update_IR() {
try {
const response = await fetch('/IR');
const text = await response.text();
document.getElementById('IR').textContent = text;
} catch (error) {document.getElementById('IR').textContent = 'Error';}}
async function update_US() {
try {
const response = await fetch('/US');
const text = await response.text();
document.getElementById('US').textContent = text;
} catch (error) {document.getElementById('US').textContent = 'Error';}}
setInterval(update_magnetic, 500);
setInterval(update_age, 100);
setInterval(update_IR, 1000);
setInterval(update_US, 500);
</script>
</body></html>
)html";