// { dg-do assemble  }
// GROUPS passed initialization
class Vector {
        double  *v;
        int             size;

public:
        Vector(int n);
        ~Vector();
};

exmpl() { Vector x(8)[16]; }// { dg-error "" } .*

