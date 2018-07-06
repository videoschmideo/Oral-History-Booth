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
#define CMD_PAUSE         0X0E
#define CMD_PLAY_FOLDER_FILE 0X0F

#define CMD_STOP_PLAY     0X16
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
byte pulses = 0; // counts pulses from rotary dial

char hookEdge = 1;  // checks edge state of reciever hook
bool hookIsUp = false;
bool formerHookState = false;

String fullNumber = ""; //  holds several previously dialed single numbers in one string
long fullNumberSendBuffer = 0; // holds full number once numLength reached

byte currentNumber = 0; // holds number just dialed on rotary phone

word dialHasFinishedRotatingAfterMs = 100;
byte debounceDelay = 10;


/****************** variables for Serial MP3 player *****************/
static int8_t Send_buf[8] = {0}; // Buffer for Send commands.  // BETTER LOCALLY
static uint8_t ansbuf[10] = {0}; // Buffer for the answers.    // BETTER LOCALLY
static int8_t pre_vol, volume = 0x0f; // Volume. 0-30 DEC values. 0x0f = 15.
String mp3Answer;           // Answer from the MP3.
boolean playing = false;    // switch Play to Pause or viceversa.

SoftwareSerial mp3(ARDUINO_RX, ARDUINO_TX);


/*********** Variables for folder trigger/file logic***************/

const byte folderMaxSize  = 5; // max # files allowed in a folder (ie, # of slots in each folder array)
const int totalNumFolders = 9; // max # of folders
const int totalNumFiles = 34; // total # files across folders

boolean folderOpen = false; // tracks if ANY folder is open
byte folderNumber; // allows feedback on which folder is open (for debugging)
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
  TITO_AYALA_SPANISH_4,

  TITO_AYALA_ENGLISH_1,
  TITO_AYALA_ENGLISH_2,
  TITO_AYALA_ENGLISH_3,
  TITO_AYALA_ENGLISH_4,

  OLIVE_KNIGHT_1,
  OLIVE_KNIGHT_2,
  OLIVE_KNIGHT_3,
  OLIVE_KNIGHT_4,

  DIALNUM_RANDOM
};




/*
//Variables for folder number, file count per folder
const byte PROGMEM folder1num = 1;
const byte PROGMEM folder2num = 2;
const byte PROGMEM folder3num = 3;
const byte PROGMEM folder4num = 4;
const byte PROGMEM folder5num = 5;
const byte PROGMEM folder6num = 6;
const byte PROGMEM folder7num = 7;
const byte PROGMEM folder8num = 8;
const byte PROGMEM folder9num = 9;
/*
  const byte PROGMEM folder10num = 10;
  const byte PROGMEM folder11num = 11;
  const byte PROGMEM folder12num = 12;
  const byte PROGMEM folder13num = 13;
  const byte PROGMEM folder14num = 14;
  const byte PROGMEM folder15num = 15;
  const byte PROGMEM folder16num = 16;
  const byte PROGMEM folder17num = 17;
  const byte PROGMEM folder18num = 18;
  const byte PROGMEM folder19num = 19;
  const byte PROGMEM folder20num = 20;


//define number of files in each folder here
// sets # of files in each folder (up to existing max limit), sends value to instances of "folder" class objects
byte folder1size;
byte folder2size;
byte folder3size;
byte folder4size;
byte folder5size;
byte folder6size;
byte folder7size;
byte folder8size;
byte folder9size;
/*
  byte folder10size;
  byte folder11size;
  byte folder12size;
  byte folder13size;
  byte folder14size;
  byte folder15size;
  byte folder16size;
  byte folder17size;
  byte folder18size;
  byte folder19size;
  byte folder20size;


// create variables for array, size, folder state here. Need one for each folder.
const word PROGMEM folder1array[folderMaxSize];
const word PROGMEM folder2array[folderMaxSize];
const word PROGMEM folder3array[folderMaxSize];
const word PROGMEM folder4array[folderMaxSize];
const word PROGMEM folder5array[folderMaxSize];
const word PROGMEM folder6array[folderMaxSize];
const word PROGMEM folder7array[folderMaxSize];
const word PROGMEM folder8array[folderMaxSize];
const word PROGMEM folder9array[folderMaxSize];
/*
  const word PROGMEM folder10array[folderMaxSize];
  const word PROGMEM folder11array[folderMaxSize];
  const word PROGMEM folder12array[folderMaxSize];
  const word PROGMEM folder13array[folderMaxSize];
  const word PROGMEM folder14array[folderMaxSize];
  const word PROGMEM folder15array[folderMaxSize];
  const word PROGMEM folder16array[folderMaxSize];
  const word PROGMEM folder17array[folderMaxSize];
  const word PROGMEM folder18array[folderMaxSize];
  const word PROGMEM folder19array[folderMaxSize];
  const word PROGMEM folder20array[folderMaxSize];
*/


