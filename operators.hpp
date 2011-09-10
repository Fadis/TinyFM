#ifndef TINYFM_OPERATORS_HPP
#define TINYFM_OPERATORS_HPP

#include "config.hpp"
#include "clock.hpp"
#include "note.hpp"

#define BEGIN_0OP( name ) \
  class name { \
    public: \
      SampleType operator()( Clock &_clock, Note &_note ) {

#define END_0OP( name ) \
      } \
  };

#define BEGIN_1OP( name ) \
  template< typename Type0 > \
  class name { \
    public: \
      SampleType operator()( Clock &_clock, Note &_note ) {

#define END_1OP( name ) \
      } \
    private: \
      Type0 _0; \
  };

#define BEGIN_2OP( name ) \
  template< typename Type0, typename Type1 > \
  class name { \
  public: \
    SampleType operator()( Clock &_clock, Note &_note ) {

#define END_2OP( name ) \
  } \
private: \
  Type0 _0; \
  Type1 _1; \
};

#define BEGIN_3OP( name ) \
  template< typename Type0, typename Type1, typename Type2 > \
  class name { \
  public: \
    SampleType operator()( Clock &_clock, Note &_note ) {

#define END_3OP( name ) \
  } \
private: \
  Type0 _0; \
  Type1 _1; \
  Type2 _2; \
};

template< typename Level, typename Source >
class Scale {
public:
  Scale() : prev_shift( 0.0f ), prev_tangent( 1.0f ) {}
  SampleType operator()( Clock &_clock, Note &_note ) {
    const TimeType old_tangent = _clock.getTangent();
    const TimeType old_shift = _clock.getShift();
    const TimeType new_tangent = old_tangent * level( _clock, _note );
    const TimeType new_shift = ( new_tangent - prev_tangent ) * _clock.getGlobal() + prev_shift;
    _clock.setTangent( new_tangent );
    _clock.setShift( new_shift );
    const SampleType result = source( _clock, _note );
    _clock.setTangent( old_tangent );
    _clock.setShift( old_shift );
    prev_tangent = new_tangent;
    prev_shift = new_shift;
    return result;
  }
private:
  TimeType prev_shift;
  TimeType prev_tangent;
  Level level;
  Source source;
};

template< typename Attack, typename Decay, typename Sustain, typename Release >
class Envelope {
public:
  Envelope() : prev_level( 0.0f ) {}
  SampleType operator()( Clock &_clock, Note &_note ) {
    TimeType current_time = _clock.getGlobal();
    if( _note.getStatus() == Note::NOTE_ON ) {
      current_time -= _clock.getNoteOnTime();
      const TimeType attack_time = attack( _clock, _note );
      if( current_time < attack_time ) {
        const TimeType tangent = 1/attack_time;
        prev_level = tangent * current_time;
        return prev_level;
      }
      const TimeType decay_time = decay( _clock, _note );
      current_time -= attack_time;
      if( current_time < decay_time ) {
        const TimeType sustain_level = sustain( _clock, _note );
        const TimeType tangent = ( sustain_level - 1 )/decay_time;
        const TimeType shift = 1;
        prev_level = tangent * current_time + shift;
        return prev_level;
      }
      const TimeType sustain_level = sustain( _clock, _note );
      prev_level = sustain_level;
      return prev_level;
    }
    else {
      current_time -= _clock.getNoteOffTime();
      const TimeType sustain_level = sustain( _clock, _note );
      const TimeType release_time = release( _clock, _note );
      const TimeType tangent = -sustain_level/release_time;
      const TimeType shift = prev_level;
      SampleType level = tangent * current_time + shift;
      if( level <= 0 ) {
        level = 0;
        _note.endRequest();
      }
      else
        _note.contRequest();
      return level;
    }
  }
private:
  SampleType prev_level;
  Attack attack;
  Decay decay;
  Sustain sustain;
  Release release;
};

template< typename Source, typename Modulator, typename Effect, typename Attack, typename Decay, typename Sustain, typename Release >
class FM {
public:
  typedef Envelope< Attack, Decay, Sustain, Release > InternalEnvelope;
  FM() {}
  SampleType operator()( Clock &_clock, Note &_note ) {
    return sint( ( source( _clock, _note ) + effect( _clock, _note) * modulator( _clock, _note ) ) ) * envelope( _clock, _note );
  }
private:
  Source source;
  Modulator modulator;
  Effect effect;
  InternalEnvelope envelope;
};


BEGIN_0OP( NoteNumber )
  return _note.getNumber();
END_0OP( NoteNumber )

BEGIN_0OP( NoteFrequency )
  return _note.getFrequency();
END_0OP( NoteFrequency )

BEGIN_0OP( Time )
  return _clock() - static_cast<int>( _clock() );
END_0OP( Time )

BEGIN_1OP( Const )
  return Type0::value;
END_1OP( Const )

BEGIN_1OP( Sin )
  return sint( _0( _clock, _note ) );
END_1OP( Sin )

BEGIN_1OP( Triangle )
  return triangle( _0( _clock, _note ) );
END_1OP( Triangle )

BEGIN_2OP( Add )
  return _0( _clock, _note ) + _1( _clock, _note );
END_2OP( Add )

BEGIN_2OP( Sub )
  return _0( _clock, _note ) - _1( _clock, _note );
END_2OP( Sub )

BEGIN_2OP( Mul )
  return _0( _clock, _note ) * _1( _clock, _note );
END_2OP( Mul )

BEGIN_2OP( Div )
  return _0( _clock, _note ) / _1( _clock, _note );
END_2OP( Div )

BEGIN_2OP( Less )
  return _0( _clock, _note ) < _1( _clock, _note );
END_2OP( Less )

BEGIN_2OP( More )
  return _0( _clock, _note ) > _1( _clock, _note );
END_2OP( More )

BEGIN_2OP( Equal )
  return _0( _clock, _note ) == _1( _clock, _note );
END_2OP( Equal )

BEGIN_2OP( NotEqual )
  return _0( _clock, _note ) != _1( _clock, _note );
END_2OP( NotEqual )

BEGIN_3OP( Branch )
  if( _0( _clock, _note ) != 0.0f )
    return _1( _clock, _note );
  else
    return _2( _clock, _note );
END_3OP( Branch )

#endif
