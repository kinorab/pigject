#pragma once
#include "effectKind.h"

namespace game {
	class Effect :
		public EffectKind<Effect> {
	public:
		explicit Effect(const Kind kind);
		virtual Kind getEffect() const;
		const Kind content;
		Effect(const Effect &) = delete;
		Effect &operator =(const Effect &) = delete;
	};
}