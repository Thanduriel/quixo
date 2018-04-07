#include "game/board.hpp"
#include "game/game.hpp"
#include "bots/player.hpp"
#include "bots/mctsbot.hpp"
#include "bots/randombot.hpp"
#include "utils/threadpool.hpp"

#include <iostream>

//Utils::ThreadPool Utils::g_threadPool(2);

int main()
{
	using namespace Game;
	using namespace Bots;

/*	Board<5> board;
	board.Move(0, 0, 4, 0, CubeState::Cross);
	board.Print(std::cout);
	std::cout << std::endl;
	board.Move(0, 0, 4, 0, CubeState::Circle);
	board.Move(0, 0, 4, 0, CubeState::Cross);
	board.Move(3, 4, 3, 0, CubeState::Circle);
	board.Move(3, 4, 3, 0, CubeState::Cross);
	board.Move(0, 2, 4, 2, CubeState::Cross);
	board.Print(std::cout);

	for(int i = 0; i < 5; ++i)
		board.Move(0, 4, 4, 4, CubeState::Circle);
	board.Print(std::cout);
	std::cout << (board.Winner() == GameResult::Circle);*/
	//std::cout << sizeof(Board<5>) << "  " << sizeof(MCTSBot<5>::Node) << std::endl;
	
//	GameManager<Bots::Player, Bots::MCTSBot<>> gameManager;
//	std::cout << GAME_RESULT[static_cast<int>(gameManager.Play())];

	GameManager<Bots::RandomBot, Bots::MCTSBot<5,500>> gameManager;
	auto res = gameManager.Play(50);
	std::cout << res[0] << ", " << res[1] << ", " << res[2];

	int i;
	std::cin >> i;
	return 0;
}