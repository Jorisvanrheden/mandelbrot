#version 120

uniform vec2 relativeX;
uniform vec2 relativeY;
uniform vec2 relativeC;
uniform vec2 screenSize;

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
	while (n < max)
	{
		float aa = a*a - b*b;
		float bb = 2 * a*b;

		a = aa + cx;
		b = bb + cy;
			
		if (a + b > 16)
		{
			break;
		}

		n++;
	}
	
	return n;
}

void main()
{		
	int maxIterations = 1000;
	
	int x = int(gl_FragCoord.x);
	int y = int(gl_FragCoord.y);
	int n = compute_iterations(maxIterations, x, y);

	float color = map(n, 0, maxIterations, 1, 0);
	float redshift = gl_FragCoord.x/screenSize.x;

    // multiply it by the color
    gl_FragColor = vec4(color,color,color,1);
}
