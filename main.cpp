/*

Written by Orçun Altınsoy on July 2014

*/

#include "all.h"

int main()
{
    srand(time(NULL));

    SetConsoleWindowSize(ROW_SIZE , COLUMN_SIZE);

    DrawBattleArea();

    WriteMenu("ARROWS = move , BPT = put , D = delete , DELETE = reset all , R = random , S = save , L = load , ENTER = start , ESC = exit");

    SetCursorPosition(starting_position);

    int rs = main_box_lower_right_position.X - main_box_upper_left_position.X - 1;
    int cs = main_box_lower_right_position.Y - main_box_upper_left_position.Y - 1;
    // Allocate memory for 'object' array...
    object = (OBJECT**)malloc(sizeof(OBJECT*) * cs);
    for(int i = 0 ; i < cs ; i++) object[i] = (OBJECT*)malloc(sizeof(OBJECT) * rs);
    // Fill 'object' array with spaces...
    for(int i = 0 ; i < cs ; i++) for(int j = 0 ; j < rs ; j++) object[i][j] = NOTHING;
    // Allocate memory for 'distance' array...
    distance = (int**)malloc(sizeof(int*) * cs);
    for(int i = 0 ; i < cs ; i++) distance[i] = (int*)malloc(sizeof(int) * rs);
    // Allocate memory for 'path' array...
    path = (int**)malloc(sizeof(int*) * cs);
    for(int i = 0 ; i < cs ; i++) path[i] = (int*)malloc(sizeof(int) * rs);

    SetObject(PATHFINDER , starting_position);
    SetObject(TARGET , ending_position);

    int c;
    for(;;)
    {
        c = getch();

        if(c == SPECIAL_KEY)
        {
            c = getch();
            if(c == UP_KEY)     { if(!IsCurrentCursorNearNorthWall()) MoveCursor(UP);    continue; }
            if(c == LEFT_KEY)   { if(!IsCurrentCursorNearWestWall())  MoveCursor(LEFT);  continue; }
            if(c == DOWN_KEY)   { if(!IsCurrentCursorNearSouthWall()) MoveCursor(DOWN);  continue; }
            if(c == RIGHT_KEY)  { if(!IsCurrentCursorNearEastWall())  MoveCursor(RIGHT); continue; }
            if(c == DELETE_KEY) { ClearMainArea(); move_number = 0; SetObject(PATHFINDER , starting_position); SetObject(TARGET , ending_position); continue; }
        }
        if(c == 'b' || c == 'B')
        {
            OBJECT o = GetCurrentObject();
            if     (o == PATHFINDER)  WriteInformation("You can't put block onto pathfinder!");
            else if(o == TARGET)      WriteInformation("You can't put block onto target!");
            else                      SetCurrentObject(BLOCK);
            continue;
        }
        if(c == 'p' || c == 'P')
        {
            OBJECT o = GetCurrentObject();
            if(o == TARGET) WriteInformation("You can't put pathfinder onto target!");
            else
            {
                COORD c;
                for(c.X = starting_position.X ; c.X <= ending_position.X ; c.X++) for(c.Y = starting_position.Y ; c.Y <= ending_position.Y ; c.Y++) if(GetObject(c) == PATHFINDER) SetObject(NOTHING , c);
                SetCurrentObject(PATHFINDER);
            }
            continue;
        }
        if(c == 't' || c == 'T')
        {
            OBJECT o = GetCurrentObject();
            if(o == PATHFINDER) WriteInformation("You can't put target onto pathfinder!");
            else
            {
                COORD c;
                for(c.X = starting_position.X ; c.X <= ending_position.X ; c.X++) for(c.Y = starting_position.Y ; c.Y <= ending_position.Y ; c.Y++) if(GetObject(c) == TARGET) SetObject(NOTHING , c);
                SetCurrentObject(TARGET);
            }
            continue;
        }
        if(c == 'd' || c == 'D')
        {
            OBJECT o = GetCurrentObject();
            if     (o == PATHFINDER) WriteInformation("You can't delete the pathfinder!");
            else if(o == TARGET)     WriteInformation("You can't delete the target!");
            else                     SetCurrentObject(NOTHING);
            continue;
        }
        if(c == 'r' || c == 'R') { Random(); continue; }
        if(c == 'l' || c == 'L') { Load(); continue; }
        if(c == 's' || c == 'S') { Save(); continue; }
        if(c == ENTER_KEY)
        {
            COORD c;
            bool f = false;
            for(c.X = starting_position.X ; c.X <= ending_position.X ; c.X++) for(c.Y = starting_position.Y ; c.Y <= ending_position.Y ; c.Y++) if(GetObject(c) == TARGET) f = true;
            if(!f) { WriteInformation("No target found!"); continue; }
            if(Explore()) Start();
            else WriteInformation("No solution!");
            continue;
        }
        if(c == ESC_KEY) return 0;
    }
}
