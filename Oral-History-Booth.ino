/*  Oral History Booth player v1.5
   by David Levin
   June 2018
   www.vivamarconi.net

/**********Libraries*************/
#include <SoftwareSerial.h>
#include <Wire.h>
/********************************/
// DO NOT DELETE ABOVE THIS LINE // 


///////////////////////////////////////////////////////
/*****set "phone numbers" for folders here******///////
///////////////////////////////////////////////////////

/*
#define   DIALNUM_FOLDER_1   6302000
#define   DIALNUM_FOLDER_2   6322200
#define   DIALNUM_FOLDER_3   6342400
#define   DIALNUM_FOLDER_4   6312100
#define   DIALNUM_FOLDER_5   1105
#define   DIALNUM_FOLDER_6   1106
#define   DIALNUM_FOLDER_7   1107
#define   DIALNUM_FOLDER_8   1108
#define   DIALNUM_FOLDER_9   1109
#define   DIALNUM_FOLDER_10  1010
#define   DIALNUM_RANDOM     2861111
*/


const byte folderMaxSize  = 9; // max # files allowed in a folder (ie, # of slots in each folder array)
const int totalNumFolders = 25; // max # of folders

long dialRandom =            2861111;
long beachLife =             6122000;
long busingCrisis =          6133000;
long changingNeigborhoods =  6144000;
long charlesRiver =          6122000;
long costOfLiving =          6166000;
long disabilities =          6177000;
long friendsAndFamily =      6188000;
long genderIdentity =        6199000;
long growingUpInBoston =     6201000;
long HIV_AIDS  =             6211100;
long immigration =           6221200;
long marriage =              6231300;
long movingToBoston  =       6241400;
long politics  =             6261600;
long raceAndRacism =         6271700;
long spirituality_religion = 6281800;
long work_vocation =         6291900;
long Charles_Coe =           6302000;
long David_Dorney =          6312100;
long Jai_Underhill_Johnson = 6322200;
long John_Vines  =           6332300;
long Maureen_Reen  =         6342400;
long Michael_Powell  =       6352500;
            
const long DIALNUM_FOLDER_1  = beachLife;
const long DIALNUM_FOLDER_2  = busingCrisis;
const long DIALNUM_FOLDER_3  = changingNeigborhoods;
const long DIALNUM_FOLDER_4  = charlesRiver;
const long DIALNUM_FOLDER_5  = costOfLiving;
const long DIALNUM_FOLDER_6  = disabilities;
const long DIALNUM_FOLDER_7  = friendsAndFamily;
const long DIALNUM_FOLDER_8  = genderIdentity;
const long DIALNUM_FOLDER_9  = growingUpInBoston;
const long DIALNUM_FOLDER_10 = HIV_AIDS;
const long DIALNUM_FOLDER_11 = immigration;
const long DIALNUM_FOLDER_12 = marriage;
const long DIALNUM_FOLDER_13 = movingToBoston;
const long DIALNUM_FOLDER_15 = politics;
const long DIALNUM_FOLDER_16 = raceAndRacism;
const long DIALNUM_FOLDER_17 = spirituality_religion;
const long DIALNUM_FOLDER_18 = work_vocation;
const long DIALNUM_FOLDER_19 = Charles_Coe;
const long DIALNUM_FOLDER_20 = David_Dorney;
const long DIALNUM_FOLDER_21 = Jai_Underhill_Johnson;
const long DIALNUM_FOLDER_22 = John_Vines;
const long DIALNUM_FOLDER_23 = Maureen_Reen;
const long DIALNUM_FOLDER_24 = Michael_Powell;

const int DIALNUM_RANDOM = dialRandom;
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
