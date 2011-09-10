#ifndef TINYFM_CLOCK_HPP
#define TINYFM_CLOCK_HPP

#include "config.hpp"

class Clock {
public:
  Clock() : current_time( 0 ), tangent( 1 ), shift( 0 ) {}
  Clock &operator+=( TimeType _time ) {
    current_time += _time;
    return *this;
  }
  void reset() {
    current_time = 0;
  }
  void noteOn() {
    note_on_time = current_time;
  }
  void noteOff() {
    note_off_time = current_time;
  }
  TimeType getNoteOnTime() const {
    return note_on_time;
  }
  TimeType getNoteOffTime() const {
    return note_off_time;
  }
  TimeType getTangent() const {
    return tangent;
  }
  TimeType getShift() const {
    return shift;
  }
  void setTangent( TimeType _tangent ) {
    tangent = _tangent;
  }
  void setShift( TimeType _shift ) {
    shift = _shift;
  }
  TimeType operator()() const {
    return tangent * current_time + shift;
  }
  TimeType getGlobal() const {
    return current_time;
  }
private:
  TimeType current_time;
  TimeType note_on_time;
  TimeType note_off_time;
  TimeType tangent;
  TimeType shift;
};

#endif
