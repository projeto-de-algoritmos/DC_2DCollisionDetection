#ifndef _COLLISION_DETECTION_HPP_
#define _COLLISION_DETECTION_HPP_

#include "Ball.hpp"

#include <cstdint>
#include <vector>

class Rectangle{
    private:
    public:
}

class QuadTree{
    private:
        int MAX_OBJECTS = 40;
        int MAX_LEVELS = 3;
        int level;
        vector<Ball *> balls;
        Rectangle bounds;

        QuadTree nodes[4];

    void clear(){
        balls.clear();
        for(int i = 0; i < 4; ++i){
            if(nodes[i] != NULL){
                nodes[i].clear();
                nodes[i] = NULL;
            }
        }
    }

    void split(){
        int subWith = (int)bounds.getWidth()/2;
        int subHeight = (int)bounds.getHeight()/2;

        int x = (int)bounds.getX();
        int y = (int)bounds.getY();

        nodes[0] = new QuadTree(level + 1, new Rectangle(
    }

    public:

        QuadTree(int plevel, Rectangle pbounds){
            level = plevel;
            bounds = pbounds;
        }

}

namespace ColDetect
{
    void EfficientCollisionDetection(std::vector<Ball *> balls, int32_t table_width, int32_t table_height)
    {
        // TODO
        // Detectar colisões entre as bolas presentes no vetor balls usando o algormto quad-tree
        // Se duas bolas estiver se colidindo, ativar o brilho de ambas: ball->turnOnBallHighlight()
    }

    void NaiveCollisionDetection(std::vector<Ball *> balls) {
        // Algoritmo ingenuo O(N^2)
        for (int i = 0; i < (int) balls.size(); i++) {
            for (int j = i+1; j < (int) balls.size(); j++) {

                if (Ball::ballsAreColliding(balls[i], balls[j])) {
                    balls[i]->turnOnBallHighlight();
                    balls[j]->turnOnBallHighlight();
                }
            }
        }
    }
}

#endif
