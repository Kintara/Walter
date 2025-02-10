
const char HIGH_VALUES[] PROGMEM = R"=====(

<!DOCTYPE html>
<html>
<head>

<style>
 h1 {
  color:red;
 text-align:centre;
 text-size:60px;
 }

 /* Style for the form layout */
      .form-row {
            display: flex;
            justify-content: space-between;
            align-items: center;
            width: 100%;
        }

        .form-row label {
           background-color: #0000ff;
  color: white;    
  border: 15px outset lime; 
  border-radius: 50px;
  padding: 10px 20px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 60px;
  margin: 0 auto;
  height: 70px; 
  width: 30%; /* Adjust as needed */
        }

        .form-row input {
           background-color: #f0f0f0;
  color: black;    
  border: 15px outset lime; 
  border-radius: 50px;
  padding: 10px 20px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 60px;
  margin: 0 auto;
  height: 70px; 
 width: 70%; /* Adjust as needed */
        }

    input[type=submit] {
           background-color: #8080ff;
  color: black;   
  opacity: 1; 
  border: 15px outset red; 
  border-radius: 50px;
  padding: 10px 20px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 60px;
  margin: 0 auto;
  height: 70px; 
 width: 70%; /* Adjust as needed */
        }
        
input:disabled {
  opacity: 0.25; /* Example: Reduce opacity to indicate disabled state */
  
           background-color: #f08080;
  color: red;    
  border: 15px outset black; 
}
input:disabled:hover {
  color: #000; /* For instance, maintain the same color on hover */
}



table  {
  background-color: #eoeoeo;
  border:none;
  }
td {
  border:none;
}
.button {  
  background-color: #4CAF50;
  color: black;
  border: 15px outset red; 
  border-radius: 20px;  
  padding: 10px 20px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 60px;
  margin: 0 auto;
  width: 33%;
  height: 150px;
}

.button:hover {
  background-color: #3e8e41;
}


.button2 {  
  background-color: #0000ff;
  color: white;    
  border: 15px outset blue; 
  border-radius: 50px;
  padding: 10px 20px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 60px;
  margin: 0 auto;
  height: 150px; 
}
.button2:disabled { /* like the version button
  opacity: 0.5; /* Example: Reduce opacity to indicate disabled state */
  
           background-color: #8080f0;
  color: black;    
  border: 15px outset black; 
}
.button2:disabled:hover {
  color: #000000; /* For instance, maintain the same color on hover */
}



        .button3 {
   background-color: #0000ff;
  color: white;    
  border: 15px outset #00ff00; 
  border-radius: 50px;
  padding: 10px 20px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 60px;
  margin: 0 auto;
  height: 150px; 
        }

.buttonClear {
 background-color: #FF8080;
  color: black;    
  border: 15px outset #00ff00; 
  border-radius: 50px;
  padding: 10px 20px;
  text-align: center;
  text-decoration: bold;
  display: inline-block;
  font-size: 60px;
  margin: 0 auto;
  height: 150px; 
}


.slider {
  -webkit-appearance: none;
  height: 100px; 
   border-radius: 40px; 
   border-width:5px;
   border-color:#ff60b0;
  background: #0b05a9;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s; 
  appearance: none;
  cursor: pointer;
    width: 100%;
    }


.slider:hover {
  opacity: 1;
}

.slider::-webkit-slider-thumb { <!--//Style for the Slider marker-->
  -webkit-appearance: none;
  appearance: none;
  width: 90px;
  height:90px;
  background: #ff0000;
  cursor: pointer;
  border-radius: 40%; 
  // background-image: url("https://ibaslogic.github.io/hosted-assets/smile.png");
  //background-size: cover;
  box-shadow: 0px 0px 0px 9px #00ff00; /* Add cool effects to your sliders! */left/right ,up/down , blur, size increase
  
}
</style>
</head>


 <body style="background-color: #ff9900" onload="process()">

<table width ="100%">
<tr>
<td>
     <button class="button2" style="width:100%; height:100%;" id="control" onclick="PressControl()">BeamZ Snow Control</button> </td>
     </tr>
     </table>
<hr>
<table width ="100%">
<tr>
<td style="width:50%">
     <button class="button2" style="width:100%; height:100%;"  id="mode">Status</button> </td>
<td style="width:50%">
    <button class="button2" style="width:100%; height:100%;"  id="status" >SNOW!</button></td>
     </tr>
     </table>

<br>
<hr>
<div>
  <button class="button" id="stop" onclick="PressStop()">Stop</button>
  <button class="button" id="skip" onclick="PressSkip()">Skip</button>
  <button class="button" id="time" onclick="PressTime()">Man 3</button>
