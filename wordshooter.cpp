//============================================================================
// Name        : cookie-crush.cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Cookie  Crush...
//============================================================================
#ifndef WORD_SHOOTER_CPP
#define WORD_SHOOTER_CPP

//#include <GL/gl.h>
//#include <GL/glut.h>
#include <iostream>
#include<string>
#include<cmath>
#include<fstream>
#include "util.h"
using namespace std;
#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 10
int S_flag = 0;
int swidth = 420;
int shooter_width = 420;
int sheight = 10;
int alphabet_iterator = 0;
int new_shooter_x = 0, new_shooter_y = 0;

int flag_arr[11][15];

int mainflag = 0;
int newarrx[165];
int newarry[165];
int newi=0;

string * dictionary;
int dictionarysize = 370099;
#define KEY_ESC 27 // A

// 20,30,30
const int bradius = 30; // ball radius in pixels...

int width = 900, height = 730;
int byoffset = bradius;

int nxcells = (width - bradius) / (2 * bradius);
int nycells = (height - byoffset /*- bradius*/) / (2 * bradius);
int nfrows = 2; // initially number of full rows //
float score = 0;
int **board; // 2D-arrays for holding the data...
int bwidth = 130;
int bheight = 10;
int bsx, bsy;
int seconds= 120;
const int nalphabets = 26;
enum alphabets {
	AL_A, AL_B, AL_C, AL_D, AL_E, AL_F, AL_G, AL_H, AL_I, AL_J, AL_K, AL_L, AL_M, AL_N, AL_O, AL_P, AL_Q, AL_R, AL_S, AL_T, AL_U, AL_W, AL_X, AL_y, AL_Z
};
GLuint texture[nalphabets];
GLuint tid[nalphabets];
string tnames[] = { "a.bmp", "b.bmp", "c.bmp", "d.bmp", "e.bmp", "f.bmp", "g.bmp", "h.bmp", "i.bmp", "j.bmp",
"k.bmp", "l.bmp", "m.bmp", "n.bmp", "o.bmp", "p.bmp", "q.bmp", "r.bmp", "s.bmp", "t.bmp", "u.bmp", "v.bmp", "w.bmp",
"x.bmp", "y.bmp", "z.bmp" };
GLuint mtid[nalphabets];
int awidth = 60, aheight = 60; // 60x60 pixels cookies...



//USED THIS CODE FOR WRITING THE IMAGES TO .bin FILE
void RegisterTextures_Write()
//Function is used to load the textures from the
// files and display
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);
	vector<unsigned char> data;
	ofstream ofile("image-data.bin", ios::binary | ios::out);
	// now load each cookies data...

	for (int i = 0; i < nalphabets; ++i) {

		// Read current cookie

		ReadImage(tnames[i], data);
		if (i == 0) {
			int length = data.size();
			ofile.write((char*)&length, sizeof(int));
		}
		ofile.write((char*)&data[0], sizeof(char) * data.size());

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ofile.close();

}
void RegisterTextures()
/*Function is used to load the textures from the
* files and display*/
{
	// allocate a texture name
	glGenTextures(nalphabets, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);

	if (!ifile) {
		cout << " Couldn't Read the Image Data file ";
		//exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char*)&length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < nalphabets; ++i) {
		// Read current cookie
		//ReadImage(tnames[i], data);
		/*if (i == 0) {
		int length = data.size();
		ofile.write((char*) &length, sizeof(int));
		}*/
		ifile.read((char*)&data[0], sizeof(char)* length);

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, awidth, aheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}
void DrawAlphabet(const alphabets &cname, int sx, int sy, int cwidth = 60,
	int cheight = 60)
	/*Draws a specfic cookie at given position coordinate
	* sx = position of x-axis from left-bottom
	* sy = position of y-axis from left-bottom
	* cwidth= width of displayed cookie in pixels
	* cheight= height of displayed cookiei pixels.
	* */
{

	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname]);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
int GetAlphabet() {
	return GetRandInRange(1, 26);
}

void Pixels2Cell(int px, int py, int & cx, int &cy) {
}
void Cell2Pixels(int cx, int cy, int & px, int &py)
// converts the cell coordinates to pixel coordinates...
{
}
void DrawShooter(int sx, int sy, int cwidth = 60, int cheight = 60)

{
	float fwidth = (float)cwidth / width * 2, fheight = (float)cheight
		/ height * 2;
	float fx = (float)sx / width * 2 - 1, fy = (float)sy / height * 2 - 1;

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, -1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}


