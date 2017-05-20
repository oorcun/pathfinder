
/*

This header file contains algorithm functions.

*/

#ifndef _ALGRITHM_H_
#define _ALGRITHM_H_

// Starts pathfinder.
void Start()
{
    COORD c , d;
    int rs = main_box_lower_right_position.X - main_box_upper_left_position.X - 1;
    int cs = main_box_lower_right_position.Y - main_box_upper_left_position.Y - 1;
    int a , b;
    bool f , t;
    char s[1024];

    // Fill 'path' array with INFINITY value...
    for(int i = 0 ; i < cs ; i++) for(int j = 0 ; j < rs ; j++) path[i][j] = INFINITY;
    // Backtrack 'distance' array and find path..
    for(c.X = starting_position.X ; c.X <= ending_position.X ; c.X++) for(c.Y = starting_position.Y ; c.Y <= ending_position.Y ; c.Y++) if(GetObject(c) == TARGET)
    {
        d = CursorToCoordinate(c);
        path[d.X][d.Y] = distance[d.X][d.Y];
        a = d.X;
        b = d.Y;
    }
    do
    {
        f = false;
        for(int i = 0 ; i < cs ; i++) for(int j = 0 ; j < rs ; j++) if(!f && a == i && b == j)
        {
            if(i > 0 && distance[i - 1][j] == distance[i][j] - 1)
            {
                path[i - 1][j] = distance[i - 1][j];
                a = i - 1;
            }
            else if(j > 0 && distance[i][j - 1] == distance[i][j] - 1)
            {
                path[i][j - 1] = distance[i][j - 1];
                b = j - 1;
            }
            else if(i < cs - 1 && distance[i + 1][j] == distance[i][j] - 1)
            {
                path[i + 1][j] = distance[i + 1][j];
                a = i + 1;
            }
            else if(j < rs - 1 && distance[i][j + 1] == distance[i][j] - 1)
            {
                path[i][j + 1] = distance[i][j + 1];
                b = j + 1;
            }
            f = true;
        }
    } while(path[a][b]);

    WriteMenu("ENTER = stop , S = increase speed , D = decrease speed");

    for(;;)
    {
        f = false;
        for(c.X = starting_position.X ; c.X <= ending_position.X ; c.X++) for(c.Y = starting_position.Y ; c.Y <= ending_position.Y ; c.Y++) if(!f && GetObject(c) == PATHFINDER)
        {
            d = CursorToCoordinate(c);
            if     (d.X > 0 && path[d.X - 1][d.Y] == path[d.X][d.Y] + 1)      MovePathfinder(UP);
            else if(d.Y > 0 && path[d.X][d.Y - 1] == path[d.X][d.Y] + 1)      MovePathfinder(LEFT);
            else if(d.X < cs - 1 && path[d.X + 1][d.Y] == path[d.X][d.Y] + 1) MovePathfinder(DOWN);
            else if(d.Y < rs - 1 && path[d.X][d.Y + 1] == path[d.X][d.Y] + 1) MovePathfinder(RIGHT);
            f = true;
        }

        move_number++;

        if(kbhit())
        {
            int i = getch();
            if     (i == 's' || i == 'S') { if(speed < 10) speed++; }
            else if(i == 'd' || i == 'D') { if(speed > 1)  speed--; }
            else if(i == ENTER_KEY) break;
        }

        sprintf(s , "Speed = %d moves/sec , Moves = %d" , speed , move_number);
        WriteInformation(s);

        Sleep(1000 / speed);

        t = false;
        for(c.X = starting_position.X ; c.X <= ending_position.X ; c.X++) for(c.Y = starting_position.Y ; c.Y <= ending_position.Y ; c.Y++) if(GetObject(c) == TARGET) t = true;
        if(!t)
        {
            sprintf(s , "Shortest path is %d moves long." , move_number);
            WriteInformation(s);
            break;
        }
    }

    WriteMenu("ARROWS = move , BPT = put , D = delete , DELETE = reset all , R = random , S = save , L = load , ENTER = start , ESC = exit");
}

// Moves pathfinder to direction specified by parameter.
void MovePathfinder(DIRECTION direction)
{
    COORD cp , cc , c;

    for(c.X = starting_position.X ; c.X <= ending_position.X ; c.X++) for(c.Y = starting_position.Y ; c.Y <= ending_position.Y ; c.Y++) if(GetObject(c) == PATHFINDER)
    {
        cp = c;
        if     (direction == UP)    { cc.X = cp.X; cc.Y = cp.Y - 1; }
        else if(direction == LEFT)  { cc.X = cp.X - 1; cc.Y = cp.Y; }
        else if(direction == DOWN)  { cc.X = cp.X; cc.Y = cp.Y + 1; }
        else if(direction == RIGHT) { cc.X = cp.X + 1; cc.Y = cp.Y; }
        SetObject(NOTHING , cp);
        SetObject(PATHFINDER , cc);
        return;
    }
}

