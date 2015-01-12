#version 400
smooth in vec4 vertColor;
in vec2 UV;

out vec4 outputColor;

uniform sampler2D myTetureSampler;

void main()
{
	outputColor = texture(myTextureSampler, UV).rgba * vetColor;
	//outputCOlor = vertColor;
}