//get position of row
int Xbox_number(int x_coordinate, int y_coordinate)
{
	int x_box=0;
	int temp_row = 610;
	int iterator = 0;
	while ( temp_row > y_coordinate ){
		temp_row = temp_row - 60;
		iterator++;
	}
	x_box = iterator;
	return x_box;
}

//get position of y
int Ybox_number(int x_coordinate, int y_coordinate)
{
	int y_box=0;
	y_box = x_coordinate % 59;
	return y_box;
}

int s_random[200];
int randomvar[11][15];
int chararr[11][15];
string alpahabetarr[1000];


void randomfunc()
{	
	//to generate random integer for first 3 rows using GetAlphabet() 
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<15; j++){
		randomvar[i][j]=GetAlphabet();
		}
	}

	//all others indexes are spaces other than 3 rows
	for (int i=3; i<11; i++)
	{
		for (int j=0; j<15; j++){
		randomvar[i][j]=-1;
		}
	}

	//random string which appear on shooter
	for (int i = 0; i< 200; i++){
	s_random[i] = GetAlphabet();
	}
}

//convert integer into ASCII value
void convert()
{
	for (int i=0; i<11; i++)
	{
		for (int j=0; j<15; j++){
		chararr[i][j] = randomvar[i][j] + 97;
		}
	}
}


void draw_new(int width, int height)
{
	alphabet_iterator ++;
	newarrx[newi] = width;
	newarry[newi] = height;
	newi++;
}


//Burst function
void new_burst()
{

// iterate over dicyonary
for (int index=0;index<370000;index++){
	for(int i=0;i<11;i++){
		for (int j=0; j<15; j++)
		{
			if(randomvar[i][j]!=-1){		
				if(dictionary[index][0]==char(chararr[i][j])){
				//cout<<"Character Matched: "<<char(chararr[i][j])<<endl;
				int matchcounter=1;
					for(int strlength=1;strlength<dictionary[index].length();strlength++)
					{
						if((j+strlength)<15&&randomvar[i][j+strlength]!=-1){
							if(dictionary[index][strlength]==char(chararr[i][j+strlength])){
								matchcounter++;
							}
							if(matchcounter==dictionary[index].length()&&matchcounter>2)
							{
								score=score+dictionary[index].length();
								cout<<matchcounter<<" Matched "<<dictionary[index]<<endl;
								//cout<<"matchcounter "<<matchcounter<<endl;
								for(int dexcounter=0;dexcounter<matchcounter;dexcounter++){
									cout<<"Matched on index "<<i<<" row "<<j+dexcounter<<" colunm "<<endl;
									randomvar[i][j+dexcounter] = GetAlphabet();
								}
								cout<<endl;
			
							}
						}
					}
					//for vertical
					for(int strlength=1;strlength<dictionary[index].length();strlength++)
					{
						if((i+strlength)<3&&randomvar[i+strlength][j]!=-1){
							if(dictionary[index][strlength]==char(chararr[i+strlength][j])){
								matchcounter++;
							}
							if(matchcounter==dictionary[index].length()&&matchcounter>2)
							{
								score=score+dictionary[index].length();
								cout<<matchcounter<<" Matched "<<dictionary[index]<<endl;
								//cout<<"matchcounter "<<matchcounter<<endl;
									for(int dexcounter=0;dexcounter<matchcounter;dexcounter++){
										cout<<"Matched on index "<<i+dexcounter<<" row "<<j<<" colunm "<<endl;
										randomvar[i+dexcounter][j] = GetAlphabet();
				
									}
							cout<<endl;
			
							}
						}
					}
	
				}		
			}
		}

	}
}
}
int flag =0;

