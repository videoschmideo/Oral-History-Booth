/* Rotary dial control based on code by Guidomax
   https://www.instructables.com/id/Interface-a-rotary-phone-dial-to-an-Arduino/
   Modified by David Levin
   May 2018
   www.vivamarconi.net
*/

void hookReleased() {
  Serial.println("Hook released.");
  Serial.println();
  fullNumber = "";
  hookIsUp = true;
  pulses = 0;

}

void hookPressed() {
  Serial.println("Hook pressed.");
  hookIsUp = false;
  pulses = 0;
  fxFile = 0;
  fullNumber = "";
  fullNumberSendBuffer = 0;

}

bool checkHookChanged() { // NEED TO DEBOUNCE THIS SWITCH
  if ((millis() - lastHookStateChangeTime) > debounceDelay) { //debounce hook
    if (digitalRead(hookPin) == LOW) {
      if (hookEdge == 0) {
        hookReleased();
      }
      hookEdge = 1;
      lastHookStateChangeTime = millis(); // reset hook debounce
    } else {
      if (hookEdge == 1) {
        hookPressed();
      }
      hookEdge = 0;
    }
    lastHookStateChangeTime = millis(); // reset hook debounce
  }
}



void combinePhoneDigits() {
  if (fullNumber.length() < numLength) {
    currentNumber = (pulses % 10); // mod the pulse count by 10 because '0' will send 10 pulses.
    // Serial.print(number);
    fullNumber += currentNumber; // adds current number to string "fullNumber" to combine multiple digits
    Serial.println(fullNumber);
    Serial.println(fullNumber.length()); // checks length of string "fullNumner"
    Serial.println();

    if ((fullNumber.length() == numLength) && hookIsUp == true) // checks to see if string "fullNumber" is  longer than phone num length
    {
      fullNumberSendBuffer = fullNumber.toInt();
      Serial.print("FullNumSendBuffer: ");
      Serial.println(fullNumberSendBuffer);
      //      if (hookIsUp == false) {
      //      fullNumber = "";
      //  }
    }
  }
}


void readRotaryDial()
{
  byte reading = digitalRead(pulsePin);
  checkHookChanged();
  if (hookIsUp == true) { // only allows dial function when reciver is UP

    if ((millis() - lastStateChangeTime) > dialHasFinishedRotatingAfterMs) {
      // the dial isn't being dialed, or has just finished being dialed.
      if (readyToSendNum) {
        // if it's only just finished being dialed, send the number down the serial
        // line and reset the count.

        combinePhoneDigits();

        readyToSendNum = 0;
        pulses = 0;
        cleared = 0;
      }
    }

    if (reading != lastState) {
      lastStateChangeTime = millis();
    }
    if ((millis() - lastStateChangeTime) > debounceDelay) {
      // debounce - this happens once it's stablized
      if (reading != trueState) {
        // detects edge of pulse switch - either just gone from closed->open or vice versa.
        trueState = reading;
        if (trueState == HIGH) {
          // increment the count of pulses if the pulse pin has gone high.
          pulses++;
          readyToSendNum = 1; //  send/print the number (once the dial has finished rotating)
        }
      }
    }
    lastState = reading;
  }
}





void wrongNumberChecker() {
  for (byte x = 0; x < totalNumFolders; x++) {
    if (fullNumberSendBuffer != phoneNumbers[x]) {
      wrongNumber = true;
    } else {
      wrongNumber = false;
      return;
    }
  }
}


bool fileDone() {  // checks if file is done playing
  if (ansbuf[3] == 0x3d) {
    Serial.println(F("DONE"));
    return true;
  }
  else {
    return false;
  }
}



