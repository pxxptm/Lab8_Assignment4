#include <iostream>
#include <windows.h>
#include <time.h>
#include <vector>

using namespace std;

#define screen_x 80
#define screen_y 25

HANDLE rHnd;
HANDLE wHnd;
DWORD fdwMode;
COORD bufferSize = { screen_x,screen_y };
SMALL_RECT windowSize = { 0,0,screen_x-1,screen_y-1 };
int shiploc[2]={0,0};
int p=0;
vector <int> starx,stary;

void setcursor(bool visible)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = 20;
    SetConsoleCursorInfo(console,&lpCursor);
}

int setConsole(int x, int y)
{
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
    SetConsoleScreenBufferSize(wHnd, bufferSize);
    setcursor(0);
    return 0;
}
int setMode()
{
    rHnd = GetStdHandle(STD_INPUT_HANDLE);
    fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT |
    ENABLE_MOUSE_INPUT;
    SetConsoleMode(rHnd, fdwMode);
    return 0;
}

void setcolor(int fg,int bg)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, bg*16+fg);
}

void gotoxy(int x, int y)
{
    COORD c = { x, y };
    SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE) , c);
}
void draw_ship(int x,int y,int color)
{
    gotoxy(x,y);
    setcolor(color,0);
    cout << "<-0->";
    setcursor(0);
}

void erase_ship(int x,int y)
{

    setcolor(0,0);
    COORD c={x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
    cout << "     ";
    setcursor(0);
}

void showscore()
{
    gotoxy(0,0);
    setcolor(7,0);
    cout << "pt : " << p;
    setcursor(0);
}

void draw_star(int x,int y)
{
    gotoxy(x,y);
    setcolor(7,0);
    cout << "*";
    setcursor(0);
}

char cursor(int x, int y)
{
    HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    char buf[2];
    COORD c = {x,y};
    DWORD num_read;

    if(!ReadConsoleOutputCharacter(hStd,(LPTSTR)buf,1,c,(LPDWORD)&num_read) )   return '\0';
    else return buf[0];

}

main()
{
    bool play = true;
    DWORD numEvents = 0;
    DWORD numEventsRead = 0;
    setConsole(screen_x, screen_y);
    setMode();
    srand(time(NULL));
    setcursor(0);
    int color=7;
    int posx=0,posy=0;
    int sx,sy;

    showscore();

    for(int j=0;j<20;j++)
    {
        sx=abs(rand())%80;
        sy=abs(rand())%25;
        if(sy<1) sy=1;
        if(sx==79&&sy==24) sy=23;
        draw_star(sx,sy);
        starx.push_back(sx);
        stary.push_back(sy);
    }

    while (play)
    {
        GetNumberOfConsoleInputEvents(rHnd, &numEvents);
        if (numEvents != 0)
        {
            INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
            ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
            for (DWORD i = 0; i < numEventsRead; ++i)
            {
                if (eventBuffer[i].EventType == KEY_EVENT&&eventBuffer[i].Event.KeyEvent.bKeyDown == true )
                {
                    if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)  play=false;
                    if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar=='c')
                    {
                        color=abs(rand())%16;
                        if(color==0) color=7;
                        setcolor(color,0);
                        draw_ship(posx,posy,color);
                    }
                    //printf("press : %c\n", eventBuffer[i].Event.KeyEvent.uChar.AsciiChar);
                }
                else if (eventBuffer[i].EventType == MOUSE_EVENT)
                {

                    if (eventBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
                    {
                        color=abs(rand())%16;
                        if(color==0) color=7;
                        draw_ship(posx,posy,color);
                        //printf("left click\n");
                    }
                    else if (eventBuffer[i].Event.MouseEvent.dwEventFlags & MOUSE_MOVED)
                    {
                        erase_ship(shiploc[0],shiploc[1]);
                        posx = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
                        posy = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
                        if(posx>75)
                        {
                            posx=75;
                            if(posy==24) posy=23;
                        }
                        if(posy<1) posy=1;
                        draw_ship(posx,posy,color);
                        shiploc[0]=posx;
                        shiploc[1]=posy;
                        //printf("mouse position : (%d,%d)\n",posx, posy);

                        for(int j=0;j<20;j++)
                        {
                            if(posx==starx[j]&&posy==stary[j])
                            {
                                p++;
                                showscore();
                                starx[j]=abs(rand())%80;
                                starx[j]=abs(rand())%25;
                                if(stary[j]<1) stary[j]=1;
                                if(starx[j]==79&&stary[j]==24) stary[j]=23;
                                draw_star(starx[j],stary[j]);
                            }
                            if(posx+1==starx[j]&&posy==stary[j])
                            {
                                p++;
                                showscore();
                                starx[j]=abs(rand())%80;
                                starx[j]=abs(rand())%25;
                                if(stary[j]<1) stary[j]=1;
                                if(starx[j]==79&&stary[j]==24) stary[j]=23;
                                draw_star(starx[j],stary[j]);
                            }
                            if(posx+2==starx[j]&&posy==stary[j])
                            {
                                p++;
                                showscore();
                                starx[j]=abs(rand())%80;
                                starx[j]=abs(rand())%25;
                                if(stary[j]<1) stary[j]=1;
                                if(starx[j]==79&&stary[j]==24) stary[j]=23;
                                draw_star(starx[j],stary[j]);
                            }
                            if(posx+3==starx[j]&&posy==stary[j])
                            {
                                p++;
                                showscore();
                                starx[j]=abs(rand())%80;
                                starx[j]=abs(rand())%25;
                                if(stary[j]<1) stary[j]=1;
                                if(starx[j]==79&&stary[j]==24) stary[j]=23;
                                draw_star(starx[j],stary[j]);
                            }
                            if(posx+4==starx[j]&&posy==stary[j])
                            {
                                p++;
                                showscore();
                                starx[j]=abs(rand())%80;
                                starx[j]=abs(rand())%25;
                                if(stary[j]<1) stary[j]=1;
                                if(starx[j]==79&&stary[j]==24) stary[j]=23;
                                draw_star(starx[j],stary[j]);
                            }
                        }
                    }
                }
            }
            delete[] eventBuffer;
        }
        Sleep(100);
        if(p==10||play==false)
        {
            system("CLS");
            gotoxy(28,11);
            cout << "total point : " << p;
            gotoxy(28,13);
            cout << "press ESC to exit";
            gotoxy(0,24);
            break;
        }
    }
    return 0;
}
