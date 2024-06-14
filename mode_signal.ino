/**
1 stálé světlo
2 blikání
3 pomalé blikání
*/
int signalMode = 1;

bool signalRozsviceno;

#define SIGNAL_STANDARD 1000
#define SIGNAL_SLOW 5000

void signalAction()
{
  if( signalRozsviceno ) {
    zhasni();
    signalRozsviceno = false;
  } else {
    rozsvit();
    signalRozsviceno = true;
  }

  if( signalMode==2 ) {
    tasker.setTimeout(signalAction, SIGNAL_STANDARD );
  } else if( signalMode==3 ) {
    tasker.setTimeout(signalAction, SIGNAL_SLOW );
  }
  // pro rezim 1 se tasker nenastavi = zustane svetlo
}


void signalZacni( int mode ) {
  signalMode = mode;
  signalRozsviceno = false;
  tasker.setTimeout(signalAction, 1000 );
}
