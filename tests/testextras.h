#ifndef _INC_TESTEXTRAS_
#define _INC_TESTEXTRAS_

class NotDefaultConstructable
{
public:
    NotDefaultConstructable() = delete;
    NotDefaultConstructable(int a, char b)
        : mA { a }, mB { b }
    {
        
    }
    
    int getA() const { return mA; }
    char getB() const { return mB; }
    
private:
    int mA;
    char mB;
};

#endif
