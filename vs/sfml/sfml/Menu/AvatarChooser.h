#pragma once

namespace sf
{
	class RenderWindow;
	class Font;
	class Text;
	class RectangleShape;
	class Texture;
	class Event;

	template<typename T> class Vector2;
	typedef Vector2<float> Vector2f;

	template<typename T> class Rect;
	typedef Rect<float> FloatRect;
}

namespace FourWins
{
	namespace Menu
	{
		enum Avatars
		{
			SMOOTH_STEVE,
			PROFESSOR_JENKINS,
			HIPSTER_HENRY,
			BOYBAND_BILLY,
			BOOKWORM_BETTY,
			FASHION_FABIENNE,
			HIPPIE_HILDY,
			SMOKIN_STACY
		};

		struct AvatarChoice
		{
			sf::RectangleShape *thumbnailShape;
			sf::RectangleShape *previewShape;
			sf::Text *previewText;
			sf::RectangleShape *previewTextShape;
			bool showPreview;
			bool isSelected;
			Avatars avatar;
		};

		class AvatarChooser
		{
		private:
			sf::RenderWindow *window;
			sf::FloatRect *bb;
			AvatarChoice *choices;
			const unsigned int CHOICE_COUNT;

			static const float THUMB_WIDTH;
			static const float THUMB_HEIGHT;

		public:
			AvatarChooser(sf::RenderWindow &window, unsigned int choiceCount);
			~AvatarChooser();
			void init();

			void setPosition(const sf::Vector2f position);
			Avatars getSelectedAvatar() const;
			void update(const sf::Event &e, const sf::Vector2f &mousePosition);
			void draw();

		private:
			void resetHover();
			void checkForHover(const sf::Vector2f &mousePosition);
			void checkForSelected(const sf::Vector2f &mousePosition);
		};
	}
}