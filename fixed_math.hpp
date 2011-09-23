#ifndef TINYFM_FIXED_MATH_HPP
#define TINYFM_FIXED_MATH_HPP

#include "fixed.hpp"
#include "sin_table.hpp"
#include "exp2_table.hpp"
#include "fuzz_table.hpp"

template< unsigned int _pos >
inline fixed32<_pos> sint( fixed32<_pos> _x ) {
  fixed32<_pos> temp;
  temp.set( sin_table[ static_cast<int>( _x << 10 ) & 0x3FF ] << ( _pos - 16 ) );
  return temp;
}

template< unsigned int _pos >
inline fixed32<_pos> cost( fixed32<_pos> _x ) {
  fixed32<_pos> temp;
  temp.set( sin_table[ static_cast<int>( ( _x << 10 ) + 512 ) & 0x3FF ] << ( _pos - 16 ) );
  return temp;
}

template< unsigned int _pos >
inline fixed32<_pos> exp2t( fixed32<_pos> _x ) {
  int scale_factor = _x;
  const unsigned int elem = ( _x - scale_factor ) << 7;
  fixed32<_pos> temp;
  if( _pos >= 14 )
    temp.set( exp2_table[ elem ] << ( _pos - 14 ) );
  return temp << scale_factor;
}

template<unsigned int _pos>
inline fixed32<_pos> noteToFrequency( fixed32<_pos> _note ) {
  static const fixed32<_pos> scale = 13.75f;
  return scale * exp2t( ( _note - 9 ) / 12 );
}

template<unsigned int _pos>
inline fixed32<_pos> triangle( fixed32<_pos> _x ) {
  fixed32<_pos> quad = _x * 4;
  if( quad < 1 )
    return quad;
  else if( quad < 3 )
    return 2 - quad;
  else
    return -4 + quad;
}

template<unsigned int _pos>
inline fixed32<_pos> fuzz( fixed32<_pos> _x ) {
  if( _x < -2 )
    return -1;
  else if( _x < 0 ) {
    fixed32<_pos> temp;
    const unsigned int elem =  -_x << 8;
    temp.set( fuzz_table[ elem ] << ( _pos - 15 ) );
    temp = -temp;
    return temp;
  }
  else if( _x < 2 ) {
    fixed32<_pos> temp;
    const unsigned int elem =  _x << 8;
    temp.set( fuzz_table[ elem ] << ( _pos - 15 ) );
    return temp;
  }
  else
    return 1;
}

#endif
