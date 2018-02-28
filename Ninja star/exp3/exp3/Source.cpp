

#include "Angel.h"
#include "freeglut.h"
#include "glew.h"
#include "glui.h"


const int NumPoints = 750 + 18;
const int circleDNS = 50;
int mainFrame;
//----------------------------------------------------------------------------


std::string tmptext="0.0";



vec2 points[NumPoints];
vec4 colors[NumPoints];
float rotationAngle = 0.0;
float rtspeed = 0.05,temp_rtspedd=0.05, wrspeed=rtspeed*100.0;
float sinnum = 360.0;
float scaleNum = 0.5,scaleSpeed=0.05,scaleCof=0.0;
bool scaleCtrl = true;
float spiralAngle = 0.0, movingSpeed = 0.05, temp_movingSpeed = 0.05, wr_movingSpeed = movingSpeed*100.0;
float tmpAng=0.0;
bool spiralMove=true,moveCtrl=true;
GLuint theta;
GLuint scalingRatio;
GLuint movingAngle;


vec4 color_type[2] = {

	vec4(1.0,1.0,0.0,1.0),vec4(0.5,0.5,0.5,1.0)
};

int i = 0;
void
triangle(vec2 a, vec2 b, vec2 c, vec4 color) {

	points[i] = a; colors[i] = color; i++;
	points[i] = b; colors[i] = color; i++;
	points[i] = c; colors[i] = color; i++;

}

void
quad(vec2 a, vec2 b, vec2 c, vec2 d, vec4 color) {

	triangle(a, b, d, color);
	triangle(b, c, d, color);

}

void
circle(float centerX, float centerY, float radius, vec4 color) {

	vec2 center = vec2(centerX, centerY);
	float angle;

	float pointX, pointY;

	vec2 circlePts[circleDNS];

	for (int j = 0; j < circleDNS; j++) {

		angle = (j*2.0* M_PI) / circleDNS;
		pointX = centerX + radius*cos(angle);
		pointY = centerY + radius*sin(angle);
		circlePts[j] = vec2(pointX, pointY);
	}

	for (int j = 0; j < circleDNS; j++) {

		if (j == circleDNS - 1) {

			triangle(circlePts[j], center, circlePts[0], color);
			break;
		}
		triangle(circlePts[j], center, circlePts[j + 1], color);

	}



}

void
shuriken(vec2 a, vec2 b, vec2 c, vec2 d, vec2 e, vec2 f, vec2 g, vec2 h, vec4 shurikenColor, vec4 backgroundColor) {


	triangle(h, a, b, shurikenColor);
	triangle(b, c, d, shurikenColor);
	triangle(d, e, f, shurikenColor);
	triangle(f, g, h, shurikenColor);
	quad(b, d, f, h, shurikenColor);


	circle(0.0, 0.0, 0.025, backgroundColor);
	circle(0.0, 0.125, 0.025, backgroundColor);
	circle(0.125, 0.0, 0.025, backgroundColor);
	circle(0.0, -0.125, 0.025, backgroundColor);
	circle(-0.125, 0.0, 0.025, backgroundColor);






}



//----------------------------------------------------------------------------

void
init(void)
{

	


	vec2 vertices[8] = {
		vec2(-0.25, 0.25), vec2(0.0, 0.125), vec2(0.25, 0.25), vec2(0.125, 0.0), vec2(0.25, -0.25), vec2(0.0, -0.125), vec2(-0.25, -0.25), vec2(-0.125, 0.0)
	};

	shuriken(vertices[0], vertices[1], vertices[2], vertices[3], vertices[4], vertices[5], vertices[6], vertices[7], color_type[1], color_type[0]);




	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// First, we create an empty buffer of the size we need by passing
	//   a NULL pointer for the data values
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), points, GL_STATIC_DRAW);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader_exp2.glsl", "fshader_exp2.glsl");
	glUseProgram(program);

	// Initialize the vertex position attribute from the vertex shader    
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	theta = glGetUniformLocation(program, "theta");
	scalingRatio = glGetUniformLocation(program, "scalingRatio");
	movingAngle = glGetUniformLocation(program, "movingAngle");

	//glEnable(GL_DEPTH_TEST);

	glClearColor(1.0, 1.0, 0.0, 1.0); /* yellow background */
}

//----------------------------------------------------------------------------

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniform1f(theta, rotationAngle);
	glUniform1f(scalingRatio, scaleNum);
	glUniform1f(movingAngle, spiralAngle);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	glutSwapBuffers();
	glFlush();
}

//----------------------------------------------------------------------------

