#include "renderboard.hpp"
#include <string>
#include "bots/mctsbot.hpp"

namespace Graphics {

	using namespace sf;

	RenderBoard::RenderBoard(int _sizeX, int _sizeY)
		: m_renderWindow(),
		m_showText(false)
	{
		sf::ContextSettings settings;
		settings.antialiasingLevel = 4;

		m_renderWindow.create(sf::VideoMode(_sizeX, _sizeY), "QuixoAI",
			sf::Style::Titlebar | sf::Style::Close, settings);

		m_crossTexture.loadFromFile("../content/cross.png");
		m_circleTexture.loadFromFile("../content/circle.png");
		m_blankTexture.loadFromFile("../content/blank.png");
		m_font.loadFromFile("../content/OpenSans.ttf");

		const int minSize = std::min(_sizeX, _sizeY);
		const int imgSize = minSize / 5;
		m_cubeSize = imgSize;
		const float scale = static_cast<float>(imgSize) / static_cast<float>(m_blankTexture.getSize().x);

		for (int iy = 0; iy < 5; ++iy)
			for (int ix = 0; ix < 5; ++ix)
			{
				sf::Sprite& sprite = m_board[ix + iy * 5];
				sprite.setTexture(m_blankTexture);
				sf::Vector2f pos = sf::Vector2f(static_cast<float>(ix * imgSize),
					static_cast<float>(iy * imgSize));
				sprite.setPosition(pos);
				sprite.setScale(sf::Vector2f(scale, scale));

				sf::Text& text = m_fieldTexts[ix + iy * 5];
				text.setFont(m_font);
				text.setPosition(pos);
				text.setString("");
				text.setCharacterSize(24);
			}

		Draw();
	}

	Game::Action RenderBoard::GetInput(const Game::Board& _state, Game::CubeState _currentPlayer)
	{
		using namespace sf;

		m_showText = false;
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
				else if (event.type == sf::Event::KeyReleased
					&& event.key.code == sf::Keyboard::C
					&& !m_showText)
				{
					ShowWinrates(_state, _currentPlayer);
				}
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
			sf::sleep(sf::milliseconds(16));
		}
		return Game::Action();
	}

	void RenderBoard::ShowEndState(const Game::Board& _state, Game::CubeState _player)
	{
		using namespace std;
		m_renderWindow.setTitle("QuixoAI - last winner: "s 
			+ Game::GAME_RESULT[static_cast<int>(_state.Winner())]);

		UpdateSprites(_state);

		Highlight(sf::Color(255, 128, 128), [&](int x, int y) mutable
		{
			Game::Board state = _state;
			state.Set(x, y, Game::CubeState::Blank);
			return state.Winner() == Game::GameResult::None; 
		});

		Draw();

		while (m_renderWindow.isOpen())
		{
			sf::Event event;
			while (m_renderWindow.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					m_renderWindow.close();
				else if (event.type == sf::Event::Event::MouseButtonReleased)
					return;
			}
			sf::sleep(sf::milliseconds(16));
		}
	}

	void RenderBoard::ShowWinrates(const Game::Board& _state, Game::CubeState _player)
	{
		std::vector<std::pair<Game::Action, float>> winrates;

		Bots::MCTSBot<3, 3000> predictor;
		predictor.SetSymbol(_player);
		auto root = predictor.BuildTree(_state);

		for (int i = 0; i < root.numChilds; ++i)
		{
			auto& node = root.childs[i];
			winrates.emplace_back(*node.action, node.numWins / node.numSimulations);
		}

		std::array<float, 5 * 5> currentMax{-1.f};
		std::array<float, 5 * 5> currentMaxDst{ -1.f };
		for (const auto&[action, winrate] : winrates)
		{
			const int indSrc = action.srcX + action.srcY * 5;
			if (winrate > currentMax[indSrc]) currentMax[indSrc] = winrate;

			const int indDst = action.dstX + action.dstY* 5;
			if (winrate > currentMaxDst[indDst]) currentMaxDst[indDst] = winrate;
		}

		for (int i = 0; i < 5 * 5; ++i)
		{
			if (currentMax[i] != -1.f)
			{
				std::string srcWin = std::to_string(currentMax[i]);
				srcWin.resize(4);
				std::string dstWin = std::to_string(currentMaxDst[i]);
				dstWin.resize(4);
				m_fieldTexts[i].setString(srcWin + " / " + dstWin);
			}
		}

		m_showText = true;
	}

	void RenderBoard::Draw()
	{
		m_renderWindow.clear();
		for (auto& sprite : m_board)
			m_renderWindow.draw(sprite);
		if (m_showText)
		{
			for (auto& text : m_fieldTexts)
				m_renderWindow.draw(text);
		}
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