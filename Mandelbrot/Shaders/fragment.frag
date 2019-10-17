#version 120

uniform vec2 relativeX;
uniform vec2 relativeY;
uniform vec2 relativeC;
uniform vec2 screenSize;
uniform int maxIterations;

float map(float value, float realMin, float realMax, float mappedMin, float mappedMax)
{
	float realPercentage = (value - realMin) / (realMax - realMin);

	float additionToMapped = realPercentage * (mappedMax - mappedMin);
	return mappedMin + additionToMapped;
}

int compute_iterations(int max, int i, int j)
{	
	float a = map(i, 0, screenSize.x, relativeX.x, relativeX.y);
	float b = map(j, 0, screenSize.y, relativeY.x, relativeY.y);
	
	float originalA = a;
	float originalB = b;
	
	float cx = relativeC.x;
	float cy = relativeC.y;

	int n = 0;
	while (a*a + b*b < 4 && n < max)
	{
		float aa = a*a - b*b;
		float bb = 2 * a*b;

		//Original
		a = aa + originalA;
		b = bb + originalB;		
		
		//Julia subset
		//a = aa + cx;
		//b = bb + cy;	

		n++;
	}
	
	return n;
}

void main()
{			
	int x = int(gl_FragCoord.x);
	int y = int(gl_FragCoord.y);
	int n = compute_iterations(maxIterations, x, y);
	
	vec4 color = vec4(1,0,0,1);
	if(n <= maxIterations)
	{
		float quotient = float(n)/float(maxIterations);
		
		if(quotient>0.5)
		{
			color = vec4(0, quotient, quotient,1);
		}
		else
		{
			color = vec4(0, quotient, 0, 1);
		}
		color = vec4(quotient,quotient,quotient, quotient);
	}
	
    // multiply it by the color
    gl_FragColor = color;
}