/*
* Main Canvas drawing function.
* */
void DisplayFunction() {
	//
	glClearColor(1/*Red Component*/, 1.0/*Green Component*/,
		1.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim red and 1 means pure red and so on.
	//#if 0
	
	if (S_flag==0)
	{
		
		DrawAlphabet((alphabets)13, 240, 490, awidth, aheight);
		DrawAlphabet((alphabets)4, 300, 490, awidth, aheight);
		DrawAlphabet((alphabets)22, 360, 490, awidth, aheight);
		DrawAlphabet((alphabets)28, 420, 490, awidth, aheight);
		DrawAlphabet((alphabets)6, 480, 490, awidth, aheight);
		DrawAlphabet((alphabets)0, 540, 490, awidth, aheight);
		DrawAlphabet((alphabets)12, 600, 490, awidth, aheight);
		DrawAlphabet((alphabets)4, 660, 490, awidth, aheight);
		
		DrawAlphabet((alphabets)16, 360, 370, awidth, aheight);
		DrawAlphabet((alphabets)20, 420, 370, awidth, aheight);
		DrawAlphabet((alphabets)8, 480, 370, awidth, aheight);
		DrawAlphabet((alphabets)19, 540, 370, awidth, aheight);
		
		glutSwapBuffers();
	
	}
	
	
	if (S_flag ==1)
	{
	
		if (seconds <124 && seconds >120 )
		{
		glClearColor(1/*Red Component*/, 1.0/*Green Component*/,
		1.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
		glClear(GL_COLOR_BUFFER_BIT); //Update the colors
		
		DrawAlphabet((alphabets)11, 240, 490, awidth, aheight);
		DrawAlphabet((alphabets)14, 300, 490, awidth, aheight);
		DrawAlphabet((alphabets)0, 360, 490, awidth, aheight);
		DrawAlphabet((alphabets)3, 420, 490, awidth, aheight);
		DrawAlphabet((alphabets)8, 480, 490, awidth, aheight);
		DrawAlphabet((alphabets)13, 540, 490, awidth, aheight);
		DrawAlphabet((alphabets)6, 600, 490, awidth, aheight);
		
		glutSwapBuffers();
		}
		else {
		//write your drawing commands here or call your drawing functions...
		//drawMenu();
		
		if (flag==0)
		{
			randomfunc();
			flag=1;
			convert();
		}
		int i=0;
		int row_iterator =0;
		int column_iterator = 610;
		for (int row=0; row<3; row++)
		{
			row_iterator =0;
			for (int column=0; column<15; column++)
			{
				//randomvar[row][column]
				DrawAlphabet((alphabets)randomvar[row][column], row_iterator, column_iterator, awidth, aheight);
				
				row_iterator = row_iterator+ 60;
				i++;
				flag_arr[row][column] = 1;
			}
			column_iterator = column_iterator - 60;
		}
		
		for (int i=0; i< newi; i++)
		{
			int temp_x =0, temp_y = 0;
			temp_x = Xbox_number(newarrx[i],newarry[i]);
			temp_y = Ybox_number(newarrx[i], newarry[i]);
			DrawAlphabet((alphabets)s_random[i], newarrx[i], newarry[i], awidth, aheight);
			//cout<<s_random[i]<<endl;
			cout<<temp_x<<temp_y<<endl;
			randomvar[temp_x][temp_y] = s_random[i];
			flag_arr[temp_x][temp_y] = 1;
		
		}
		
		convert();
		new_burst();
		
		//string with appear on shooter
		
		DrawAlphabet((alphabets)s_random[alphabet_iterator], swidth, sheight, awidth, aheight);
		
		//score and time strings
		DrawString(40, height - 20, width, height + 5, "Score " + Num2Str(score), colors[RED]);
		DrawString(width / 2 + 250, height - 25, width, height,
			"Time Left:" + Num2Str(seconds) + " secs", colors[RED]);

		// #----------------- Write your code till here ----------------------------#
		//DO NOT MODIFY THESE LINES
		DrawShooter(420, 0, 60, bheight);
		glutSwapBuffers();
		//DO NOT MODIFY THESE LINES..
	}
	}
	else if (S_flag==2){
		DrawString(420, 550 + 30, width, height + 5, "Score: " + Num2Str(score), colors[RED]);
		DrawAlphabet((alphabets)16, 360, 370, awidth, aheight);
		DrawAlphabet((alphabets)20, 420, 370, awidth, aheight);
		DrawAlphabet((alphabets)8, 480, 370, awidth, aheight);
		DrawAlphabet((alphabets)19, 540, 370, awidth, aheight);

	    	glutSwapBuffers();
	}
}




/* Function sets canvas size (drawing area) in pixels...
*  that is what dimensions (x and y) your game will have
*  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
* */
void SetCanvasSize(int width, int height) {
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.*/
}






/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
* is pressed from the keyboard
*
* You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
*
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
*
* */

void NonPrintableKeys(int key, int x, int y) {
int tempx = 0, tempy=0;
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...
		tempx = Xbox_number(swidth, sheight);
		tempy = Ybox_number(swidth, sheight);
		if (swidth > 0)
		{
			if (flag_arr[tempx ][tempy - 1] == 0){
				//cout<<swidth<<" "<<sheight<<" "<<tempx<<" "<< tempy<<endl;
				if (sheight ==10){
					shooter_width = shooter_width - 60;
				}
				swidth = swidth - 60;
			}
			if (flag_arr[tempx ][tempy - 1] == 1){
				new_shooter_x = swidth;
				new_shooter_y = sheight;
				draw_new(new_shooter_x, new_shooter_y);
			}
		}	
	}
	else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {
		tempx = Xbox_number(swidth, sheight);
		tempy = Ybox_number(swidth, sheight);
		if (swidth < 840)
		{
			if (flag_arr[tempx][tempy + 1] == 0){
				//cout<<swidth<<" "<<sheight<<" "<<tempx<<" "<< tempy<<endl;
				if (sheight ==10){
					shooter_width = shooter_width + 60;
				}
				swidth = swidth + 60;
			}
		}
	}
	else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
	
		if (sheight < 610){
			tempx = Xbox_number(swidth, sheight);
			tempy = Ybox_number(swidth, sheight);
			if (flag_arr[tempx-1][tempy] == 0){
				//cout<<swidth<<" "<<sheight<<" "<<tempx<<" "<< tempy<<endl;
				sheight = sheight + 60;
			}
			else {
			
				new_shooter_x = swidth;
				new_shooter_y = sheight;
				draw_new(new_shooter_x, new_shooter_y);
				new_burst();
				mainflag = 1;
				swidth = 420;
				sheight = 10;
				
			}
		}
	}
	else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	* this function*/
	/*
	glutPostRedisplay();
	*/
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
*
* You will have to add the necessary code here for finding the direction of shooting
*
* This function has two arguments: x & y that tells the coordinate of current position of move mouse
*
* */

void MouseMoved(int x, int y) {
	//If mouse pressed then check than swap the balls and if after swaping balls dont brust then reswap the balls

//cout<<x<<" " <<y<<endl;

}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
*
* You will have to add the necessary code here for shooting, etc.
*
* This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
* x & y that tells the coordinate of current position of move mouse
*
* */


int tflag= 0;


void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{
		if (state == GLUT_UP)
		{
		//cout<<"x "<<x<<" y "<<y<<endl;
		if ((S_flag != 1 && S_flag != 2) && (x>270 && x< 820) && (y > 180 && y< 240))
		{
			tflag =1;
			S_flag =1;
			seconds = 123;
		}
		else if ((S_flag != 1) && (x>410 && x< 685) && (y > 300 && y< 360))
		{
			exit(1);
		}
		}
	
	}
	else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{

	}
	glutPostRedisplay();
}
/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
* is pressed from the keyboard
* This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
* program coordinates of mouse pointer when key was pressed.
* */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == KEY_ESC/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}
}

