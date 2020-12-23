const char MAIN_page[] = R"=====(
<!DOCTYPE html>
<html>
<body>

<div id="demo">
  <h1>Pixel Tilt Photo Sphere</h1>

  <table style="width:85%">
      <caption>Motor Actions</caption>
      <tr>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td><button type="button" onclick="sendData(0, 2, 1000)" class="MoveButtons">Up 1000</button></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
      </tr>
      <tr>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td><button type="button" onclick="sendData(0, 2, 500)" class="MoveButtons">Up 500</button></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
      </tr>
      <tr>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td><button type="button" onclick="sendData(0, 2, 100)" class="MoveButtons">Up 100</button></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
      </tr>
      <tr>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td><button type="button" onclick="sendData(0, 2, 10)" class="MoveButtons">Up 10</button></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
      </tr>
      <tr>
        <td><button type="button" onclick="sendData(0, 1, 1000)" class="MoveButtons">Left 1000</button></td>
        <td><button type="button" onclick="sendData(0, 1, 500)" class="MoveButtons">Left 500</button></td>
        <td><button type="button" onclick="sendData(0, 1, 100)" class="MoveButtons">Left 100</button></td>
        <td><button type="button" onclick="sendData(0, 1, 10)" class="MoveButtons">Left 10</button></td>
        <td></td>
        <td><button type="button" onclick="sendData(1, 1, 10)" class="MoveButtons">Right 10</button></td>
        <td><button type="button" onclick="sendData(1, 1, 100)" class="MoveButtons">Right 100</button></td>
        <td><button type="button" onclick="sendData(1, 1, 500)" class="MoveButtons">Right 500</button></td>
        <td><button type="button" onclick="sendData(1, 1, 1000)" class="MoveButtons">Right 1000</button></td>
      </tr>
      <tr>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td><button type="button" onclick="sendData(1, 2, 10)" class="MoveButtons">Down 10</button></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
      </tr>
      <tr>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td><button type="button" onclick="sendData(1, 2, 100)" class="MoveButtons">Down 100</button></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
      </tr>
      <tr>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td><button type="button" onclick="sendData(1, 2, 500)" class="MoveButtons">Down 500</button></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
      </tr>
      <tr>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
        <td><button type="button" onclick="sendData(1, 2, 1000)" class="MoveButtons">Down 1000</button></td>
        <td></td>
        <td></td>
        <td></td>
        <td></td>
      </tr>
    </table>
    <span id="PhotoProcessResult">NA</span>
    <br/>
    <br/>



  <label>Time Delay</label>
  <input id="TimeDelayClock" value="100"/>
  <label>Steps</label>
  <input id="clockStepsClock" value="10"/>
  <button type="button" onclick="sendDataOld(1)">Turn Clockwise</button>
  <br/>
  <br/>
  <br/>
  <label>Time Delay</label>
  <input id="TimeDelayCount" value="100"/>
  <label>Steps</label>
  <input id="clockStepsCount" value="10"/>
  <button type="button" onclick="sendDataOld(0)">Turn Counter Clock</button><BR>
  <br/>
  <br/>
  <label>Motor Number</label>
  <input id="MotorNumber" value="1"/>
  <br/>
  <br/>
  <label>Photo Actions</label>
  <div class="input_wrap">
    <input type="text" id="PhotoActions" value="%PhotoActions%"/>
  </div>
  <button type="button" onclick="updateActions()">Set Actions</button><BR>
  <span id="actionsState">NA</span>
  <br/>
  <br/>
  <button type="button" onclick="RunPhotoTake()">Run Photo Procoess</button><BR>
  <span id="PhotoProcessResult">NA</span>
</div>
  <br/>
  <br/>
  <br/>
<div>
  ADC Value is : <span id="ADCValue">0</span><br>
    LED State is : <span id="LEDState">NA</span>
</div>
<script>
function sendData(rotation, motorNumber, rotateValue) {
  document.getElementById("LEDState").innerHTML = "Processing"
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("LEDState").innerHTML = this.responseText;
    }
  };
  var delayValue = 10;
  xhttp.open("GET", "SetMotorLocation?CLOCK=" + rotation + "&STEPS=" + rotateValue + "&TIMEDELAY=" + delayValue + "&MOTORNUMBER=" + motorNumber, true);
  xhttp.send();
}


function sendDataOld(rotation) {
  document.getElementById("LEDState").innerHTML = "Processing"
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("LEDState").innerHTML = this.responseText;
    }
  };
  var rotateValue = document.getElementById("clockStepsClock").value;
  var delayValue = document.getElementById("TimeDelayClock").value;
  var motorNumber = document.getElementById("MotorNumber").value;
  if(rotation == 0){
    rotateValue = document.getElementById("clockStepsCount").value;
    delayValue = document.getElementById("TimeDelayCount").value;
  }
  xhttp.open("GET", "SetMotorLocation?CLOCK=" + rotation + "&STEPS=" + rotateValue + "&TIMEDELAY=" + delayValue + "&MOTORNUMBER=" + motorNumber, true);
  xhttp.send();
}

function updateActions() {
  document.getElementById("actionsState").innerHTML = "Processing"
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("actionsState").innerHTML = this.responseText;
    }
  };
  var actionsValue = document.getElementById("PhotoActions").value;
  xhttp.open("GET", "SetPhotoActions?ACTIONS=" + actionsValue + "", true);
  xhttp.send();
}

function RunPhotoTake() {
  document.getElementById("PhotoProcessResult").innerHTML = "Processing"
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("PhotoProcessResult").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "RunPhotoSphere", true);
  xhttp.send();
}

</script>
<style>
td {
    padding: 10px;  
    width: 100%;
    align: center;
}
button {
    width: 150px;
    height: 55px;
    text-align: center;
}
div {
    padding: 6px 17px; /* equal to negative input's margin for mimic normal `div` box-sizing */
}

input {
    width: 100%; /* force to expand to container's width */ 
    border: 7px solid #DFDFDF;  
    padding: 0 10px;
    margin: 0 -17px; /* negative margin = border-width + horizontal padding */ 
} 
</style>
<br><br>
</body>
</html>
)=====";