</div>

<br>
<hr>
<br>
 

<table width ="100%">
<tr>
<td style="width:50%">
     <button  class="button2" style="width:100%; height:100%; ">Woah!</button> </td>
<td style="width:25%">
     <button class="button2"  id="intervalSet" style="width:100%; height:100%;">888</button></td>
<td style="width:25%">
     <button class="button2" id="intervalRemain" style="width:100%; height:100%;">888</button></td>
     </tr>
     </table>
        <input type="range" overflow: hidden;  border-radius: 16px; margin="10" class="slider" width="100%"  height= "90px" min="0" max="200" step="10" value = "10" id="intervalSlider" oninput="UPDATE_INTERVALSLIDER(this.value)"> 
  
<br>
<hr>
<br>
<table width ="100%">
<tr>
<td style="width:50%">
     <button class="button2" style="width:100%; height:100%;">Snow!</button> </td>
<td style="width:25%" !importent>
     <button class="button2"  id="durationSet" style="width:100%; height:100%;">888</button></td>
<td style="width:25%" !importent>
     <button class="button2"  id="durationRemain" style="width:100%; height:100%;">888</button></td>
     </tr>
     </table>
     
<input type="range" class="slider"  min="0" max="60" value = "30" step="10" id="durationlSlider" oninput="UPDATE_DURATIONSLIDER(this.value)">   </td>
<br>
<hr>
<br>
<table width ="100%">
<tr>
<td style="width:50%">
  
     <button class="button2"  id="blow"  onclick="PressBlow()"  style="width:100%; height:100%;">Blow +</button></td>
<td style="width:25%">
     <button class="button2" id="airSet" style="width:100%; height:100%;">888</button></td>
<td style="width:25%">

     <button class="button2"  id="RJM"  onclick="PressRJM()"  style="width:100%; height:100%;">RJM</button></td>
     </tr>
     </table>
     
<input type="range" class="slider" id="airSlider"  min="1" max="255" value = "25"  step="1" oninput="UPDATE_AIRSLIDER(this.value)"> 
<br>
<hr>
<br>
<table width ="100%">
<tr>
<td style="width:50%">
   <button class="button2"  id="flow"  onclick="PressBlow()"  style="width:100%; height:100%;">Flow +</button></td>
   
<td style="width:25%" !importent>
     <button class="button2"  id="flowSet" style="width:100%; height:100%;">888</button></td>
<td style="width:25%">
     <button class="button2"  id="Reset"  onclick="PressReset()"  style="width:100%; height:100%;" disabled>XXX</button></td>
     </tr>
     </table>
     
 <input type="range" class="slider"   id="flowSlider" min="0" max="255" value = "1" step="1" oninput="UPDATE_FLOWSLIDER(this.value)">

<br>

<hr> 
   <form id="myForm" action="SaveIPCredentials" method="post"  autocomplete="on">
        <div class="form-row">
            <label for="SSID" >SSID:</label>
            <input type="text" id="ssid" name="ssid" placeholder="WiFi SSID" >
        </div>
        <div class="form-row">
            <label for="password" >Password:</label>
             <input type="password" id="password" name="password" placeholder="WiFi Password" >
        </div>
      
        <!-- Add more form rows as needed -->
   
<!-- ******************************************************************************************************************************** -->


<br>
<hr> 
<table width ="100%">
<tr>
<td style="width:50%">
        <!-- Submit button (initially disabled) -->
        <input  type="submit"  value="Submit" id="submitButton"  style="width:100%; height:100%;" disabled> </td>

<td style="width:50%">
        <!-- Button to clear input fields -->
        <button class="buttonClear" type="button" onclick="clearFields()" style="width:100%; height:100%;">Clear Fields</button></td>
        </tr>
        </table>
    </form>

