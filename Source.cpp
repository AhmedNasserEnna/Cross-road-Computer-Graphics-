#define STB_IMAGE_IMPLEMENTATION
#define glGenerateMipmaps ;
#include<cmath>
#include<Windows.h>
#include<gl/stb_image.h>
#include<gl/glut.h>
#include<iostream>
#include<string>

struct RGBColor {
	float r;
	float g;
	float b;
};

//............................ GLOBAL VARIABLES .............................
unsigned int texture;           //texture 
int width, height, nrChannels;  //texture
unsigned char* data = NULL;     //texture
bool fullScreen;    //specialKeyboard 
float ratio /*reshape*/ ,
angle, eyey = 60, eyez = 5, eyex = 50, upx = 50, upy = 0, upz = -70; // mydraw (camera)
///////ball variables and score
int ball_x = 50, ball_y = 2, ball_z = -5,//مكان الكورة فى  فى بدايه اللعبة 
attemptss = 3;// عدد المحاولات حتى الخسارة والبداية من جديد 
//level counter
float level = 3;
///////cars
int fast = 20;
GLfloat x1, x2; // handling animation in x-axis
float car1, car2, car3;       // animation variable
///////collision
bool pause = false;


//............................... FUNCTIONS PROTOTYPES ............................
void background();
void mydraw();
void reshape(int, int);
void timer(int);
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);
void load(int imgnum);
void check(unsigned char* data);
void initRendering();
void drawBall(int, int, int);
void car();
void coliision();
void attempts();
void startscreen();
void mouse(int, int, int, int);


//................................. MAIN FUNCTION ......................................
int main(int argc, char** argv) {
	// Initialize GLUT and set the display mode
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	// Create a window with the specified title
	glutCreateWindow(" Cross the Road Game");

	// Set the window to full screen
	glutFullScreen();

	// Call the background function to initialize the background
	background();

	// Call the initRendering function to initialize OpenGL rendering
	initRendering();

	// Set the display function to the startscreen function
	glutDisplayFunc(startscreen);

	// Set the idle function to the startscreen function
	glutIdleFunc(startscreen);

	// Set the reshape function to the reshape function
	glutReshapeFunc(reshape);

	// Set the timer function to the timer function
	glutTimerFunc(0, timer, 0);

	// Set the keyboard function to the keyboard function
	glutKeyboardFunc(keyboard);

	// Set the special keyboard function to the specialKeyboard function
	glutSpecialFunc(specialKeyboard);

	// Set the mouse function to the mouse function
	glutMouseFunc(mouse);

	// Enter the main event processing loop
	glutMainLoop();

	// Return from the main function
	return 0;
}

//....................................BACKGROUND FUNCTION .......................................
void background() {
	glClearColor(0, 0, 0, 1);  // Set background color to black and opaque
	glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling rendering distance form camera 
	glEnable(GL_TEXTURE_2D);   // Enable smooth shading apply texture on 3d object
}

//................................... game start screen FUNCTION .................................
void startscreen() {
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the color for the text
	glColor3f(1, 0, 0);

	// Set the position for the title text
	glRasterPos3f(-20, 20, -30);
	char Title[] = "Cross The Road Game";
	for (int i = 0; i <= strlen(Title); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, Title[i]);
	}

	// Set the position for the "Press space to start" message
	glRasterPos3f(-20, 18, -30);
	char msg1[] = "Press space to start the game";
	for (int i = 0; i <= strlen(msg1); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, msg1[i]);
	}

	// Set the position for the "Press ESC to exit" message
	glRasterPos3f(-20, 16, -30);
	char msg2[] = "or Press ESC to exit the game";
	for (int i = 0; i <= strlen(msg2); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, msg2[i]);
	}

	// Swap the front and back buffers to display the screen
	glutSwapBuffers();
}

