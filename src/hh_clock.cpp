#include "hh_clock.h"
#include "Arduino.h" //debug


// IMPORTANT! READ ME FIRST:
//
// Some lines contain hints for todos later on. You can search for these lines
// by pressing Ctrl and F on your keyboard at the same time, then typing in what
// to search for and clicking `Find`. (If you use macOS, press Cmd(⌘) and F at 
//  the same time instead.) All hints are written with two slashes and a space
// in front, like `// HINTX`. There are also CLUES for some todos at the very
// bottom of the file.
//
// All todos (except making your hedgehog dance) can be done in one line each, 
// and you do not need to remove anything. Every TODO needs to be completed 
// for this project to work fully. If a TODO mentions something in between
// backticks that you need to copy, eg `*copythis`, don't copy the backticks
// themselves!
//
// Some todos also require you to look in other files.


const unsigned long secsPerDisplayUpdate = 10;
const unsigned long secsPerNtcUpdate = 10 * 60;

void setup() {

  // TODO: start motors. This line is the same as one in Challenge 2. [1] [see CLUE1]
  //  (Challenge 2 can be found at http://hedgehogbot.digimakers.co.uk/challenges/2-bot).

  display.begin(SSD1306_SWITCHCAPVCC); // Start talking to our display ( hello, display :D ).
  display.setTextColor(WHITE);
  display.setTextWrap(false);

  wifi = new HHWifi("ssid", "password"); // TODO: Fill these with the name and passphrase of the
                                         //  network your hedgehog will connect to. [2]
  ntp = new HHNtp("pool.ntp.org");

  data = HHROM::get(); // We don't need to put a type in front of data, since we've already
                       //  defined it in `hh_clock.h`. The `#include` at the top of this
                       //  file tells the compiler to have a look there first for stuff we 
                       //  need. 
                       // `data` is a pointer: it doesn't actually contain any information,
                       //  but contains an address we can find it at (just like you can use
                       //  your address to find your house). If you have a look in
                       //  `hh_clock.h`, you'll see that (on line 26) data has a star in
                       //  front of it: this is how we mark pointers in C.
  bool restored = (data != NULL); // this is a really short way to say `restored` should tell us if
                                  //  data points somewhere. NULL is a special address which points
                                  //  nowhere, and != means "is not"...

  if (!restored) { // ...so, this means 'if `data` does not point anywhere'
    data = new HHROM::HHSchema(); // as we've not loaded any data, we need to create
                                          //  somewhere to save information about our alarm clock, 
                                          //  like the time zone and when the alarm should ring.
                                          // `HHROM` is a `namespace` - think of it like a
                                          //  toybox to keep similar things together. We get things
                                          //  out of the toybox with two colons.
                                          // `HHSchema` is a struct, which is like a form (such as
                                          //  the forms you fill out to be allowed to go on school
                                          //  trips) and keeps similar information together in a 
                                          //  structured way.

    data->verify = 'X';
    data->version = 'X'; // TODO: replace the Xs with the right letters, by finding them in
                         //  `hh_rom.cpp`!
                         // We made the decision to start the HHSchema with two characters to make
                         //  sure the information will make sense.
                         // As `data` is a pointer, we access the fields (like spaces on a form) by
                         //  pointing to them with an arrow. [3]

    if (wifi->visualConnect(display) != WL_CONNECTED) {
      while (1) { delay(1); } // not connected; do nothing
    }

    data->epoch = ntp->visualConnect(display);
    if (!ntp->isOnline()) {
      while (1) { delay(1); } // not connected; do nothing
    }

    // set a default alarm and timezone
    data->alarm = 8 * 60; // 8am
    data->timeZone = 1; // UTC+1 British Summer Time

  }

  HHClockFace::Face faces[] = {HHClockFace::Face::FACE_ALARM_INFO, HHClockFace::Face::FACE_IP};
    // `faces` is an array (a list) of Faces. `Face` is an enum, which maps names to numbers - 
    //  just like you might have a student number for exams. This is much more efficient for the
    // computer than storing the  names of the faces as text.
    // We get members of an `enum` with two colons, just like a namespace: it's like a smaller
    //  toybox.
  face = new HHClockFace(faces, sizeof(faces) / sizeof(faces[0]), *data, wifi->localIP());
    // HHClockFace isn't a namespace, but a class. Namespaces are accessed directly by their name,
    //  but instances of classes need to be created (with the word `new`), and we assign them a
    //  name individually to access them with. The only exceptions are types (`enums`, `structs`.
    //  `classes` and `namespaces`) within them which can be accessed directly, like above.
    // Namespaces are like toyboxes to tidy similar things away together - but classes are like
    //  types, or blueprints, for things.

  if (restored) {
    face->show(display, HHClockFace::Face::FACE_WIFICONNECT, data->epoch, data->timeZone); // HINT1
    
    if (wifi->connect() != WL_CONNECTED) { // try to reconnect to network
      wifi->visualFail(display);
      data->verify = 'G';
      HHROM::write(*data);
      while (1) { delay(1); }
    }

    face->ip = wifi->localIP();
    // TODO: show FACE_NTPCONNECT (the same way we showed FACE_WIFICONNECT in HINT1) [4] [CLUE2]

    unsigned long tempEpoch = ntp->connect(); // HINT2
    if (ntp->isOnline()) {
      data->epoch = tempEpoch;
      if (Time::minuteOfDay(data->epoch, data->timeZone) == data->alarm) { // if the alarm should be going off...
        alarmRang = true; // HINT3                                         // ...prevent it going off for now!
      }
    } else {
      // TODO: show FACE_NTPOFFLINE (the same way we showed FACE_WIFICONNECT in HINT1) [5]
      delay(secsPerDisplayUpdate * 1000);
    }
  }

  HHServer::beginServer(data);
  
  os_timer_setfn(&clock_, onTick, NULL); // This tells the computer to run `onTick()` every
                                        //  time its internal clock ticks...
  os_timer_arm(&clock_, 1000, true);     // ...and this starts the internal clock, and makes it
                                        //  tick every 1000 milliseconds.

  // goto loop
}

