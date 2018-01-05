#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class ParticleSystem : public Drawable, public Transformable {

public:

	explicit ParticleSystem(unsigned int);

	void setEmitter(Vector2f);
	void update(const float &timeSpan, const bool &light);

private:

	virtual void draw(RenderTarget & target, RenderStates states) const;
	void resetParticle(size_t);

	struct Particle {
		Vector2f velocity;
		float lifetime;
	};
	vector<Particle>m_particles;
	VertexArray m_vertices;
	float m_lifetime;
	Vector2f m_emitter;
};
