#ifndef _SHAPE_H_
#define _SHAPE_H_

class Shape
{
public:
	Shape();
	struct Vertex
	{
		float pos[4];
		float color[4];
		float uv[2];
	};
	~Shape();
private:
};

#endif 