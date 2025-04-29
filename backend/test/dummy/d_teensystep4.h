#pragma once
// Dummy version of TeensyStep4 for native tests

namespace TS4
{

    class Stepper
    {
    public:
        Stepper()
            : pos(0)
        {
        }
        float getPosition() const { return pos; }
        void  setPosition(float p) { pos = p; }

    private:
        float pos;
    };

} // namespace TS4