//................................... game enviroment FUNCTION .................................
void mydraw() {
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Load the identity matrix
	glLoadIdentity();

	// Set the camera position and orientation
	gluLookAt(eyex, eyey, eyez, upx, upy, upz, 0, 1, 0);

	// Translate to the origin point of the game box
	glTranslatef(0, 0, 0);

	// Draw the ball at its current position
	drawBall(ball_x, ball_y, ball_z);

	// Draw the cars
	car();

	// Check for collisions
	coliision();

	// Show the number of attempts
	attempts();

	// Draw the road texture
	load(1);
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2d(0, 0); 
	glVertex3f(100, 0, 0);
	glTexCoord2d(4, 0);
	glVertex3f(0, 0, 0);
	glTexCoord2d(4, 4);
	glVertex3f(0, 0, -70);
	glTexCoord2d(0, 4);
	glVertex3f(100, 0, -70);
	glEnd();

	// Draw the roof texture
	load(1);
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2d(0, 0);
	glVertex3f(100, 100, 0);
	glTexCoord2d(5, 0);
	glVertex3f(0, 100, 0);
	glTexCoord2d(5, 5);
	glVertex3f(0, 100, -70);
	glTexCoord2d(0, 5);
	glVertex3f(100, 100, -70);
	glEnd();

	// Draw the background texture
	load(3);
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2d(0, 0);
	glVertex3f(0, 0, -70);
	glTexCoord2d(1, 0);
	glVertex3f(100, 0, -70);
	glTexCoord2d(1, 1);
	glVertex3f(100, 100, -70);
	glTexCoord2d(0, 1);
	glVertex3f(0, 100, -70);
	glEnd();

	// Draw the left wall texture
	load(2);
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2d(0, 0);
	glVertex3f(0, 0, -70);
	glTexCoord2d(1, 0);
	glVertex3f(0, 0, 0);
	glTexCoord2d(1, 1);
	glVertex3f(0, 100, 0);
	glTexCoord2d(0, 1);
	glVertex3f(0, 100, -70);
	glEnd();

	// Draw the right wall texture
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	glTexCoord2d(0, 0);
	glVertex3f(100, 0, -70);
	glTexCoord2d(1, 0);
	glVertex3f(100, 0, 0);
	glTexCoord2d(1, 1);
	glVertex3f(100, 100, 0);
	glTexCoord2d(0, 1);
	glVertex3f(100, 100, -70);
	glEnd();

	// Swap the front and back buffers to display the screen
	glutSwapBuffers();
}
//................................... BALL FUNCTION .................................
void drawBall(int ball_x, int ball_y, int ball_z)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	// inital ball_x = 50, ball_y = 2, ball_z = -5 ده مكان الكورة الاصلى فى الاول خالص 
	GLfloat position[] = { 50.0f, 10.0f, 15.20f, 1.0f };// المكان اللى هيبتدى الضوء يظهر فيه
	GLfloat color[] = { 0.59f,0.68f,1.0f,1.0f };//ده لو الضوء اللى هيظهر 
	GLfloat color2[] = { 0.59f,0.68f,1.0f,1.0f };//ده لو الضوء اللى هيظهر 
	GLfloat color3[] = { 0.59f,0.68f,1.0f,1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color);//  انتشار الضوء فى الشكل 
	glLightfv(GL_LIGHT0, GL_SPECULAR, color2);//البريق 
	glLightfv(GL_LIGHT0, GL_AMBIENT, color3);
	///////////////////////////////////////////////////
	// Set the material properties of the sphere
	GLfloat amb[] = { 0.07f,0.67f,1.0f,1.0f };
	GLfloat diff[] = { 0.0f,0.6f,1.0f,1.0f };
	GLfloat spec[] = { 0.8f,0.84f,0.87f,1.0f };
	GLfloat shine = 22.0f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);//المحيط كله 
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shine);//السطوح 
	// Set the properties of a point light source
	//احنا بنستخدام ال push ,pop
	// علشان نحافظ على االنظام الاحداثى القديم ونقدر نرجعه تانى 
	glPushMatrix();
	// Center Ball
	glTranslatef(ball_x, ball_y, ball_z);
	//بيرسم كورة بنصف قطر 3 و
	glutSolidSphere(3, 30, 30);// دى دله بتستخدم لرسم كورة و30 و30 بيمثلوا التفاضيل فى الكورة سواء  راسى او افقى 

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

