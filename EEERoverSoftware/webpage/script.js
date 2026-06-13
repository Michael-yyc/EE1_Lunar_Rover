async function test() {
try {
const response = await fetch('/magnetic', { cache: 'no-store' });
const text = await response.text();
document.getElementById('magnetic').textContent = text;
} catch (error) {
document.getElementById('magnetic').textContent = 'Error';
}
}
setInterval(test, 5000);