#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class ParticleSystem : public Drawable, public Transformable {

public:

	explicit ParticleSystem(unsigned int);

	void setEmitter(Vector2f);

	void update(Time elapsed);

private:

	virtual void draw(RenderTarget & target, RenderStates states) const;

	struct Particle{

		Vector2f velocity;
		Time lifetime;
	};

	void resetParticle(size_t);

	vector<Particle>m_particles;
	VertexArray m_vertices;
	Time m_lifetime;
	Vector2f m_emitter;
};