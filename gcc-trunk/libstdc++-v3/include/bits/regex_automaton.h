// class template regex -*- C++ -*-

// Copyright (C) 2013 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/**
 *  @file bits/regex_automaton.h
 *  This is an internal header file, included by other library headers.
 *  Do not attempt to use it directly. @headername{regex}
 */

namespace std _GLIBCXX_VISIBILITY(default)
{
namespace __detail
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

  /**
   *  @defgroup regex-detail Base and Implementation Classes
   *  @ingroup regex
   *  @{
   */

  typedef int _StateIdT;
  typedef std::set<_StateIdT> _StateSet;
  static const _StateIdT _S_invalid_state_id  = -1;

  template<typename _CharT>
    using _Matcher = std::function<bool (_CharT)>;

  /// Operation codes that define the type of transitions within the base NFA
  /// that represents the regular expression.
  enum _Opcode
  {
      _S_opcode_unknown,
      _S_opcode_alternative,
      _S_opcode_backref,
      _S_opcode_line_begin_assertion,
      _S_opcode_line_end_assertion,
      _S_opcode_word_boundry,
      _S_opcode_subexpr_lookahead,
      _S_opcode_subexpr_begin,
      _S_opcode_subexpr_end,
      _S_opcode_dummy,
      _S_opcode_match,
      _S_opcode_accept,
  };

  template<typename _CharT, typename _TraitsT>
    class _State
    {
    public:
      typedef int                        _OpcodeT;
      typedef _Matcher<_CharT>           _MatcherT;

      _OpcodeT     _M_opcode;           // type of outgoing transition
      _StateIdT    _M_next;             // outgoing transition
      union // Since they are mutually exclusive.
      {
	unsigned int _M_subexpr;        // for _S_opcode_subexpr_*
	unsigned int _M_backref_index;  // for _S_opcode_backref
	struct
	{
	  // for _S_opcode_alternative.
	  _StateIdT  _M_quant_index;
	  // for _S_opcode_alternative or _S_opcode_subexpr_lookahead
	  _StateIdT  _M_alt;
	  // for _S_opcode_word_boundry or _S_opcode_subexpr_lookahead or
	  // quantifiers(ungreedy if set true)
	  bool       _M_neg;
	};
      };
      _MatcherT      _M_matches;        // for _S_opcode_match

      explicit _State(_OpcodeT __opcode)
      : _M_opcode(__opcode), _M_next(_S_invalid_state_id)
      { }

#ifdef _GLIBCXX_DEBUG
      std::ostream&
      _M_print(std::ostream& ostr) const;

      // Prints graphviz dot commands for state.
      std::ostream&
      _M_dot(std::ostream& __ostr, _StateIdT __id) const;
#endif
    };

  /// Base class for, um, automata.  Could be an NFA or a DFA.  Your choice.
  template<typename _CharT, typename _TraitsT>
    class _Automaton
    {
    public:
      typedef unsigned int _SizeT;

    public:
      virtual _SizeT
      _M_sub_count() const = 0;

#ifdef _GLIBCXX_DEBUG
      virtual std::ostream&
      _M_dot(std::ostream& __ostr) const = 0;
#endif
    };

