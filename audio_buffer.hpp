#ifndef TINYFM_AUDIO_BUFFER_HPP
#define TINYFM_AUDIO_BUFFER_HPP

#include "mbed.h"
#include "normalizer.hpp"
#include "config.hpp"

AnalogOut audio_out(p18);

class AudioBuffer {
public:
  inline void set( SampleType _value ) {
    normalizer.set( _value );
    SampleType value = normalizer.get();
    buffer[ write_head ] = value * 1 / 4 + write_prev * 3 / 4;
    buffer[ write_head ] = ( buffer[ write_head ] + 1 ) / 2;
    ++write_head;
    write_head &= 0x3F;
    buffer[ write_head ] = value * 2 / 4 + write_prev * 2 / 4;
    buffer[ write_head ] = ( buffer[ write_head ] + 1 ) / 2;
    ++write_head;
    write_head &= 0x3F;
    buffer[ write_head ] = value * 3 / 4 + write_prev * 1 / 4;
    buffer[ write_head ] = ( buffer[ write_head ] + 1 ) / 2;
    ++write_head;
    write_head &= 0x3F;
    buffer[ write_head ] = value;
    buffer[ write_head ] = ( buffer[ write_head ] + 1 ) / 2;
    ++write_head;
    write_head &= 0x3F;
    write_prev = value;
  }
  inline SampleType get() {
    if( size() ) {
      SampleType value = buffer[ read_head ];
      ++read_head;
      read_head &= 0x3F;
      read_prev = value;
      return value;
    }
    else {
      return read_prev;
    }
  }
  inline unsigned int size() const {
    if( write_head >= read_head )
      return write_head - read_head;
    else
      return 64 - ( read_head - write_head );
  }
  inline bool readyToSet() const {
    return size() <= 60;
  }
  inline static AudioBuffer &getInstance() {
    static AudioBuffer buffer;
    return buffer;
  }
private:
  AudioBuffer() : read_head( 0 ), write_head( 0 ), write_prev( 0 ), read_prev( 0 ) {
    audio_stream.attach_us( &AudioBuffer::audioOutput, 1000000/(freq*4) );
  }
  static void audioOutput() {
    audio_out.write_u16( AudioBuffer::getInstance().get().get() );
  }
  Ticker audio_stream;
  unsigned int read_head;
  unsigned int write_head;
  SampleType buffer[ 64 ];
  SampleType write_prev;
  SampleType read_prev;
  Normalize normalizer;
};

#endif
