#version 150

in  vec4 vPosition;
in vec4 vColor;
out vec4 color;

uniform float theta;
uniform float scalingRatio;
uniform float movingAngle;

void
main()
{
	mat4 scale = mat4(
	scalingRatio, 0.0, 0.0, 0.0,	
	0.0, scalingRatio, 0.0, 0.0,
	0.0, 0.0, scalingRatio, 0.0,
	0.0, 0.0, 0.0, 1.0	
	
	
	);
	mat4 rotx = mat4(
	cos(radians(theta)), -sin(radians(theta)), 0.0, 0.0,	
	sin(radians(theta)), cos(radians(theta)), 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0	
	
	);
	
	mat4 transl = mat4(
	1.0 , 0.0 , 0.0 , 0.0,
	0.0 , 1.0 , 0.0 , 0.0,
	0.0 , 0.0 , 1.0 , 0.0,
	-0.045*radians(movingAngle)*cos(radians(movingAngle)), 0.045*radians(movingAngle)*sin(radians(movingAngle)), 0.0 , 1.0
	
	);
	
	gl_Position=transl*rotx*scale*vPosition;
	
	
	color = vColor;
	
}

