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
#include "piano.hpp"
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
    io_poll.attach( &IO::poll, 1.0f/10.0f );
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
    gpos += 2.0f/5.0f;
    led1 = static_cast<float>( ( sint( gpos ) + 1 ) / 2 );
    led2 = static_cast<float>( ( sint( gpos + gpos_diff ) + 1 ) / 2 );
    led3 = static_cast<float>( ( sint( gpos + gpos_diff * 2 ) + 1 ) / 2 );
    led4 = static_cast<float>( ( sint( gpos + gpos_diff * 3 ) + 1 ) / 2 );
  }
  Ticker io_poll;
};

struct EV_01 {
  static const float value = 3.0f;
};

struct EV_02 {
  static const float value = 0.997f;
};

struct EV_03 {
  static const float value = 1.002f;
};

struct EV_04 {
  static const float value = 0.51f;
};

struct EV_05 {
  static const float value = 0.51f;
};

struct EV_06 {
  static const float value = 9.0f;
};

struct EV_07 {
  static const float value = 0.275f;
};

struct EV_08 {
  static const float value = 0.2475f;
};

struct HT {
  static const float level_16 = 0.5f;
  static const float level_8 = 1.0f;
  static const float level_513 = 0.5f;
  static const float level_4 = 0.3f;
  static const float level_223 = 1.0f;
  static const float level_2 = 0.5f;
  static const float level_135 = 0.8f;
  static const float level_113 = 0.1f;
  static const float level_1 = 0.2f;
};

int main() {
  AudioBuffer::getInstance();
  IO::getInstance();
while( 1 ) {
//   typedef FM< Time, Const00, Const10, Const00, Const01, Const03, Const10 > Mod1;
/*   typedef ConstFM< Time, Const00, Const10, C_00, C_01, C_03, C_03 > Mod1;
   typedef ConstFM< Time, Mod1, Const10, C_00, C_05, C_05, C_10 > Mod2;
   typedef Scale< Const< EV_04 >, Mod2 > Mod3;
   typedef Mul< Scale< Const05, Piano< Time > >, ConstEnvelope< C_00, C_05, C_00, C_17 > > Mod5;
   typedef Scale< Const< EV_04 >, Mod5 > Mod6;
   typedef Scale< NoteFrequency, Add< Mod2, Mod3 > > Mod7;
   typedef Scale< NoteFrequency, Fuzz< Add< Const04, Mod5 > > > Mod8;
   typedef Mul< Triangle< Time >, ConstEnvelope< C_00, C_07, C_03, C_06 > > Mod9;
   typedef Scale< NoteFrequency, Mod9 > Mod10;*/
   typedef Scale< NoteFrequency,
     Mul<
       Rectangle< C_02, Time >,
       ConstEnvelope< C_01, C_05, C_03, C_10 >
     >
   > Rect;
   typedef Scale< NoteFrequency, Add<
     ConstFM< Time,
       ConstFM< Time,
         Const00,
         C_10,
         C_00, C_17, C_00, C_20
       >,
       C_03,
       C_01, C_15, C_00, C_20
     >,
     ConstFM< Time,
       ConstFM< Scale< Const< EV_06 >, Time >,
         Const00,
         C_10,
         C_00, C_05, C_00, C_20
       >,
       C_02,
       C_00, C_07, C_00, C_20
     >
   > > EleP;
   typedef Scale< NoteFrequency, Fuzz<
     Mul<
       Scale< Const05, Piano< Time > >,
       ConstEnvelope< C_00, C_05, C_00, C_17 >
     >
   > > EleG;
   typedef Scale< NoteFrequency,
     Mul<
       Add< Const08, Mul< Const02, Sin< Scale< Const01, Time > > > >,
       Mul<
         Scale< Const05, Piano< Time > >,
         ConstEnvelope< C_00, C_05, C_00, C_17 >
       >
     >
   > Piano;
   typedef Scale< NoteFrequency,
     ConstFM< Time,
       Scale<
         Const< EV_03 >,
         ConstFM< Time,
           Scale<
             Const< EV_02 >,
             ConstFM< Time,
               Scale<
                 Const< EV_01 >,
                 ConstFM< Time, Const00, C_00, C_00, C_10, C_10, C_20 >
               >,
               EV_07, C_00, C_10, C_10, C_20
             >
           >,
           EV_08, C_00, C_10, C_10, C_20
         >
       >,
       EV_08, C_01, C_02, C_07, C_20
     >
   > Strings;
   typedef Scale< NoteFrequency,
     Mul<
       Hammond< Time, HT >,
       ConstEnvelope< C_00, C_10, C_10, C_20 >
     >
   > Organ;
   SimpleReader< Polyphony< Rect, 16 > > inst( score0 );
   SimpleReader< Polyphony< EleP, 3 > > inst1( score0 );
   SimpleReader< Polyphony< EleG, 6 > > inst2( score0 );
   SimpleReader< Polyphony< Piano, 6 > > inst3( score0 );
   SimpleReader< Polyphony< Strings, 3 > > inst4( score0 );
   SimpleReader< Polyphony< Organ, 3 > > inst5( score0 );
    while( !inst.isEnd() ) {
      inst.read();
      for( int count = 0; AudioBuffer::getInstance().readyToSet() && count != 256; ++count ) {
        AudioBuffer::getInstance().set( inst() );
      }
    }
    while( !inst5.isEnd() ) {
      inst5.read();
      for( int count = 0; AudioBuffer::getInstance().readyToSet() && count != 256; ++count ) {
        AudioBuffer::getInstance().set( inst5() );
      }
    }
    while( !inst1.isEnd() ) {
      inst1.read();
      for( int count = 0; AudioBuffer::getInstance().readyToSet() && count != 256; ++count ) {
        AudioBuffer::getInstance().set( inst1() );
      }
    }
    while( !inst4.isEnd() ) {
      inst4.read();
      for( int count = 0; AudioBuffer::getInstance().readyToSet() && count != 256; ++count ) {
        AudioBuffer::getInstance().set( inst4() );
      }
    }
    while( !inst3.isEnd() ) {
      inst3.read();
      for( int count = 0; AudioBuffer::getInstance().readyToSet() && count != 256; ++count ) {
        AudioBuffer::getInstance().set( inst3() );
      }
    }
    while( !inst2.isEnd() ) {
      inst2.read();
      for( int count = 0; AudioBuffer::getInstance().readyToSet() && count != 256; ++count ) {
        AudioBuffer::getInstance().set( inst2() );
      }
    }
  }
}