/*
* This function is called after every 1000.0/FPS milliseconds
* (FPS is defined on in the beginning).
* You can use this function to animate objects and control the
* speed of different moving objects by varying the constant FPS.
*
* */

void Timer(int m) {
	if (tflag==0)
	{
		glutPostRedisplay();
		glutTimerFunc(1000, Timer, 0);
	}
	if (tflag == 1 && S_flag == 1)
	{
		if (seconds == 0)
		{
			S_flag = 2;
			tflag = 2;
			glutTimerFunc(1000, Timer, 0);
		}
		glutPostRedisplay();
		glutTimerFunc(1000, Timer, 0);
		seconds--;
	}
	else if (tflag == 2 && S_flag == 2)
	{
		glutTimerFunc(1000, Timer, 0);
		glutPostRedisplay();
	}
	
	
}

/*
* our gateway main function
* */
int main(int argc, char*argv[]) {

int width =1024, heigtt = 670;
	InitRandomizer(); // seed the random number generator...

	//Dictionary for matching the words. It contains the 370099 words.
	dictionary = new string[dictionarysize]; 
	ReadWords("words_alpha.txt", dictionary); // dictionary is an array of strings
	//print first 5 words from the dictionary
	//for(int i=0; i < 50; ++i)
	//	cout<< " word "<< i << " =" << dictionary[i] <<endl;

	//Write your code here for filling the canvas with different Alphabets. You can use the Getalphabet function for getting the random alphabets



	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	
	
	
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("ITCs Word Shooter"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...
	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	RegisterTextures();
	//glutDisplayFunc(drawMenu);
	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutMouseFunc(MouseClicked);
	glutPassiveMotionFunc(MouseMoved); // Mouse

	//// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0/FPS, Timer, 0);
	
	//// now handle the control to library and it will call our registered functions when
	//// it deems necessary...

	glutMainLoop();

	return 1;
}
#endif /* */
