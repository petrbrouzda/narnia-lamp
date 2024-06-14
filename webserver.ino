
const char hlavicka[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Narnia lampa</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: left;}
    h2 {font-size: 2.0rem;}
    h3 {font-size: 1.65rem; font-weight: 600}
    p {font-size: 1.4rem;}
    input {font-size: 1.4rem;}
    input#text {width: 100%;}
    select {font-size: 1.4rem;}
    form {font-size: 1.4rem;}
    body {max-width: 600px; margin:10px ; padding-bottom: 25px;}
  </style>
</head>
<body>
<h2>Narnia lampa</h2>
<form method="GET" action="/">
 <input type="submit" name="obnov" value="Obnov stav" > 
</form>
)rawliteral";

const char paticka[] PROGMEM = R"rawliteral(
</body>
</html>
)rawliteral";


class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");
    return true;
  }

  void handleRequest(AsyncWebServerRequest *request) {
    // request->send_P(200, "text/html", index_html); 
    logger->log( "  > %s%s", request->host().c_str(), request->url().c_str() );
    request->redirect("http://192.168.1.1/");
  }
};


const char* CONTENT_TYPE = "text/html; charset=utf-8";

void vlozJas( AsyncResponseStream *response ) {
    response->print("<p>Jas: <select name=\"jas\" >");
    response->printf("<option value=\"255\" %s>100 %%</option>", brightness==255 ? "selected" : "" );
    response->printf("<option value=\"128\" %s>50 %%</option>", brightness==128 ? "selected" : "" );
    response->printf("<option value=\"64\" %s>25 %%</option>", brightness==64 ? "selected" : "" );
    response->printf("<option value=\"32\" %s>12 %%</option></select>", brightness==32 ? "selected" : "" );
  }

void vlozBarvu( AsyncResponseStream *response ) {
    response->print("<p>Barva: <select name=\"barva\" >");
    response->printf("<option value=\"16777215\" %s>Bílá</option>", color==16777215 ? "selected" : "" );
    response->printf("<option value=\"16711680\" %s>Červená</option>", color==16711680 ? "selected" : "" );
    response->printf("<option value=\"65280\" %s>Zelená</option>", color==65280 ? "selected" : "" );
    response->printf("<option value=\"255\" %s>Modrá</option></select>", color==255 ? "selected" : "" );
  }




void onRequestStatus(AsyncWebServerRequest *request){
  logger->log( "* status");

  //Handle Unknown Request
  AsyncResponseStream *response = request->beginResponseStream(CONTENT_TYPE);
  response->print( hlavicka );

  response->printf( "<p>Režim: %d</p>", aktualniRezim );

  if( aktualniRezim==3 ) {
    response->printf( "<p>Morseovka [<b>%s</b>], pozice %d/%d, pocet odeslani: %d.</p>", morseText, morseSrcPos+1, strlen(morseText), pocetVysilani );
  } 
  if( aktualniRezim==1 ) {
    response->printf( "<p>Oheň</p>" );
  }
  if( aktualniRezim>1 ) {
    const char * barva;
    if( color==16777215 ) {
      barva = "bílá";
    } else if( color==16711680 ) {
      barva = "červená";
    }else if( color==65280 ) {
      barva = "zelená";
    }else if( color==255 ) {
      barva = "modrá";
    } else {
      barva = "???";
    }

    response->printf( "<p>Jas: %d %%, barva: %s</p>", ((brightness+1)*100)/256, barva );
  }

const char part1a[] PROGMEM = R"rawliteral(
<hr>
<h3>Nastavení režimu</h3>
  
  <p><a name="0"></a><b>Vypnuto</b></p>

  <form method="GET" action="/rezim0">
  <input type="submit" name="nastav" value="Zhasni lampu" > 
  </form>

<hr width="40%">
  <p><a name="1"></a><b>Oheň</b></p>

  <form method="GET" action="/rezim1">
)rawliteral";

const char part1b[] PROGMEM = R"rawliteral(
  <p><input type="submit" name="nastav" value="Zapal oheň" > 
  </form>
  
<hr width="40%">
  <p><a name="2"></a><b>Signál</b></p>

  <form method="GET" action="/rezim2">
)rawliteral";

  response->print( part1a );
  vlozJas( response );
  response->print( part1b );

    vlozJas( response );
    vlozBarvu( response );

  response->print("<p>Režim: <select name=\"mode\" >");
    response->printf("<option value=\"1\" %s>stálé světlo</option>" , signalMode==1 ? "selected" : "");
	  response->printf("<option value=\"2\" %s>blikání</option>" , signalMode==2 ? "selected" : "");
	  response->printf("<option value=\"3\" %s>pomalé blikání</option>" , signalMode==3 ? "selected" : "");

