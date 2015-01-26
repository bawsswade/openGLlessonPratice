#ifndef _STARS_H_
#define _STARS_H_

class Stars
{
public:
	Stars();
	Stars(int num);
	~Stars();

private:
	struct Vertex
	{
		float pos[4];
		float color[4];
		float uv[2];
	};
};

//void Create(int amount);

#endif