#ifndef TINYFM_CLOCK_HPP
#define TINYFM_CLOCK_HPP

#include "config.hpp"

class Clock {
public:
  Clock() : current_time( 0 ), tangent( 1 ), shift( 0 ) {}
  inline Clock &operator+=( TimeType _time ) {
    current_time += _time;
    return *this;
  }
  inline void reset() {
    current_time = 0;
  }
  inline void noteOn() {
    note_on_time = current_time;
  }
  inline void noteOff() {
    note_off_time = current_time;
  }
  inline TimeType getNoteOnTime() const {
    return note_on_time;
  }
  inline TimeType getNoteOffTime() const {
    return note_off_time;
  }
  inline TimeType getTangent() const {
    return tangent;
  }
  inline TimeType getShift() const {
    return shift;
  }
  inline void setTangent( TimeType _tangent ) {
    tangent = _tangent;
  }
  inline void setShift( TimeType _shift ) {
    shift = _shift;
  }
  inline TimeType operator()() const {
    return tangent * current_time + shift;
  }
  inline TimeType getGlobal() const {
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
