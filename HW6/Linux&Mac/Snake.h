#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "basic.h"
#include <vector>
#include "Team.h"
#include "Food.h"

class Snake {
   private:
    
    //std::vector<int> _finalbody;
    Action _last;
    Action _next;
    bool _isAlive;
    bool _eatFood;
    int _hit;  // 0: hit wall, 1: hit other snake's body, 2: hit other snake's
               // head
    int _accUsedTime;

   public:
    Team m_team;
    int _finalBodyLength;
    std::vector<int> _body;
    
    Snake(int initPos, Action initAct, Map& map, Team t)
        : m_team(t), _last(initAct), _next(initAct) {
        _body.push_back(initPos);

        int bodyPos;
        switch(initAct){
            case U_Act:
                bodyPos = initPos + map.getWidth();
                break;
            case D_Act:
                bodyPos = initPos - map.getWidth();
                break;
            case L_Act:
                bodyPos = initPos + 1;
                break;
            case R_Act:
                bodyPos = initPos - 1;
                break;
        }
        _body.push_back(bodyPos);
        

        _isAlive = true;
        _eatFood = false;
        _accUsedTime = 1100;
        _finalBodyLength = _body.size();
        //_body.clear();

        draw(map);
    }
    int getHeadPos() const {
        if (_body.size() == 0)
            return -1;
        else
            return _body[0];
    }
    std::vector<int> getBody() const{ return _body; }
    //std::vector<int> getFinalBody() const {};

    Action getNext() const{ return _next; }
    Action getLast() const{ return _last; }

    bool isDead() const { return !_isAlive; }
    void remove(Map& map) {
        std::vector<int>::iterator it;
        it = _body.begin();

        switch (_hit) {
            case 0:
                map[*it] = '#';
                break;
            case 1:
                map[*it] = '.';
                break;
            case 2:
                map[*it] = ' ';
                break;
        }

        for (; it != _body.end(); it++) {
            map[*it] = '.';
        }
        if( _body.size()>0)_finalBodyLength = _body.size();
	
        _body.clear();
    }
    bool hitSelfCheck() {
        std::vector<int>::iterator it;
        it = _body.begin() + 1;
        for (; it != _body.end(); it++) {
            if (_body[0] == *it) {
                _isAlive = false;
                _hit = 2;
                return true;
            }
        }
        return false;
    }
    bool hitSnakeCheck(Snake* snake) {
        std::vector<int>::iterator it;
        it = snake->_body.begin();
        for (; it != snake->_body.end(); it++) {
            if (_body[0] == *it) {
                _isAlive = false;
                if (it == snake->_body.begin())
                    _hit = 2;
                else
                    _hit = 1;
                return true;
            }
        }
        return false;
    }
    bool hitWallCheck(const Map& map) {
        bool hit = map.isWall(_body[0]);
        if (hit) {
            _hit = 0;
            _isAlive = false;
        }
        return hit;
    }
    bool eatFoodCheck(const Map& map) {
        bool ate = map.isFood(_body[0]);
        if (ate) {
            _eatFood = true;
            return true;
        }
        return false;
    }
    void update(Map& map, const int timer) {
        if (!isValidAction(_next, _last, timer) || _next == noAct)
            _next = _last;

        updateBodyPos(map, timer);

        if (_next != ACC_Act)
            _last = _next;
	if(_isAlive)  _finalBodyLength = _body.size();
    }
    void receiveAction(const Action act) {
        _next = act;
    }
    int getPos() const { return _body[0]; }
    bool isCleared() const { return _body.size() == 0; }
    int getTeamID() const { return m_team.getTeamID(); }

   private:
    void draw(Map& map) {
        std::vector<int>::iterator it;
        it = _body.begin();
        for (; it != _body.end(); it++) {
            if (it == _body.begin())
                map[*it] = m_team.colorChar('@');
            else
                map[*it] = '*';
        }
    }
    bool isValidAction(const Action a1, const Action a2, const int timer) {
        bool con1 = (a1 == U_Act && a2 == D_Act);
        bool con2 = (a1 == D_Act && a2 == U_Act);
        bool con3 = (a1 == L_Act && a2 == R_Act);
        bool con4 = (a1 == R_Act && a2 == L_Act);
        bool con5 = (a1 == ACC_Act || a2 == ACC_Act);
             //&& (_accUsedTime < timer + 100);
        if (con1 || con2 || con3 || con4 || con5)
            return false;
        else
            return true;
    }
    void updateBodyPos(Map& map, const int timer) {
        int nextPos;
        if (_next == ACC_Act) {
            _accUsedTime = timer;
            int step = 2;
            for (int s = 0; s < step; s++) {
                nextPos = getNextPos(_last, map);
                move(map, nextPos);
            }
        } else {
            nextPos = getNextPos(_next, map);
            move(map, nextPos);
        }
    }
    int getNextPos(const Action& act, Map& map) {
        std::vector<int>::iterator it;
        it = _body.begin();
        int nextPos;
        switch (_next) {
            case U_Act:
                nextPos = *it - map.getWidth();
                break;
            case D_Act:
                nextPos = *it + map.getWidth();
                break;
            case L_Act:
                nextPos = *it - 1;
                break;
            case R_Act:
                nextPos = *it + 1;
                break;
            // FIXME
            default:
                nextPos = *it + 1;
                break;
        }
        return nextPos;
    }
    void move(Map& map, const int nextPos) {
        std::vector<int>::iterator it;
        it = _body.begin();

        map[*it] = '*';
        it = _body.begin();
        _body.insert(it, nextPos);

        eatFoodCheck(map);

        it = _body.begin();
        if (map[*it] == ' ' || map[*it] == '.') {
            map[*it] = m_team.colorChar('@');
        }

        if (_eatFood)
            _eatFood = false;
        else {
            it = _body.end() - 1;
            if (map[*it] == '*')
                map[*it] = ' ';
            _body.pop_back();
        }

        _finalBodyLength = _body.size();
    }
};

#endif
