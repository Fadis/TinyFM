#ifndef TINYFM_OPERATORS_HPP
#define TINYFM_OPERATORS_HPP

#include "config.hpp"
#include "clock.hpp"
#include "note.hpp"

#define BEGIN_0OP( name ) \
  class name { \
    public: \
      inline SampleType operator()( Clock &_clock, Note &_note ) {

#define END_0OP( name ) \
      } \
  };

#define BEGIN_1OP( name ) \
  template< typename Type0 > \
  class name { \
    public: \
      inline SampleType operator()( Clock &_clock, Note &_note ) {

#define END_1OP( name ) \
      } \
    private: \
      Type0 _0; \
  };

#define BEGIN_2OP( name ) \
  template< typename Type0, typename Type1 > \
  class name { \
  public: \
    inline SampleType operator()( Clock &_clock, Note &_note ) {

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
    inline SampleType operator()( Clock &_clock, Note &_note ) {

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
  inline SampleType operator()( Clock &_clock, Note &_note ) {
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
  inline SampleType operator()( Clock &_clock, Note &_note ) {
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
      const TimeType tangent = -release( _clock, _note );
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

template< typename Attack, typename Decay, typename Sustain >
class ConstEnvelopeTable {
public:
  SampleType get( SampleType _pos ) const {
    const SampleType scaled_pos = _pos / total_length;
    if( scaled_pos < 0 )
      return 0;
    else if( scaled_pos < 1 )
      return table[ static_cast<int>( scaled_pos << 8 ) ];
    else
      return Sustain::value;
  }
  inline static const ConstEnvelopeTable< Attack, Decay, Sustain > &getInstance() {
    static const ConstEnvelopeTable< Attack, Decay, Sustain > table;
    return table;
  }
private:
  ConstEnvelopeTable() : total_length( Attack::value + Decay::value ) {
    if( Decay::value > 0.0001f ) {
      const SampleType ratio = Attack::value / Decay::value;
      const int attack_size = ( 128 * ratio );
      const int decay_size = 256 - attack_size;
      int index;
      for( index = 0; index != attack_size; ++index )
        set( index, static_cast< SampleType >( index ) / attack_size );
      for( index = 0; index != decay_size; ++index )
        set( index + attack_size, static_cast< SampleType >( Sustain::value - 1.0f ) * index / decay_size + 1 );
    }
    else {
      for( int index = 0; index != 256; ++index )
        set( index, static_cast< SampleType >( index ) / 256 );
    }
  }
  void set( int _pos, SampleType _value ) {
    table[ _pos ] = _value;
  }
  SampleType table[ 256 ];
  const SampleType total_length;
};

template< typename Attack, typename Decay, typename Sustain, typename Release >
class ConstEnvelope {
public:
  ConstEnvelope() : prev_level( 0.0f ) {
  }
  inline SampleType operator()( Clock &_clock, Note &_note ) {
    TimeType current_time = _clock.getGlobal();
    if( _note.getStatus() == Note::NOTE_ON ) {
      current_time -= _clock.getNoteOnTime();
      prev_level = ConstEnvelopeTable< Attack, Decay, Sustain >::getInstance().get( current_time );
      return prev_level;
    }
    else {
      current_time -= _clock.getNoteOffTime();
      const TimeType tangent = -Release::value;
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
};



template< typename Source, typename Modulator, typename Effect, typename Attack, typename Decay, typename Sustain, typename Release >
class FM {
public:
  typedef Envelope< Attack, Decay, Sustain, Release > InternalEnvelope;
  FM() {}
  inline SampleType operator()( Clock &_clock, Note &_note ) {
    return sint( ( source( _clock, _note ) + effect( _clock, _note) * modulator( _clock, _note ) ) ) * envelope( _clock, _note );
  }
private:
  Source source;
  Modulator modulator;
  Effect effect;
  InternalEnvelope envelope;
};

template< typename Source, typename Modulator, typename Effect, typename Attack, typename Decay, typename Sustain, typename Release >
class ConstFM {
public:
  typedef ConstEnvelope< Attack, Decay, Sustain, Release > InternalEnvelope;
  ConstFM() {}
  inline SampleType operator()( Clock &_clock, Note &_note ) {
    return sint( ( source( _clock, _note ) + static_cast< SampleType >( Effect::value ) * modulator( _clock, _note ) ) ) * envelope( _clock, _note );
  }
private:
  Source source;
  Modulator modulator;
  InternalEnvelope envelope;
};


template< typename Source >
class Wave {
public:
  Wave() {}
  inline SampleType operator()( Clock &_clock, Note &_note ) {
    return get( source( _clock, _note ) );
  }
  virtual SampleType get( SampleType ) = 0;
private:
  Source source;
};

template< typename Source, typename Traits >
class Hammond {
public:
  Hammond(){}
  fixed32< 16 > operator()( Clock &_clock, Note &_note ) {
    static const fixed32< 16 > scale_16 = 0.5f;
    static const fixed32< 16 > scale_8 = 1.0f;
    static const fixed32< 16 > scale_513 = 1.5f;
    static const fixed32< 16 > scale_4 = 2.0f;
    static const fixed32< 16 > scale_223 = 3.0f;
    static const fixed32< 16 > scale_2 = 4.0f;
    static const fixed32< 16 > scale_135 = 5.0f;
    static const fixed32< 16 > scale_113 = 6.0f;
    static const fixed32< 16 > scale_1 = 8.0f;
    SampleType time = source( _clock, _note );
    fixed32< 16 > sum = 0.0f;
    sum += sint( time * scale_16 ) * Traits::level_16;
    sum += sint( time * scale_8 ) * Traits::level_8;
    sum += sint( time * scale_513 ) * Traits::level_513;
    sum += sint( time * scale_4 ) * Traits::level_4;
    sum += sint( time * scale_223 ) * Traits::level_223;
    sum += sint( time * scale_2 ) * Traits::level_2;
    sum += sint( time * scale_135 ) * Traits::level_135;
    sum += sint( time * scale_113 ) * Traits::level_113;
    sum += sint( time * scale_1) * Traits::level_1;
    fixed32< 16 > max = 0.0f;
    max += Traits::level_16;
    max += Traits::level_8;
    max += Traits::level_513;
    max += Traits::level_4;
    max += Traits::level_223;
    max += Traits::level_2;
    max += Traits::level_135;
    max += Traits::level_113;
    max += Traits::level_1;
    sum /= max;
    return sum;
  }
private:
  Source source;
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
  static const SampleType value = Type0::value;
  return value;
END_1OP( Const )

BEGIN_1OP( Sin )
  return sint( _0( _clock, _note ) );
END_1OP( Sin )

BEGIN_1OP( Triangle )
  return triangle( _0( _clock, _note ) );
END_1OP( Triangle )

BEGIN_2OP( Rectangle )
  static const SampleType duty = Type0::value;
  const SampleType value = _1( _clock, _note );
  if( duty < value )
    return static_cast< SampleType >( 1 );
  else
    return static_cast< SampleType >( 0 );
END_2OP( Rectangle )

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

BEGIN_1OP( Fuzz )
  return exp2t( _0( _clock, _note ) );
END_1OP( Fuzz )

#endif
