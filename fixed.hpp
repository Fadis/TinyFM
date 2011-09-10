#ifndef TINYFM_FIXED_HPP
#define TINYFM_FIXED_HPP

#include "stdint.h"

template< unsigned int _pos >
class fixed32 {
private:
  int32_t value;
public:
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
  operator char() { return value >> _pos; }
  operator unsigned char() { return value >> _pos; }
  operator short int() { return value >> _pos; }
  operator unsigned short int() { return value >> _pos; }
  operator int() { return value >> _pos; }
  operator unsigned int() { return value >> _pos; }
  operator long long int() { return value >> _pos; }
  operator unsigned long long int() { return value >> _pos; }
  operator float() { return static_cast< float >( value ) / ( 1 << _pos ); }
  operator double() { return static_cast< double >( value ) / ( 1 << _pos ); }
  operator long double() { return static_cast< long double >( value ) / ( 1 << _pos ); }

  void set( int32_t _value ) {
    value = _value;
  }

  int32_t get() const {
    return value;
  }

  bool operator==( fixed32<_pos> _with ) const {
    return value == _with.value;
  }
  bool operator!=( fixed32<_pos> _with ) const {
    return value != _with.value;
  }
  bool operator<( fixed32<_pos> _with ) const {
    return value < _with.value;
  }
  bool operator>( fixed32<_pos> _with ) const {
    return value > _with.value;
  }
  bool operator<=( fixed32<_pos> _with ) const {
    return value <= _with.value;
  }
  bool operator>=( fixed32<_pos> _with ) const {
    return value >= _with.value;
  }

  fixed32<_pos> &operator++() {
    value += ( 1 << _pos );
    return *this;
  }
  fixed32<_pos> operator++(int) {
    fixed32<_pos> old = *this;
    value += ( 1 << _pos );
    return old;
  }
  fixed32<_pos> &operator--() {
    value += ( 1 << _pos );
    return *this;
  }
  fixed32<_pos> operator--(int) {
    fixed32<_pos> old = *this;
    value += ( 1 << _pos );
    return old;
  }
  fixed32<_pos> operator-() const {
    fixed32<_pos> temp;
    temp.value = -value;
    return temp;
  }
  fixed32<_pos> &operator+=( fixed32<_pos> _right ) {
    value += _right.value;
    return *this;
  }
  fixed32<_pos> &operator-=( fixed32<_pos> _right ) {
    value -= _right.value;
    return *this;
  }
  fixed32<_pos> &operator*=( fixed32<_pos> _right ) {
    value = ( static_cast<int64_t>( value ) * static_cast<int64_t>( _right.value ) ) >> _pos;
    return *this;
  }
  fixed32<_pos> &operator/=( fixed32<_pos> _right ) {
    value = ( static_cast<int64_t>( value ) << _pos ) / _right.value;
    return *this;
  }
  fixed32<_pos> &operator<<=( int _right ) {
    value <<= _right;
    return *this;
  }
  fixed32<_pos> &operator>>=( int _right ) {
    value >>= _right;
    return *this;
  }

  fixed32<_pos> operator+( fixed32<_pos> _right ) const {
    fixed32<_pos> temp;
    temp.value = value + _right.value;
    return temp;
  }
  fixed32<_pos> operator-( fixed32<_pos> _right ) const {
    fixed32<_pos> temp;
    temp.value = value - _right.value;
    return temp;
  }
  fixed32<_pos> operator*( fixed32 _right ) const {
    fixed32<_pos> temp;
    temp.value = ( static_cast<int64_t>( value ) * static_cast<int64_t>( _right.value ) ) >> _pos;
    return temp;
  }
  fixed32<_pos> operator/( fixed32<_pos> _right ) const {
    fixed32<_pos> temp;
    temp.value = ( static_cast<int64_t>( value ) << _pos ) / _right.value;
    return temp;
  }

  fixed32<_pos> operator<<( int _right ) const {
    fixed32<_pos> temp;
    temp.value = value << _right;
    return temp;
  }
  fixed32<_pos> operator>>( int _right ) const {
    fixed32<_pos> temp;
    temp.value = value >> _right;
    return temp;
  }
};


template<unsigned int _pos, typename Type>
fixed32<_pos> &operator+=( fixed32<_pos> &_left, Type _right ) {
  return _left += static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
Type &operator+=( Type &_left, fixed32<_pos> _right ) {
  return _left += static_cast< Type >( _right );
}
template<unsigned int _pos, typename Type>
fixed32<_pos> &operator-=( fixed32<_pos> &_left, Type _right ) {
  return _left -= static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
Type &operator-=( Type &_left, fixed32<_pos> _right ) {
  return _left -= static_cast< Type >( _right );
}
template<unsigned int _pos, typename Type>
fixed32<_pos> &operator*=( fixed32<_pos> &_left, Type _right ) {
  return _left *= static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
Type &operator*=( Type &_left, fixed32<_pos> _right ) {
  return _left *= static_cast< Type >( _right );
}
template<unsigned int _pos, typename Type>
fixed32<_pos> &operator/=( fixed32<_pos> &_left, Type _right ) {
  return _left /= static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
Type &operator/=( Type &_left, fixed32<_pos> _right ) {
  return _left /= static_cast< Type >( _right );
}

template<unsigned int _pos, typename Type>
fixed32<_pos> operator+( fixed32<_pos> _left, Type _right ) {
  return _left + static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
fixed32<_pos> operator+( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) + _right;
}
template<unsigned int _pos, typename Type>
fixed32<_pos> operator-( fixed32<_pos> _left, Type _right ) {
  return _left - static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
fixed32<_pos> operator-( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) - _right;
}
template<unsigned int _pos, typename Type>
fixed32<_pos> operator*( fixed32<_pos> _left, Type _right ) {
  return _left * static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
fixed32<_pos> operator*( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) * _right;
}
template<unsigned int _pos, typename Type>
fixed32<_pos> operator/( fixed32<_pos> _left, Type _right ) {
  return _left / static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
fixed32<_pos> operator/( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) / _right;
}
template<unsigned int _pos, typename Type>
bool operator==( fixed32<_pos> _left, Type _right ) {
  return _left == static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
bool operator==( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) == _right;
}
template<unsigned int _pos, typename Type>
bool operator!=( fixed32<_pos> _left, Type _right ) {
  return _left != static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
bool operator!=( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) != _right;
}
template<unsigned int _pos, typename Type>
bool operator<( fixed32<_pos> _left, Type _right ) {
  return _left < static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
bool operator<( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) < _right;
}
template<unsigned int _pos, typename Type>
bool operator>( fixed32<_pos> _left, Type _right ) {
  return _left > static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
bool operator>( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) > _right;
}
template<unsigned int _pos, typename Type>
bool operator<=( fixed32<_pos> _left, Type _right ) {
  return _left <= static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
bool operator<=( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) <= _right;
}
template<unsigned int _pos, typename Type>
bool operator>=( fixed32<_pos> _left, Type _right ) {
  return _left >= static_cast< fixed32<_pos> >( _right );
}
template<unsigned int _pos, typename Type>
bool operator>=( Type _left, fixed32<_pos> _right ) {
  return static_cast< fixed32<_pos> >( _left ) >= _right;
}

#endif
