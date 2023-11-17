#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

Servo servo;
ESP8266WebServer server;
    
    uint8_t pin_led1 = 0;
    char* ssid = "Redmi Note 10S";
    char* password = "galletasyleche1";
    
    char webpage[] PROGMEM = R"=====(
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <!--CSS CODE-->
        <style>
            *, *::before, *::after{
                box-sizing: border-box;
            }
            :root{
                --clr-primary1: #0A3200;
                --clr-primary2: #379634;

                --clr-neutral1: #0A3200;
                --clr--neutral2: #379634;

                /* fonts */
                --ff-primary: 'Roboto', sans-serif;
                --ff-accent: 'Playfair Display', serif;

            }
            body{
                font-family: var(--ff-primary);
                font-weight: 400;
                font-size: 1.3125rem;
                line-height: 1.6;
            }
            h1, h2, h3{
                font-family: var(--ff-accent);
                font-weight: 900;
                line-height: 1;
                color: var(--clr-primary2);
            }
            h2, h3, p{
                margin-bottom: 1em;
            }
            img{
                max-width: 100%;
                display: block;
            }
            .text-center, .split{
                text-align: center;
            }
            header, section{
                padding: 4rem 0;
            }
            @media(min-width: 40em){
                header, section{
                    padding: 7rem 0;
                }
            }
            .container{
                margin-inline: auto;
                width: min(90%, 70.5rem);
            }
            .split{
                display: flex;
                flex-direction: column;
            }
            @media(min-width: 40em){
                .split{
                    flex-direction: row;
                }
                .split > *{
                    flex-basis: 100%;
                }
                .split > * + *{
                    margin-left: 2em;
                }
            }
            .container--narrow{
                max-width: 34rem;
            }
            .bg-light{
                background-color: var(--clr-primary1);
            }
            button{
                width: 70%;
                height: 5rem;
                background-color: var(--clr-primary1);
                border-radius: 1rem;
                border-style: none;
                cursor: pointer;
                text-transform: uppercase;
                font-size: 20px;
                padding: 1rem;
            }
            button:hover{
                background-color: #BFFFF1;
                color: black;
            }
        </style>
        <!--CSS ENDING-->
        <meta charset="UTF-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>PETFED</title>
    </head>
    <body>
        <header class="bg-light text-center">
            <div class="container container--narrow"></div>
            <h1>PetFed</h1>
            <h3>An intelligent feeder for your pet.</h3>
        </header>
        <section>
            <div class="container">
                <h2 class="text-center">Activate Dispenser</h2>
                <div class="split">
                    <div>
                        <p>Dispense a food portion</p>
                        <button onclick="myFunction()">Feed</button>
                    </div>
                </div>
            </div>
        </section>
    </body>
    
    </html>
    <script>
    function myFunction()
    {
      console.log("boton presionado");
      var xhr = new XMLHttpRequest();
      var url = "/ledstate";
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("led-state").innerHTML = this.responseText;
        }
      };
      xhr.open("GET", url, true);
      xhr.send();
    };
    document.addEventListener('DOMContentLoaded', myFunction, false);
    </script>
    </html>
    )=====";
    
    void setup()
    {
      servo.attach(5); //D1
      pinMode(pin_led1, OUTPUT);
      WiFi.begin(ssid,password);
      Serial.begin(115200);
      while(WiFi.status()!=WL_CONNECTED)
      {
        Serial.print(".");
        delay(500);
      }
      Serial.println("");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
    
      server.on("/",[](){server.send_P(200,"text/html", webpage);});
      server.on("/ledstate",getLEDState);
      server.begin();
    }
    
    void loop()
    {
      server.handleClient();
    }
    
    void toggleLED()
    {
      digitalWrite(pin_led1,!digitalRead(pin_led1));
      
       servo.write(0);
    
        delay(2000);
    
        servo.write(180);
    
        delay(5000);
        
        servo.write(0);
      
    }
    
    void getLEDState()
    {
      toggleLED();
      String led_state = digitalRead(pin_led1) ? "ON" : "OFF";
      server.send(200,"text/plain", led_state);
    }
