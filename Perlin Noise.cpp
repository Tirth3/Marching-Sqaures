#include<SFML/Graphics.hpp>
#include<iostream>
#include<math.h>



float Random(float x) //range : [min, max]
{

    float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / x));
    return r;
}

float func(float a){
    float e = 2.71828182846;
    return sin(a);
    //return 1 / (1 + pow(e , -a));
    //return tan(a);
}


sf::Vector2f Normalize(sf::Vector2f v){
    float len = sqrt(v.x * v.x + v.y * v.y);
    v.x /= len;
    v.y /= len;
    return v;
}


sf::Vector2f RandomGradient(int ix, int iy) {
    // No precomputed gradients mean this works for any number of grid coordinates
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2; // rotation width
    unsigned a = ix, b = iy;
    a *= 3284157443; b ^= (a << s) | (a >> (w - s) );
    b *= 1911520717; a ^= (b << s) | (b >> (w - s));
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
    sf::Vector2f v;
    v.x = cos(random); v.y = sin(random);
    return v;
}


float DotGradient(sf::Vector2f vec1 , sf::Vector2f vec2){

    sf::Vector2f vGradient = RandomGradient((int)vec1.x , (int)vec1.y);

    float dx = vec1.x - vec2.x;
    float dy = vec1.y - vec2.y;

    return vGradient.x * dx + vGradient.y * dy;

}

float DotGradient(float x1 , float y1 , float x2 , float y2){

    sf::Vector2f vGradient = RandomGradient((int)x1 , (int)y1);

    float dx = x1 - x2;
    float dy = y1 - y2;

    return vGradient.x * dx + vGradient.y * dy;

}

float Interpolate(float a , float b , float x){
    //a * (1 - x) + b * x;
    float ft = x * 3.1415927;
	float f = (1 - cos(ft)) * 0.5;
	return  a*(1-f) + b*f;

}



float InterpolatedNoise(float x ,float y){
    int x0 = floor(x);
    int y0 = floor(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float sx = func(x);
    float sy = func(y);

    float n0 , n1 , i1 , i2 , fvalue;

    n0 = DotGradient((float)x0, (float)y0, x, y);
    n1 = DotGradient((float)x1, (float)y0, x, y);
    i1 = Interpolate(n0, n1, sx);

    n0 = DotGradient((float)x0, (float)y1, x, y);
    n1 = DotGradient((float)x1, (float)y1, x, y);
    i2 = Interpolate(n0, n1, sx);

    fvalue = Interpolate(i1 , i2 , sy);
    return fvalue;
}

float PerlinNoise2D(float x , float y  , float nOctave){
    float fTotal = 0;
    float n = nOctave - 1;
    float fPersistance = 0.005f;

    for(int i = 0 ; i <= n ; i++){
        float fFreq = pow(2 , i);
        float fAmp = pow(fPersistance , i);
        fTotal = fTotal + InterpolatedNoise(x * fFreq , y * fFreq) * fAmp;
    }
    return fTotal;
}

/*void DrawText(sf::RenderWindow &win , sf::Vector2f pos , float num ,int nsize , sf::Color col = sf::Color::White){
    sf::Text tex;

    tex.setFont(font);
    tex.setCharacterSize(nsize);
    tex.setString(to_string(num));
    tex.setFillColor(col);
    tex.setPosition(pos);

    win.draw(tex);
}

void DrawText(sf::RenderWindow &win , sf::Vector2f pos , int num ,int nsize , sf::Color col = sf::Color::White){
    sf::Text tex;

    tex.setFont(font);
    tex.setCharacterSize(nsize);
    tex.setString(to_string(num));
    tex.setFillColor(col);
    tex.setPosition(pos);

    win.draw(tex);
}

void DrawLine(sf::RenderWindow &win , sf::Vector2f sPos , sf::Vector2f ePos , sf::Color col = sf::Color(255 , 255, 255 , 100)){
    sf::Vertex line[]={
      sf::Vertex(sPos , col),
      sf::Vertex(ePos, col)
    };
    win.draw(line , 2 , sf::Lines);

}*/

class Noise{
public:
    float x , y;
    int nOctave = 8;
public:
    Noise(float x_ , float y_){
        x = x_;
        y = y_;
    }

    float CalcNoise(){
        float n1 = PerlinNoise2D(x , y , nOctave);
        float n2 = PerlinNoise2D(x + Random(10) , y + Random(10) , nOctave);
        return n1 + n2;
    }

};



/*

using namespace std;

const int sWidth = 300;
const int sHeight = 300;
const int nOctave = 8;

sf::Font font;



int main(){
    srand(time(NULL));

    sf::RenderWindow window(sf::VideoMode(sWidth , sHeight) , "Perlin Noise");
    if(!font.loadFromFile("Arialn.ttf")){
        cout<<"UNABLE TO LOAD FONT"<<"\n";
    }
    float xoff = 10 , yoff = 10;
    int nCount = sWidth * sHeight;
    //vector<float>vValues;
    float vValues[nCount];

    for(int x = 1 ;x < sWidth ; x++){
        for(int y = 0 ; y < sHeight ; y++){
            int index = y * sWidth + x;
            float n1 = PerlinNoise2D(xoff , yoff);
            xoff += 0.5;  yoff+=0.5;
            float n2 = PerlinNoise2D(xoff , yoff);
            vValues[index] = SumNoise(n1 , n2);//PerlinNoise2D(xoff , yoff);
            //vValues.push_back(PerlinNoise2D(xoff , yoff));

        }

    }

    while(window.isOpen()){
        sf::Event evnt;
        while(window.pollEvent(evnt)){
            if(evnt.type == sf::Event::Closed)
                window.close();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
        }

        window.clear();
        for(int x = 0 ; x < sWidth ; x++){
            for(int y = 0 ; y < sHeight ; y++){
                int index = y * sWidth + x;
                float val = vValues[index];

                sf::Vertex point(sf::Vector2f(x, y), sf::Color(255 * val , 255 * val , 255 * val));
                window.draw(&point, 1, sf::Points);
            }
        }
        window.display();
    }

    return 0;
}*/