</body>



  <script type = "text/javascript"> 

        // Clear input fields
        function clearFields() {
            document.getElementById('myForm').reset();
        }


    // global variable visible to all java functions
    var xmlHttp=createXmlHttpObject();

    // function to create XML object
    function createXmlHttpObject(){
      if(window.XMLHttpRequest){
        xmlHttp=new XMLHttpRequest();
      }
      else{
        xmlHttp=new ActiveXObject("Microsoft.XMLHTTP");
      }
      return xmlHttp;
    }


    
    // function to handle slider from HTML code above
    // and send a processing string back to server
    // This processing string is use in the .on method
    // From the 'oninput="UPDATE_INTERVALSLIDER(this.value)'
    function UPDATE_INTERVALSLIDER(value) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
                document.getElementById("intervalSet").innerHTML=this.responseText; //Set the text on the 'intervalSet' element
        }
      }
      xhttp.open("PUT", "intervalSlider?VALUE="+value, true);
      xhttp.send();
    }


    // function to handle slider from HTML code above
    // and send a processing string back to server
    // This processing string is use in the .on method
    // From the 'oninput="UPDATE_DURATIONSLIDER(this.value)'
    function UPDATE_DURATIONSLIDER(value) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
                document.getElementById("durationSet").innerHTML=this.responseText; //Set the text on the 'durationSet' element
        }
      }
      xhttp.open("PUT", "durationSlider?VALUE="+value, true);
      xhttp.send();
    }
    // function to handle slider from HTML code above
    // and send a processing string back to server
    // This processing string is use in the .on method
    // From the 'oninput="UPDATE_AIRSLIDER(this.value)'
    function UPDATE_AIRSLIDER(value) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
                document.getElementById("airSet").innerHTML=this.responseText; //Set the text on the 'airSet' element
        }
      }
      xhttp.open("PUT", "airSlider?VALUE="+value, true);
      xhttp.send();
    }
   
    // function to handle slider from HTML code above
    // and send a processing string back to server
    // This processing string is use in the .on method
    // From the 'oninput="UPDATE_FLOWSLIDER(this.value)'
    function UPDATE_FLOWSLIDER(value) {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
                document.getElementById("flowSet").innerHTML=this.responseText; //Set the text on the 'flowSet' element
        }
      }
      xhttp.open("PUT", "flowSlider?VALUE="+value, true);
      xhttp.send();
    }
   
     // general processing code from the web page to ask for an XML steam
    // this is actually why you need to keep sending data to the page to 
    // force this call with stuff like this
    // server.on("/xml", SendXML);
    // otherwise the page will not request XML from the ESP, and updates will not happen
    function process(){ //this is loaded from the <body> tag which is loaded each time a server.send
     
     if(xmlHttp.readyState==0 || xmlHttp.readyState==4) {
        xmlHttp.open("PUT","xml",true);
        xmlHttp.onreadystatechange=response;
        xmlHttp.send(null);
      }       
        // you may have to play with this value, big pages need more porcessing time, and hence
        // a longer timeout
        setTimeout("process()",100);
    } 

 // function to handle the response from the ESP i.e. the values of stuff
    function response(){
      var message;
      var barwidth;
      var currentsensor;
      var xmlResponse;
      var xmldoc;
      var dt = new Date();
      var color = "#e8e8e8";
     
      // get the xml stream
      xmlResponse=xmlHttp.responseXML;
  
   
      //get value for Version 
 xmldoc = xmlResponse.getElementsByTagName("V1");
      message = xmldoc[0].firstChild.nodeValue;
      if (message==0){
        
       document.getElementById("Reset").innerHTML="Reset!";
      } else{
       document.getElementById("Reset").innerHTML="v" + message;
       }
  
       //get value for Interval Set
 xmldoc = xmlResponse.getElementsByTagName("IM");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("intervalSet").innerHTML=message;

       //get value for Interval Remain
 xmldoc = xmlResponse.getElementsByTagName("IR");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("intervalRemain").innerHTML=message;

       //get value for Duration Set 
 xmldoc = xmlResponse.getElementsByTagName("DM");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("durationSet").innerHTML=message;

       //get value for Duration Remain 
 xmldoc = xmlResponse.getElementsByTagName("DR");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("durationRemain").innerHTML=message;

       //get value for Air 
 xmldoc = xmlResponse.getElementsByTagName("AF");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("airSet").innerHTML=message;


       //get value for Flow 
 xmldoc = xmlResponse.getElementsByTagName("FF");
      message = xmldoc[0].firstChild.nodeValue;
      document.getElementById("flowSet").innerHTML=message;

      
       //get value for TimingMode 
 xmldoc = xmlResponse.getElementsByTagName("TM");
      message = xmldoc[0].firstChild.nodeValue;
       if (message == 0){                    
                document.getElementById("stop").innerHTML="RESUME!";                       
                document.getElementById("stop").style.backgroundColor="rgb(00, 200, 0)";       
                document.getElementById("status").innerHTML="STOPPED!";                         
                document.getElementById("time").innerHTML="MANUAL";                
                document.getElementById("mode").innerHTML="TIMER says";  
                 document.getElementById("mode").style.backgroundColor="rgb(0,200,0)";                        
                document.getElementById("time").style.backgroundColor="rgb(200, 0, 0)";                        
                document.getElementById("skip").innerHTML="Skip";                              
                document.getElementById("skip").style.backgroundColor=" rgb(76, 175, 80)"; 
                document.getElementById("status").style.backgroundColor="rgb(200, 0, 0)";   
               
              }
                  if (message == 1){
                document.getElementById("stop").style.backgroundColor="rgb(200, 0, 0)";               
                document.getElementById("stop").innerHTML="STOP!";                     
                document.getElementById("time").innerHTML="MANUAL";  
                 document.getElementById("time").style.backgroundColor="rgb(200,00,00)";                        
                document.getElementById("skip").innerHTML="Skip";                                
                document.getElementById("skip").style.backgroundColor=" rgb(76, 175, 80)";              
                document.getElementById("mode").innerHTML="TIMER says";  
                 document.getElementById("mode").style.backgroundColor="rgb(0,200,0)";                            
                document.getElementById("status").innerHTML="WAIT!";
                document.getElementById("status").style.backgroundColor="rgb(200, 0, 0)";        
              }
              if (message == 2){
                 document.getElementById("stop").style.backgroundColor="rgb(200,00,00)";             
                document.getElementById("stop").innerHTML="STOP!";                   
                document.getElementById("time").innerHTML="MANUAL";  
                 document.getElementById("time").style.backgroundColor="rgb(200,00,00)";                       
                document.getElementById("skip").innerHTML="Skip";                                               
                document.getElementById("skip").style.backgroundColor=" rgb(76, 175, 80)";               
                document.getElementById("mode").innerHTML="TIMER says";  
                 document.getElementById("mode").style.backgroundColor="rgb(0,200,0)";                               
                document.getElementById("status").innerHTML="SNOW!";
                document.getElementById("status").style.backgroundColor="rgb(00,200,00)";
               }
              if (message == 3){
                 document.getElementById("stop").style.backgroundColor="rgb(200,00,00)";             
                document.getElementById("stop").innerHTML="STOP!";                   
                document.getElementById("time").innerHTML="TIMER";  
                 document.getElementById("time").style.backgroundColor="rgb(0,200,00)";                         
                document.getElementById("skip").innerHTML="Skip";                                                    
                document.getElementById("skip").style.backgroundColor=" rgb(76, 175, 80)";                 
                document.getElementById("mode").innerHTML="MANUAL says";  
                 document.getElementById("mode").style.backgroundColor="rgb(200,0,0)";                               
                document.getElementById("status").innerHTML="Always SNOW";
                document.getElementById("status").style.backgroundColor="rgb(00,200,00)";                            
                document.getElementById("durationRemain").innerHTML="X";                            
                document.getElementById("intervalRemain").innerHTML="X";
               }
   if (message == 4){
                document.getElementById("stop").innerHTML="RPi";                               
                document.getElementById("skip").innerHTML="Has";                   
                document.getElementById("time").innerHTML="Control";  
                 document.getElementById("stop").style.backgroundColor="rgb(200,200,00)";          
                 document.getElementById("skip").style.backgroundColor="rgb(200,200,00)";          
                 document.getElementById("time").style.backgroundColor="rgb(200,200,00)";                  
                document.getElementById("mode").innerHTML="RPi says";  
                 document.getElementById("mode").style.backgroundColor="rgb(200,200,0)";                               
                document.getElementById("status").innerHTML="Primed";
                document.getElementById("status").style.backgroundColor="rgb(200,200,00)";                            
                document.getElementById("durationRemain").innerHTML="X";                            
                document.getElementById("intervalRemain").innerHTML="X";
               }
 if (message == 5){
                 
                document.getElementById("stop").innerHTML="RPi";                               
                document.getElementById("skip").innerHTML="Has";                   
                document.getElementById("time").innerHTML="Control";  
                 document.getElementById("stop").style.backgroundColor="rgb(200,0,200)";  
                 document.getElementById("skip").style.backgroundColor="rgb(200,0,200)"; 
                 document.getElementById("time").style.backgroundColor="rgb(200,0,200)";                   
                document.getElementById("mode").innerHTML="RPi says";  
                 document.getElementById("mode").style.backgroundColor="rgb(200,0,200)";                               
                document.getElementById("status").innerHTML="Go! Go!";
                document.getElementById("status").style.backgroundColor="rgb(200,0,200)";                            
                document.getElementById("durationRemain").innerHTML="X";                            
                document.getElementById("intervalRemain").innerHTML="X";
               }

 
       


    }