//................................... collision update FUNCTION .................................
void coliision()
{
	if (ball_z < -10 && ball_z > -60)// identify area of affected ball
		// اقص من الاول عشرة ومن الاخر عشره البدايه من اول -10 والنهاية لحد -60 
	{
		//car1  &  car2
		if ((ball_z / 10) % 2 == 0)//بتحدد انت ماشى من اليمين للشمال ولا من الشمال لليمين 
		{
			// Is the ball in the way of the car or not
			if (((ball_z % 10) < -1) && ((ball_z % 10) > -7))// ان الكرة محور الزيد بتاعها ماشى فى طريق العربيات ولا لا 
			{
				if ((car1 + 12) - ball_x >= -2 && car1 - ball_x <= 2)//بحسب الكورة 
					/* ball in front of the car1 */
				{
					std::cout << "Car1" << "\n";
					std::cout << (ball_x) << "\n";
					std::cout << (ball_z) << "\n";
					std::cout << (car1) << "\t";
					std::cout << (car1 + 12) << "\n";
					//return to the origin point
					ball_x = 50, ball_y = 2, ball_z = -5;
					eyey = 60, eyez = 5, eyex = 50;
					upx = 50, upy = 0, upz = -70;
					attemptss -= 1; //1 attempt has been lost
					attempts(); //call the function to check if this is the third collision

					std::cout << attemptss << "\n";
				}
				else if ((car2 + 36) - ball_x >= -2 && (car2 + 24) - ball_x <= 2) /* ball in front of the car2 */
				{
					std::cout << "Car2" << "\n";
					std::cout << (ball_x) << "\n";
					std::cout << ball_z << "\n";
					std::cout << (car2 + 24) << "\t";
					std::cout << (car2 + 36) << "\n";
					//return to the origin point
					ball_x = 50, ball_y = 2, ball_z = -5;
					eyey = 60, eyez = 5, eyex = 50;
					upx = 50, upy = 0, upz = -70;
					attemptss -= 1; //1 attempt has been lost
					attempts(); //call the function to check if this is the third collision

					std::cout << attemptss << "\n";
					//
				}
			}
		}

		//car3
		else
		{
			//x1 = 100 - 0 - car3, x2 = 100 - 12 - car3
			// Is the ball in the way of the car or not

			if (((ball_z % 10) < -1) && ((ball_z % 10) > -7))
			{
				if ((50 - 12 - car3) - ball_x <= 2 && (50 - car3) - ball_x >= -2) /*ball in front of the car3*/
				{
					std::cout << "Car3" << "\n";
					std::cout << (ball_x + 2) << "\n";
					std::cout << ball_z << "\n";
					std::cout << (100 - car3) << "\t";
					std::cout << (100 - 12 - car3) << "\n";
					//return to the origin point
					ball_x = 50, ball_y = 2, ball_z = -5;
					eyey = 60, eyez = 5, eyex = 50;
					upx = 50, upy = 0, upz = -70;
					attemptss -= 1; //1 attempt has been lost
					attempts(); //call the function to check if this is the third collision

				}
			}
		}
	}

}

