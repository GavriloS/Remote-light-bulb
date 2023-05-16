# Remote-light-bulb

I create a remote light switch. I designed the circuit for the ESP32 myself and created an android app to use the switch

## ESP32

I used an ESP32 because of its wifi capabilities. A relay is used to switch the lights off and on. Buttons are there too so when there is no wifi you can still use it.

The ESP32 is connected to a firebase database. It listenes to changes to the light values, if the light value is 0 it turns off the light if it's 1 it turns it on.

## Android

I created an android app that can change the values of the light's in the firebase database.

### App screenshot

![](https://github.com/GavriloS/Remote-light-bulb/blob/main/images/light_switch_app.jpg)



