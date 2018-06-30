/* Folder class written by David Levin
 *  May 2018
 *  Used to control multiple folders of audio independently
 *  www.videoschmideo.com
 */

class Folder
{
    // Class Member Variables. These are initialized at startup

    //  int arrayMaxSize;     // placeholder for max folder array size, as assigned in class def;
    int *arrPass;     // passes folder array pointer between functions
    byte folderNumber; // keeps track of folder number assigned in class def

    byte arraySlot; // keeps track of slots in array (to let us cycle through array incrementally)
    int buttonCounter; // counts # times folder is opened

    byte fileNumber; // keeps track of current file playing
    byte num; // number of files in a folder
    // Constructor - creates a "Drawer" and initializes the member variables and state

  public:
    Folder(int *arrPtr, byte fldrNum) // defines inputs to class
    {
      arrPass = arrPtr;
      folderNumber = fldrNum;
      buttonCounter = 0; // counts # times switch is triggered'
    }

    void allPlayedChecker(int numFilesInFolder)  // checks to see if all the files in a folder have been played
    {
      buttonCounter++;
      //  Serial.print(F("number of files in array played:  "));
      //  Serial.println(buttonCounter);
      if  (buttonCounter % numFilesInFolder == 0) {
        shuffle(numFilesInFolder);     // reshuffle file array if we get all the way through the file list
        buttonCounter = 0;     // reset buttonCounter to zero. NOTE: SEEMS TO ONLY SHUFFLE AFTER BUTTON IS RELEASED
      }
    }

    void shuffle(int numFiles)  // Shuffles the play order of files within a folder
    {
      int r;             // placeholder for folder shuffle output

      for (byte a = 0; a < numFiles; a++) {
        arrPass[a] = a;
      }

      for (byte a = 0; a < numFiles; a++) {
        r = random(a, numFiles); // random from a to total array number included.
        int temp = arrPass[a];
        arrPass[a] = arrPass[r];
        arrPass[r] = temp;
      }
         Serial.print(F("SHUFFLED! New order: "));
      for (byte a = 0; a < numFiles; a++) {

             Serial.print(arrPass[a] + 1);
             Serial.print(" ");
        if (a == (numFiles)) {
                Serial.println();
           delay(500);
        }
      }
         Serial.println();
    }


    void play() { // plays file based on inputs - folderNumber, FileNumber)
      fileNumber = arrPass[buttonCounter] + 1; //
      sendCommand(CMD_PLAY_FOLDER_FILE, folderNumber, fileNumber);

      //Uncomment for Debugging
      Serial.println("PLAYING");
        Serial.print("folder: ");
        Serial.println(folderNumber);
        Serial.print(F("file: "));
        Serial.println(fileNumber);  
         playing = true;
    }

 

}; // end of class



