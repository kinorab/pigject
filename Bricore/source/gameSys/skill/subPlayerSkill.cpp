#include "subPlayerSkill.h"
#include "skillHandler.h"
#include "../bar/energyBar.h"
#include "../../manager/textureManager.h"
#include "../../definition/gameState.h"
#include "../../event/eventListener.h"
#include "../../stuff/subPlayer.h"
#include <SFML/Graphics.hpp>

using namespace game;

size_t SubPlayerSkill::uMaxCarry(1);
size_t SubPlayerSkill::uCurrentCarry(0);
size_t SubPlayerSkill::uMaxOnField(2);
size_t SubPlayerSkill::uCurrentOnField(0);
SkillHandler<SubPlayerSkill> SubPlayerSkill::handler;
std::map<SubPlayerSkill::Kind, std::map<SubPlayerSkill::State, std::string>> SubPlayerSkill::fileNames({
	std::pair(ForceField
		, std::map<SubPlayerSkill::State, std::string>{
			std::pair(State::OnCharging, "pioneerSkill/forcefield_onCharging")
			, std::pair(State::OnFirstField, "pioneerSkill/forcefield_onFirstField")
			, std::pair(State::OnSecondField, "pioneerSkill/forcefield_onSecondField")
			, std::pair(State::OnThirdField, "pioneerSkill/forcefield_onThirdField")
			, std::pair(State::OnFourthField, "pioneerSkill/forcefield_onFourthField")
			, std::pair(State::Using, "pioneerSkill/forcefield_using")
			, std::pair(State::Display, "pioneerSkill/forcefield_display")
		})
	});

SubPlayerSkill::SubPlayerSkill(const Kind skillName, const sf::Time duration
	, std::vector<std::pair<Effect::Kind, bool>> && effects, std::vector<Attribute::Kind> && attributes
	, const bool exist, const std::shared_ptr<EnergyBar> energyBar, const SubPlayer * subPlayer)
	: skill(SkillContent{ skillName, State::Waiting, nullptr })
	, SkillSystem(duration, exist)
	, origin()
	, bInitialize(false)
	, bTypeSkill(false)
	, m_energyBar(std::move(energyBar))
	, c_subPlayer(subPlayer) {
	// initialize effects
	std::for_each(effects.begin(), effects.end(), [&](const std::pair<Effect::Kind, bool> & effect) {
		skillEffects.push_back(std::make_shared<Effect>(effect.first, effect.second));
	});
	// initialize attributes
	std::for_each(attributes.begin(), attributes.end(), [&](const Attribute::Kind element) {
		skillAttributes.push_back(std::make_shared<Attribute>(element));
	});
}

void SubPlayerSkill::initialize() {
	if (bInitialize) return;
	handler.insert(shared_from_this());
	addListener(std::make_shared<EventListener<KeyPressedEvent>>([this](auto & event) { onKeyPressed(event); }));
	addListener(std::make_shared<EventListener<MousePressedEvent>>([this](auto & event) { onMousePressed(event); }));
	addListener(std::make_shared<EventListener<GameStartedEvent>>([this](auto & event) { onGameStarted(event); }));
	addListener(std::make_shared<EventListener<GameReadyEvent>>([this](auto & event) { onGameReady(event); }));
	addListener(std::make_shared<EventListener<GameFinishedLevelEvent>>([this](auto & event) { onGameFinishedLevel(event); }));
	addListener(std::make_shared<EventListener<GamePreparedEvent>>([this](auto & event) { onGamePrepared(event); }));
	bInitialize = true;
}

void SubPlayerSkill::update() {
	if (status != Status::Selected) return;
	using namespace std::placeholders;
	State currentState = skill.currentState;
	switch (currentState) {
	case State::OnCharging:
		if (!m_energyBar->isFull() || bLocked) break;
		if (handler.tryEnterField(std::bind(&SubPlayerSkill::setState, this, _1))) {
			m_energyBar->clear();
		}
		break;
	case State::OnSecondField:
		if (bLocked) break;
		handler.tryForward(currentState, std::bind(&SubPlayerSkill::setState, this, _1));
		break;
	case State::OnThirdField:
		if (bLocked) break;
		handler.tryForward(currentState, std::bind(&SubPlayerSkill::setState, this, _1));
		break;
	case State::OnFourthField:
		if (bLocked) break;
		handler.tryForward(currentState, std::bind(&SubPlayerSkill::setState, this, _1));
		break;
	case State::Waiting:
		handler.tryAppear(shared_from_this());
		break;
	case State::Using:
		SkillSystem::useSkill();
		if (SkillSystem::elapsed()) {
			SkillSystem::exhausted();
			setState(State::Waiting);
		}
		break;
	default:
		break;
	}
}

bool SubPlayerSkill::containsPoint(const sf::Vector2f & point) const {
	if (skill.currentState == State::Waiting) return false;
	return getGlobalBounds().contains(point);
}

std::shared_ptr<sf::Drawable> SubPlayerSkill::getDrawable() const {
	return std::const_pointer_cast<sf::Drawable>(std::static_pointer_cast<const sf::Drawable>(shared_from_this()));
}

void SubPlayerSkill::setPosition(const sf::Vector2f & position) {
	skill.context->setPosition(position);
}

