// #include "Arduino.h" //debug
#include "hh_server.h"

ESP8266WebServer _server(80);

const char* indexPage PROGMEM = "<html lang=en>"
                                "<meta content='initial-scale=1,width=device-width'name=viewport>"
                                "<title>Hedgehog Clock</title>"
                                "<style>*{margin:0;padding:0}.page{font-family:-apple-system-font,Helvetica Neue,Helvetica,sans-serif;line-height:1.6;display:block;position:absolute;top:0;right:0;bottom:0;left:0;background:#f8f8f8}.weui-cells__title{margin-top:.77em;margin-bottom:.3em;padding-left:15px;padding-right:15px;color:#999;font-size:14px}.weui-cells{margin-top:1.17647059em;background-color:#fff;line-height:1.4705824;font-size:17px;overflow:hidden;position:relative}.weui-cells:before{content:' ';position:absolute;left:0;top:0;right:0;height:1px;border-top:1px solid #e5e5e5;transform-origin:0 0;-webkit-transform-origin:0 0;transform:scaleY(.5);-webkit-transform:scaleY(.5);z-index:2}.weui-cells__title+.weui-cells{margin-top:0}.weui-cell{padding:10px 15px;position:relative;display:-webkit-box;display:-webkit-flex;display:flex;-webkit-box-align:center;-webkit-align-items:center;align-items:center}.weui-label{display:block;width:105px;word-wrap:break-word;word-break:break-all}.weui-cell__bd{-webkit-box-flex:1;-webkit-flex:1;flex:1}.weui-cells_form input{-webkit-tap-highlight-color:transparent}.weui-input{width:100%;border:0;outline:0;-webkit-appearance:none;background-color:transparent;font-size:inherit;color:inherit;height:1.47058824em;line-height:1.47058824}.weui-btn-area{margin:1.17647059em 15px .3em}.weui-btn{position:relative;display:block;margin-left:auto;margin-right:auto;padding-left:14px;padding-right:14px;box-sizing:border-box;font-size:18px;text-align:center;text-decoration:none;color:#fff;line-height:2.55555556;border-radius:5px;-webkit-tap-highlight-color:transparent;overflow:hidden}.weui-btn_primary{background-color:#1aad19}.weui-btn_primary:not(.weui-btn_disabled):active{color:rgba(255,255,255,0,6);background-color:#179b16}.weui-mask_transparent{position:fixed;z-index:1000;top:0;right:0;left:0;bottom:0}.weui-toast{position:fixed;z-index:5000;width:7.6em;min-height:7.6em;top:180px;left:50%;margin-left:-3.8em;background:rgba(17,17,17,.7);text-align:center;border-radius:5px;color:#fff}.weui-loading,.weui-success{width:20px;height:20px;display:inline-block;vertical-align:middle}.weui-loading{background:transparent url(data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSIxMjAiIGhlaWdodD0iMTIwIiB2aWV3Qm94PSIwIDAgMTAwIDEwMCI+PHBhdGggZmlsbD0ibm9uZSIgZD0iTTAgMGgxMDB2MTAwSDB6Ii8+PHJlY3Qgd2lkdGg9IjciIGhlaWdodD0iMjAiIHg9IjQ2LjUiIHk9IjQwIiBmaWxsPSIjRTlFOUU5IiByeD0iNSIgcnk9IjUiIHRyYW5zZm9ybT0idHJhbnNsYXRlKDAgLTMwKSIvPjxyZWN0IHdpZHRoPSI3IiBoZWlnaHQ9IjIwIiB4PSI0Ni41IiB5PSI0MCIgZmlsbD0iIzk4OTY5NyIgcng9IjUiIHJ5PSI1IiB0cmFuc2Zvcm09InJvdGF0ZSgzMCAxMDUuOTggNjUpIi8+PHJlY3Qgd2lkdGg9IjciIGhlaWdodD0iMjAiIHg9IjQ2LjUiIHk9IjQwIiBmaWxsPSIjOUI5OTlBIiByeD0iNSIgcnk9IjUiIHRyYW5zZm9ybT0icm90YXRlKDYwIDc1Ljk4IDY1KSIvPjxyZWN0IHdpZHRoPSI3IiBoZWlnaHQ9IjIwIiB4PSI0Ni41IiB5PSI0MCIgZmlsbD0iI0EzQTFBMiIgcng9IjUiIHJ5PSI1IiB0cmFuc2Zvcm09InJvdGF0ZSg5MCA2NSA2NSkiLz48cmVjdCB3aWR0aD0iNyIgaGVpZ2h0PSIyMCIgeD0iNDYuNSIgeT0iNDAiIGZpbGw9IiNBQkE5QUEiIHJ4PSI1IiByeT0iNSIgdHJhbnNmb3JtPSJyb3RhdGUoMTIwIDU4LjY2IDY1KSIvPjxyZWN0IHdpZHRoPSI3IiBoZWlnaHQ9IjIwIiB4PSI0Ni41IiB5PSI0MCIgZmlsbD0iI0IyQjJCMiIgcng9IjUiIHJ5PSI1IiB0cmFuc2Zvcm09InJvdGF0ZSgxNTAgNTQuMDIgNjUpIi8+PHJlY3Qgd2lkdGg9IjciIGhlaWdodD0iMjAiIHg9IjQ2LjUiIHk9IjQwIiBmaWxsPSIjQkFCOEI5IiByeD0iNSIgcnk9IjUiIHRyYW5zZm9ybT0icm90YXRlKDE4MCA1MCA2NSkiLz48cmVjdCB3aWR0aD0iNyIgaGVpZ2h0PSIyMCIgeD0iNDYuNSIgeT0iNDAiIGZpbGw9IiNDMkMwQzEiIHJ4PSI1IiByeT0iNSIgdHJhbnNmb3JtPSJyb3RhdGUoLTE1MCA0NS45OCA2NSkiLz48cmVjdCB3aWR0aD0iNyIgaGVpZ2h0PSIyMCIgeD0iNDYuNSIgeT0iNDAiIGZpbGw9IiNDQkNCQ0IiIHJ4PSI1IiByeT0iNSIgdHJhbnNmb3JtPSJyb3RhdGUoLTEyMCA0MS4zNCA2NSkiLz48cmVjdCB3aWR0aD0iNyIgaGVpZ2h0PSIyMCIgeD0iNDYuNSIgeT0iNDAiIGZpbGw9IiNEMkQyRDIiIHJ4PSI1IiByeT0iNSIgdHJhbnNmb3JtPSJyb3RhdGUoLTkwIDM1IDY1KSIvPjxyZWN0IHdpZHRoPSI3IiBoZWlnaHQ9IjIwIiB4PSI0Ni41IiB5PSI0MCIgZmlsbD0iI0RBREFEQSIgcng9IjUiIHJ5PSI1IiB0cmFuc2Zvcm09InJvdGF0ZSgtNjAgMjQuMDIgNjUpIi8+PHJlY3Qgd2lkdGg9IjciIGhlaWdodD0iMjAiIHg9IjQ2LjUiIHk9IjQwIiBmaWxsPSIjRTJFMkUyIiByeD0iNSIgcnk9IjUiIHRyYW5zZm9ybT0icm90YXRlKC0zMCAtNS45OCA2NSkiLz48L3N2Zz4=) no-repeat;background-size:100%;-webkit-animation:weuiLoading 1s steps(12,end) infinite;animation:weuiLoading 1s steps(12,end) infinite}.weui-success{background:transparent url(data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0idXRmLTgiPz48IURPQ1RZUEUgc3ZnIFBVQkxJQyAiLS8vVzNDLy9EVEQgU1ZHIDEuMS8vRU4iICJodHRwOi8vd3d3LnczLm9yZy9HcmFwaGljcy9TVkcvMS4xL0RURC9zdmcxMS5kdGQiPjxzdmcgdmVyc2lvbj0iMS4xIiBpZD0iTGF5ZXJfMSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB4bWxuczp4bGluaz0iaHR0cDovL3d3dy53My5vcmcvMTk5OS94bGluayIgeD0iMHB4IiB5PSIwcHgiIHdpZHRoPSI1MTJweCIgaGVpZ2h0PSI1MTJweCIgdmlld0JveD0iMCAwIDUxMiA1MTIiIHN0eWxlPSJlbmFibGUtYmFja2dyb3VuZDpuZXcgMCAwIDUxMiA1MTI7IiB4bWw6c3BhY2U9InByZXNlcnZlIj48Zz48cGF0aCBmaWxsPSIjZmZmIiBkPSJNMjU2LDQ4QzE0MS4xLDQ4LDQ4LDE0MS4xLDQ4LDI1NnM5My4xLDIwOCwyMDgsMjA4YzExNC45LDAsMjA4LTkzLjEsMjA4LTIwOFMzNzAuOSw0OCwyNTYsNDh6IE0yMjMuOSwzMjkuN2MtMi40LDIuNC01LjgsNC40LTguOCw0LjRzLTYuNC0yLjEtOC45LTQuNWwtNTYtNTZsMTcuOC0xNy44bDQ3LjIsNDcuMmwxMjQuOC0xMjUuN2wxNy41LDE4LjFMMjIzLjksMzI5Ljd6Ii8+PC9nPjwvc3ZnPg==) no-repeat;background-size:100%}.weui-icon_toast.weui-loading,.weui-icon_toast.weui-success{margin:30px 0 0;width:38px;height:38px;vertical-align:baseline}.weui-toast__content{margin:0 0 15px}#toast,.weui-success{display:none}@-webkit-keyframes weuiLoading{0%{transform:rotate3d(0,0,1,0deg)}100%{transform:rotate3d(0,0,1,360deg)}}@keyframes weuiLoading{0%{transform:rotate3d(0,0,1,0deg)}100%{transform:rotate3d(0,0,1,360deg)}}</style>"
                                "<body class=page>"
                                "<div class=weui-cells__title>Hedgehog Clock Settings</div>"
                                "<div class='weui-cells weui-cells__form'>"
                                  "<div class=weui-cell>"
                                    "<div class=weui-cell__hd>"
                                      "<label class=weui-label>Alarm Time</label>"
                                    "</div>"
                                    "<div class=weui-cell__bd>"
                                      "<input class=weui-input id=time type=time value=08:00></div>"
                                    "</div>"
                                  "</div>"
                                  "<div class=weui-cell>"
                                    "<div class=weui-cell__hd>"
                                      "<label class=weui-label>Time Zone</label>"
                                    "</div>"
                                    "<div class=weui-cell__bd>"
                                      "<input class=weui-input id=tz type=number min=-12 max=12 step='.5' value=1></div>"
                                    "</div>"
                                  "</div>"
                                  "<div class=weui-btn-area>"
                                    "<a class='weui-btn weui-btn_primary'href=javascript: id=submit>Save</a>"
                                  "</div>"
                                  "<div id=toast>"
                                    "<div class=weui-mask_transparent></div>"
                                    "<div class=weui-toast>"
                                      "<i class='weui-icon_toast weui-loading'id=loading></i>"
                                      "<i class='weui-icon_toast weui-success'id=success></i>"
                                      "<p class=weui-toast__content id=message>Saving"
                                    "</div>"
                                  "</div>"
                                  "<script>var req=new XMLHttpRequest;req.onreadystatechange=function(){req.readyState===XMLHttpRequest.DONE&&(200===req.status?(document.getElementById('loading').display='none',document.getElementById('success').display='inline-block',window.setTimeout(function(){document.getElementById('toast').style.display='none',document.getElementById('loading').display='inline-block',document.getElementById('success').display='none'},500)):document.getElementById('message').innerHTML='Error')},document.getElementById('submit').onclick=function(){document.getElementById('toast').style.display='block',req.open('POST','/update',!0),req.setRequestHeader('Content-Type','application/x-www-form-urlencoded'),"
                                  "req.send('alarm='+document.getElementById('time').value+'&tz='+document.getElementById('tz').value)"
                                  "}</script>";

