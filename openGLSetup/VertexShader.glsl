#version 400

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 vertexUV;

uniform mat4 MVP;

smooth out vec4 vertColor;
out vec2 UV;

void main()
{
	vertColor = color;
        UV = vertexUV;

        vec4 scaledPosition = MVP * position;

	gl_Position = scaledPosition;
}