//................................... car drawing FUNCTION .................................
void car() {
	glDisable(GL_TEXTURE_2D);

	// car1 colors
	RGBColor car1_top_color = { 0.471, 0.09, 0.09 };
	RGBColor car1_bottom_color = { 1, 0.278, 0.278 };

	// car2 colors
	RGBColor car2_top_color = { 0.09, 0.471, 0.376 };
	RGBColor car2_bottom_color = { 0, 0.878, 0.616 };

	// car3 colors
	RGBColor car3_top_color = { 0.427, 0.275, 0.012 };
	RGBColor car3_bottom_color = { 1, 0.635, 0 };

	for (int i = 0; i < 5; i++)//عدد الطرق اللى العربيات هتمشى فيها 
	{

		int y1, y2, z1, z2;
		// Roads 1,3 direction: left -> right
		if (i % 2)// 1 ,3 =>give 1  |  0 2 4 => give 0 
		{

			//.................................... Car1 ..................................
			//------------------Top part-------------------//
			//black car
			x1 = 2 + car1, x2 = 10 + car1, y1 = 6.0, y2 = 10.0, z1 = (-12 - 10 * i), z2 = (-15 - 10 * i);
			//x1=2 x2=10 y=6 y2=10  z1=-12 z2=-15

			glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
			glColor3f(car1_top_color.r, car1_top_color.g, car1_top_color.b);
			// Define vertices in counter-clockwise (CCW) order with normal pointing out

		   // Top face (y = 2.0f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x1 + 1, y2, z2);     ///////
			glVertex3f(x1 + 1, y2, z1);
			glVertex3f(x2 - 1, y2, z1);

			// Bottom face (y = 1.0f)
			glVertex3f(x2, y1, z2);
			glVertex3f(x1, y1, z2);     ////////////
			glVertex3f(x1, y1, z1);      ///////////// 
			glVertex3f(x2, y1, z1);

			// Front face  (z = 1.0f)
			glVertex3f(x2 - 1, y2, z1);      //top 
			glVertex3f(x1 + 1, y2, z1);    //top
			glVertex3f(x1, y1, z1);     //bottom
			glVertex3f(x2, y1, z1);     //bottom

			// Back face (z = -1.0f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x1 + 1, y2, z2);    ////////////
			glVertex3f(x1, y1, z2);
			glVertex3f(x2, y1, z2);

			// Left face (x = -1.2 & -2 f)
			glVertex3f(x1 + 1, y2, z1);
			glVertex3f(x1 + 1, y2, z2);
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);

			// Right face (x = 1.2 & 2 f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x2 - 1, y2, z1);
			glVertex3f(x2, y1, z1);
			glVertex3f(x2, y1, z2);
			glEnd();  // End of drawing color-cube

			//------------------Bottom cube-------------------//

			x1 = 0 + car1, x2 = 12 + car1, y1 = 1, y2 = 6, z1 = (-12 - 10 * i), z2 = (-15 - 10 * i);
			// x1=0 y1=1 z1=-12 x2=12 y2=6 z2=-15
			glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
			glColor3f(car1_bottom_color.r, car1_bottom_color.g, car1_bottom_color.b);
			// Define vertices in counter-clockwise (CCW) order with normal pointing out

		   // Top face (y = 2.0f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x1, y2, z2);     ////////////
			glVertex3f(x1, y2, z1);      ///////////// 
			glVertex3f(x2, y2, z1);

			// Bottom face (y = 1.0f)
			glVertex3f(x2, y1, z2);
			glVertex3f(x1, y1, z2);     ////////////
			glVertex3f(x1, y1, z1);      ///////////// 
			glVertex3f(x2, y1, z1);

			// Front face  (z = 1.0f)
			glVertex3f(x2, y2, z1);      //top 
			glVertex3f(x1, y2, z1);    //top
			glVertex3f(x1, y1, z1);     //bottom
			glVertex3f(x2, y1, z1);     //bottom

			// Back face (z = -1.0f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x1, y2, z2);    ////////////
			glVertex3f(x1, y1, z2);
			glVertex3f(x2, y1, z2);

			// Left face (x = -1.2 & -2 f)
			glVertex3f(x1, y2, z1);
			glVertex3f(x1, y2, z2);
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);

			// Right face (x = 1.2 & 2 f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x2, y2, z1);
			glVertex3f(x2, y1, z1);
			glVertex3f(x2, y1, z2);
			glEnd();  // End of drawing color-cube


			//.................................... Car2 ..................................
			//------------------Top part-------------------//
			x1 = 36 + car2, x2 = 44 + car2, y1 = 6.0, y2 = 10.0, z1 = (-12 - 10 * i), z2 = (-15 - 10 * i);
			glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
			glColor3f(car2_top_color.r, car2_top_color.g, car2_top_color.b);
			// Define vertices in counter-clockwise (CCW) order with normal pointing out

		   // Top face (y = 2.0f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x1 + 1, y2, z2);     ////////////
			glVertex3f(x1 + 1, y2, z1);      ///////////// 
			glVertex3f(x2 - 1, y2, z1);

			// Bottom face (y = 1.0f)
			glVertex3f(x2, y1, z2);
			glVertex3f(x1, y1, z2);     ////////////
			glVertex3f(x1, y1, z1);      ///////////// 
			glVertex3f(x2, y1, z1);

			// Front face  (z = 1.0f)
			glVertex3f(x2 - 1, y2, z1);      //top 
			glVertex3f(x1 + 1, y2, z1);    //top
			glVertex3f(x1, y1, z1);     //bottom
			glVertex3f(x2, y1, z1);     //bottom

			// Back face (z = -1.0f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x1 + 1, y2, z2);    ////////////
			glVertex3f(x1, y1, z2);
			glVertex3f(x2, y1, z2);

			// Left face (x = -1.2 & -2 f)
			glVertex3f(x1 + 1, y2, z1);
			glVertex3f(x1 + 1, y2, z2);
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);

			// Right face (x = 1.2 & 2 f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x2 - 1, y2, z1);
			glVertex3f(x2, y1, z1);
			glVertex3f(x2, y1, z2);
			glEnd();  // End of drawing color-cube

			//------------------Bottom cube-------------------//
			x1 = 34 + car2, x2 = 46 + car2, y1 = 1, y2 = 6, z1 = (-12 - 10 * i), z2 = (-15 - 10 * i);
			glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
			glColor3f(car2_bottom_color.r, car2_bottom_color.g, car2_bottom_color.b);
			// Define vertices in counter-clockwise (CCW) order with normal pointing out

		   // Top face (y = 2.0f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x1, y2, z2);     ////////////
			glVertex3f(x1, y2, z1);      ///////////// 
			glVertex3f(x2, y2, z1);

			// Bottom face (y = 1.0f)
			glVertex3f(x2, y1, z2);
			glVertex3f(x1, y1, z2);     ////////////
			glVertex3f(x1, y1, z1);      ///////////// 
			glVertex3f(x2, y1, z1);

			// Front face  (z = 1.0f)
			glVertex3f(x2, y2, z1);      //top 
			glVertex3f(x1, y2, z1);    //top
			glVertex3f(x1, y1, z1);     //bottom
			glVertex3f(x2, y1, z1);     //bottom

			// Back face (z = -1.0f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x1, y2, z2);    ////////////
			glVertex3f(x1, y1, z2);
			glVertex3f(x2, y1, z2);

			// Left face (x = -1.2 & -2 f)
			glVertex3f(x1, y2, z1);
			glVertex3f(x1, y2, z2);
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);

			// Right face (x = 1.2 & 2 f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x2, y2, z1);
			glVertex3f(x2, y1, z1);
			glVertex3f(x2, y1, z2);
			glEnd();  // End of drawing color-cube

		}

		// Roads 0,2,4 direction: right -> left
		else
		{
			//.................................... Car3 ..................................
			//------------------Top part-------------------//
			x1 = 50 - 10 - car3, x2 = 50 - 2 - car3, y1 = 6, y2 = 10, z1 = (-12 - 10 * i), z2 = (-15 - 10 * i);
			glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
			glColor3f(car3_top_color.r, car3_top_color.g, car3_top_color.b);
			// Define vertices in counter-clockwise (CCW) order with normal pointing out

		   // Top face (y = 2.0f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x1 + 1, y2, z2);     ////////////
			glVertex3f(x1 + 1, y2, z1);      ///////////// 
			glVertex3f(x2 - 1, y2, z1);

			// Bottom face (y = 1.0f)
			glVertex3f(x2, y1, z2);
			glVertex3f(x1, y1, z2);     ////////////
			glVertex3f(x1, y1, z1);      ///////////// 
			glVertex3f(x2, y1, z1);

			// Front face  (z = 1.0f)
			glVertex3f(x2 - 1, y2, z1);      //top 
			glVertex3f(x1 + 1, y2, z1);    //top
			glVertex3f(x1, y1, z1);     //bottom
			glVertex3f(x2, y1, z1);     //bottom

			// Back face (z = -1.0f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x1 + 1, y2, z2);    ////////////
			glVertex3f(x1, y1, z2);
			glVertex3f(x2, y1, z2);

			// Left face (x = -1.2 & -2 f)
			glVertex3f(x1 + 1, y2, z1);
			glVertex3f(x1 + 1, y2, z2);
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);

			// Right face (x = 1.2 & 2 f)
			glVertex3f(x2 - 1, y2, z2);
			glVertex3f(x2 - 1, y2, z1);
			glVertex3f(x2, y1, z1);
			glVertex3f(x2, y1, z2);
			glEnd();  // End of drawing color-cube


			//------------------Bottom cube-------------------//

			x1 = 50 - 0 - car3, x2 = 50 - 12 - car3, y1 = 1, y2 = 6, z1 = (-12 - 10 * i), z2 = (-15 - 10 * i);
			glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
			glColor3f(car3_bottom_color.r, car3_bottom_color.g, car3_bottom_color.b);

			// Top face (y = 2.0f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x1, y2, z2);     ////////////
			glVertex3f(x1, y2, z1);      ///////////// 
			glVertex3f(x2, y2, z1);

			// Bottom face (y = 1.0f)
			glVertex3f(x2, y1, z2);
			glVertex3f(x1, y1, z2);     ////////////
			glVertex3f(x1, y1, z1);      ///////////// 
			glVertex3f(x2, y1, z1);

			// Front face  (z = 1.0f)
			glVertex3f(x2, y2, z1);      //top 
			glVertex3f(x1, y2, z1);    //top
			glVertex3f(x1, y1, z1);     //bottom
			glVertex3f(x2, y1, z1);     //bottom

			// Back face (z = -1.0f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x1, y2, z2);    ////////////
			glVertex3f(x1, y1, z2);
			glVertex3f(x2, y1, z2);

			// Left face (x = -1.2 & -2 f)
			glVertex3f(x1, y2, z1);
			glVertex3f(x1, y2, z2);
			glVertex3f(x1, y1, z2);
			glVertex3f(x1, y1, z1);

			// Right face (x = 1.2 & 2 f)
			glVertex3f(x2, y2, z2);
			glVertex3f(x2, y2, z1);
			glVertex3f(x2, y1, z1);
			glVertex3f(x2, y1, z2);
			glEnd();  // End of drawing color-cube

		}
	}
	glEnable(GL_TEXTURE_2D);
}			// Define vertices in counter-clockwise (CCW) order with normal pointing out


