/*  Oral History Booth player v1.5
   by David Levin
   June 2018
   www.vivamarconi.net

   Based on demo code for Serial MP3 Player Catalex (YX5300 chip)
   Hardware: Serial MP3 Player *1
   Board:  Arduino Pro Mini
   http://www.dx.com/p/uart-control-serial-mp3-music-player-module-for-arduino-avr-arm-pic-blue-silver-342439#.VfHyobPh5z0



  /*********MP3 player constants*********/
//If using Arduino Pro Mini:
#define ARDUINO_RX 5  //should connect to TX of the Serial MP3 Player module 
#define ARDUINO_TX 6  //connect to RX of the module

/**********Dialer Constants******/
#define numLength 7
#define hookPin 4
#define pulsePin 3

#define HUNG_UP 0
#define DIAL_TONE 1
#define CHECKING_NUMBER 2
#define RINGING 3
#define READY_TO_PLAY_FOLDER 4


/************ MP3 player Serial Command bytes **************************/

#define CMD_NEXT_SONG     0X01  // Play next song.
#define CMD_PREV_SONG     0X02  // Play previous song.
#define CMD_PLAY_W_INDEX  0X03
#define CMD_VOLUME_UP     0X04
#define CMD_VOLUME_DOWN   0X05
#define CMD_SET_VOLUME    0X06

#define CMD_SNG_CYCL_PLAY 0X08  // Single Cycle Play.
#define CMD_SEL_DEV       0X09
#define CMD_SLEEP_MODE    0X0A
#define CMD_WAKE_UP       0X0B
#define CMD_RESET         0X0C
#define CMD_PLAY          0X0D
#define CMD_PAUSE      MD_STOP_PLAY   0X0E
#define CMD_PLAY_FOLDER_FILE 0X0F

#define CMD_STOP_PLAY    0X16
#define CMD_FOLDER_CYCLE  0X17
#define CMD_SHUFFLE_PLAY  0x18 //
#define CMD_SET_SNGL_CYCL 0X19 // Set single cycle.

#define CMD_SET_DAC 0X1A
#define DAC_OFF 0X01

#define CMD_PLAY_W_VOL        0X22
#define CMD_PLAYING_N         0x4C
#define CMD_QUERY_STATUS      0x42
#define CMD_QUERY_VOLUME      0x43
#define CMD_QUERY_FLDR_TRACKS 0x4e
#define CMD_QUERY_TOT_TRACKS  0x48
#define CMD_QUERY_FLDR_COUNT  0x4f

#define SINGLE_CYCLE_OFF  0X01

/*********************************************************************/


/////////////////////////////////
//// GLOBAL VARIABLES (BELOW) //
///////////////////////////////

/************variables for timer/delay**************/

long previousMillis = 0;
int ringerMinDelay = 3; // in whole seconds
int ringerMaxDelay = 8; // in whole seconds
int ringerDelay = 0;
/************variables for dialer control**************/

long tempNumBuffer = 0; // holds phone number temporarily
bool readyToSendNum = 0;
bool trueState = LOW; // counts number of pulses only on leading edge of pulse
bool lastState = LOW; // compares current state of pulse (high or low) to last state to find edge
long lastStateChangeTime = 0; // debounces dial pulses
long lastHookStateChangeTime = 0; // debounces hook switch
bool cleared = 0;
int pulses = 0; // counts pulses from rotary dial

char hookEdge = 1;  // checks edge state of reciever hook
bool hookIsUp = false;
bool formerHookState = false;

String fullNumber = ""; //  holds several previously dialed single numbers in one string
long fullNumberSendBuffer = 0; // holds full number once numLength reached

int currentNumber = 0; // holds number just dialed on rotary phone

int dialHasFinishedRotatingAfterMs = 100;
int debounceDelay = 10;


/****************** variables for Serial MP3 player *****************/
static int8_t Send_buf[8] = {0}; // Buffer for Send commands.  // BETTER LOCALLY
static uint8_t ansbuf[10] = {0}; // Buffer for the answers.    // BETTER LOCALLY
static int8_t pre_vol, volume = 0x0f; // Volume. 0-30 DEC values. 0x0f = 15.
String mp3Answer;           // Answer from the MP3.
boolean playing = false;    // switch Play to Pause or viceversa.

SoftwareSerial mp3(ARDUINO_RX, ARDUINO_TX);


/*********** Variables for folder trigger/file logic***************/


boolean folderOpen = false; // tracks if ANY folder is open
int folderNumber; // allows feedback on which folder is open (for debugging)
byte incomingByte; // incoming byte from mp3 player


bool initSent = false; // checks to see if folders initialized
int fileNumArray[totalNumFolders]; // holds array of # of files in each folder
int currentPhoneState = 0; // variable to reflect which FX file is playing - (dial tone, ring tone, wrong number/busy signal, etc)

bool wrongNumber = false; // toggles true if number dialed isn't in the list of allowable numbers

const long phoneNumbers[(totalNumFiles + 1)] = {
  CHARLES_COE_1,
  CHARLES_COE_2,
  CHARLES_COE_3,
  CHARLES_COE_4,
  JAI_UNDERHILL_JOHNSON_1,
  JAI_UNDERHILL_JOHNSON_2,
  JAI_UNDERHILL_JOHNSON_3,
  JAI_UNDERHILL_JOHNSON_4,
  MAUREEN_REEN_1,
  MAUREEN_REEN_2,
  MAUREEN_REEN_3,
  MAUREEN_REEN_4,
  MICHAEL_POWELL_1,
  MICHAEL_POWELL_2,
  MICHAEL_POWELL_3,
  JOHN_VINES_1,
  JOHN_VINES_2,
  JOHN_VINES_3,
  DAVID_DORNEY_1,
  DAVID_DORNEY_2,
  DAVID_DORNEY_3,
  DAVID_DORNEY_4,
  TITO_AYALA_SPANISH_1,
  TITO_AYALA_SPANISH_2,
  TITO_AYALA_SPANISH_3,
  OLIVE_KNIGHT_1,
  OLIVE_KNIGHT_2,
  OLIVE_KNIGHT_3,
  OLIVE_KNIGHT_4,
  PROJECT_INTRO,
  DIALNUM_RANDOM

};





//Variables for folder number, file count per folder
int folder1num = 1;
int folder2num = 2;
/*int folder3num = 3;
int folder4num = 4;
int folder5num = 5;
int folder6num = 6;
int folder7num = 7;
int folder8num = 8;
int folder9num = 9;

*/
//define number of files in each folder here
// sets # of files in each folder (up to existing max limit), sends value to instances of "folder" class objects
int folder1size;
int folder2size;
/*int folder3size;
int folder4size;
int folder5size;
int folder6size;
int folder7size;
int folder8size;
int folder9size;

*/

// create variables for array, size, folder state here. Need one for each folder.
int folder1array[folderMaxSize];
int folder2array[folderMaxSize];
/*int folder3array[folderMaxSize];
int folder4array[folderMaxSize];
int folder5array[folderMaxSize];
int folder6array[folderMaxSize];
int folder7array[folderMaxSize];
int folder8array[folderMaxSize];
int folder9array[folderMaxSize];
/*


