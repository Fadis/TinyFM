#ifndef TINYFM_NORMALIZER_HPP
#define TINYFM_NORMALIZER_HPP

#include "config.hpp"

class Top {
public:
  Top() : top( 1 ), until( 0 ) {
  }
  void set( SampleType _value ) {
    if( get() < _value ) {
      top = _value;
      until = 1;
    }
    else if( until > 0 )
      until -= getGain();
  }
  SampleType get() const {
    const SampleType value = top * until;
    if( value < 1 )
      return 1;
    else
      return value;
  }
private:
  SampleType getGain() const {
    static const SampleType gain = 0.0000625f;
    return gain;
  }
  SampleType top;
  SampleType until;
};

class Normalize {
public:
  Normalize() : head( 0u ) {
    for( unsigned int index = 0; index != 9; ++index ) {
      wave_buffer[ index ] = 0;
      top_buffer[ index ] = 1;
    }
  }
  void set( SampleType _value ) {
    wave_buffer[ head ] = _value;
    if( _value < 0 )
      _value = -_value;
    top.set( _value * getScale() );
    top_buffer[ head ] = top.get();
    ++head;
    head %= 9;
  }
  SampleType get() const {
    SampleType sum = 0;
    for( unsigned int index = 0; index != 9; ++index ) {
      sum += top_buffer[ ( index + head ) % 9 ] * getWeight( index );
    }
    sum /= getNormal();
    return wave_buffer[ ( head + 5 ) % 9 ] / sum;
  }
private:
  SampleType getWeight( unsigned int _index ) const {
    static const SampleType savgol_weight[ 9 ] = {
      -21, 14, 39, 54, 59, 54, 39, 14, -21
    };
    return savgol_weight[ _index ];
  }
  SampleType getNormal() const {
    static const SampleType savgol_normal = 231;
    return savgol_normal;
  }
  SampleType getScale() const {
    static const SampleType scale = 1.5f;
    return scale;
  }
  Top top;
  unsigned int head;
  SampleType wave_buffer[9];
  SampleType top_buffer[9];
};

#endif
