#include "hh_clock.h"
#include "Arduino.h" //debug


// IMPORTANT! READ ME FIRST:
//
// Some lines contain hints for todos later on. You can search for these lines
// by pressing Ctrl and F on your keyboard at the same time, then typing in what
// to search for and clicking `Find`. All hints are written with two slashes and
// a space in front, like `// HINTX`.
// If you use macOS, press Command(⌘) and F at the same time instead.
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
  // TODO: start motors. This line is the same as one in Challenge 2. [1]

  display.begin(SSD1306_SWITCHCAPVCC); // Start talking to our display ( hello, display :D ).
  display.setTextColor(WHITE);
  display.setTextWrap(false);

  wifi = new HHWifi("ssid", "password"); // TODO: Fill these with the SSID and passphrase of the 
                                         //  network your hedgehog will connect to. [2]
  ntp = new HHNtp("pool.ntp.org");

  Serial.begin(9600); //debug

  data = HHPersistence::get(); // We don't need to put a type in front of data, since we've already
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
    data = new HHPersistence::HHSchema(); // as we've not loaded any data, we need to create
                                          //  somewhere to save information about our alarm clock, 
                                          //  like the time zone and when the alarm should ring.
                                          // `HHPersistence` is a `namespace` - think of it like a
                                          //  toybox to keep similar things together. We get things
                                          //  out of the toybox with two colons.
                                          // `HHSchema` is a struct, which is like a form (such as
                                          //  the forms you fill out to be allowed to go on school
                                          //  trips) and keeps similar information together in a 
                                          //  structured way.
                                          // You can have a look at `HHPersistence` in
                                          //  `../lib/hh_persistence`.

    data->verify = 'X';
    data->version = 'X'; // TODO: replace the Xs with the right letters, by finding them in
                         //  `../lib/hh_persistence/hh_persistence.cpp`!
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
    // `faces` is an array (a list) of Faces. `Face` is an enum, which gives names to numbers
    //  so we know what they mean. This is much more efficient for the computer than just storing
    //  the faces as text, as it needs to store one number instead of a number for each letter.
    // We get members of an `enum` with two colons, just like a namespace: it's like a smaller
    //  toybox.
  face = new HHClockFace(faces, sizeof(faces) / sizeof(faces[0]), *data, wifi->localIP());
    // HHClockFace isn't a namespace, but a class. Namespaces are accessed directly by their name,
    //  but instances of classes need to be created (with the word `new`), and we assign them a
    //  name individually to access them with. The only exceptions are types (`enums`, `structs`.
    //  `classes` and `namespaces`) within them which can be accessed directly, like above.

  if (restored) {
    face->show(display, HHClockFace::Face::FACE_WIFICONNECT, data->epoch, data->timeZone); // HINT1
    
    if (wifi->connect() != WL_CONNECTED) { // try to reconnect to network
      wifi->visualFail(display);
      data->verify = 'G';
      HHPersistence::write(*data);
      while (1) { delay(1); }
    }

    face->ip = wifi->localIP();
    // TODO: show FACE_NTPCONNECT (the same way we showed FACE_WIFICONNECT in HINT1) [4]

    unsigned long tempEpoch = ntp->connect(); // HINT2
    if (ntp->isOnline()) {
      data->epoch = tempEpoch;
      if ((data->epoch % 86400) / 60 == data->alarm) { // if the alarm should be going off...
        alarmRang = true; // HINT4                     // ...prevent it going off for now!
      }
    } else {
      // TODO: show FACE_NTPOFFLINE (the same way we showed FACE_WIFICONNECT in HINT1) [5]
      delay(secsPerDisplayUpdate * 1000); // HINT3
    }
  }

  HHServer::beginServer(data);
  
  os_timer_setfn(&clock, onTick, NULL); // This tells the computer to run `onTick()` every
                                        //  time its internal clock ticks...
  os_timer_arm(&clock, 1000, true);     // ...and this starts the internal clock, and makes it
                                        //  tick every 1000 milliseconds.

  // goto loop
}

unsigned int _displayi = 0;
void loop() {
  if (_displayi == 0) {
    _displayi = secsPerDisplayUpdate;

    if ((data->epoch % 86400) / 60 == data->alarm && !alarmRang) {
      // TODO: set alarmRang to true [6]
      
      // TODO: dance pony dance! Just like you did in Challenge 2, give your hedgehog some
      //  dancemoves to show off when the alarm goes off! You'll need to be really clever
      //  here - moves before the while statement will happen once, and moves inside the curly
      //  brackets (called braces) will loop forever - but be careful with the delays!
      // Remember, you need to use Motor.stop() if you want your hedgehog to stop moving! [6]
      Motor.right();
      while (1) {
        face->alarmFace(display, data->epoch); // strobe display
        delay(200);
      }
      // END TODO: dance pony dance!
    } else if (ntp->getLastTime() + secsPerNtcUpdate < data->epoch) {
      // TODO: show FACE_NTPCONNECT (the same way we showed FACE_WIFICONNECT in HINT1) [7]
      unsigned long tempEpoch = 0; // TODO: replace 0 with something NTP related...
                                   // ...maybe see line HINT2?
      if (0) { // TODO: replace 0 with something that tells us whether NTP is online...
               // ..maybe see the line after HINT2? [8]
        // TODO: copy our temporary time to our data. Maybe see two lines after HINT2? [9]
        // TODO: Write our data to storage, using a function inside `HHPersistence`. You'll need to
        //  put `*data` in the brackets, to send the information `data` points to. [10]
      } else {
        face->show(display, HHClockFace::Face::FACE_NTPOFFLINE, data->epoch, data->timeZone);
        // TODO: wait for the next display update using `delay`. Maybe see HINT3? [11]
      }
    } else {
      face->next(display, data->epoch, data->timeZone);

      if ((data->epoch % 86400) / 60 > data->alarm) {
        bool a = false; // TODO: reset the alarm by changing `bool a` to something else...
                        // ...that maybe on line HINT4? [12]
      }
    }
  }

  // TODO: handle a client to our server. You'll need to look in `../src/hh_server.h` for the right
  //  function, which takes no arguments (has empty brackets after the name), and is in the 
  //  `HHServer` namespace. [13]

}

void inline onTick(void *pArg) {
  ++data->epoch; // add a second to the time
  --_displayi;   // count down to the next display update
}
