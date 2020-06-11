const char MAIN_page[] = R"=====(
<!DOCTYPE html>
<html>
<body>

<div id="demo">
  <h1>Pixel Tilt Photo Sphere</h1>
  <label>Time Delay</label>
  <input id="TimeDelayClock" value="100"/>
  <label>Steps</label>
  <input id="clockStepsClock" value="10"/>
  <button type="button" onclick="sendData(1)">Turn Clockwise</button>
  <br/>
  <br/>
  <br/>
  <label>Time Delay</label>
  <input id="TimeDelayCount" value="100"/>
  <label>Steps</label>
  <input id="clockStepsCount" value="10"/>
  <button type="button" onclick="sendData(0)">Turn Counter Clock</button><BR>
  <br/>
  <br/>
  <label>Motor Number</label>
  <input id="MotorNumber" value="1"/>
  
  
</div>
  <br/>
  <br/>
  <br/>
<div>
  ADC Value is : <span id="ADCValue">0</span><br>
    LED State is : <span id="LEDState">NA</span>
</div>
<script>
function sendData(rotation) {
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

</script>
<br><br>
</body>
</html>
)=====";
