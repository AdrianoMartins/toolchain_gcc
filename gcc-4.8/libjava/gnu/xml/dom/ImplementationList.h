
// DO NOT EDIT THIS FILE - it is machine generated -*- c++ -*-

#ifndef __gnu_xml_dom_ImplementationList__
#define __gnu_xml_dom_ImplementationList__

#pragma interface

#include <java/lang/Object.h>
extern "Java"
{
  namespace gnu
  {
    namespace xml
    {
      namespace dom
      {
          class ImplementationList;
      }
    }
  }
  namespace org
  {
    namespace w3c
    {
      namespace dom
      {
          class DOMImplementation;
      }
    }
  }
}

class gnu::xml::dom::ImplementationList : public ::java::lang::Object
{

public: // actually package-private
  ImplementationList(::java::util::List *);
public:
  virtual jint getLength();
  virtual ::org::w3c::dom::DOMImplementation * item(jint);
private:
  ::java::util::List * __attribute__((aligned(__alignof__( ::java::lang::Object)))) list;
public:
  static ::java::lang::Class class$;
};

#endif // __gnu_xml_dom_ImplementationList__
