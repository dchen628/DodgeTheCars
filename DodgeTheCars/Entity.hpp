#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <vector>
#include "object.hpp"


class Entity : public Object
{
public:
    Entity() {rev = 0;}
    void setSpeed (int s) {speed = s;}
    double getSpeed() const {return speed;}
    int createCycle(int r, int w, int h, int amount, int speed);
    void setCurAnimation (int c) {begin = 0; curAnim = c;}
    int getCurAnimation() const {return curAnim;}
    void updateAnimation();
    void reverse(bool r) {rev =r;}
    void reverse(bool r, int nA) {rev =r; nAb = 1; nA = newAnim;}
private:
    double speed;
    struct cycle {
        int row;
        int w;
        int h;
        int amount;
        int speed;
        int tick;
    };
    vector<cycle> animations;
    int curAnim;
    int begin;
    bool rev, nAb;
    int newAnim;
};
#endif