void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 033:
		exit(EXIT_SUCCESS);
		break;
	

	}
}
void
idle()
{
	/*Rotation Control */

		if (sinnum <= 0.0) {
		
			sinnum = 360.0;
		}
		rotationAngle = sinnum;
	
		sinnum -= rtspeed;
	
		if (scaleCof > 180.0) {

			scaleCof = 0.0;
		}
		
		/*Scaling Control */
		scaleNum = (sin(scaleCof*M_PI/180.0 ) + 0.5);
		scaleCof += scaleSpeed;

		/*Spiral Move Control*/
		if (spiralMove) {

			tmpAng += movingSpeed;
			if(movingSpeed<0.0){
			
				if (tmpAng <= -630.0) {

					spiralMove = false;
				}
			}else{
				if (tmpAng >= 630.0) {

					spiralMove = false;
				}
			}
			


			spiralAngle = tmpAng;
		}
		else {

			tmpAng -= movingSpeed;

			if(movingSpeed<0.0){
				if (tmpAng >= 0.0) {
					spiralMove = true;
				}
			}else{
				if (tmpAng <= 0.0) {
					spiralMove = true;
				}
			}
		
			spiralAngle = -tmpAng;
		}
		


	if (glutGetWindow() != mainFrame)
		glutSetWindow(mainFrame);

	
	GLUI_Master.sync_live_all();
	glutPostRedisplay();
}
//----------------------------------------------------------------------------
/*Buttons Control Block*/
void
glui_cb(int control) {

	switch (control)
	{

		/*Rotation Start Stop*/
	case 0:
		if (rtspeed != 0.0) {
			
			temp_rtspedd = rtspeed;
			rtspeed = 0.0;
			wrspeed = rtspeed*100.0;

		}
		else {
			
			rtspeed = temp_rtspedd;
			wrspeed = rtspeed*100.0;
			
			
		}
		break;
		/*Rotation Speed increase*/
	case 1:

		if (rtspeed >= 0.09) {


		}else {
			rtspeed += 0.01;
			wrspeed = rtspeed*100.0;

		}
		
		
		break;
		/*Rotation Speed Decrease*/
	case 2:
		if (rtspeed <= -0.1) {


		}else {

			rtspeed -= 0.01;
			wrspeed = rtspeed*100.0;
		}
		
		
		break;
	case 3:
		
		

		break;
		/*Scaling Start Stop*/
	case 4:

		if (scaleCtrl) {

			scaleSpeed = 0.0;
			scaleCtrl = false;
		}
		else {

			scaleSpeed = 0.05;
			scaleCtrl = true;
		}

		break;
		/*Spiral Moving Start Stop*/
	case 5:

		if (moveCtrl) {
			temp_movingSpeed = movingSpeed;
			movingSpeed = 0.0;
			moveCtrl = false;
			wr_movingSpeed = movingSpeed*100.0;
		}
		else {
			movingSpeed = temp_movingSpeed;
			wr_movingSpeed = movingSpeed*100.0;
			moveCtrl = true;
			
		}
		break;
		/*Spiral Moving Speed Increase*/
	case 6:

		if (movingSpeed >= 0.09) {


		}
		else {
			movingSpeed += 0.01;
			wr_movingSpeed = movingSpeed*100.0;
			temp_movingSpeed = movingSpeed;

		}
		

		break;
		/*Spiral Moving Speed Decrease*/
	case 7:
		if (movingSpeed <= -0.1) {


		}
		else {
			movingSpeed -= 0.01;
			wr_movingSpeed = movingSpeed*100.0;
			temp_movingSpeed = movingSpeed;

		}

		break;
	case 8:
		break;
	}


}
//----------------------------------------------------------------------------
int
main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	//glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextVersion(3, 1);
	//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	mainFrame=glutCreateWindow("Simple GLSL example");

	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);

	/*Glui initialization*/
	GLUI *glui = GLUI_Master.create_glui("Control Window");
	new GLUI_StaticText(glui,"Control Panel");

	GLUI_Panel *pnl = new GLUI_Panel(glui,"Rotation Control");

	GLUI_Button *btn = new GLUI_Button(pnl,"Start/Stop Rotation",0,glui_cb);
	btn = new GLUI_Button(pnl,"Increase Rotation Speed",1,glui_cb);
	btn = new GLUI_Button(pnl, "Decrease Rotation Speed", 2, glui_cb);

	glui->add_column_to_panel(pnl,1);

	GLUI_EditText *edit = new GLUI_EditText(pnl,"Rotation Speed:",&wrspeed,3,glui_cb);
	
	pnl = new GLUI_Panel(glui,"Scaling Control");
	btn = new GLUI_Button(pnl,"Start/Stop Scaling",4,glui_cb);

	pnl = new GLUI_Panel(glui,"Spiral Moving Control");
	btn = new GLUI_Button(pnl,"Start/Stop Spiral Moving",5,glui_cb);
	btn = new GLUI_Button(pnl,"Increase Moving Speed",6,glui_cb);
	btn = new GLUI_Button(pnl,"Decrease Moving Speed",7,glui_cb);

	glui->add_column_to_panel(pnl, 1);
	edit = new GLUI_EditText(pnl,"Spiral Moving Speed",&wr_movingSpeed,8,glui_cb);
	glutMainLoop();
	return 0;
}
