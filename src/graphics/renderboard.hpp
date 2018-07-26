#pragma once

#include "SFML/Graphics.hpp"
#include "game/board.hpp"

namespace Graphics {

	class RenderBoard 
	{
	public:
		RenderBoard(int _sizeX = 768, int _sizeY = 768);

		Game::Action GetInput(const Game::Board& _state, Game::CubeState _currentPlayer);

		// Display the given state and highlight winning lines
		void ShowEndState(const Game::Board& _state, Game::CubeState _player);

		// Estimate win-rates from random play-out.
		void ShowWinrates(const Game::Board& _state, Game::CubeState _player);
	private:
		void Draw();
		void UpdateSprites(const Game::Board& _state);
		
		template<typename Pred>
		void Highlight(const sf::Color& _color, Pred _predicate)
		{
			for (int iy = 0; iy < 5; ++iy)
				for (int ix = 0; ix < 5; ++ix)
					m_board[ix + iy * 5].setColor(_predicate(ix, iy) ? _color 
						: sf::Color(255,255,255));
		}

		sf::RenderWindow m_renderWindow;
		sf::Texture m_crossTexture;
		sf::Texture m_circleTexture;
		sf::Texture m_blankTexture;

		std::array<sf::Sprite, 5 * 5> m_board;
		std::array<sf::Text, 5 * 5> m_fieldTexts;
		sf::Font m_font;
		bool m_showText;
		int m_cubeSize;
	};
}