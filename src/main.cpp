#include "game/board.hpp"
#include "game/game.hpp"
#include "bots/player.hpp"
#include "bots/mctsbot.hpp"
#include "bots/randombot.hpp"
#include "utils/threadpool.hpp"
#include "eval/league.hpp"

#include <iostream>

//Utils::ThreadPool Utils::g_threadPool(2);

int main()
{
	using namespace Game;
	using namespace Bots;

	/*	Board board;
		board.Move(Action(1, 0, 4, 0), CubeState::Cross);
		board.Move(Action(1, 0, 0, 0), CubeState::Cross);
		board.Print(std::cout);
		board.Move(Action(4, 0, 0, 0), CubeState::Cross);
		board.Move(Action(4, 2, 0, 2), CubeState::Cross);
		board.Move(Action(4, 2, 0, 2), CubeState::Circle);
		board.Move(Action(4, 2, 0, 2), CubeState::Cross);
		board.Move(Action(0, 3, 4, 3), CubeState::Cross);
		board.Move(Action(0, 3, 4, 3), CubeState::Cross);
		board.Print(std::cout);
		std::cout << std::endl;
		board.Move(Action(1, 4, 1, 0), CubeState::Cross);
		board.Move(Action(1, 4, 1, 0), CubeState::Cross);
		board.Move(Action(4, 4, 4, 0), CubeState::Cross);
		board.Print(std::cout);
		std::cout << std::endl;
		board.Move(Action(2, 0, 2, 4), CubeState::Cross);
		board.Move(Action(2, 0, 2, 4), CubeState::Cross);
		board.Print(std::cout);
		std::cout << std::endl;
		std::cout << (board.Winner() == GameResult::Cross);
		board.Move(Action(2, 0, 2, 4), CubeState::Cross);
		std::cout << (board.Winner() == GameResult::Cross);*/


//	Bots::MCTSBot<5, 50> bot1;
/*	Bots::Player bot1;
	Bots::Player bot2;
	GameManager<> gameManager(bot1, bot2);
	std::cout << GAME_RESULT[static_cast<int>(gameManager.Play())];*/

		//	GameManager<Bots::MCTSBot<5, 100>, Bots::MCTSBot<5,50>> gameManager(PrintMode::None);
	Bots::MCTSBot<5, 500> bot1;
	Bots::MCTSBot<5, 1000> bot3;
	Bots::RandomBot bot2;
	Bots::RandomBot bot4;
	GameManager<> gameManager(bot4, bot2, PrintMode::None);
	auto res = gameManager.Play(50);
	std::cout << res[0] << ", " << res[1] << ", " << res[2];
	Eval::League league;
	league.Add(bot1);
	league.Add(bot2);
	league.Add(bot3);
	league.Run(200);

	int i;
	std::cin >> i;
	return 0;
}