# xmas_star
Use a WS2811 string of leds to make a color changing xmas star. adapted from example in fastled.

Every point in the star consists of 10 leds, 5 per side. Light movement is from the center outward. It is possible to attach a second star and have it behave in unison. Only the twinkles are random over both (or more) stars

## Dimming

It is possible to attach a LDR to the controller board to adjust the lightlevel to the ambient ligt level. the code is disabeled by default, but can be turned on in the loop() function. Every ten seconds the light value will be taken and the dimlevel adjusted accordingly. a userguide to attach a LDR is in the header of the sketch
