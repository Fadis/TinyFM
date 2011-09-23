#ifndef TINYFM_FIXED_HPP
#define TINYFM_FIXED_HPP

#include "stdint.h"

template< unsigned int _pos >
class fixed32 {
private:
  int32_t value;
public:
  static const unsigned int pos = _pos;
  fixed32() {}
  fixed32( const fixed32<_pos> &_source ) : value( _source.value ) {}
  fixed32( char _value ) : value( static_cast<int32_t>( _value ) << _pos ) {}
  fixed32( unsigned char _value ) : value( static_cast<int32_t>( _value ) << _pos ) {}
  fixed32( short int _value ) : value( static_cast<int32_t>( _value ) << _pos ) {}
  fixed32( unsigned short int _value ) : value( static_cast<int32_t>( _value ) << _pos ) {}
  fixed32( int _value ) : value( static_cast<int32_t>( _value ) << _pos ) {}
  fixed32( unsigned int _value ) : value( static_cast<int32_t>( _value ) << _pos ) {}
  fixed32( long long int _value ) : value( static_cast<int32_t>( _value ) << _pos ) {}
  fixed32( unsigned long long int _value ) : value( static_cast<int32_t>( _value ) << _pos ) {}
  fixed32( float _value ) : value( _value * ( 1 << _pos ) ) {}
  fixed32( double _value ) : value( _value * ( 1 << _pos ) ) {}
  fixed32( long double _value ) : value( _value * ( 1 << _pos ) ) {}
  inline operator char() { return value >> _pos; }
  inline operator unsigned char() { return value >> _pos; }
  inline operator short int() { return value >> _pos; }
  inline operator unsigned short int() { return value >> _pos; }
  inline operator int() { return value >> _pos; }
  inline operator unsigned int() { return value >> _pos; }
  inline operator long long int() { return value >> _pos; }
  inline operator unsigned long long int() { return value >> _pos; }
  inline operator float() { return static_cast< float >( value ) / ( 1 << _pos ); }
  inline operator double() { return static_cast< double >( value ) / ( 1 << _pos ); }
  inline operator long double() { return static_cast< long double >( value ) / ( 1 << _pos ); }

  inline void set( int32_t _value ) {
    value = _value;
  }

  inline int32_t get() const {
    return value;
  }

  inline bool operator==( fixed32<_pos> _with ) const {
    return value == _with.value;
  }
  inline bool operator!=( fixed32<_pos> _with ) const {
    return value != _with.value;
  }
  inline bool operator<( fixed32<_pos> _with ) const {
    return value < _with.value;
  }
  inline bool operator>( fixed32<_pos> _with ) const {
    return value > _with.value;
  }
  inline bool operator<=( fixed32<_pos> _with ) const {
    return value <= _with.value;
  }
  inline bool operator>=( fixed32<_pos> _with ) const {
    return value >= _with.value;
  }

  inline fixed32<_pos> &operator++() {
    value += ( 1 << _pos );
    return *this;
  }
  inline fixed32<_pos> operator++(int) {
    fixed32<_pos> old = *this;
    value += ( 1 << _pos );
    return old;
  }
  inline fixed32<_pos> &operator--() {
    value += ( 1 << _pos );
    return *this;
  }
  inline fixed32<_pos> operator--(int) {
    fixed32<_pos> old = *this;
    value += ( 1 << _pos );
    return old;
  }
  inline fixed32<_pos> operator-() const {
    fixed32<_pos> temp;
    temp.value = -value;
    return temp;
  }
  inline fixed32<_pos> &operator+=( fixed32<_pos> _right ) {
    value += _right.value;
    return *this;
  }
  inline fixed32<_pos> &operator-=( fixed32<_pos> _right ) {
    value -= _right.value;
    return *this;
  }
  inline fixed32<_pos> &operator*=( fixed32<_pos> _right ) {
    value = ( static_cast<int64_t>( value ) * static_cast<int64_t>( _right.value ) ) >> _pos;
    return *this;
  }
  inline fixed32<_pos> &operator/=( fixed32<_pos> _right ) {
    value = ( static_cast<int64_t>( value ) << _pos ) / _right.value;
    return *this;
  }
  inline fixed32<_pos> &operator<<=( int _right ) {
    value <<= _right;
    return *this;
  }
  inline fixed32<_pos> &operator>>=( int _right ) {
    value >>= _right;
    return *this;
  }

