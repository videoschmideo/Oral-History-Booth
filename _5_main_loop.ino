//create instances of folder object here using info above
/*
  Folder folder1(folder1array, folder1num, folder1size);
  Folder folder2(folder2array, folder2num, folder2size);
  Folder folder3(folder3array, folder3num, folder3size);
  Folder folder4(folder4array, folder4num, folder4size);
  Folder folder5(folder5array, folder5num, folder5size);
  Folder folder6(folder6array, folder6num, folder6size);
  Folder folder7(folder7array, folder7num, folder7size);
  Folder folder8(folder8array, folder8num, folder8size);
  Folder folder9(folder9array, folder9num, folder9size);
  // Folder folder10(folder10array, folder10num, folder10size);
*/
void setup()
{


  Serial.begin(9600);
  randomSeed(analogRead(0));
  mp3.begin(9600);
  delay(500);
  sendCommand(CMD_SEL_DEV, 0, 0);
  sendCommand(CMD_SET_VOLUME, 0, 16);

  delay(200);

  delay (10);
  for (byte x = 2; x < 4; x++)
  {
    pinMode(x, INPUT); // set pins on mcp chip as inputs. Uncomment to enable.
    pinMode(x, INPUT_PULLUP); // set input pin to normally low
  }
  Serial.begin(9600);
  pinMode(13, OUTPUT);  // use an LED on Pin 13 for debugging

  delay(100);

  checkNumFilesInfolders();

  folder1size = fileNumArray[0];
  folder2size = fileNumArray[1];
  /*  folder3size = fileNumArray[2];
    folder4size = fileNumArray[3];
    folder5size = fileNumArray[4];
    folder6size = fileNumArray[5];
    folder7size = fileNumArray[6];
    folder8size = fileNumArray[7];
    folder9size = fileNumArray[8];

      folder10size = fileNumArray[9];
      folder11size = fileNumArray[10];
      folder12size = fileNumArray[11];
      folder13size = fileNumArray[12];
      folder14size = fileNumArray[13];
      folder15size = fileNumArray[14];
      folder15size = fileNumArray[15];
      folder16size = fileNumArray[16];
      folder17size = fileNumArray[17];
      folder18size = fileNumArray[18];
      folder19size = fileNumArray[19];
      folder20size = fileNumArray[20];


    /*
    //  Serial.println();
    folder1.shuffle(folder1size);
    folder2.shuffle(folder2size);
    folder3.shuffle(folder3size);
    folder4.shuffle(folder4size);
    folder5.shuffle(folder5size);
    folder6.shuffle(folder6size);
    folder7.shuffle(folder7size);
    folder8.shuffle(folder8size);
    folder9.shuffle(folder9size);
    /*
       folder10.shuffle(folder10size);
        folder11.shuffle(folder11size);
       folder12.shuffle(folder12size);
       folder13.shuffle(folder13size);
       folder14.shuffle(folder14size);
       folder15.shuffle(folder15size);
       folder16.shuffle(folder16size);
       folder17.shuffle(folder17size);
       folder18.shuffle(folder18size);
       folder19.shuffle(folder19size);
       folder20.shuffle(folder20size);
  */

  Serial.println();
  Serial.println("***** READY! *****");
  //  Serial.println();


}

void checkNumFilesInfolders() {  // reads SD card, returns number of files in each folder

  if (initSent == false)  // if memory chest hasn't initialized (sensed # files in each folder)...
  {
    for (byte i = 1; i <= (totalNumFolders + 1); i++)  // run through all the folders...
    {
      sendSingleCommand(CMD_QUERY_FLDR_TRACKS, i);  //...and ask them how many files are inside.
      delay(100); // delay to give mp3 chip time to respond

      if (mp3.available())  // if there's data coming from the mp3 chip...
      {
        decodeMP3Answer();  // decode it...
        Serial.print(F("Recieving: "));
        Serial.println(decodeMP3Answer());
        Serial.print(F("File Number array: "));

        if (ansbuf[3] == 0x40)  // If the "error" code (0x40) comes up (meaning no files in a folder)...
        {
          fileNumArray[i - 2] = 0; // add 0 at fileNumArray slot "i - 2" (goes back and overwrites chip init / "message correct" feedback", which returns two zeros.)
          delay(10);
        }
        else
          fileNumArray[i - 2] = ansbuf[6]; // otherwise, add the number of files to slot "i - 2"
        delay(10);
        mp3.flush();
      }
      for (byte x = 0; x < (totalNumFolders); x++) {
        Serial.print(fileNumArray[x], DEC);
        Serial.print(" ");
      }
      Serial.println();
      Serial.println();

      initSent = true; // all done with initializing. Only need to do it once.
    }
  }
}

