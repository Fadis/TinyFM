#include "mbed.h"

#include "fixed_math.hpp"
#include "audio_buffer.hpp"
#include "normalizer.hpp"
#include "operators.hpp"
#include "clock.hpp"
#include "note.hpp"
#include "const.hpp"
#include "monophony.hpp"
#include "polyphony.hpp"
#include "simple.hpp"
#include "config.hpp"

PwmOut led1(LED1);
PwmOut led2(LED2);
PwmOut led3(LED3);
PwmOut led4(LED4);
DigitalIn up_button(p19);
DigitalIn down_button(p20);

int current_note_no = 52;
bool up_pressed = false;
bool down_pressed = false;
TimeType gpos = 0;
TimeType gpos_diff = 0.25f;

class IO {
public:
  static IO &getInstance() {
    static IO io;
    return io;
  }
private:
  IO() {
    io_poll.attach( &IO::poll, 1.0f/20.0f );
  }
  static void poll() {
    if( up_button )
      up_pressed = true;
    else if( up_pressed ) {
      up_pressed = false;
      ++current_note_no;
    }
    if( down_button )
      down_pressed = true;
    else if( down_pressed ) {
      down_pressed = false;
      --current_note_no;
    }
    gpos += 1.0f/20.0f;
    led1 = static_cast<float>( ( sint( gpos ) + 1 ) / 2 );
    led2 = static_cast<float>( ( sint( gpos + gpos_diff ) + 1 ) / 2 );
    led3 = static_cast<float>( ( sint( gpos + gpos_diff * 2 ) + 1 ) / 2 );
    led4 = static_cast<float>( ( sint( gpos + gpos_diff * 3 ) + 1 ) / 2 );
  }
  Ticker io_poll;
};


struct EV_04 {
  static const float value = 1.51f;
};

struct EV_05 {
  static const float value = 1.01f;
};

int main() {
  AudioBuffer::getInstance();
  IO::getInstance();
while( 1 ) {
   typedef  Scale< Const< EV_05 >, FM< Time, Const10, Const07, Const05, Const07, Const03, Const10 > > Mod1;
   typedef  FM< Time, Mod1, Const10, Const00, Const10, Const05, Const10 > Mod2;
   typedef Scale< Const< EV_04 >, Mod2 > Mod3;
    SimpleReader< Polyphony< Scale< NoteFrequency, Add< Mod2, Mod3 > >, 3 > > inst( score0 );
/*
    int note_id = inst.noteOn( current_note_no, 1.0f );
    for( ; inst.getGlobalTime() < 1.0; ) {
      while( !AudioBuffer::getInstance().readyToSet() );
      AudioBuffer::getInstance().set( inst() );
    }
    int note_id2 = inst.noteOn( current_note_no + 12, 1.0f );
    for( ; inst.getGlobalTime() < 2.0; ) {
      while( !AudioBuffer::getInstance().readyToSet() );
      AudioBuffer::getInstance().set( inst() );
    }
    int note_id3 = inst.noteOn( current_note_no + 24, 1.0f );
    inst.noteOff( note_id );
    for( ; inst.getGlobalTime() < 3.0; ) {
      while( !AudioBuffer::getInstance().readyToSet() );
      AudioBuffer::getInstance().set( inst() );
    }
    inst.noteOff( note_id2 );
    for( ; inst.getGlobalTime() < 4.0; ) {
      while( !AudioBuffer::getInstance().readyToSet() );
      AudioBuffer::getInstance().set( inst() );
    }
    inst.noteOff( note_id3 );
    for( ; inst.getGlobalTime() < 5.0; ) {
      while( !AudioBuffer::getInstance().readyToSet() );
      AudioBuffer::getInstance().set( inst() );
    }
  */
    while( !inst.isEnd() ) {
      inst.read();
      for( int count = 0; AudioBuffer::getInstance().readyToSet() && count != 256; ++count ) {
        AudioBuffer::getInstance().set( inst() );
      }
    }
  }
}
