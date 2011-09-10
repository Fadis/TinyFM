#ifndef TINYFM_POLYPHONY_HPP
#define TINYFM_POLYPHONY_HPP

#include "config.hpp"
#include "clock.hpp"
#include "note.hpp"
#include "monophony.hpp"

template< typename Module, unsigned int max_monophony >
class Polyphony {
public:
  Polyphony() : current_note_id( 0 ), default_pitch( 0 ) {
    for( int index = 0; index != max_monophony; ++index )
      active_since[ index ] = 0;
  }
  SampleType operator()() {
    SampleType result = 0;
    for( int index = 0; index != max_monophony; ++index ) {
      result += monophony[ index ]();
    }
    return result;
  }
  int noteOn( int _note_number, SampleType _pressure ) {
    int monophony_id = getNewMonophony();
    if( monophony_id == -1 )
      monophony_id = killOldMonophony();
    monophony[ monophony_id ].noteOn( _note_number, _pressure );
    ++current_note_id;
    active_since[ monophony_id ] = current_note_id;
    return current_note_id;
  }
  void noteOff( int _note_id ) {
    int monophony_id = findMonophony( _note_id );
    if( monophony_id != -1 )
      monophony[ monophony_id ].noteOff();
  }
  void pitchBend( int _note_id, SampleType _pitch ) {
    int monophony_id = findMonophony( _note_id );
    if( monophony_id != -1 )
      monophony[ monophony_id ].pitchBend( _pitch );
  }
  void pitchBend( SampleType _pitch ) {
    for( int index = 0; index != max_monophony; ++index )
      monophony[ int ].pitchBend( _pitch );
  }
  void reset() {
    current_note_id = 0;
    default_pitch = 0;
    for( int index = 0; index != max_monophony; ++index ) {
      monophony[ index ].reset();
      active_since[ index ] = 0;
    }
  }
  TimeType getGlobalTime() const {
    return monophony[ 0 ].getGlobalTime();
  }
private:
  int findMonophony( int _note_id ) const {
    for( int index = 0; index != max_monophony; ++index )
      if( active_since[ index ] == _note_id )
        return index;
    return -1;
  }
  int getNewMonophony() const {
    for( int index = 0; index != max_monophony; ++index )
      if( monophony[ index ].getStatus() == Note::NOTE_WAIT )
        return index;
    return -1;
  }
  int killOldMonophony() {
    int oldest_monophony = -1;
    unsigned int oldest_time = current_note_id;
    for( int index = 0; index != max_monophony; ++index ) {
      if( active_since[ index ] <= oldest_time ) {
        if( monophony[ index ].getStatus() == Note::NOTE_OFF ) {
          oldest_monophony = index;
          oldest_time = active_since[ index ];
        }
      }
    }
    if( oldest_monophony != -1 ) {
      monophony[ oldest_monophony ].noteWait();
      return oldest_monophony;
    }
    else {
      for( int index = 0; index != max_monophony; ++index ) {
        if( active_since[ index ] <= oldest_time ) {
          oldest_monophony = index;
          oldest_time = active_since[ index ];
        }
      }
      monophony[ oldest_monophony ].noteOff();
      monophony[ oldest_monophony ].noteWait();
      return oldest_monophony;
    }
  }
  Monophony< Module > monophony[ max_monophony ];
  unsigned int current_note_id;
  SampleType default_pitch;
  unsigned int active_since[ max_monophony ];
  Clock clock;
};

#endif
