//Webpage to return when root is requested

#include <webpage.h>

const char webpage[] = \
"<html>\
    <head>\
        <style>\
            .btn {background-color: inherit;padding: 14px 28px;font-size: 16px;}\
            .btn:hover {background: #eee;}\
        </style>\
    </head>\
    <p>\
        Magnetic Field Direction:\
        <span id=\"magnetic\">\
            Scanning...\
        </span>\
        Rock Age:\
        <span id=\"age\">\
            Scanning...\
        </span>\
    </p>\
    <script>\
        var xhttp = new XMLHttpRequest();\
        xhttp.onreadystatechange = function() {if (this.readyState == 4 && this.status == 200) {document.getElementById(\"state\").innerHTML = this.responseText;}};\
        function ledOn() {xhttp.open(\"GET\", \"/on\"); xhttp.send();}\
        function ledOff() {xhttp.open(\"GET\", \"/off\"); xhttp.send();}\
        function forward() {xhttp.open(\"GET\", \"/F\"); xhttp.send();}\
        function backward() {xhttp.open(\"GET\", \"/B\"); xhttp.send();}\
        function left() {xhttp.open(\"GET\", \"/L\"); xhttp.send();}\
        function right() {xhttp.open(\"GET\", \"/R\"); xhttp.send();}\
        function stop() {xhttp.open(\"GET\", \"/S\"); xhttp.send();}\
        async function update_data(data, scan_type) {\
            const response = await fetch(data);\
            const value = await response.text();\
            document.getElementById(scan_type).textContent = value;\
        }\
        setInterval(update_data(\"/age\", \"age\"), 1000);\
        setInterval(update_data(\"/magnetic\", \"magnetic\"), 1000);\
    </script>\
    <body>\
        <button class=\"btn\" onclick=\"ledOn()\">LED On</button>\
        <button class=\"btn\" onclick=\"ledOff()\">LED Off</button>\
        <br>LED STATE: <span id=\"state\">OFF</span>\
        <button class=\"btn\" onclick=\"forward()\">Forward</button>\
        <button class=\"btn\" onclick=\"left()\">Left</button>\
        <button class=\"btn\" onclick=\"right()\">Right</button>\
        <button class=\"btn\" onclick=\"backward()\">Backward</button>\
        <button class=\"btn\" onclick=\"stop()\">Stop</button>\
    </body>\
</html>";