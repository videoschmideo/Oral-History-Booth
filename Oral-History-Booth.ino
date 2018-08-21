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
/////////////////////////////////////¨//////////////////
const int folderMaxSize  = 50; // max # files allowed in a folder (ie, # of slots in each folder array)
const int totalNumFolders = 2; // max # of folders
const int totalNumFiles = 30; // total # files across folders

#define   CHARLES_COE_1            6302000 // Black and Gold: A Blues Song
#define   CHARLES_COE_2            6302001 // Going Up South
#define   CHARLES_COE_3            6302002 // Recovering from Loss
#define   CHARLES_COE_4            6302003 // Passage: A Poem

#define   JAI_UNDERHILL_JOHNSON_1  6322203 // The Noise I Make In My Life
#define   JAI_UNDERHILL_JOHNSON_2  6322202 // Standing Up for Young People
#define   JAI_UNDERHILL_JOHNSON_3  6322201 // I’m Sorry That Happened to You
#define   JAI_UNDERHILL_JOHNSON_4  6322200 // A Sculpture for Trayvon

#define   MAUREEN_REEN_1           6342400 // A Drug Dealer Slashed My Tires
#define   MAUREEN_REEN_2           6342401 // Clash at a Charlestown Swim Meet
#define   MAUREEN_REEN_3           6342402 // My Sister Left the… Knife in Her Bra
#define   MAUREEN_REEN_4           6342403 // Visiting a South Boston Beach

#define   MICHAEL_POWELL_1         6352500 // The Freedom House
#define   MICHAEL_POWELL_2         6352501 // How Martin Met Coretta
#define   MICHAEL_POWELL_3         6352502 // I Grew Up in St. Joseph’s…Roxbury

#define   JOHN_VINES_1             6362501 // Advice for Boston's Mayor
#define   JOHN_VINES_2             6362502 // Living with Disability
#define   JOHN_VINES_3             6362503 // Safe and Nurturing Touch

#define   DAVID_DORNEY_1           6312100 // A Library is My Second Home
#define   DAVID_DORNEY_2           6312101 // Gold Stars in the Windows 
#define   DAVID_DORNEY_3           6312102 // I Ran Upstairs to Get a Sheath Knife
#define   DAVID_DORNEY_4           6312103 // Life Lessons Learned

#define   TITO_AYALA_SPANISH_1     6372700 // La Importancia del Arte
#define   TITO_AYALA_SPANISH_2     6372701 // Tocando la Bomba
#define   TITO_AYALA_SPANISH_3     6272702 // Todos Somos Hijos de Dios

#define   OLIVE_KNIGHT_1           6382803 // Some of My Favorite Patrons  
#define   OLIVE_KNIGHT_2           6382804 // Why I Love Libraries 
#define   OLIVE_KNIGHT_3           6382800 // A Visit to the Planetarium  
#define   OLIVE_KNIGHT_4           6382801 // On Being Followed in a Boutique 

#define   PROJECT_INTRO            3111313
#define   DIALNUM_RANDOM           2861111