void SubPlayerSkill::setPosition(const float x, const float y) {
	skill.context->setPosition(x, y);
}

void SubPlayerSkill::setOrigin(const sf::Vector2f & origin) {
	this->origin = origin;
}

void SubPlayerSkill::setOrigin(const float x, const float y) {
	origin = { x, y };
}

void SubPlayerSkill::extendCarry(const size_t number) {
	uMaxCarry += number;
}

void SubPlayerSkill::setOwnToPlayer(const bool giveOwn) {
	if (!isExist()) throw std::invalid_argument("Skill not exist in setOwn.");
	if (giveOwn) {
		status = UnSelected;
	}
	else {
		status = None;
	}
}

void SubPlayerSkill::extendField(const size_t number) {
	if (number + uMaxOnField > static_cast<size_t> (maxField)) {
		throw std::out_of_range("Excess max field number");
	}
	uMaxOnField += number;
}

void SubPlayerSkill::setState(const State nextState) {
	// skip if same
	if (skill.currentState == nextState) return;
	// set field number
	if (skill.currentState == State::OnCharging) {
		if (uCurrentOnField > uMaxOnField) throw std::invalid_argument("Too many skills on field.");
		++uCurrentOnField;
	}
	else if (nextState == State::Using) {
		if (uCurrentOnField == 0) throw std::invalid_argument("Field not have any skills.");
		--uCurrentOnField;
	}
	// set skill context
	if (nextState == State::Waiting) {
		skill.context = nullptr;
	}
	else {
		auto stateTexture = TextureManager::getInstance().get(fileNames[skill.name][skill.currentState]);
		skill.context.reset(new sf::Sprite(*stateTexture));
		skill.context->setOrigin(origin);
		// show gray filter if player no own this skill
		if (nextState == State::Display && status == Status::None) {
			skill.context->setColor(sf::Color(150, 150, 150, 180));
		}
		else {
			skill.context->setColor(sf::Color::White);
		}
	}
	// set skill state
	skill.currentState = nextState;
}

void SubPlayerSkill::swapSkill(const std::shared_ptr<SubPlayerSkill> & other) {
	State state = skill.currentState;
	State inputState = other->skill.currentState;
	std::swap(skill, other->skill);
	setState(inputState);
	other->setState(state);
}

size_t SubPlayerSkill::getMaxCarry() {
	return uMaxCarry;
}

size_t SubPlayerSkill::getCurrentCarry() {
	return uCurrentCarry;
}

size_t SubPlayerSkill::getMaxOnField() {
	return uMaxOnField;
}

size_t SubPlayerSkill::getCurrentOnField() {
	return uCurrentOnField;
}

bool SubPlayerSkill::isOwnToPlayer() const {
	if (status == Status::None) return false;
	return true;
}

bool SubPlayerSkill::isInitialize() const {
	return bInitialize;
}

SubPlayerSkill::State SubPlayerSkill::getState() const {
	return skill.currentState;
}

SkillKind<SubPlayerSkill>::Kind SubPlayerSkill::getSkillName() const {
	return skill.name;
}

sf::FloatRect SubPlayerSkill::getLocalBounds() const {
	return skill.context->getLocalBounds();
}

sf::FloatRect SubPlayerSkill::getGlobalBounds() const {
	return skill.context->getGlobalBounds();
}

SubPlayerSkill::~SubPlayerSkill() {
}

void SubPlayerSkill::onKeyPressed(KeyPressedEvent & event) {
	if (skill.currentState == State::Display || skill.currentState == State::Prepare) return;
	if (skill.currentState == State::OnFirstField) {
		if (bSilenced) return;
		if (event.pressed.code == c_subPlayer->getKey().turnSkillToTypeSkill) {
			bTypeSkill = true;
		}
		if (event.pressed.code == c_subPlayer->getKey().subSkill || c_subPlayer->isAutoUse()) setState(State::Using);
	}
	if (event.pressed.code != c_subPlayer->getKey().subSkillSwap) return;
	if (handler.trySwap()) {

	}
	else {

	}
}

void SubPlayerSkill::onMousePressed(MousePressedEvent & event) {
	if (skill.currentState != State::Prepare) return;
	// left mouse to choose
	if (uCurrentCarry < uMaxCarry && SkillSystem::selectOn() && event.pressed.button == sf::Mouse::Left) {
		skill.context->setColor(sf::Color::White);
		++uCurrentCarry;
		return;
	}
	// left or right mouse to cancel choose
	if (SkillSystem::selectOff() &&
		(event.pressed.button == sf::Mouse::Left || event.pressed.button == sf::Mouse::Right)) {
		skill.context->setColor(sf::Color(180, 180, 180));
		--uCurrentCarry;
		return;
	}
}

void SubPlayerSkill::onGameStarted(GameStartedEvent & event) {
	update();
}

void SubPlayerSkill::onGameReady(GameReadyEvent & event) {
	setState(State::Waiting);
}

void SubPlayerSkill::onGameFinishedLevel(GameFinishedLevelEvent & event) {
	setState(State::Display);
}

void SubPlayerSkill::onGamePrepared(GamePreparedEvent & event) {
	setState(State::Prepare);
}

void SubPlayerSkill::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	if (skill.currentState == State::Waiting) return;
	target.draw(*skill.context, states);
}
