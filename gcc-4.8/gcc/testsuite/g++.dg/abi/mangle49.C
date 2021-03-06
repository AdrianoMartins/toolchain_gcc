// PR c++/49932
// { dg-options "-std=c++0x -fabi-version=0" }

template < typename T >
auto
f1( T x ) // ICE on here
  -> typename decltype( x )::type {}

template < typename T >
typename decltype( T() )::type
f2( T x ) {} // ICE on here

struct S { typedef void type; };

void g()
{
  f1( S() );
  f2( S() );
}

// { dg-final { scan-assembler "_Z2f1I1SENDtfp_E4typeET_" } }
// { dg-final { scan-assembler "_Z2f2I1SENDTcvT__EE4typeES1_" } }