function PressStop() {
      var xhttp = new XMLHttpRequest(); 
      var message;
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
                  message = this.responseText;
               if (message == 0){
                document.getElementById("stop").style.backgroundColor="rgb(00,200,00)";              
                document.getElementById("stop").innerHTML="RESUME!";               
                document.getElementById("status").innerHTML="PAUSED!"; 
                document.getElementById("status").style.backgroundColor="rgb(200,00,000)";
              }
                  if (message == 1){
                document.getElementById("stop").style.backgroundColor="rgb(200, 0, 0)";               
                document.getElementById("stop").innerHTML="STOP!";                           
                document.getElementById("status").innerHTML="WAIT!";
                document.getElementById("status").style.backgroundColor="rgb(200, 0, 0)";        
              }
              if (message == 2){
                 document.getElementById("stop").style.backgroundColor="rgb(200,00,00)";             
                document.getElementById("stop").innerHTML="STOP!";                            
                document.getElementById("status").innerHTML="SNOW!";
                document.getElementById("status").style.backgroundColor="rgb(00,200,00)";
               }
                if (message == 3){
                 document.getElementById("stop").style.backgroundColor="rgb(200,00,00)";             
                document.getElementById("stop").innerHTML="STOP!";                            
                document.getElementById("status").innerHTML="SNOW!";
                document.getElementById("status").style.backgroundColor="rgb(00,200,00)";
               }

        }
      }    
      xhttp.open("PUT", "PressStop", false);
      xhttp.send();
    }