//.................................... RESHAPE FUNCTION ..................................
void reshape(int w, int h) {
	// Check if the height is 0
	if (h == 0) {
		h = 1;
		std::cout << "This line will never be executed :)" << std::endl;
	}

	// Calculate the aspect ratio
	ratio = w / (float)h;

	// Set the viewport to cover the entire screen
	glViewport(0, 0, w, h);

	// Switch to the projection matrix and load the identity matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the perspective projection parameters
	gluPerspective(90, ratio, 1, 120);

	// Switch back to the modelview matrix
	glMatrixMode(GL_MODELVIEW);
}

//.................................... RENDERING FUNCTION ..................................
void initRendering() {
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
}
//................................... TIMER FUNCTION ...................................
void timer(int v) {
	// Repaint the screen
	glutPostRedisplay();

	// Set the timer to call this function again after a certain amount of time
	glutTimerFunc(1000 / fast, timer, 0);

	// Move the cars based on the current level
	car1 += level;
	car2 += level;
	car3 += level;

	// If a car goes off-screen, reset its position to the start
	if (car1 > 100)
		car1 = 0;
	if (car2 > 66)
		car2 = -34;
	if (car3 > 50)
		car3 = -50;
}

//................................... KEYBOARED FUNCTION ................................
void keyboard(unsigned char key, int x, int y) {
	angle += 4;

	// If the escape key is pressed, exit the program
	if (key == 27)
		exit(0);

	// If the spacebar is pressed, start a new game
	if (key == 32) {
		attemptss = 3;
		glutDisplayFunc(mydraw);
		glutIdleFunc(mydraw);
	}

	// If the 'n' or 'N' key is pressed, start a new game with a higher speed
	if (key == 'n' || key == 'N') {
		fast = 20;
		attemptss = 3;
		glutDisplayFunc(mydraw);
		glutIdleFunc(mydraw);
	}

	// If the 'm' or 'M' key is pressed, go back to the start screen
	if (key == 'm' || key == 'M') {
		attemptss = 3;
		glutDisplayFunc(startscreen);
		glutIdleFunc(startscreen);
	}

	// If the 'l' or 'L' key is pressed, start a new level with a higher speed
	if (key == 'l' || key == 'L') {
		ball_x = 50, ball_y = 2, ball_z = -5;
		eyey = 60, eyez = 5, eyex = 50;
		upx = 50, upy = 0, upz = -70;
		fast += 30;
		attemptss = 3;
		glutDisplayFunc(mydraw);
		glutIdleFunc(mydraw);
	}

	// Move the ball if the arrow keys are pressed
	if (key == 'd' || key == 'D') {
		if (ball_x < 100)
			ball_x += 2;
	}
	if (key == 'a' || key == 'A') {
		if (ball_x > 0)
			ball_x -= 2;
	}
	if (key == 's' || key == 'S') {
		if (ball_z < -5) {
			ball_z += 2;
			eyez += 1;
		}
	}
	if (key == 'w' || key == 'W') {
		if (ball_z > -65) {
			ball_z -= 2;
			eyez -= 1;
		}
	}

	// Request a screen repaint
	glutPostRedisplay();
}

