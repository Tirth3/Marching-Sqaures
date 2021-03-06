#include <SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
#include"Noise.h"
using namespace std;

const int sWidth = 512;
const int sHeight = 512;
const int nSpacing = 32;

const int cols = 1 + (sWidth / nSpacing);
const int rows = 1 + (sHeight / nSpacing);

float fField[cols][rows];
int nField[cols][rows];



void InitField(float xoff , float yoff){
    //cout<<"                                                           "<<"\n";
    Noise noise(10 ,10);
    for(int i = 0 ; i < cols ; i++){
        for(int j = 0 ; j < rows ; j++){
            fField[i][j] = noise.CalcNoise(xoff * i , yoff * j);

           if(fField[i][j] < 0.5){
                nField[i][j] = 0;
            }else{
                nField[i][j] = 1;
            }
           //cout<<nField[i][j]<<" ";
        }
        //cout<<"\n";
    }
}


void DrawField(sf::RenderWindow &win){

    for(int i = 0 ; i <= cols ; i++){
        for(int j = 0 ; j <= rows ; j++){
            sf::CircleShape c;
            c.setRadius(1.0f);
            c.setPosition(sf::Vector2f((i * nSpacing) ,(j * nSpacing)));
            c.setFillColor(sf::Color(255 * fField[i][j], 255 * fField[i][j], 255 * fField[i][j]));
            win.draw(c);
        }
    }
}

int State(int a ,int b ,int c ,int d){
    return a * 8 + b * 4 + c * 2 + d * 1;
}

void DrawLine(sf::RenderWindow &win , sf::Vector2f sPos , sf::Vector2f ePos){
    sf::Vertex line[]={
      sf::Vertex(sPos , sf::Color::White),
      sf::Vertex(ePos, sf::Color::White)
    };
    win.draw(line , 2 , sf::Lines);

}

void MarchingSquare(sf::RenderWindow &win){
    for(int i = 0 ; i <= cols - 1 ; i++){
        for(int j = 0 ; j <= rows - 1 ; j++){
            int nState = State(nField[i][j] , nField[i + 1][j] , nField[i + 1][j + 1] , nField[i][j+1]);
            float x = i * nSpacing;
            float y = j * nSpacing;

            sf::Vector2f a = sf::Vector2f(x + nSpacing * 0.5, y);
			sf::Vector2f b = sf::Vector2f(x + nSpacing , y + nSpacing * 0.5);
			sf::Vector2f c = sf::Vector2f(x + nSpacing * 0.5, y + nSpacing);
			sf::Vector2f d = sf::Vector2f(x, y + nSpacing * 0.5);

            switch(nState){
                case 0:
                    break;
                case 1:
                    DrawLine(win , d , c);
                    break;

                case 2:
                    DrawLine(win , b , c);
                    break;

                case 3:
                    DrawLine(win , d , b);
                    break;

                case 4:
                    DrawLine(win ,a , b);
                    break;

                case 5:
                    DrawLine(win , a , d);

                    DrawLine(win , b , c);
                    break;

                case 6:
                    DrawLine(win , a , c);
                    break;

                case 7:
                    DrawLine(win , a , d);
                    break;

                case 8:
                    DrawLine(win , a , d);
                    break;

                case 9:
                    DrawLine(win , a , c);
                    break;

                case 10:
                    DrawLine(win , a , b);
                    DrawLine(win , d , c);
                    break;

                case 11:
                    DrawLine(win , a , b);
                    break;

                case 12:
                    DrawLine(win , d , b);
                    break;

                case 13:
                    DrawLine(win , b , c);
                    break;

                case 14:
                    DrawLine(win , d , c);
                    break;

                case 15:
                    break;
            }
        }
    }
}

float Random(float x) //range : [min, max]
{

    float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / x));
    return r;
}



int main()
{
    srand( time(NULL) );

    float xoff = 300, yoff = 300;
    InitField(xoff , yoff);

    sf::RenderWindow window(sf::VideoMode(sWidth, sHeight), "SFML works!");

    while (window.isOpen())
    {
        sf::Event evnt;
        while (window.pollEvent(evnt))
        {
            if (evnt.type == sf::Event::Closed)
                window.close();


            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)){
                xoff += Random(100); yoff += Random(100);
                InitField(xoff , yoff);
            }
        }

        //xoff += Random(100); yoff += Random(100);
        //InitField(xoff , yoff);

        window.clear(sf::Color(100 , 100 , 100));

        MarchingSquare(window);
        DrawField(window);

        window.display();
    }

    return 0;
}
