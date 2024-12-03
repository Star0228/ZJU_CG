#include <math.h>
#include <GL/glut.h>
#define pi 3.1415926

// The angle of the planet
float planetAngle ;
// The speed of the planet
float moveSpeed = 0.1f; 
// The window size
int windowWidth = 600;
int windowHeight = 600;
// The move(left/right) direction
bool isMoveLeft;
bool isMoveRight;
// The camera position for lookAt
float eyeX = 5.0f, eyeY = 0.0f, eyeZ = 5.0f; // The camera position
float centerX = 0.0f, centerY = 0.0f, centerZ = 0.0f; // The center position
float upX = 0.0f, upY = 0.0f, upZ = 1.0f; // The up vector
// The normal vector of the (center and eye vector)
#define normalX ((centerY-eyeY)*upZ-(centerZ-eyeZ)*upY)
#define normalY ((centerZ-eyeZ)*upX-(centerX-eyeX)*upZ)
#define normalZ ((centerX-eyeX)*upY-(centerY-eyeY)*upX)
// The standard normal vector
float normalLength;
float temp_normalX;
float temp_normalY;
float temp_normalZ;

// The function of the timer for time refresh
void timer(int p)
{
    planetAngle += 1;   // Update the angle of the planet
    glutPostRedisplay();
    glutTimerFunc(7, timer, 0);
}
// The function of the reshape for the window size
void reshape(int width, int height)
{
    glViewport(0, 0, width, height); // Set the viewport size
    glMatrixMode(GL_PROJECTION);     // Reset the projection matrix
    glLoadIdentity();                // Load the identity matrix
    // Set the perspective projection matrix (field of view, aspect ratio, near clipping plane, far clipping plane)
    gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);      // Reset the modelview matrix
}
// The function of the passive motion for the mouse move
void passiveMotion(int x, int y) 
{ 
    windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    if(isMoveLeft || isMoveRight)
    {
        return;
    }
    centerZ =  5.0*(windowHeight/2-y)/windowHeight;
    centerX =  (-eyeY)*0.5*(x-windowWidth/2)/windowWidth;
    centerY = (eyeX)*0.5*(x-windowWidth/2)/windowWidth;
} 
// The function of the keyboard for the key press
void keyboard(unsigned char key, int x, int y)
{
    normalLength = sqrt(normalX*normalX+normalY*normalY+normalZ*normalZ);
    temp_normalX = normalX/normalLength;
    temp_normalY = normalY/normalLength;
    temp_normalZ = normalZ/normalLength;
    switch (key)
    {
        case 'w': // move forward
            eyeX += moveSpeed*(centerX-eyeX);
            eyeY += moveSpeed*(centerY-eyeY); 
            eyeZ += moveSpeed*(centerZ-eyeZ);
            break;
        case 's': // move backward
            eyeX -= moveSpeed*(centerX-eyeX);
            eyeY -= moveSpeed*(centerY-eyeY); 
            eyeZ -= moveSpeed*(centerZ-eyeZ);
            break;
        case 'a': // move left
            eyeX -= moveSpeed*temp_normalX;
            eyeY -= moveSpeed*temp_normalY;
            eyeZ -= moveSpeed*temp_normalZ;
            isMoveLeft = true;
            centerX -= moveSpeed*temp_normalX;
            centerY -= moveSpeed*temp_normalY;
            centerZ -= moveSpeed*temp_normalZ;
            break;
        case 'd': // move right
            eyeX += moveSpeed*temp_normalX;
            eyeY += moveSpeed*temp_normalY;
            eyeZ += moveSpeed*temp_normalZ;
            isMoveRight = true;
            centerX += moveSpeed*temp_normalX;
            centerY += moveSpeed*temp_normalY;
            centerZ += moveSpeed*temp_normalZ;
            break;
    }
    isMoveLeft = isMoveRight = false;
    glutPostRedisplay(); // Refresh the display
}