void play(int fldrNum, int fileNum) { // plays file based on inputs - folderNumber, FileNumber
  int fileNumber = fileNum;
  int folderNumbner = fldrNum;
  sendCommand(CMD_PLAY_FOLDER_FILE, folderNumber, fileNumber);
 
  //Uncomment for Debugging
  Serial.println("PLAYING");
  Serial.print("folder: ");
  Serial.println(folderNumber);
  Serial.print(F("file: "));
  Serial.println(fileNumber);
  playing = true;
}

void setRandomDelayTime () {
  int randomDelayNum = random(ringerMinDelay, ringerMaxDelay);
  ringerDelay = (randomDelayNum * 1000);
  Serial.println(ringerDelay);

}

void loop() {

  unsigned long currentMillis = millis(); // sets up timer for ringer

  readRotaryDial(); // reads rotary dial
  wrongNumberChecker(); // checks for wrong numbers

  static bool playingAudio = false; // variable tracks whether mp3 chip is playing
  static bool doOnlyOnce = false; // corrects error on Mp3 chip that sends serial reply twice (only allows the first reply through)

  if (hookIsUp) { // if the hook is up...
    if (currentPhoneState == HUNG_UP && !playingAudio) // if audio isn't playing,
    {
      currentPhoneState = DIAL_TONE;   // set FX variable to "dial tone" status (1)
      sendCommand(CMD_PLAY_W_INDEX, 0, 1);    // play dial tone...
      Serial.println();
      Serial.println("dial tone");
      Serial.println();
      folderOpen = true;    // tell system there is in fact a folder open...
      playingAudio = true;  // and tell the system that there's audio playing...
    }

    unsigned long currentMillis = millis(); // sets up timer for ringer

    if (currentPhoneState == DIAL_TONE && (fullNumber.length() == numLength)) //.. if dial tone playing AND a legit phone number length has been dialed AND the hook is up...
    {
      currentPhoneState = RINGING;

      if (currentPhoneState == RINGING && wrongNumber) {
        Serial.println();
        Serial.println("wrong number");
        Serial.println();
        sendCommand(CMD_PLAY_W_INDEX, 0, 3);   // play operator error message
        folderOpen = true;
        playingAudio = true;
      }
      else if (currentPhoneState == RINGING && !wrongNumber) // if the phone is ringing... AND the number dialed is correct...
      {
        Serial.println();
        Serial.println("ringing");
        Serial.println();
        setRandomDelayTime();
        sendCommand(CMD_PLAY_W_INDEX, 0, 2);    // play ringing tone...
        folderOpen = true;
        playingAudio = true;
        //delay(delayTime); // give ringing a chance to happen
        previousMillis = currentMillis;
      }
    }

    if (currentPhoneState == RINGING && (currentMillis - previousMillis > ringerDelay)) { // randomize this value for varied ringing

      currentPhoneState = READY_TO_PLAY_FOLDER;

      if (fullNumberSendBuffer == DIALNUM_RANDOM) {
        int randomPhoneNum = random(0, (totalNumFolders - 1));
        fullNumberSendBuffer = phoneNumbers[randomPhoneNum];
        Serial.println(randomPhoneNum);
        Serial.println(fullNumberSendBuffer);
      }

      else if (fullNumberSendBuffer != DIALNUM_RANDOM) {
        for (int i = 0; i < totalNumFiles; i++) {
          if (fullNumberSendBuffer == phoneNumbers[i]) { // if dialed number is legit...
            folderNumber = 1;     // set folderNumber to this folder (only used when calling "folder" class - sends folder # info to class)
            int playFile = i + 1;
            folderOpen = true;    // tell system there is in fact a folder open...
            play(folderNumber, playFile);       // and then play the first file.
            playingAudio = true;  // Now tell the system that there's audio playing..

          }
        }
      }
    }
  }


  if (mp3.available()) // Checks for reply from mp3 player when player is queried
  {
    decodeMP3Answer(); // decode binary and turn bytes into HEX
    if (ansbuf[3] == 0x3d) // if the HEX code for "file is done" comes in,
    {
      doOnlyOnce = !doOnlyOnce; // ... only pay attention to that first reply,
     /*if (doOnlyOnce)
      {
        Serial.println();
        Serial.println(F("finished playing audio file"));
        Serial.println();
        playingAudio = false; //...and tell the system there's NO AUDIO playing.
       
      }
*/
    }
  }



  if (folderOpen && !hookIsUp) // if a folder is active and hook is closed,
  {
    sendCommand(CMD_STOP_PLAY, 0, 0); // tell mp3 player to stsop what it's doing...
    fullNumberSendBuffer = 0;
    fullNumber = "";
    playingAudio = 0;                 // tell the rest of the system there's no audio playing...
    folderNumber = 0;
    folderOpen = false;               // ...and that there are no folders are active.
    playing = false;
    currentPhoneState = 0;
    Serial.println();
    Serial.println(F("all folders CLOSED!"));
    Serial.println();

  }

  delay(10);
}