//.................................. SPECIALKEYBOARED FUNCTION ............................
void specialKeyboard(int key, int x, int y) {
	// If the F1 key is pressed, toggle full-screen mode
	if (key == GLUT_KEY_F1) {
		fullScreen = !fullScreen;
		if (fullScreen) {
			glutFullScreen();
		}
		else {
			glutReshapeWindow(GetSystemMetrics(SM_CXSCREEN) / 2,
				GetSystemMetrics(SM_CYSCREEN) / 2);
			glutPositionWindow(GetSystemMetrics(SM_CXSCREEN) / 4,
				GetSystemMetrics(SM_CYSCREEN) / 4);
		}
	}

	// Move the camera if the arrow keys are pressed
	if (key == GLUT_KEY_LEFT) {
		upx -= .9 * cos(.1);
		upz -= .9 * sin(.1);
	}
	if (key == GLUT_KEY_RIGHT) {
		upx += .9 * cos(.1);
		upz += .9 * sin(.1);
	}
	if (key == GLUT_KEY_UP) {
		eyez -= 2;
		eyey += .3 * abs(cos(angle));
	}
	if (key == GLUT_KEY_DOWN) {
		eyez += 2;
		eyey += .3 * abs(cos(angle));
	}

	// Request a screen repaint
	glutPostRedisplay();
}

