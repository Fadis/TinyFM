#ifndef TINYFM_NOTE_HPP
#define TINYFM_NOTE_HPP

#include "config.hpp"

class Note {
public:
  enum KeyState {
    NOTE_ON,
    NOTE_OFF,
    NOTE_WAIT
  };
  Note() : status( NOTE_WAIT ) {}
  void noteOn( unsigned int _number, SampleType _pressure ) {
    status = NOTE_ON;
    number = _number;
    frequency = noteToFrequency( static_cast< SampleType >( number ) );
    pressure = _pressure;
  }
  void noteOff() {
    status = NOTE_OFF;
  }
  void noteWait() {
    status = NOTE_WAIT;
  }
  TimeType getFrequency() const {
    return frequency;
  }
  unsigned int getNumber() const {
    return number;
  }
  KeyState getStatus() const {
    return status;
  }
  void endRequest() {
    ++end_request;
  }
  void contRequest() {
    ++cont_request;
  }
  void resetVote() {
    end_request = 0;
    cont_request = 0;
  }
  bool isEnd() const {
    return cont_request == 0;
  }
  void pitchBend( SampleType _pitch ) {
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
