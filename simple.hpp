/***************************************************************************
 *   Copyright (C) 2009 by Naomasa Matsubayashi   *
 *   harps@quaternion.sakura.ne.jp   *
 *                                                                         *
 *   All rights reserved.                                                  *
 *                                                                         *
 *   Redistribution and use in source and binary forms, with or without    *
 *   modification, are permitted provided that the following conditions    *
 *   are met:                                                              *
 *     * Redistributions of source code must retain the above copyright    *
 *       notice, this list of conditions and the following disclaimer.     *
 *     * Redistributions in binary form must reproduce the above copyright *
 *       notice, this list of conditions and the following disclaimer in   *
 *       the documentation and/or other materials provided with the        *
 *       distribution.                                                     *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS   *
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT     *
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR *
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT  *
 *   OWNER OR                                                              *
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, *
 *   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,   *
 *   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR    *
 *   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY   *
 *   OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT          *
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE *
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  *
 ***************************************************************************/

#ifndef TINYFM_INPUT_SIMPLE
#define TINYFM_INPUT_SIMPLE

#include "audio_buffer.hpp"
#include "clock.hpp"
#include "note.hpp"
#include "config.hpp"


#define NL( name ) \
  ( ( name ) * ( 60.0f / tempo ) )

#define N32 \
  NL( 0.125f )

#define N16 \
  NL( 0.25f )

#define N8 \
  NL( 0.5f )

#define N4 \
  NL( 1.0f )

#define N2 \
  NL( 2.0f )

#define N1 \
  NL( 4.0f )

#define END \
  -1.0f

enum {
  C_0 = 24, CS0, D_0, DS0, E_0, F_0, FS0, G_0, GS0, A_0, AS0, B_0,
  C_1, CS1, D_1, DS1, E_1, F_1, FS1, G_1, GS1, A_1, AS1, B_1,
  C_2, CS2, D_2, DS2, E_2, F_2, FS2, G_2, GS2, A_2, AS2, B_2,
  C_3, CS3, D_3, DS3, E_3, F_3, FS3, G_3, GS3, A_3, AS3, B_3,
  C_4, CS4, D_4, DS4, E_4, F_4, FS4, G_4, GS4, A_4, AS4, B_4,
  C_5, CS5, D_5, DS5, E_5, F_5, FS5, G_5, GS5, A_5, AS5, B_5,
  C_6, CS6, D_6, DS6, E_6, F_6, FS6, G_6, GS6, A_6, AS6, B_6,
  C_7, CS7, D_7, DS7, E_7, F_7, FS7, G_7, GS7, A_7, AS7, B_7
};

struct Score {
  float pos;
  float length;
  unsigned int note;
  float touch;
};

class ReleaseStack {
private:
  struct ReleaseStackElement {
    TimeType release_time;
    int note_id;
  };
public:
  ReleaseStack() : head( 0 ) {}
  void push( TimeType _release_time, int _note_id ) {
    if( head != 64 ) {
      array[ head ].release_time = _release_time;
      array[ head ].note_id = _note_id;
      ++head;
    }
  }
  void pop() {
    if( head )
      --head;
  }
  TimeType getReleaseTime() const {
    return array[ head - 1 ].release_time;
  }
  TimeType getNoteID() const {
    return array[ head - 1 ].note_id;
  }
  bool empty() const {
    return head == 0;
  }
private:
  unsigned int head;
  ReleaseStackElement array[ 64 ];
};

template< typename Instrument >
class SimpleReader {
public:
  SimpleReader ( const Score *_score ) : score ( _score ), current_score ( _score ), is_end ( false ), prev_score_time( 0 ) {}
  inline void read() {
    TimeType time = inst.getGlobalTime();
    while ( !release_stack.empty() && time >= release_stack.getReleaseTime() ) {
      inst.noteOff ( release_stack.getNoteID() );
      release_stack.pop();
    }
    while ( time >= prev_score_time + current_score->pos ) {
      if ( current_score->length == END ) {
        is_end = true;
      }
      else {
        release_stack.push (
          prev_score_time + current_score->pos + current_score->length,
          inst.noteOn ( current_score->note, exp2t ( static_cast< TimeType >( ( current_score->touch - 1.0f ) * 10.0f ) ) )
        );
        prev_score_time += current_score->pos;
      }
      ++current_score;
    }
  }
  inline SampleType operator()() {
    return inst();
  }
  inline bool isEnd() const {
    return is_end;
  }
private:
  Instrument inst;
  ReleaseStack release_stack;
  const Score *current_score;
  const Score * const score;
  bool is_end;
  TimeType prev_score_time;
};

const float tempo = 150.0f;

