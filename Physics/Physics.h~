#ifndef _PHYSICS
#define _PHYSICS
#include <Box2D/Box2D.h>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "convexhull.h"
#include "Const.h"
#include <algorithm>

using namespace std;

class Simulator
{
private:
public:
	b2World *world;
	b2Body* addRect(int x,int y,int w,int h,bool dyn=true, double friction = 0.5)
	{
		b2BodyDef bodydef;
		bodydef.position.Set(x*P2M,y*P2M);
		if(dyn)
			bodydef.type=b2_dynamicBody;
		bodydef.linearDamping = 0.000f;
		bodydef.angularDamping = 0.000f;

		b2Body* body=world->CreateBody(&bodydef);

		b2PolygonShape shape;
		shape.SetAsBox(P2M*w/2,P2M*h/2);

		b2FixtureDef fixturedef;
		fixturedef.shape=&shape;
		fixturedef.density=1.0;
		fixturedef.restitution = 0.5;
		fixturedef.friction = friction;
		body->CreateFixture(&fixturedef);
		return NULL;
	}

	/*return true if the polygon is valid*/
	void addPolygon(vector<b2Vec2> points, bool canMove, double density = 1, double friction = 0.00)
	{
		fprintf(stderr, "ADD POLYGON\n");
		for (int i = 0; i < points.size(); ++i)
			points[i].x *= P2M, points[i].y *= P2M;
		points = convexhull(points);
		fprintf(stderr, "check return\n");
		if (points.size() < 3)
			return;
		fprintf(stderr, "convexhull SUCCESS %d\n", (int)points.size());

		b2BodyDef bodydef;
		b2Vec2 Gcenter;
		Gcenter = b2Vec2(0, 0);
		float area = 0;
		for (int i = 1; i < (int)points.size() - 1; ++i)
		{
			float now_area = det(points[i] - points[0], points[i + 1] - points[0]);
			area += now_area;
			b2Vec2 now_point = points[0] + points[i] + points[i + 1];
			now_point.x *= now_area / 3;
			now_point.y *= now_area / 3;
			Gcenter = Gcenter + now_point;
		}
		Gcenter.x /= area, Gcenter.y /= area;
		bodydef.position.Set(Gcenter.x, Gcenter.y);
		if(canMove)
			bodydef.type=b2_dynamicBody;
		bodydef.linearDamping = 0;
		bodydef.angularDamping = 0;

		b2Body* body=world->CreateBody(&bodydef);

		fprintf(stderr, "fuck1\n");

		b2PolygonShape shape;
		b2Vec2 *f = new b2Vec2[points.size()];
		for(int i = 0; i < points.size(); ++i)
		{
			f[i] = points[i] - Gcenter;
		}
		fprintf(stderr, "start Set\n");
		shape.Set(f, points.size());
		fprintf(stderr, "end Set\n");
		delete f;

		fprintf(stderr, "fuck2\n");

		b2FixtureDef fixturedef;
		fixturedef.shape=&shape;
		fixturedef.density=density;
		fixturedef.restitution = 0.5;
		fixturedef.friction = friction;
		body->CreateFixture(&fixturedef);

        fprintf(stderr, "ADD POLYGON SUCCESS\n");
	}

	void init(const vector<pair<vector<b2Vec2>, bool> > &GameMap, const vector<pair<b2Vec2, double> > &field)
	{
		addRect(WIDTH/2,0,WIDTH,10,false);
		addRect(WIDTH/2,HEIGHT-10,WIDTH,20,false);
		addRect(3,0,17,HEIGHT * 10,false);
		addRect(WIDTH,0,10,HEIGHT * 10,false);
		for(int i = 0; i < GameMap.size(); ++i)
			addPolygon(GameMap[i].first, GameMap[i].second);
	}
	Simulator()
	{
	}
	~Simulator()
	{
		delete world;
		world = NULL;
	}

	void create(vector<pair<vector<b2Vec2>, bool> > GameMap, vector<pair<b2Vec2, double> > field)
	{
		world = new b2World(b2Vec2(0, g), false);
		init(GameMap, field);
	}

	void destroy()
	{
		delete world;
		world = NULL;
	}

	/*The return value should be a image*/
	void simulateNextStep()
	{
		world -> Step(1.0 / 60, 8, 8);
	}
};

#endif
