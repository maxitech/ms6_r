#pragma once
// Dummy version of TeensyStep4 for native tests

namespace TS4
{

    class Stepper
    {
    public:
        Stepper(int stepPin, int dirPin)
            : _pos(0)
        {
        }
        float getPosition() const { return _pos; }
        void  setPosition(float p) { _pos = p; }

    private:
        float _pos;
    };

} // namespace TS4