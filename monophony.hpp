#ifndef TINYFM_MONOPHONY_HPP
#define TINYFM_MONOPHONY_HPP

#include "config.hpp"
#include "clock.hpp"
#include "note.hpp"

template< typename Module >
class Monophony {
public:
  Monophony() : tick( 1.0f/freq ) {}
  inline SampleType operator()() {
    if( getStatus() == Note::NOTE_ON ) {
      SampleType result = module( clock, note );
      clock += tick;
      return result;
    }
    else if( getStatus() == Note::NOTE_OFF ) {
      note.resetVote();
      SampleType result = module( clock, note );
      clock += tick;
      if( note.isEnd() )
        noteWait();
      return result;
    }
    else {
      clock += tick;
      return 0;
    }
  }
  inline int noteOn( int _note_number, SampleType _pressure ) {
    clock.noteOn();
    note.noteOn( _note_number, _pressure );
    return 0;
  }
  inline void noteOff( int _dummy = 0 ) {
    clock.noteOff();
    note.noteOff();
  }
  inline void noteWait( int _dummy = 0 ) {
    note.noteWait();
  }
  inline void pitchBend( SampleType _pitch ) {
    note.pitchBend( _pitch );
  }
  inline void reset() {
    clock.reset();
  }
  inline Note::KeyState getStatus() const {
    return note.getStatus();
  }
  inline TimeType getGlobalTime() const {
    return clock.getGlobal();
  }
private:
  Clock clock;
  Note note;
  Module module;
  const TimeType tick;
};

#endif