void _handleUpdate(HHPersistence::HHSchema *data) {
//   Serial.println("REQUEST /update WITH"); //debug
//   Serial.println(_server.arg("alarm")); //debug
  char *_alarm = strdup(_server.arg("alarm").c_str());
  char *_tz = strdup(_server.arg("tz").c_str());
  if (!_server.hasArg("alarm") || !_server.hasArg("tz") || strlen(_alarm) != 5) {
//     Serial.println("BAD FORMAT"); //debug
    _server.sendHeader("Connection", "close");
    _server.send(400, "text/plain", "BAD_ARG");
    return;
  }
  const char *_hour = strtok(_alarm, ":");
  if (_hour == NULL || atoi(_hour) < 0 || atoi(_hour) > 23) {
//     Serial.println("BAD HOUR"); //debug
    _server.sendHeader("Connection", "close");
    _server.send(400, "text/plain", "BAD_ARG");
    return;
  }
  const char* _min = strtok(NULL, ":");
  if (_min == NULL || atoi(_min) < 0 || atoi(_min) > 59) {
//     Serial.println("BAD MINUTE"); //debug
    _server.sendHeader("Connection", "close");
    _server.send(400, "text/plain", "BAD_ARG");
    return;
  }

  int tz = atoi(_tz);
  if (tz > 12 || tz < -12) {
//     Serial.println("BAD TIMEZONE"); //debug
    _server.sendHeader("Connection", "close");
    _server.send(400, "text/plain", "BAD_ARG");
    return;
  }
//   Serial.println("SET ALARM TO"); //debug
//   Serial.println((atoi(_hour) * 60) + atoi(_min)); //debug
  data->alarm = (atoi(_hour) * 60) + atoi(_min);
  data->timeZone = tz;
  bool ok = HHPersistence::write(*data);
  
  _server.sendHeader("Connection", "close");
  _server.send(200, "text/plain", "OK");
  
//   // Serial.println(ok); //debug
//   // Serial.println(data->alarm); //debug
}

void HHServer::beginServer(HHPersistence::HHSchema *data) {
  _server.on("/", []() {
//     Serial.println("REQUEST /"); //debug
    _server.sendHeader("Connection", "close");
    _server.send(200, "text/html", indexPage);
  });
  _server.on("/update", std::bind(&_handleUpdate, data));
  _server.begin();
}

void HHServer::handleClient() {
  _server.handleClient();
}