function PressSkip() {
      var xhttp = new XMLHttpRequest(); 
      var message;
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
                  message = this.responseText;
                  //0=Off 1=interval 2= Duration;
               if (message == 0){
               
              }
                  if (message == 1){
                document.getElementById("stop").style.backgroundColor="rgb(200, 0, 0)";               
                document.getElementById("stop").innerHTML="STOP!";                           
                document.getElementById("status").innerHTML="WAIT!";
                document.getElementById("status").style.backgroundColor="rgb(200, 0, 0)";        
              }
              if (message == 2){
                 document.getElementById("stop").style.backgroundColor="rgb(200,00,00)";             
                document.getElementById("stop").innerHTML="STOP!";                            
                document.getElementById("status").innerHTML="SNOW!";
                document.getElementById("status").style.backgroundColor="rgb(00,200,00)";
               }

        }
      }    
      xhttp.open("PUT", "PressSkip", false);
      xhttp.send();
    }


function PressControl() {
      var xhttp = new XMLHttpRequest(); 
      var message;
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
                  message = this.responseText;
                  //0=Off 1=interval 2= Duration;
          //     if (message == 0){               
          //    }
                         
                document.getElementById("control").innerHTML=message;

        }
      }    
      xhttp.open("PUT", "PressControl", false);
      xhttp.send();
    }











function PressTime() {
      var xhttp = new XMLHttpRequest(); 
      var message;
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
                  message = this.responseText;  
        switch (message) {
          
case 0: 
 document.getElementById("stop").style.backgroundColor="rgb(200, 0, 0)";               
                document.getElementById("stop").innerHTML="STOP!";                          
                document.getElementById("time").innerHTML="SSSS!";
                document.getElementById("status").style.backgroundColor="rgb(200, 0, 0)";      

break;

case 3: 
 document.getElementById("stop").style.backgroundColor="rgb(200, 0, 0)";               
                document.getElementById("stop").innerHTML="STOP!";                           
                document.getElementById("status").innerHTML="SNOW!";                        
                document.getElementById("time").innerHTML="TM3!";
                document.getElementById("status").style.backgroundColor="rgb(200, 0, 0)";      

break;

}

        }
      }    
      xhttp.open("PUT", "PressTime", false);
      xhttp.send();
    }



function PressBlow() {
  document.getElementById("blow").style.backgroundColor="rgb(200, 0, 0)";               
                document.getElementById("blow").innerHTML="REFRESH";
                document.getElementById("flow").style.backgroundColor="rgb(200, 0, 0)";               
                document.getElementById("flow").innerHTML="PAGE!";            
      var xhttp = new XMLHttpRequest(); 
      var message;
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
                  message = this.responseText;
        }
        }  
      xhttp.open("PUT", "ProcessBlow", false);
      xhttp.send();
    }
function PressRJM() {
 var x = document.getElementById("submitButton").disabled;
                document.getElementById("submitButton").disabled = false;
                document.getElementById("Reset").disabled = false;
                
      var xhttp = new XMLHttpRequest(); 
      var message;
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
                  message = this.responseText;
        }
        }  
      xhttp.open("PUT", "ProcessRJM", false);
      xhttp.send();
    }



function PressReset() {
   document.body.style.backgroundColor = "red";
      var xhttp = new XMLHttpRequest(); 
      var message;
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
                  message = this.responseText;
        }
      }    
      xhttp.open("PUT", "ProcessReset", false);
      xhttp.send();
    }












</script>
</html>
)=====";