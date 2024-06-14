
const char * morseCharacters[] PROGMEM = {
    ".-",
    "-...",
    "-.-.",
    "-..",
    ".",
    "..-.",
    "--.",
    "....",
    "..",
    ".---",
    "-.-",
    ".-..",
    "--",
    "-.",
    "---",
    ".--.",
    "--.-",
    ".-.",
    "...",
    "-",
    "..-",
    "...-",
    ".--",
    "-..-",
    "-.--",
    "--.."
  };

const char * morseNumbers[] PROGMEM = {
    ".----",
    "..---",
    "...--",
    "....-",
    ".....",
    "-....",
    "--...",
    "---..",
    "----.",
    "-----"
  };

/* czech dialect of Morse code contains character "ch" */
const char * morseLiteral_Ch PROGMEM = "----";

int morseSpeed;
char morseText[200];

int nextCharOffset = 1;

/** pozice ve zdrojovem textu */
int morseSrcPos;
/** pozice v codetab aktualniho znaku */
int morseInCharPos;

bool vysilamZnacku;

int pocetVysilani;

void morseZacni( char * text, int speed ) {
  if( strlen(text)==0 ) {
    strcpy( morseText, "SOS" );
  } else {
    strcpy( morseText, text );
  }
  strlwr( morseText );
  morseSpeed = speed;
  logger->log( "+++ morse: [%s] speed=%d", morseText, morseSpeed);

  morseSrcPos = 0;
  morseInCharPos = 0;
  pocetVysilani = 0;
  vysilamZnacku = false;
  nextCharOffset = 1;

  tasker.setTimeout(morseAction, 1 );
}

#define BASE_SPEED_CARKA 1000
#define BASE_SPEED_TECKA 250
#define BASE_SPEED_PAUZA 1000
#define BASE_SPEED_MEZERA 3000
#define BASE_SPEED_KONEC 6000

void doDelay( int baseDelay ) {
  double d = (double)baseDelay;
  double factor = ((double)morseSpeed) / 100.0;
  d = d / factor;
  tasker.setTimeout(morseAction, (long)d );
}

char logBuffer[100];



void posliJednuZnacku( char * table ) {
  if( vysilamZnacku ) {
    // nedelam nic, pauza po znacce
    logger->log( "%s mezera", logBuffer );
    vysilamZnacku = false;
    zhasni();
    doDelay( BASE_SPEED_PAUZA );
    return;
  }

  char znacka = table[morseInCharPos];
  if( znacka==0 ) {
    logger->log( "%s konec", logBuffer );
    morseSrcPos+=nextCharOffset;
    morseInCharPos = 0;
    zhasni();
    doDelay( BASE_SPEED_MEZERA );
    return;
  }

  vysilamZnacku = true;
  if( znacka=='.' ) {
    logger->log( "%s *", logBuffer );
    morseInCharPos++;
    rozsvit();
    doDelay( BASE_SPEED_TECKA );
    return;
  }
  if( znacka=='-' ) {
    logger->log( "%s --", logBuffer );
    morseInCharPos++;
    rozsvit();
    doDelay( BASE_SPEED_CARKA );
    return;
  }

  logger->log( "CHYBA, tady nemam byt");
}



void morseAction() 
{
  char * pos = morseText + morseSrcPos;
  if( *pos == 0 ) {
    logger->log( "M: konec, zacinam znovu");
    pocetVysilani++;
    morseSrcPos = 0;
    morseInCharPos = 0;
    vysilamZnacku = false;
    doDelay( BASE_SPEED_KONEC );
    return;
  } 
  if( *pos == ' ' || *pos == ',' || *pos == '.' ) {
    logger->log( "M: mezera");
    morseSrcPos++;
    doDelay( BASE_SPEED_MEZERA );
    return;
  }
  if( *pos=='c' && *(pos+1)=='h' ) {
    // vysilame ch
    nextCharOffset=2;
    sprintf(logBuffer, "M: ch %d", morseInCharPos );
    char * table = (char*)morseLiteral_Ch;
    posliJednuZnacku( table );
    return;
  }
  if( *pos>='a' && *pos<='z' ) {
    // vysilame pismeno
    nextCharOffset=1;
    sprintf(logBuffer, "M: %c %d", *pos, morseInCharPos );
    char * table = (char*) morseCharacters[(*pos)-'a'];
    posliJednuZnacku( table );
    return;
  }
  if( *pos>='0' && *pos<='9' ) {
    // vysilame cislo
    nextCharOffset=1;
    sprintf(logBuffer, "M: %c %d", *pos, morseInCharPos );
    char * table = (char*) morseNumbers[(*pos)-'0'];
    posliJednuZnacku( table );
    return;
  }
  // je to nejaky nesmysl
  morseSrcPos++;
}


