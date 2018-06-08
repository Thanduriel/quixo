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


/*	Bots::MCTSBot<3, 50> bot1;
//	Bots::Player bot1;
	Bots::Player bot2;
	GameManager<> gameManager(bot1, bot2);
	std::cout << GAME_RESULT[static_cast<int>(gameManager.Play())];*/

		//	GameManager<Bots::MCTSBot<5, 100>, Bots::MCTSBot<5,50>> gameManager(PrintMode::None);
	Bots::MCTSBot<2, 50> bot1;
	Bots::MCTSBot<3, 100> bot3;
	Bots::RandomBot bot2;
//	Bots::RandomBot bot4;
	Bots::MCTSBot<2, 100> bot4;
//	Bots::MCTSBot<2, 200> bot5;
//	Bots::MCTSBot<2, 400> bot6;
//	GameManager<> gameManager(bot4, bot2, PrintMode::None);
//	auto res = gameManager.Play(50);
//	std::cout << res[0] << ", " << res[1] << ", " << res[2];
	Eval::League league;
	league.Add(bot1);
	league.Add(bot2);
	league.Add(bot4);
//	league.Add(bot5);
//	league.Add(bot6);
	league.Run(1000);

	/*MCTSBot<2, 50>   484.292
Random   -121.413
MCTSBot<2, 100>   531.544
MCTSBot<2, 200>   545.54
MCTSBot<2, 400>   560.037*/
	/*
		MCTSBot<3, 50>   528.188
		Random   -201.838
		MCTSBot<3, 100>   547.518
		MCTSBot<5, 100>   396.615
		MCTSBot<2, 100>   716.646
		MCTSBot<4, 100>   412.871
	*/
	int i;
	std::cin >> i;
	return 0;
}