void drawSolarSystem() 
{
    // draw the sun's obrbit
    glColor3f(0.7f, 0.7f, 0.7f); // gray
    glPushMatrix();
    glutWireTorus(0.001, 0.7, 10, 100); // the orbit of radius 0.7 so thin that it looks like a circle
    glPopMatrix();

    // draw the first sun
    glPushMatrix();
    glRotatef( planetAngle *0.1, 0.0f, 0.0f, 1.0f); // rotate around z axis
    glTranslatef(0.7f, 0.0f, 0.0f); // move to the orbit position
    glColor3f(1.0f, 1.0f, 0.0f); // yellow
    glutSolidSphere(0.2f, 50, 50); // the solid sphere of radius 0.2
    glPopMatrix();
    // draw the second sun
    glPushMatrix();
    glRotatef(planetAngle*0.1, 0.0f, 0.0f, 1.0f); // rotate around z axis
    glTranslatef(-0.7f, 0.0f, 0.0f); //  move to the orbit position
    glColor3f(1.0f, 1.0f, 0.0f); // yellow
    glutSolidSphere(0.2f, 50, 50); // the solid sphere of radius 0.2
    glPopMatrix();

    // draw the first planet's orbit
    glColor3f(0.7f, 0.7f, 0.7f); // gray
    glPushMatrix();
    // apply the rotation transformation (rotate 45 degrees around the x axis)
    glRotatef(135.0f, 1.0f, 0.0f, 0.0f);
    glutWireTorus(0.001, 1.5, 10, 100); // the torus of radius 1.5 so thin that it looks like a circle
    glPopMatrix();

    // draw the first planet
    glPushMatrix();
    glRotatef(planetAngle, 0.0f, 1.0f, 1.0f); // rotate around (0,1,1) axis
    glTranslatef(1.5f, 0.0f, 0.0f); // move to the orbit position
    glColor3f(0.0f, 0.0f, 1.0f); // blue
    glutSolidSphere(0.08f, 50, 50); // the solid sphere of radius 0.08
    glPopMatrix();

    // draw the first planet's satellite orbit of radius 0.5
    glColor3f(0.7f, 0.7f, 0.7f); // gray
    glPushMatrix();
    glRotatef(planetAngle, 0.0f, 1.0f, 1.0f); // rotate around (0,1,1) axis
    glTranslatef(1.5f, 0.0f, 0.0f); // move to the orbit position
    glutWireTorus(0.001, 0.5, 10, 100); // the torus of radius 0.5 so thin that it looks like a circle
    glPopMatrix();

    // draw the first planet's satellite
    glPushMatrix();
    glRotatef(planetAngle, 0.0f, 1.0f, 1.0f); // rotate around (0,1,1) axis
    glTranslatef(1.5f, 0.0f, 0.0f); // move to the orbit position
    // glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(planetAngle*0.2, 0.0f, 0.0f, 1.0f); // rotate around z axis
    glTranslatef(-0.5f, 0.0f, 0.0f); // move to the orbit position
    glColor3f(0.0f, 1.0f, 0.0f); // green
    glutSolidSphere(0.03f, 50, 50); // draw the solid sphere of radius 0.03
    glPopMatrix();

    // draw the second planet's orbit of radius 2.5
    glColor3f(0.7f, 0.7f, 0.7f); // gray
    glPushMatrix();
    // apply the rotation transformation (rotate 45 degrees around the x axis)
    glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
    glutWireTorus(0.001, 2.5, 10, 100); // the torus of radius 2.5 so thin that it looks like a circle
    glPopMatrix();

    // draw the second planet
    glPushMatrix();
    glRotatef(planetAngle*0.5, 0.0f, -1.0f, 1.0f); // rotate around (0,-1,1) axis
    glTranslatef(-2.5f, 0.0f, 0.0f); // move to the orbit position
    glColor3f(1.0f, 0.0f, 0.0f); // red
    glutSolidSphere(0.10f, 50, 50); // draw the solid sphere of radius 0.1
    glPopMatrix();

    // draw the second planet's satellite orbit of radius 0.5

    glColor3f(0.7f, 0.7f, 0.7f); // gray
    glPushMatrix();
    glRotatef(planetAngle*0.5, 0.0f, -1.0f, 1.0f); // rotate around (0,-1,1) axis
    glTranslatef(-2.5f, 0.0f, 0.0f); // move to the orbit position
    glutWireTorus(0.001, 0.5, 10, 100); // the torus of radius 0.5 so thin that it looks like a circle
    glPopMatrix();

    // draw the second planet's satellite
    glPushMatrix();
    glRotatef(planetAngle*0.5, 0.0f, -1.0f, 1.0f); // rotate around (0,-1,1) axis
    glTranslatef(-2.5f, 0.0f, 0.0f); // move to the orbit position
    // glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(planetAngle*0.1, 0.0f, 0.0f, 1.0f); // rotate around z axis
    glTranslatef(-0.5f, 0.0f, 0.0f); // move to the orbit position
    glColor3f(0.0f, 1.0f, 0.0f); // green
    glutSolidSphere(0.03f, 50, 50); // draw the solid sphere of radius 0.03
    glPopMatrix();

    // draw the third planet's orbit
    glColor3f(0.7f, 0.7f, 0.7f); // gray
    glPushMatrix();
    glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
    glutWireTorus(0.001, 3.5, 10, 100); // the torus of radius 3.5 so thin that it looks like a circle
    glPopMatrix();

    // draw the third planet
    glPushMatrix();
    glRotatef(planetAngle*0.05, 0.0f, 0.0f, 1.0f); // rotate around (0,0,1) axis
    glTranslatef(3.5f, 0.0f, 0.0f); // move to the orbit position
    glColor3f(0.7f, 0.5f, 0.5f); 
    glutSolidSphere(0.08f, 50, 50); // the solid sphere of radius 0.08
    glPopMatrix();

}

void display() 
{ 
	float green[3]={0,0,1}; 
	glClearColor(0,0,0,0); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	glLoadIdentity(); 
    // Set the camera position
    gluLookAt(eyeX, eyeY, eyeZ,     // The camera position
              centerX, centerY, centerZ, // The center position
              upX, upY, upZ);       // The up vector    

	glColor3fv(green);
	drawSolarSystem();
	glFlush(); 
} 

int main(int argc, char** argv) 
{ 
	glutInit(&argc, argv); 
	glutInitDisplayMode (GLUT_SINGLE| GLUT_RGBA); 
	glutInitWindowSize (600, 600);// Set thr initial window size
	glutInitWindowPosition(500,500); // Set the initial window position
	glutCreateWindow ("Solar System");     // Create the window
	glutTimerFunc(7, timer, 0); // Set the timer for the time refresh and its frequency is about 144Hz
	glutReshapeFunc(reshape); // Set the reshape function
	glutDisplayFunc (display);     // Set the display function
	glutPassiveMotionFunc(passiveMotion); // Set the passive motion function
	glutKeyboardFunc(keyboard); // Set the keyboard function
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();     
	return 0; 
} 
