#include "game.h"
#include "manager/audioManager.h"
#include "definition/gameState.h"
#include "definition/utility.h"
#include "stage.h"
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <future>

Game::Game() :
	stage(new game::Stage),
	mouseHandler({ static_cast<int>(GAME_WIDTH), static_cast<int>(GAME_HEIGHT) }) {
	window.reset(new sf::RenderWindow(sf::VideoMode(static_cast<size_t>(GAME_WIDTH), static_cast<size_t>(GAME_HEIGHT)),
		"BricksEx", sf::Style::Close, graph.getSettings()));
}

Game::~Game() = default;

void Game::run() {
	settleWindow();
	AudioManager::getInstance().initialize();
	std::future<void> renderThread = std::async(std::launch::async, &Game::renderFunc, this);
	sf::Event nextEvent;
	while (renderThread.wait_for(std::chrono::seconds(0)) != std::future_status::ready && window->waitEvent(nextEvent)) {
		eventQueue.push(nextEvent);
	}
	window->close();
}

void Game::settleWindow() {
	window->setMouseCursorVisible(false);
	window->setVerticalSyncEnabled(true);
	window->setPosition(sf::Vector2i(window->getPosition().x, 20));
	ImmAssociateContext(window->getSystemHandle(), 0);
	//window.setIcon(graph.getIconSize().x, graph.getIconSize().y, graph.getIcon());
	window->setActive(false);
}

void Game::renderFunc() {
	float elapsed = 0;
	float renderElapsed = 0;
	sf::Clock clock;
	for (bool finishing = false; !finishing;) {
		// display in milliseconds
		constexpr float updateSpan = 13.f;
		const float distribute = clock.restart().asSeconds() * 1000.f;
		// maximum elapsed cap
		handleEvents(finishing);
		elapsed = std::min<float>(elapsed + distribute, updateSpan * 1.5f);
		while (elapsed > 0.0f) {
			float updateRatio = std::min<float>(elapsed, updateSpan) / updateSpan;
			stage->update(updateRatio);
			elapsed -= updateSpan * updateRatio;
		}
		// max fixed at 1.5x current fps
		renderElapsed = std::min<float>(renderElapsed + distribute, graph.getFrameSpan() * 1.5f);
		if (renderElapsed >= graph.getFrameSpan()) {
			window->draw(*stage);
			window->display();
			renderElapsed -= graph.getFrameSpan();
		}
	}
	// finalize...
	window->setActive(false);
}

void Game::handleEvents(bool & finishing) {
	while (!eventQueue.empty()) {
		sf::Event currentEvent = eventQueue.pop();
		mouseHandler.handle(currentEvent, *stage);
		keyboardHandler.handle(currentEvent, *stage);
		if (currentEvent.type == sf::Event::Closed) {
			finishing = true;
		}
	}
}