// Creates random labyrinth.
void Random()
{
    COORD c , d = GetCursorPosition();
    char s[1024] , ss[1024];

    do
    {
        strcpy(s , "Enter block infrequency factor (1 < digit): ");
        WriteInformation(s);
        c = info_position;
        c.X += strlen(s);
        SetCursorPosition(c);
        gets(ss);
    } while(!isdigit(ss[0]) || ss[0] < '2');

    block_infrequency_factor = ss[0] - 48;

    for(c.X = starting_position.X ; c.X <= ending_position.X ; c.X++) for(c.Y = starting_position.Y ; c.Y <= ending_position.Y ; c.Y++)
    {
        if(rand() % block_infrequency_factor) SetObject(NOTHING , c);
        else                                  SetObject(BLOCK , c);
    }

    c.X = starting_position.X + rand() % (ending_position.X - starting_position.X);
    c.Y = starting_position.Y + rand() % (ending_position.Y - starting_position.Y);
    SetObject(PATHFINDER , c);

    do
    {
        c.X = starting_position.X + rand() % (ending_position.X - starting_position.X);
        c.Y = starting_position.Y + rand() % (ending_position.Y - starting_position.Y);
    } while(GetObject(c) == PATHFINDER);
    SetObject(TARGET , c);

    SetCursorPosition(d);
}

// Calculates Manhattan distance between two points.
int Distance(COORD distance1 , COORD distance2)
{
    return abs(distance1.Y - distance2.Y) + abs(distance1.X - distance2.X);
}

// All neighbours' distance values become 1 more of distance value specified by c coordinate if its current value is greater than aforementioned c coordinate's value.
// Coordinates are relative to main area.
void CalculateNeighbours(COORD coordinate)
{
    COORD c = CursorToCoordinate(coordinate);
    int k = distance[c.X][c.Y];

    if(!IsCursorNearNorthWall(coordinate)) if(distance[c.X - 1][c.Y] > k) distance[c.X - 1][c.Y] = k + 1;
    if(!IsCursorNearWestWall(coordinate))  if(distance[c.X][c.Y - 1] > k) distance[c.X][c.Y - 1] = k + 1;
    if(!IsCursorNearSouthWall(coordinate)) if(distance[c.X + 1][c.Y] > k) distance[c.X + 1][c.Y] = k + 1;
    if(!IsCursorNearEastWall(coordinate))  if(distance[c.X][c.Y + 1] > k) distance[c.X][c.Y + 1] = k + 1;
}

// Explore labyrinth from pathfinder until target is found.
bool Explore()
{
    COORD c , cp , ct , d , dt;
    int k;
    int rs = main_box_lower_right_position.X - main_box_upper_left_position.X - 1;
    int cs = main_box_lower_right_position.Y - main_box_upper_left_position.Y - 1;

    // Fill 'distance' array with infinity values...
    for(int i = 0 ; i < cs ; i++) for(int j = 0 ; j < rs ; j++) distance[i][j] = INFINITY;

    // Locate pathfinder, target and blocks..
    OBJECT o;
    for(c.X = starting_position.X ; c.X <= ending_position.X ; c.X++) for(c.Y = starting_position.Y ; c.Y <= ending_position.Y ; c.Y++)
    {
        o = GetObject(c);
        if     (o == PATHFINDER) cp = c;
        else if(o == TARGET)     ct = c;
        else if(o == BLOCK)
        {
            d = CursorToCoordinate(c);
            distance[d.X][d.Y] = -1;
        }
    }

    dt = CursorToCoordinate(ct);

    // Calculate all distances until target is found..
    d = CursorToCoordinate(cp);
    distance[d.X][d.Y] = 0;
    CalculateNeighbours(cp);
    k = 1;
    do
    {
        no_solution = true;
        for(c.X = 0 ; c.X < cs ; c.X++) for(c.Y = 0 ; c.Y < rs ; c.Y++) if(distance[c.X][c.Y] == k)
        {
            d = CoordinateToCursor(c);
            CalculateNeighbours(d);
            no_solution = false;
        }
        k++;
    } while(distance[dt.X][dt.Y] == INFINITY && no_solution == false);

    return !no_solution;
}

#endif