  template<typename _CharT, typename _TraitsT>
    class _NFA
    : public _Automaton<_CharT, _TraitsT>,
      public std::vector<_State<_CharT, _TraitsT>>
    {
    public:
      typedef _State<_CharT, _TraitsT>            _StateT;
      typedef const _Matcher<_CharT>&             _MatcherT;
      typedef unsigned int                        _SizeT;
      typedef regex_constants::syntax_option_type _FlagT;

      _NFA(_FlagT __f)
      : _M_flags(__f), _M_start_state(0), _M_subexpr_count(0),
      _M_has_backref(false), _M_quant_count(0)
      { }

      _FlagT
      _M_options() const
      { return _M_flags; }

      _StateIdT
      _M_start() const
      { return _M_start_state; }

      const _StateSet&
      _M_final_states() const
      { return _M_accepting_states; }

      _SizeT
      _M_sub_count() const
      { return _M_subexpr_count; }

      _StateIdT
      _M_insert_accept()
      {
	auto __ret = _M_insert_state(_StateT(_S_opcode_accept));
	_M_accepting_states.insert(__ret);
	return __ret;
      }

      _StateIdT
      _M_insert_alt(_StateIdT __next, _StateIdT __alt, bool __neg)
      {
	_StateT __tmp(_S_opcode_alternative);
	// It labels every quantifier to make greedy comparison easier in BFS
	// approach.
	__tmp._M_quant_index = _M_quant_count++;
	__tmp._M_next = __next;
	__tmp._M_alt = __alt;
	__tmp._M_neg = __neg;
	return _M_insert_state(__tmp);
      }

      _StateIdT
      _M_insert_matcher(_MatcherT __m)
      {
	_StateT __tmp(_S_opcode_match);
	__tmp._M_matches = __m;
	return _M_insert_state(__tmp);
      }

      _StateIdT
      _M_insert_subexpr_begin()
      {
	auto __id = _M_subexpr_count++;
	_M_paren_stack.push_back(__id);
	_StateT __tmp(_S_opcode_subexpr_begin);
	__tmp._M_subexpr = __id;
	return _M_insert_state(__tmp);
      }

      _StateIdT
      _M_insert_subexpr_end()
      {
	_StateT __tmp(_S_opcode_subexpr_end);
	__tmp._M_subexpr = _M_paren_stack.back();
	_M_paren_stack.pop_back();
	return _M_insert_state(__tmp);
      }

      _StateIdT
      _M_insert_backref(unsigned int __index);

      _StateIdT
      _M_insert_line_begin()
      { return _M_insert_state(_StateT(_S_opcode_line_begin_assertion)); }

      _StateIdT
      _M_insert_line_end()
      { return _M_insert_state(_StateT(_S_opcode_line_end_assertion)); }

      _StateIdT
      _M_insert_word_bound(bool __neg)
      {
	_StateT __tmp(_S_opcode_word_boundry);
	__tmp._M_neg = __neg;
	return _M_insert_state(__tmp);
      }

      _StateIdT
      _M_insert_lookahead(_StateIdT __alt, bool __neg)
      {
	_StateT __tmp(_S_opcode_subexpr_lookahead);
	__tmp._M_alt = __alt;
	__tmp._M_neg = __neg;
	return _M_insert_state(__tmp);
      }

      _StateIdT
      _M_insert_dummy()
      { return _M_insert_state(_StateT(_S_opcode_dummy)); }

      _StateIdT
      _M_insert_state(_StateT __s)
      {
	this->push_back(__s);
	return this->size()-1;
      }

      // Eliminate dummy node in this NFA to make it compact.
      void
      _M_eliminate_dummy();

#ifdef _GLIBCXX_DEBUG
      std::ostream&
      _M_dot(std::ostream& __ostr) const;
#endif

      std::vector<unsigned int> _M_paren_stack;
      _StateSet                 _M_accepting_states;
      _FlagT                    _M_flags;
      _StateIdT                 _M_start_state;
      _SizeT                    _M_subexpr_count;
      _SizeT                    _M_quant_count;
      bool                      _M_has_backref;
    };

  /// Describes a sequence of one or more %_State, its current start
  /// and end(s).  This structure contains fragments of an NFA during
  /// construction.
  template<typename _CharT, typename _TraitsT>
    class _StateSeq
    {
    public:
      typedef _NFA<_CharT, _TraitsT> _RegexT;

    public:
      _StateSeq(_RegexT& __nfa, _StateIdT __s)
      : _StateSeq(__nfa, __s, __s)
      { }

      _StateSeq(_RegexT& __nfa, _StateIdT __s, _StateIdT __end)
      : _M_nfa(__nfa), _M_start(__s), _M_end(__end)
      { }

      // Append a state on *this and change *this to the new sequence.
      void
      _M_append(_StateIdT __id)
      {
	_M_nfa[_M_end]._M_next = __id;
	_M_end = __id;
      }

      // Append a sequence on *this and change *this to the new sequence.
      void
      _M_append(const _StateSeq& __s)
      {
	_M_nfa[_M_end]._M_next = __s._M_start;
	_M_end = __s._M_end;
      }

      // Clones an entire sequence.
      _StateSeq
      _M_clone();

    public:
      _RegexT&  _M_nfa;
      _StateIdT _M_start;
      _StateIdT _M_end;
    };

 //@} regex-detail
_GLIBCXX_END_NAMESPACE_VERSION
} // namespace __detail
} // namespace std

#include <bits/regex_automaton.tcc>
