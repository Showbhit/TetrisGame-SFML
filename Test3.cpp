#include <SFML/Graphics.hpp>
#include <time.h>
using namespace sf;

const int M = 25, N = 20;

int field[M][N] = { 0 };

class Point {
public:
    int x, y;
};

Point a[4], b[4];

Point figures[7][4] =
{
     {{1,0},{1,1},{1,2},{1,3}},// I
 {{0,1},{1,1},{1,2},{2,2}},
 {{0,1},{1,1},{2,1},{1,2}},
 {{0,0},{1,0},{1,1},{1,2}},
 {{0,1},{1,1},{0,2},{1,2}},
 {{0,2},{1,1},{1,2},{2,1}},
 {{1,0},{1,1},{1,2},{2,2}} // z
};

bool check()
{
    for (int i = 0;i < 4;i++)
        if (a[i].x < 0 || a[i].x >= N ||  a[i].y >= M || a[i].y == 0)
            return 0;
        else if (field[a[i].y][a[i].x]) 
            return 0;

    return 1;
};


int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(412, 549), "Tetris!");

    Texture t1, t2, t3;
    t1.loadFromFile("tiles.png");
    t2.loadFromFile("back.jpg");
    t3.loadFromFile("fram.png");

    Sprite tile(t1), background(t2), frame(t3);

    int dx = 0, colorNum = 0;
    bool rotate = 0; 
    float timer = 0, delay = 0.4;

    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed)
                window.close();
            if (e.KeyPressed && e.key.code == Keyboard::Escape)
   				window.close();

            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Up) 
                    rotate = true;
                else if (e.key.code == Keyboard::Left) 
                    dx = -1;
                else if (e.key.code == Keyboard::Right) 
                    dx = 1;
        }

        if (Keyboard::isKeyPressed(Keyboard::Down)) 
            delay = 0.05;

        //// <- Move -> ///
        for (int i = 0;i < 4;i++) { 
            b[i] = a[i]; 
            a[i].x += dx; 
        }
        if (!check()) 
            for (int i = 0;i < 4;i++) 
                a[i] = b[i];

        //////Rotate//////
        if (rotate)
        {
            Point p = a[1]; //center of rotation
            for (int i = 0;i < 4;i++)
            {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
            if (!check()) 
                for (int i = 0;i < 4;i++) 
                    a[i] = b[i];
        }

        ///////Tick//////
        if (timer > delay)
        {
            for (int i = 0;i < 4;i++) { 
                b[i] = a[i]; 
                a[i].y += 1; 
            }

            if (!check())
            {
                for (int i = 0;i < 4;i++) 
                    field[b[i].y][b[i].x] = colorNum;

                colorNum = 1 + rand() % 7;
                int n = rand() % 7;
                
                for (int i = 0;i < 4;i++)
                {
                    a[i].x = figures[n][i].x;
                    a[i].y = figures[n][i].y;
                }
            }

            timer = 0;
        }

        ///////check lines//////////
        int k = M - 1;
        for (int i = M - 1;i > 0;i--)
        {
            int count = 0;
            for (int j = 0;j < N;j++)
            {
                if (field[i][j]) 
                    count++;
                field[k][j] = field[i][j];
            }
            if (count < N) 
                k--;
        }

        dx = 0; rotate = 0; delay = 0.5;

        //draw
        window.clear(Color::White);
        window.draw(background);

        for (int i = 0;i < M;i++)
            for (int j = 0;j < N;j++)
            {
                if (field[i][j] == 0) 
                    continue;
                tile.setTextureRect( IntRect(field[i][j] * 18, 0, 18, 18));
                tile.setPosition( j * 18, i * 18);
            //    tile.move(28, 32); //offset
                window.draw(tile);
            }

        for (int i = 0;i < 4;i++)
        {
            tile.setTextureRect( IntRect(colorNum * 18, 0, 18, 18));
            tile.setPosition(a[i].x * 18, a[i].y * 18);
         //   tile.move(28, 32); //offset
            window.draw(tile);
        }

        window.draw(frame);
        window.display();
    }

    return 0;
}