unsigned int _displayi = 0;
void loop() {
  if (_displayi == 0) {
    _displayi = secsPerDisplayUpdate;

    if (Time::minuteOfDay(data->epoch, data->timeZone) == data->alarm && !alarmRang) {
      // TODO: set alarmRang to true [6] [CLUE3]
      

      unsigned long ringTime = data->epoch;

      // TODO: dance pony dance! Just like you did in Challenge 2, give your hedgehog some
      //  dancemoves to show off when the alarm goes off! You'll need to be really clever
      //  here - moves before the while statement will happen once, and moves inside the curly
      //  brackets (called braces) will loop forever!
      // Remember, you need to use Motor.stop() if you want your hedgehog to stop moving! [7]
      // DO NOT DELETE THE `while (1)` - pressing the RESET button on the board should stop the 
      //  alarm ringing.
      Motor.right();

      while (1) {
        face->alarmFace(display, data->epoch); // strobe display
        
        if (data->epoch - ringTime == 2) Motor.stop();
          // ⬆⬆⬆ If you'd like to do something after some time, copy this line and replace `2`
          //  with the number of seconds you'd like to wait after the alarm goes off, and
          //  `Motor.stop();` with what you'd like to do. Using lots of `delay`s messes up the
          //   updating of the display!
        
        delay(200); // WARNING: if you suffer from photosensitive epilepsy, DO NOT reduce the
                    //  number in this `delay` - and if you feel unwell when the alarm goes off,
                    //  increase it to at least 330.
      }
      // END TODO: dance pony dance!
    } else if (ntp->getLastTime() + secsPerNtcUpdate < data->epoch) {
      // TODO: show FACE_NTPCONNECT (the same way we showed FACE_WIFICONNECT in HINT1) [8]
      unsigned long tempEpoch = 0; // TODO: replace 0 with something NTP related...
                                   // ...maybe see line HINT2? [9] [CLUE4]
      if (0) { // TODO: replace 0 with something that tells us whether NTP is online...
               // ..maybe see the line after HINT2? [10] [CLUE5]
        // TODO: copy our temporary time to our data. Maybe see two lines after HINT2? [11] [CLUE6]
        // TODO: Write our data to storage, using a function inside `HHROM`. You'll need to
        //  put `*data` in the brackets, to send the information `data` points to. [12] [CLUE7]
      } else {
        face->show(display, HHClockFace::Face::FACE_NTPOFFLINE, data->epoch, data->timeZone);
        delay(secsPerDisplayUpdate * 1000);
      }
    } else {
      face->next(display, data->epoch, data->timeZone);

      if (Time::minuteOfDay(data->epoch, data->timeZone) > data->alarm) {
        bool a = false; // TODO: reset the alarm by changing `bool a` to something else...
                        // ...that maybe on line HINT3? [13] [CLUE8]
      }
    }
  }

  // TODO: handle a client to our server. You'll need to look in `hh_server.h` for the right
  //  function, which takes no arguments (has empty brackets after the name), and is in the 
  //  `HHServer` namespace. [14]
}

void inline onTick(void *pArg) {
  ++data->epoch; // add a second to the time
  --_displayi;   // count down to the next display update
}



















// Each clue is the solution to a todo - all you need to do is fill the gaps!
// Remember, don't copy the backticks - just what's inside them.

// CLUE1: `_____._____(D8, D7, D6, D5, D4, D3, D0, RX);`
// CLUE2: `____->____(_______, HHClockFace::____::_______________, data->epoch, data->timeZone);`
// CLUE3: `_________ = ____;`
// CLUE4: `___->_______()`
// CLUE5: `___->________()`
// CLUE6: `____->_____ = _________;`
// CLUE7: `HHROM::_____(*____);`
// CLUE8: `_________`
