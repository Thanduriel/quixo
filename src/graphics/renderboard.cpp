#include "renderboard.hpp"

namespace Graphics {

	RenderBoard::RenderBoard(int _sizeX, int _sizeY)
		: m_renderWindow()
	{
		sf::ContextSettings settings;
		settings.antialiasingLevel = 4;

		m_renderWindow.create(sf::VideoMode(_sizeX, _sizeY), "QuixoAI",
			sf::Style::Titlebar | sf::Style::Close, settings);

		m_crossTexture.loadFromFile("../content/cross.png");
		m_circleTexture.loadFromFile("../content/circle.png");
		m_blankTexture.loadFromFile("../content/blank.png");

		const int minSize = std::min(_sizeX, _sizeY);
		const int imgSize = minSize / 5;
		m_cubeSize = imgSize;
		const float scale = static_cast<float>(imgSize) / static_cast<float>(m_blankTexture.getSize().x);

		for (int iy = 0; iy < 5; ++iy)
			for (int ix = 0; ix < 5; ++ix)
			{
				sf::Sprite& sprite = m_board[ix + iy * 5];
				sprite.setTexture(m_blankTexture);
				sprite.setPosition(sf::Vector2f(static_cast<float>(ix * imgSize), 
					static_cast<float>(iy * imgSize)));
				sprite.setScale(sf::Vector2f(scale, scale));
			}

		Draw();
	}

	Game::Action RenderBoard::GetInput(const Game::Board& _state, Game::CubeState _currentPlayer)
	{
		using namespace sf;

		UpdateSprites(_state);
		

		Vector2i currentCube(0,0);
		Vector2i srcCube;
		Vector2i dstCube;
		bool pickFirst = true;

		auto Reset = [&]() 
		{
			Highlight(Color(128, 128, 128), [&](int x, int y)
			{
				return _state.IsOuter(x, y) && (_state.Get(x, y) == _currentPlayer
					|| _state.Get(x, y) == Game::CubeState::Blank);
			});
			pickFirst = true;
		};
		Reset();

		while (m_renderWindow.isOpen())
		{
			// handle events
			sf::Event event;
			while (m_renderWindow.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					m_renderWindow.close();
				Vector2i pos = Mouse::getPosition(m_renderWindow);
				pos.x /= m_cubeSize;
				pos.y /= m_cubeSize;

				if (pos != currentCube && pos.x >= 0 && pos.x < 5 && pos.y >= 0 && pos.y < 5)
				{
				//	m_board[pos.x + pos.y * 5].setColor(sf::Color(128, 128, 128));
				//	m_board[currentCube.x + currentCube.y * 5].setColor(sf::Color(255, 255, 255));
					currentCube = pos;
				}

				if (event.type == Event::MouseButtonReleased)
				{
					if (event.mouseButton.button == sf::Mouse::Right)
						Reset();
					else if (event.mouseButton.button == sf::Mouse::Left)
					{
						if (pickFirst)
						{
							pickFirst = false;
							srcCube = currentCube;
							Highlight(Color(128, 128, 128), [&](int x, int y) 
							{
								return _state.IsLegal(Game::Action(srcCube.x, srcCube.y, x, y));
							});
						}
						else
						{
							dstCube = currentCube;
							Game::Action action(srcCube.x, srcCube.y, dstCube.x, dstCube.y);
							if (_state.IsLegal(action) &&
								(_state.Get(srcCube.x, srcCube.y) == _currentPlayer
									|| _state.Get(srcCube.x, srcCube.y) == Game::CubeState::Blank))
							{
								// simulate and show new state
								Game::Board nextState(_state);
								nextState.Move(action, _currentPlayer);
								UpdateSprites(nextState);
								Highlight(Color(255, 255, 255), [](int, int) {return true; });
								Draw();
								return action;
							}
							else Reset();
						}
					}
				}
			}

			Draw();
		}
		return Game::Action();
	}

	void RenderBoard::Draw()
	{
		m_renderWindow.clear();
		for (auto& sprite : m_board)
			m_renderWindow.draw(sprite);
		m_renderWindow.display();
	}

	void RenderBoard::UpdateSprites(const Game::Board& _state)
	{
		for (int iy = 0; iy < 5; ++iy)
			for (int ix = 0; ix < 5; ++ix)
			{
				sf::Sprite& sprite = m_board[ix + iy * 5];

				const Game::CubeState state = _state.Get(ix, iy);
				if (state == Game::CubeState::Blank) sprite.setTexture(m_blankTexture);
				else if(state == Game::CubeState::Cross) sprite.setTexture(m_crossTexture);
				else if (state == Game::CubeState::Circle) sprite.setTexture(m_circleTexture);
			}
	}


}