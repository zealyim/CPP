// CpSc 2150 
// G. Monagan
#include <string>
#include <fstream>
#include <cstdlib>

// now debugging
#define NDEBUG

// declaration of the struct Cell  which consists of two values
#include "Cell.h"

// from the STL
#include <stack>
using std::stack;

#include <iostream>
using std::cout;
using std::endl;
using std::cin;

// size of maze is NxN
// N is a global CONSTANT accessed by several routines
const int N = 10;

// these are the values expected in the input file
// note that there is no error checking.
const char ENTRY = '*';     
const char GOAL = '%';
const char FREE = '.';
const char BLOCKED = 'X';

// Read into the maze data from a file name obtained from standard input
// there is no error checking for corrupt data
// precondition:
//    the data in the file consists exclusively of character values
//  ENTRY = '*';     
//  GOAL = '%';
//  FREE = '.';
// BLOCKED = 'X'  (upper case X)
//    there is only one ENTRY point  and one GOAL point
//    there is exactly NxN non-blank characters to be read in
//    the array maze has been declared to be at least NxN
//    the size of the maze N is a global constant
// postcondition:
//    the array maze has the characters read in
//    the values of the array were read row by row
void read_maze(char maze[][N]);
   
// convert maze[i][j] with BLOCKED to m[i][j] = false
// and maze[i][j] with ENTRY, EXIT, FREE convert to m[i][j] = true
// precondition
//     maze is full of characters ENTRY, EXIT, FREE, BLOCKED
// postcondition
//    m is a boolean array of what is free (true) and what is blocked (false)
// if there is more than one goal or more than one entry, or if there is no goal or no entry
// exit program abruptly
void convert(const char maze[][N], 
              bool m[][N], int N, Cell &entry, Cell &goal);
              
// output the NxN array as characters .... this routines basically prints out
// the input that was read from a file
// expected characters in the output are the ENTRY point, the BLOCKED characters, ... etc
// precondition:
//    N is a global variable
void dump_matrix(const char maze[][N]);

// output to standard output a line of characters with 
// the first corner as + and the last corner as +
// and with n dashes inbetween
void print_line(int n);

// prints the stack by popping each of its Cells
// postcondition:
//   the stack st is empty
void pop_and_print_stack(stack<Cell> st);

// very primitive,  way of printing out a solution to standard output
// note that I waste lots of memory allocating another array
// precondition:
//   N, the side of the grid, has been globally defined
//   when fancy is true, it means that we want a 2d array printed
//   when fancy is false, it means that we output the stack as a list of coordinates
//   a stack (that is possibly empty) needs to be passed and this stack has the solutin
//  postcondition:
//    all the elements ofthe stack are popped and the stack is empty at the end of
//    print_solution
void print_solution(stack<Cell>& st, bool fancy=true);
   
// find a path from (start_i,start_j) to (goal_i,goal_j) and output the solution
void path(int start_i, int start_j, // coordinates of entry point
          int goal_i, int goal_j,   // coordinates of exit point
          bool maze[][N]);  // the maze with the grid  
   
void read_maze(char maze[][N]) { 
   std::string file_name;
   cout << "The size of the maze is " << N << endl;
   cout << "File with the characters of the maze > ";
   cin >> file_name;
   std::ifstream fin;
   fin.open(file_name.c_str());
   // continue trying to open a file if the open failed
   while (!fin) {
      fin.clear();
      cout << "cout not open file \"" << file_name << "\"... try again: >";
      cin >> file_name;
      fin.open(file_name.c_str());
   } 
   for (int i=0; i< N; i++) 
      for (int j=0; j< N; j++) 
         fin >> maze[i][j];
   fin.close();
}
// convert maze[i][j] with the character BLOCKED to m[i][j] = false
// and convert the maze[i][j] with ENTRY, EXIT, FREE convert to m[i][j] = true
// m stands for the 'marked array'
// precondition
//     maze is full of characters ENTRY, EXIT, FREE, BLOCKED
// postcondition
//    m is a boolean array of what is free (true) and what is blocked (false)
// if there is more than one goal or more than one entry, or if there is no goal or no entry
// exit program abruptly
void convert(const char maze[][N], 
              bool m[][N], int N, Cell &entry, Cell &goal) {
   int num_goal = 0;
   int num_entry = 0;
   for (int i=0; i < N; i++)
      for (int j=0; j < N; j++) {
         if (maze[i][j] == BLOCKED)
            m[i][j] = false;
         else {
            m[i][j] = true;
            if (maze[i][j] == ENTRY) {
               entry.i = i;
               entry.j = j;
               num_entry++;
            }
            else if (maze[i][j] == GOAL) {
               goal.i = i;
               goal.j = j;
               num_goal++;
            }
         }
      } // for j
   if ((num_entry != 1) && (num_goal != 1)) {
      cout << "entry or goal is missing or there are too many.... bye\n";
      exit(1);
   }
}

