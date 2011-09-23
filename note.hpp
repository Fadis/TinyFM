#ifndef TINYFM_NOTE_HPP
#define TINYFM_NOTE_HPP

#include "fixed_math.hpp"
#include "config.hpp"

class Note {
public:
  enum KeyState {
    NOTE_ON,
    NOTE_OFF,
    NOTE_WAIT
  };
  Note() : status( NOTE_WAIT ) {}
  inline void noteOn( unsigned int _number, SampleType _pressure ) {
    status = NOTE_ON;
    number = _number;
    frequency = noteToFrequency( static_cast< SampleType >( number ) );
    pressure = _pressure;
  }
  inline void noteOff() {
    status = NOTE_OFF;
  }
  inline void noteWait() {
    status = NOTE_WAIT;
  }
  inline TimeType getFrequency() const {
    return frequency;
  }
  inline unsigned int getNumber() const {
    return number;
  }
  inline KeyState getStatus() const {
    return status;
  }
  inline void endRequest() {
    ++end_request;
  }
  inline void contRequest() {
    ++cont_request;
  }
  inline void resetVote() {
    end_request = 0;
    cont_request = 0;
  }
  inline bool isEnd() const {
    return cont_request == 0;
  }
  inline void pitchBend( SampleType _pitch ) {
    frequency = noteToFrequency( static_cast< SampleType >( number ) + _pitch );
  }
private:
  KeyState status;
  unsigned int number;
  TimeType frequency;
  SampleType pressure;
  int end_request;
  int cont_request;
};

#endif
