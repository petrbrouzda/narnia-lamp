
const char hlavicka[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Narnia lampa</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: left;}
    h2 {font-size: 2.0rem;}
    p {font-size: 1.5rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
  </style>
</head>
<body>
)rawliteral";

const char paticka[] PROGMEM = R"rawliteral(
</body>
</html>
)rawliteral";


const char* CONTENT_TYPE = "text/html; charset=utf-8";

//TODO: male pismo na tlacitkach

void onRequestStatus(AsyncWebServerRequest *request){
  logger->log( "* status");

  //Handle Unknown Request
  AsyncResponseStream *response = request->beginResponseStream(CONTENT_TYPE);
  response->print( hlavicka );
  response->print("<h2>Narnia lampa</h2>");
  response->printf( "<p>Rezim: %d</p>", aktualniRezim );

  response->print("<p>Test</p>" );

  response->print("<form method=\"GET\" action=\"/rezim1\">");
  response->print("<input type=\"submit\" name=\"nastav\" value=\"režim 1\" >" );
  response->print("</form>" );

  response->print("<p>Test</p>" );

  response->print("<form method=\"GET\" action=\"/rezim2\">");
  response->print("<input type=\"submit\" name=\"nastav\" value=\"režim 2\" >" );
  response->print("</form>" );

  response->print("<p>Test</p>" );

  response->printf( "<p>Lokalni cas (sec od bootu): %d</p>", time(NULL) );

  response->print( paticka );
  request->send(response);
}

void onRequestRezim1(AsyncWebServerRequest *request){
  logger->log( "* rezim 1");

  aktualniRezim = 1;
  saveConfigData();

  request->redirect("/");
}

void onRequestRezim2(AsyncWebServerRequest *request){
  logger->log( "* rezim 2");

  aktualniRezim = 2;
  saveConfigData();

  request->redirect("/");
}

void webserverBegin() 
{
  server.begin();

  server.on("/", HTTP_GET, onRequestStatus );
  server.on("/rezim1", HTTP_GET, onRequestRezim1 );
  server.on("/rezim2", HTTP_GET, onRequestRezim2 );
}

/*
  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/updateVal", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    // GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>

    if(request->hasParam("V5") ) {
      inputMessage1 = request->getParam("V5")->value();
      if( strcmp(inputMessage1.c_str(),"TotalHeslo" )==0 ) {

        if(request->hasParam("V1") ) {
          inputMessage1 = request->getParam("V1")->value();
          limitBaterka = atof(inputMessage1.c_str() );
        }

        if(request->hasParam("V6") ) {
          inputMessage1 = request->getParam("V6")->value();
          varovaniBaterka = atof(inputMessage1.c_str() );
        }
    
        if(request->hasParam("V2") ) {
          inputMessage1 = request->getParam("V2")->value();
          delkaSpusteni = atol(inputMessage1.c_str() );
          if( delkaSpusteni>900 ) delkaSpusteni = 900;
        }
        if(request->hasParam("V3") ) {
          inputMessage1 = request->getParam("V3")->value();
          delkaPauzy = atol(inputMessage1.c_str() );
        }
        if(request->hasParam("V4") ) {
          inputMessage1 = request->getParam("V4")->value();
          pocetSpusteni = atol(inputMessage1.c_str() );
        }

        saveConfigData();
        loadConfigData();

        request->redirect("/nastav");
      } else { 
        request->redirect("/");
      }
    } else {
      request->redirect("/");
    } 
  });
  */