  inline fixed32<_pos> operator+( fixed32<_pos> _right ) const {
    fixed32<_pos> temp;
    temp.value = value + _right.value;
    return temp;
  }
  inline fixed32<_pos> operator-( fixed32<_pos> _right ) const {
    fixed32<_pos> temp;
    temp.value = value - _right.value;
    return temp;
  }
  inline fixed32<_pos> operator*( fixed32 _right ) const {
    fixed32<_pos> temp;
    temp.value = ( static_cast<int64_t>( value ) * static_cast<int64_t>( _right.value ) ) >> _pos;
    return temp;
  }
  inline fixed32<_pos> operator/( fixed32<_pos> _right ) const {
    fixed32<_pos> temp;
    temp.value = ( static_cast<int64_t>( value ) << _pos ) / _right.value;
    return temp;
  }

  inline fixed32<_pos> operator<<( int _right ) const {
    fixed32<_pos> temp;
    temp.value = value << _right;
    return temp;
  }
  inline fixed32<_pos> operator>>( int _right ) const {
    fixed32<_pos> temp;
    temp.value = value >> _right;
    return temp;
  }
};


template<unsigned int _pos, typename Type>
inline fixed32<_pos> &operator+=( fixed32<_pos> &_left, Type _right ) {
  return _left += static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
inline Type &operator+=( Type &_left, fixed32<_pos> _right ) {
  return _left += static_cast< Type >( _right );
}
template<unsigned int _pos, typename Type>
inline fixed32<_pos> &operator-=( fixed32<_pos> &_left, Type _right ) {
  return _left -= static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
inline Type &operator-=( Type &_left, fixed32<_pos> _right ) {
  return _left -= static_cast< Type >( _right );
}
template<unsigned int _pos, typename Type>
inline fixed32<_pos> &operator*=( fixed32<_pos> &_left, Type _right ) {
  return _left *= static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
inline Type &operator*=( Type &_left, fixed32<_pos> _right ) {
  return _left *= static_cast< Type >( _right );
}
template<unsigned int _pos, typename Type>
inline fixed32<_pos> &operator/=( fixed32<_pos> &_left, Type _right ) {
  return _left /= static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
inline Type &operator/=( Type &_left, fixed32<_pos> _right ) {
  return _left /= static_cast< Type >( _right );
}

template<unsigned int _pos, typename Type>
inline fixed32<_pos> operator+( fixed32<_pos> _left, Type _right ) {
  return _left + static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
inline fixed32<_pos> operator+( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) + _right;
}
template<unsigned int _pos, typename Type>
inline fixed32<_pos> operator-( fixed32<_pos> _left, Type _right ) {
  return _left - static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
inline fixed32<_pos> operator-( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) - _right;
}
template<unsigned int _pos, typename Type>
inline fixed32<_pos> operator*( fixed32<_pos> _left, Type _right ) {
  return _left * static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
inline fixed32<_pos> operator*( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) * _right;
}
template<unsigned int _pos, typename Type>
inline fixed32<_pos> operator/( fixed32<_pos> _left, Type _right ) {
  return _left / static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
inline fixed32<_pos> operator/( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) / _right;
}
template<unsigned int _pos, typename Type>
inline bool operator==( fixed32<_pos> _left, Type _right ) {
  return _left == static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
inline bool operator==( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) == _right;
}
template<unsigned int _pos, typename Type>
inline bool operator!=( fixed32<_pos> _left, Type _right ) {
  return _left != static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
inline bool operator!=( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) != _right;
}
template<unsigned int _pos, typename Type>
inline bool operator<( fixed32<_pos> _left, Type _right ) {
  return _left < static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
inline bool operator<( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) < _right;
}
template<unsigned int _pos, typename Type>
inline bool operator>( fixed32<_pos> _left, Type _right ) {
  return _left > static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
inline bool operator>( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) > _right;
}
template<unsigned int _pos, typename Type>
inline bool operator<=( fixed32<_pos> _left, Type _right ) {
  return _left <= static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
inline bool operator<=( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) <= _right;
}
template<unsigned int _pos, typename Type>
inline bool operator>=( fixed32<_pos> _left, Type _right ) {
  return _left >= static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
inline bool operator>=( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) >= _right;
}

#endif
