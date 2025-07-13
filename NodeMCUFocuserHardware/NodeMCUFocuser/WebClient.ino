//------Web Pages----------------------------------

String Get404Page() {
  //returns html page for error 404
  String response = R"(<!DOCTYPE html><html>
  <head><meta name="viewport" content="width=device-width, initial-scale=1">
  <style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
  p {padding: 10px 20px; font-size: 30px;}
  </style></head>
  <body><h1>ASCOM NodeMCU WiFi Focuser</h1>
  <p>Page not found</p>  
  </body></html>)";
  return response;
}

String GetCientPage() {
  //returns html page for thin client
  String response = R"(<!DOCTYPE html><html>
  <head><meta name="viewport" content="width=device-width, initial-scale=1">
  <style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
  p {padding: 10px 20px; font-size: 30px;}
  input[type=text]{ font-size: 30px; margin: 2px;}
  input[type=submit]{ background-color: #195B6A; border: none; color: white; padding: 3px 10px;
  text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
  .button { background-color: #195B6A; border: none; color: white; padding: 5px 10px;
  text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}
  .button2 {background-color: #77878A;}</style></head>
  <body><h1>ASCOM NodeMCU WiFi Focuser</h1>)";
  response += "<p>Focuser position:" + String(currentPosition) + "</p>\r\n";
  response += "<p>Target position:" + String(targetPosition) + "</p>\r\n";
  response += "<p>Moving:" + String(isMoving) + "</p>\r\n";
  response += R"(<p>
  <a href="/?move=-250"><button class="button">&#8666;</button></a>
  <a href="/?move=-25"><button class="button">&#8656;</button></a>  
  <a href="/?stop=1"><button class="button">&#x220E;</button></a>  
  <a href="/?move=25"><button class="button">&#8658;</button></a>
  <a href="/?move=250"><button class="button">&#8667;</button></a>
  </p>
  <form><p><input type="text" id="move" value="100" name="move" size="5"><input style="button" type="submit" value="Move"></p></form>  
  <p><a href="/?stopBrake=1"><button class="button">Stop brake</button></a></p>
  <p>Cover: <a href="/?opencover=1"><button class="button">Open</button></a><a href="/?opencover=0"><button class="button">Close</button></a></p>
  <p>Light: <a href="/?setlight=1"><button class="button">On</button></a><a href="/?setlight=0"><button class="button">Off</button></a></p>
  </body></html>)";
  return response;
}
