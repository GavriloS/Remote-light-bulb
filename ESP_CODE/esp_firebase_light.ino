      
      #include <Adafruit_GFX.h>
      #include <Adafruit_SSD1306.h>
      #include <FirebaseESP32.h>
      
      #define PIN18_LIGHT_ONE_RELAY 18
      #define PIN19_LIGHT_TWO_RELAY 19
      #define PIN4_LIGHT_ONE_BUTTON 4
      #define PIN27_LIGHT_TWO_BUTTON 27
      
      #define SCREEN_WIDTH 128 // OLED display width, in pixels
      #define SCREEN_HEIGHT 32 // OLED display height, in pixels
      
      //Change with your values
      #define FIREBASE_HOST "###################################"
      
      #define FIREBASE_AUTH "######################"
      
      const char* WIFI_SSID = "##########";
      const char* WIFI_PASSWORD = "###########";
      
      
      
      // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
      Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
      
      
      //Define the Firebase Data object
      FirebaseData stream;
      FirebaseData fbdo;
      
      int lightOneState = -1;
      int lightTwoState = -1;
      
      // paths
      
      String childPath[2] = {"/one", "/two"};
      // For updating;
      FirebaseJson updateData;
      
      
      
      
      
      //listener for the firebase datase
      // function is called when the values in the database are changed
      void streamCallback(MultiPathStreamData stream){
        size_t numChild = sizeof(childPath) / sizeof(childPath[0]);
        
        
        
        for (size_t i = 0; i < numChild; i++)
        {
        
          if(stream.get(childPath[i])){
              int value = atoi(stream.value.c_str());
              if(i == 0){
                  
                 if(lightOneState != value ){
                    lightOneState = value;
                    display.setCursor(0,0);
                    if(value == 0){
                      display.println("Svetlo 1: OFF");
                      digitalWrite(PIN18_LIGHT_ONE_RELAY,HIGH);
                   }else{
                      display.println("Svetlo 1: ON ");
                      digitalWrite(PIN18_LIGHT_ONE_RELAY,LOW);
                    }
                      display.display();
                 }
              }else if(i == 1){
                if(lightTwoState != value ){
                    lightTwoState = value;
                    display.setCursor(0,25);
                    if(value == 0){
                      display.println("Svetlo 2: OFF");
                      digitalWrite(PIN19_LIGHT_TWO_RELAY,HIGH);
                   }else{
                      display.println("Svetlo 2: ON ");
                      digitalWrite(PIN19_LIGHT_TWO_RELAY,LOW);
                    }
                      display.display();
                 }
              }
            
            }
          
   
          
        }
      
        
      }
      
      
      
      void setup() {
        Serial.begin(115200);
        
     
        //For display
         if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
          Serial.println(F("SSD1306 allocation failed"));
          for (;;);
        }
        delay(2000);
        display.clearDisplay();
      
        display.setTextSize(1);
        display.setTextColor(WHITE,BLACK);
      
        display.setTextWrap(true);
        display.display();
       
        int wifiCheck = 0;
        //Connect to wifi
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        Serial.print("Connecting to Wi-Fi");
        while (WiFi.status() != WL_CONNECTED) {
          if(wifiCheck > 10){
            break;
          }
          Serial.print(".");
          delay(300);
          wifiCheck +=1;
        }
        display.setCursor(0,0);
        if(wifiCheck > 10){
          display.println("Internet ne radi");
          display.println("Paljenje na dugnice");
          display.display();
        }
        WiFi.setAutoReconnect(true);
      
        // set the pins for the relay and the buttons
        pinMode(PIN18_LIGHT_ONE_RELAY,OUTPUT);
        pinMode(PIN19_LIGHT_TWO_RELAY,OUTPUT);
        pinMode(PIN4_LIGHT_ONE_BUTTON,INPUT);
        pinMode(PIN27_LIGHT_TWO_BUTTON,INPUT);
      
        digitalWrite(PIN18_LIGHT_ONE_RELAY,HIGH);
        digitalWrite(PIN19_LIGHT_TWO_RELAY,HIGH);
      
        
      
      
        //connect to firebase
        Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
        Firebase.reconnectWiFi(true);
        
    
        // set the listener
        char* parentPath = "/lights";
        if (!Firebase.beginMultiPathStream(stream, parentPath))
          Serial.printf("sream begin error, %s\n\n", stream.errorReason().c_str());
        Firebase.setMultiPathStreamCallback(stream, streamCallback, streamTimeoutCallback);
      }

      void streamTimeoutCallback(bool timeout)
      {
        if(timeout){
          //Stream timeout occurred
          Serial.println("Stream timeout, resume streaming...");
        }  
      
      }


      
      
      
      
      void loop() {
        //Check if button for light one is pressed and acts acordingly by turning on the light or turning it off
        // sends an acurate update to the firebase database
        if(digitalRead(PIN4_LIGHT_ONE_BUTTON) == HIGH){
          updateData.clear();
          if(lightOneState == 0){
            lightOneState = 1;
            digitalWrite(PIN18_LIGHT_ONE_RELAY,LOW);
            display.setCursor(0,0);
            display.println("Svetlo 1: ON ");
            display.display();
            updateData.set("one",1);
            
          }else{
            lightOneState = 0;
            digitalWrite(PIN18_LIGHT_ONE_RELAY,HIGH);
            display.setCursor(0,0);
            display.println("Svetlo 1: OFF");
            display.display();
            updateData.set("one",0);
            
          }
          Firebase.updateNodeAsync(fbdo, "/lights", updateData);
          delay(300);
        }
        //Check if button for light one is pressed and acts acordingly by turning on the light or turning it off
        // sends an acurate update to the firebase database
        if(digitalRead(PIN27_LIGHT_TWO_BUTTON) == HIGH){
          updateData.clear();
          if(lightTwoState == 0){
            lightTwoState = 1;
            digitalWrite(PIN19_LIGHT_TWO_RELAY,LOW);
            display.setCursor(0,25);
            display.println("Svetlo 2: ON ");
            display.display();
            updateData.set("two",1);
            
          }else{
            lightTwoState = 0;
            digitalWrite(PIN19_LIGHT_TWO_RELAY,HIGH);
            display.setCursor(0,25);
            display.println("Svetlo 2: OFF");
            display.display();
            updateData.set("two",0);
            
          }
          Firebase.updateNodeAsync(fbdo, "/lights", updateData);
          delay(300);
        }
        
        
      
      
      }
