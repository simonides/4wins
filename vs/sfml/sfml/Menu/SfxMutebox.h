#pragma once

namespace sf
{
	class RenderWindow;
	class RectangleShape;
	class Texture;

	template<typename T> class Vector2;
	typedef Vector2<float> Vector2f;

	template<typename T> class Rect;
	typedef Rect<int> IntRect;
}
class ResourceManager;
class SoundManager;

class SfxMutebox
{
private:
	sf::RectangleShape *shape;
	sf::IntRect *textureRect;
	sf::IntRect *textureCheckedRect;
	SoundManager *soundManager;
	bool isChecked;

public:
	explicit SfxMutebox();
	~SfxMutebox();
	void init(ResourceManager &resourceManager, SoundManager &soundManager);

	inline SfxMutebox &setPosition(const sf::Vector2f &position)
	{
		this->shape->setPosition(position);
		return *this;
	}
	inline SfxMutebox &setSize(const sf::Vector2f &size)
	{
		this->shape->setSize(size);
		return *this;
	}

	inline bool getIsChecked() const
	{
		return this->isChecked;
	}

	void update(bool isButtonRelease, const sf::Vector2f &mousePosition);
	void draw(sf::RenderWindow &window);

private:
	void hover();
	void resetHover();
	void switchState();
};