//.................................... LAOD FUNCTION ....................................
void load(int imgnum) {
	if (imgnum == 1) {
		data = stbi_load("road.jpg", &width, &height, &nrChannels, 0);
		check(data);
	}
	else if (imgnum == 2) {
		data = stbi_load("wall.jpg", &width, &height, &nrChannels, 0);
		check(data);
	}
	else if (imgnum == 3) {
		data = stbi_load("background.jpg", &width, &height, &nrChannels, 0);
		check(data);
	}
}
//................................ CHECK FUNCTION .........................................
void check(unsigned char* data) {
	if (data) {
		glGenTextures(1, &texture);// texture id
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

//................................ attempts FUNCTION .........................................
void attempts() {
	if (attemptss == 0) { // If the user has no attempts left
		ball_x = 50, ball_y = 2, ball_z = -5; // Return the ball to its original position
		eyey = 60, eyez = 5, eyex = 50;
		upx = 50, upy = 0, upz = -70;

		// Display a game over message and instructions
		glColor3f(1, 0.0, 0.0);
		glRasterPos3f(10, 40, -68);
		char scoremsg[] = { "Game over" };
		for (int i = 0; i <= strlen(scoremsg); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoremsg[i]);
		}

		glColor3f(1, 0.0, 0.0);
		glRasterPos3f(10, 30, -68);
		char scoremsg2[] = "Press  M -> main page";
		for (int i = 0; i <= strlen(scoremsg2); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoremsg2[i]);
		}

		glColor3f(1, 0.0, 0.0);
		glRasterPos3f(10, 20, -68);
		char scoremsg3[] = "Press  N -> new game";
		for (int i = 0; i <= strlen(scoremsg3); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, scoremsg3[i]);
		}
	}
	else if (ball_z <= -60) { // If the ball has reached the end
		// Display a win message and instructions to move to the next level
		glColor3f(1, 0.0, 0.0);
		glRasterPos3f(10, 40, -69);
		char winmsg[] = { "You Won, Congrats!" };
		for (int i = 0; i <= strlen(winmsg); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, winmsg[i]);
		}

		glRasterPos3f(10, 20, -69);
		char Levelmsg[] = { "Press L to move to the next level!" };
		for (int i = 0; i <= strlen(Levelmsg); i++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, Levelmsg[i]);
		}
	}
}

//................................ mouse FUNCTION .........................................
// Callback function for mouse events in a GLUT window
void mouse(int button, int state, int x, int y) {
	// If the left mouse button is pressed down, set the background color to white
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		glClearColor(1, 1, 1, 1);
	}
	// If the left mouse button is released, set the background color to green
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		glClearColor(0,0, 0, 1);
	}
	// If the right mouse button is pressed down, set the background color to green
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		glClearColor(0, 0, 0, 1);
	}
	// If the right mouse button is released, set the background color to white
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		glClearColor(1, 1, 1, 1);
	}

}