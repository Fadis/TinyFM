#ifndef TINYFM_AUDIO_BUFFER_HPP
#define TINYFM_AUDIO_BUFFER_HPP

#include "mbed.h"
#include "normalizer.hpp"
#include "config.hpp"

AnalogOut audio_out(p18);

class AudioBuffer {
public:
  void set( SampleType _value ) {
      normalizer.set( _value );
      SampleType value = normalizer.get();
      buffer[ write_head ] = value * 1 / 4 + prev * 3 / 4;
      buffer[ write_head ] = ( buffer[ write_head ] + 1 ) / 2;
      ++write_head;
      write_head &= 0x3F;
      buffer[ write_head ] = value * 2 / 4 + prev * 2 / 4;
      buffer[ write_head ] = ( buffer[ write_head ] + 1 ) / 2;
      ++write_head;
      write_head &= 0x3F;
      buffer[ write_head ] = value * 3 / 4 + prev * 1 / 4;
      buffer[ write_head ] = ( buffer[ write_head ] + 1 ) / 2;
      ++write_head;
      write_head &= 0x3F;
      buffer[ write_head ] = value;
      buffer[ write_head ] = ( buffer[ write_head ] + 1 ) / 2;
      ++write_head;
      write_head &= 0x3F;
      prev = value;
  }
  SampleType get() {
    if( size() ) {
      SampleType value = buffer[ read_head ];
      ++read_head;
      read_head &= 0x3F;
      return value;
    }
    else {
      SampleType value = buffer[ read_head ];
      return 0;
    }
  }
  unsigned int size() const {
    if( write_head >= read_head )
      return write_head - read_head;
    else
      return 64 - ( read_head - write_head );
  }
  bool readyToSet() const {
    return size() <= 60;
  }
  static AudioBuffer &getInstance() {
    static AudioBuffer buffer;
    return buffer;
  }
private:
  AudioBuffer() : read_head( 0 ), write_head( 0 ), prev( 0 ) {
    audio_stream.attach( &AudioBuffer::audioOutput, 1.0f/(freq*4.0f) );
  }
  static void audioOutput() {
    SampleType value = AudioBuffer::getInstance().get();
    audio_out.write_u16( value.get() );
  }
  Ticker audio_stream;
  unsigned int read_head;
  unsigned int write_head;
  SampleType buffer[ 64 ];
  SampleType prev;
  Normalize normalizer;
};

#endif