const char part2[] PROGMEM = R"rawliteral(
</select>
  <p><input type="submit" name="nastav" value="Spusť signál" > 
  </form>  

<hr width="40%">
  <p><a name="3"></a><b>Morseovka</b></p>

  <form method="GET" action="/rezim3">
)rawliteral";

  response->print( part2 );

  response->printf("Text:<br><input type=\"text\" name=\"text\" id=\"text\" value=\"abcd\"> ");
    vlozJas( response );
    vlozBarvu( response );

  	response->print("<p>Rychlost: <select name=\"speed\" > ");
	    response->printf(" <option value=\"200\">2x rychleji</option> ");
	    response->printf(" <option value=\"100\" selected>Standard</option> ");
	    response->printf(" <option value=\"50\">50 %</option> ");
	    response->printf(" <option value=\"25\">25 %</option> ");

      response->print("</select> <p><input type=\"submit\" name=\"nastav\" value=\"Vysílej\" ></form>");
  
  response->printf( "<hr><p>Lokalni cas (sec od bootu): %d</p>", time(NULL) );

  response->print( paticka );
  request->send(response);
}

void clearTimers() {
  tasker.setTimeout(morseAction, 0 );
  tasker.setTimeout(fireAction, 0 );
  tasker.setTimeout(signalAction, 0 );
}

void onRequestRezim0(AsyncWebServerRequest *request){
  clearTimers();

  logger->log( "* rezim 0 - zhasnuto");

  aktualniRezim = 0;
  saveConfigData();

  request->redirect("/#0");
}

void onRequestRezim1(AsyncWebServerRequest *request){
  clearTimers();

  logger->log( "* rezim 1 - ohen");

  String input;
  if(request->hasParam("jas") ) { 
    input = request->getParam("jas")->value();
    brightness = atol( input.c_str() );
    brightnessChange = true;
  }
  
  aktualniRezim = 1;
  saveConfigData();

  tasker.setTimeout(fireAction, 1 );

  request->redirect("/#1");
}

void onRequestRezim2(AsyncWebServerRequest *request){
  clearTimers();

  logger->log( "* rezim 2 - blikej");
  
  color = 0xffffff;
  int mode = 1;

  String input;
  if(request->hasParam("jas") ) { 
    input = request->getParam("jas")->value();
    brightness = atol( input.c_str() );
    brightnessChange = true;
  }
  if(request->hasParam("barva") ) { 
    input = request->getParam("barva")->value();
    color = atol( input.c_str() );
  }
  if(request->hasParam("mode") ) { 
    input = request->getParam("mode")->value();
    mode = atol( input.c_str() );
  }
  aktualniRezim = 2;
  
  char buffer[20];
  sprintf( buffer, "%d", mode );
  config.setValue( "signalMode", buffer );
  saveConfigData();

  signalZacni( mode );

  request->redirect("/#2");
}

void onRequestRezim3(AsyncWebServerRequest *request){
  clearTimers();

  logger->log( "* rezim 3 - morse");

  // text, barva, speed
  char text[200];
  color = 0xffffff;
  int speed = 100;
  strcpy( text, "" );

  String input;
  if(request->hasParam("jas") ) { 
    input = request->getParam("jas")->value();
    brightness = atol( input.c_str() );
    brightnessChange = true;
  }
  if(request->hasParam("text") ) { 
    input = request->getParam("text")->value();
    strncpy( text, input.c_str(), 199 );
    text[199] = 0;
  }
  if(request->hasParam("speed") ) { 
    input = request->getParam("speed")->value();
    speed = atol( input.c_str() );
  }
  if(request->hasParam("barva") ) { 
    input = request->getParam("barva")->value();
    color = atol( input.c_str() );
  }

  char buffer[20];
  config.setValue( "morseText", text );
  sprintf( buffer, "%d", speed );
  config.setValue( "morseSpeed", buffer );

  aktualniRezim = 3;
  saveConfigData();

  morseZacni( text, speed );

  request->redirect("/#3");
}

void webserverBegin() 
{
  server.on("/", HTTP_GET, onRequestStatus );
  server.on("/rezim0", HTTP_GET, onRequestRezim0 );
  server.on("/rezim1", HTTP_GET, onRequestRezim1 );
  server.on("/rezim2", HTTP_GET, onRequestRezim2 );
  server.on("/rezim3", HTTP_GET, onRequestRezim3 );

  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER);//only when requested from AP

  server.begin();
}