const Score score0[] = {
  { 0, N8, E_4, 1.0f },
  { N8, N8, G_3, 1.0f },
  { 0, N8, C_2, 1.0f },
  { N8, N8, F_3, 1.0f },
  { N8, N8, D_4, 1.0f },
  { 0, N8, C_2, 1.0f },
  { N8, N8, E_4, 1.0f },
  { N8, N8, G_3, 1.0f },
  { 0, N8, C_2, 1.0f },
  { N8, N8, F_3, 1.0f },
  { N8, N16, FS4, 1.0f },
  { 0, N8, C_2, 1.0f },
  { N16, N16+N8, G_4, 1.0f },
  { N16+N8, N8, B_3, 1.0f },
  { 0, N8, C_2, 1.0f },
  { N8, N8, FS4, 1.0f },
  { N8, N8, A_3, 1.0f },
  { 0, N8, C_2, 1.0f },
  { N8, N8, E_4, 1.0f },
  { N8, N8, G_3, 1.0f },
  { 0, N8, C_2, 1.0f },
  { N8, N8, FS3, 1.0f },
  { N8, N8, D_4, 1.0f },
  { 0, N8, D_2, 1.0f },

  { N8, N8, E_4, 1.0f },
  { N8, N8, G_3, 1.0f },
  { 0, N8, E_2, 1.0f },
  { N8, N8, F_3, 1.0f },
  { N8, N8, D_4, 1.0f },
  { 0, N8, E_2, 1.0f },
  { N8, N8, E_4, 1.0f },
  { N8, N8, G_3, 1.0f },
  { 0, N8, E_2, 1.0f },
  { N8, N8, F_3, 1.0f },
  { N8, N16, FS4, 1.0f },
  { 0, N8, E_2, 1.0f },
  { N16, N16+N8, G_4, 1.0f },
  { N16+N8, N8, B_3, 1.0f },
  { 0, N8, E_2, 1.0f },
  { N8, N8, A_4, 1.0f },
  { N8, N8, FS4, 1.0f },
  { 0, N8, E_2, 1.0f },
  { N8, N8, G_4, 1.0f },
  { N8, N8, E_3, 1.0f },
  { 0, N8, E_2, 1.0f },
  { N8, N8, FS3, 1.0f },
  { N8, N8, D_4, 1.0f },
  { 0, N8, D_2, 1.0f },

  { N8, N8, E_4, 1.0f },
  { N8, N8, G_3, 1.0f },
  { 0, N8, C_2, 1.0f },
  { N8, N8, F_3, 1.0f },
  { N8, N8, D_4, 1.0f },
  { 0, N8, C_2, 1.0f },
  { N8, N8, E_4, 1.0f },
  { N8, N8, G_3, 1.0f },
  { 0, N8, C_2, 1.0f },
  { N8, N8, F_3, 1.0f },
  { N8, N16, FS4, 1.0f },
  { 0, N8, C_2, 1.0f },
  { N16, N16+N8, G_4, 1.0f },
  { N16+N8, N8, B_3, 1.0f },
  { 0, N8, D_2, 1.0f },
  { N8, N8, FS4, 1.0f },
  { N8, N8, A_3, 1.0f },
  { 0, N8, D_2, 1.0f },
  { N8, N8, E_4, 1.0f },
  { N8, N8, G_3, 1.0f },
  { 0, N8, D_2, 1.0f },
  { N8, N8, FS3, 1.0f },
  { N8, N8, D_4, 1.0f },
  { 0, N8, D_2, 1.0f },

  { N8, N8, E_4, 1.0f },
  { N8, N8, G_3, 1.0f },
  { 0, N8, E_2, 1.0f },
  { N8, N8, F_3, 1.0f },
  { N8, N8, D_4, 1.0f },
  { 0, N8, E_2, 1.0f },
  { N8, N8, E_4, 1.0f },
  { N8, N8, G_3, 1.0f },
  { 0, N8, E_2, 1.0f },
  { N8, N8, F_3, 1.0f },
  { N8, N16, FS4, 1.0f },
  { 0, N8, E_2, 1.0f },
  { N16, N16+N8, G_4, 1.0f },
  { N16+N8, N8, B_3, 1.0f },
  { 0, N8, E_2, 1.0f },
  { N8, N8, A_4, 1.0f },
  { N8, N8, FS4, 1.0f },
  { 0, N8, E_2, 1.0f },
  { N8, N8, G_4, 1.0f },
  { N8, N8, E_3, 1.0f },
  { 0, N8, E_2, 1.0f },
  { N8, N8, FS3, 1.0f },
  { N8, N8, D_4, 1.0f },
  { 0, N8, E_2, 1.0f },


  { N1, END, 0, 1.0f },
};

#endif
