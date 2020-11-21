# Korkteller
Neon-oransje dings på øljekken på Regi hybel som sporer Regis hobbyalkoholisme i sanntid

# Hvordan funker det?
Kjernen i korktelleren er en ESP8266 Wemos D1 mini som er koblet opp mot det åpne klientnettet på Samfundet. En LCD-skjerm (HD44780) med I2C-driver (PCF8574) 
viser status på skjermen og kan kanskje en gang i fremtiden også vise hvor mange korker som har gått på en kveld eller den siste uka. Korkene telles av en 5mW laser
som skyter mot en fotodiode. Når korken passerer mellom dioden og laseren skaper det en skygge som får fotodioden til å trekke i en interrupt-pin på ESPen. 
Den sender et HTTP-Post Request til et skranglete jalla PHP5-endepunkt på den gamle nettsiden.
