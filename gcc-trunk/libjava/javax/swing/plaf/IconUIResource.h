
// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __javax_swing_plaf_IconUIResource__
#define __javax_swing_plaf_IconUIResource__

#pragma interface

#include <java/lang/Object.h>
extern "Java"
{
  namespace java
  {
    namespace awt
    {
        class Component;
        class Graphics;
    }
  }
  namespace javax
  {
    namespace swing
    {
        class Icon;
      namespace plaf
      {
          class IconUIResource;
      }
    }
  }
}

class javax::swing::plaf::IconUIResource : public ::java::lang::Object
{

public:
  IconUIResource(::javax::swing::Icon *);
  virtual void paintIcon(::java::awt::Component *, ::java::awt::Graphics *, jint, jint);
  virtual jint getIconWidth();
  virtual jint getIconHeight();
public: // actually package-private
  static const jlong serialVersionUID = 3327049506004830542LL;
private:
  ::javax::swing::Icon * __attribute__((aligned(__alignof__( ::java::lang::Object)))) delegate;
public:
  static ::java::lang::Class class$;
};

#endif // __javax_swing_plaf_IconUIResource__
