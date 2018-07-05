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

#define   CHARLES_COE_1            6302000
#define   CHARLES_COE_2            6322201
#define   CHARLES_COE_3            6322202
#define   CHARLES_COE_4            6322203

#define   JAI_UNDERHILL_JOHNSON_1  6322200
#define   JAI_UNDERHILL_JOHNSON_2  6322201
#define   JAI_UNDERHILL_JOHNSON_3  6322202
#define   JAI_UNDERHILL_JOHNSON_4  6322203

#define   MAUREEN_REEN_1           6342400
#define   MAUREEN_REEN_2           6342401
#define   MAUREEN_REEN_3           6342402
#define   MAUREEN_REEN_4           6342403

#define   MICHAEL_POWELL_1         6352500
#define   MICHAEL_POWELL_2         6352501
#define   MICHAEL_POWELL_3         6352502

#define   JOHN_VINES_1             6362500
#define   JOHN_VINES_2             6362501
#define   JOHN_VINES_3             6362502

#define   DAVID_DORNEY_1           6312100
#define   DAVID_DORNEY_2           6312101
#define   DAVID_DORNEY_3           6312102
#define   DAVID_DORNEY_4           6312103

#define   TITO_AYALA_SPANISH_1     6382800
#define   TITO_AYALA_SPANISH_2     6382801
#define   TITO_AYALA_SPANISH_3     6382802
#define   TITO_AYALA_SPANISH_4     6382803

#define   TITO_AYALA_ENGLISH_1     6382850
#define   TITO_AYALA_ENGLISH_2     6382851
#define   TITO_AYALA_ENGLISH_3     6382852
#define   TITO_AYALA_ENGLISH_4     6382853

#define   OLIVE_KNIGHT_1           6392950
#define   OLIVE_KNIGHT_2           6392951
#define   OLIVE_KNIGHT_3           6392952
#define   OLIVE_KNIGHT_4           6392953

#define   DIALNUM_RANDOM           2861111


/*
  #define   DIALNUM_FOLDER_1   1111
  #define   DIALNUM_FOLDER_2   1112
  #define   DIALNUM_FOLDER_3   1113
  #define   DIALNUM_FOLDER_4   1114
  #define   DIALNUM_FOLDER_5   1105
  #define   DIALNUM_FOLDER_6   1106
  #define   DIALNUM_FOLDER_7   1107
  #define   DIALNUM_FOLDER_8   1108
  #define   DIALNUM_FOLDER_9   1109
  #define   DIALNUM_FOLDER_10  1010
  #define   DIALNUM_RANDOM     1211
*/
