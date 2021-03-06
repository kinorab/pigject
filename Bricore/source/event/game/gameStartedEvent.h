#pragma once
#include "../event.h"
#include "../../definition/gameState.h"

namespace game {
	class GameStartedEvent :
		public Event {
	public:
		explicit GameStartedEvent(const GameStateEvent::StartedEvent eventType);
		virtual void accept(EventListenerBase & visitor) override;
		virtual ~GameStartedEvent() = default;
		GameStateEvent::StartedEvent started;
	};
}