void dump_matrix(const char maze[][N]) {
   cout << endl;
   for (int i=0; i<N; i++) {
      for (int j=0; j<N; j++) 
         cout << maze[i][j] << " ";
      cout << endl;
   }
   cout << endl;
}

void print_line(int n) {
   cout << "+";
   for (int i=0; i<n; i++) 
      cout << "- "; 
   cout << "+\n";
}

void pop_and_print_stack(stack<Cell> st) {
   Cell x;
   while (!st.empty()) {
      x = st.top();
      st.pop();
      cout << x;
   }
   cout << endl;
}
   
void print_solution(stack<Cell>& st, bool fancy) {
   cout << endl;
   // print the path  by getting the cells from the stack
   if (!fancy) {
      pop_and_print_stack(st);
      return;
   }
   // the boolean variable fancy is true meaning tha
   // initialize a board of characters to blanks
   char t[N][N];
   cout << endl;
    for (int i=0; i<N; i++) 
      for (int j=0; j<N; j++) 
         t[i][j] = ' ';
   
   // mark the output path by popping the stack
   while (!st.empty()) {
      Cell a = st.top();
      st.pop();
      t[a.i][a.j] = 'o';
   }
   // print a top line
   print_line(N-1);
   // print out the array of characters
   for (int i=0; i<N; i++) {
      cout << "|";
      for (int j=0; j<N-1; j++) 
         cout << t[i][j] << ' ';
      cout << "|" << endl;
   }
   // print the bottom line
   print_line(N-1);
}
bool inBound(int i, int j)
{
   if( i >= 0 && i <= N && j >= 0 && j <= N)
      return true;
   else
      return false;
}
// find a path from (start_i,start_j) to (goal_i,goal_j) and output the solution
// this is the function that needs to be written
void path(int start_i, int start_j, // coordinates of entry point
          int goal_i, int goal_j,   // coordinates of exit point
          bool maze[][N]) {         // maze with true and false values
   std::stack<Cell> st;
   /*******************************************************************************************************/
   int cur_i, cur_j;
   cur_i = start_i;
   cur_j = start_j;
   bool visited[N][N];
   visited[cur_i][cur_j] = true;
   cout << "Start: (" << cur_i<<":"<<cur_j<<") Goal: ("<<goal_i<<":"<<goal_j<<")\n";
   Cell temp;
   temp.i = cur_i;
   temp.j = cur_j;
   st.push(temp);
      
   while(!(cur_i == goal_i && cur_j == goal_j) || !st.empty())
   {
      if(inBound(cur_i+1,cur_j) && visited[cur_i+1][cur_j] == false && maze[cur_i+1][cur_j] != false )
      {
         cur_i++;
         temp.i = cur_i;
         temp.j = cur_j;
         st.push(temp);
         visited[cur_i][cur_j] = true;
         //cout<<"tag1\n";
      }
      else if(inBound(cur_i,cur_j+1) && visited[cur_i][cur_j+1] == false && maze[cur_i][cur_j+1] != false )
      {
         cur_j++;
         temp.i = cur_i;
         temp.j = cur_j;
         st.push(temp);
         visited[cur_i][cur_j] = true;
         //cout<<"tag2\n";
      }
      else if(inBound(cur_i-1,cur_j) && visited[cur_i-1][cur_j] == false && maze[cur_i-1][cur_j] != false )
      {
         cur_i--;
         temp.i = cur_i;
         temp.j = cur_j;
         st.push(temp);
         visited[cur_i][cur_j] = true;
         //cout<<"tag3\n";
      }
      else if(inBound(cur_i,cur_j-1) && visited[cur_i][cur_j-1] == false && maze[cur_i][cur_j-1] != false )
      {
         cur_j--;
         temp.i = cur_i;
         temp.j = cur_j;
         st.push(temp);
         visited[cur_i][cur_j] = true;
         //cout<<"tag4\n";
      }
      else
      {
         visited[cur_i][cur_j] = true;
         cur_i = st.top().i;
         cur_j = st.top().j;
         st.pop();
         //cout<<"tag5\n";
      }
      if((cur_i == goal_i && cur_j == goal_j) || st.empty())
         break;
   }
   /*******************************************************************************************************/
   
   if (st.empty())
      cout << "SORRY: no answer\n";
   else 
      print_solution(st);
}


int main() {
   char maze[N][N];
   read_maze(maze);   // read from a file of characters the maze
   dump_matrix(maze);  // output what was read
      
   // convert the ENTRY point, EXIT point and FREE to true
   // and the BLOCKED to false in the array m which is boolean
   bool bool_maze[N][N];    // internal information  of free and blocked cells
   Cell entry, goal;
   convert(maze, bool_maze, N, entry, goal);
   
   // solve the maze and print out the maze
   cout << "One path to the goal ...\n";
   path(entry.i, entry.j, goal.i, goal.j, bool_maze);
   
   // system("PAUSE");
   return 0;
}

