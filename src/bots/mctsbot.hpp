#pragma once

#include "game/board.hpp"
#include "bot.hpp"
#include "utils/random.hpp"
#include "utils/stackalloc.hpp"
#include <memory>
#include <vector>
#include <chrono>
#include <iostream>

namespace Bots {

	template<unsigned MaxDepth = 5, unsigned TurnTime = 2500>
	class MCTSBot : public BasicBot
	{
	public:
		struct Node
		{
			// for the root node
			Node() : action(nullptr), winner(Game::GameResult::None) {}
			Node(const Game::Board<5>& _board, const Game::Action* _action, Node* _parent, Game::CubeState _player)
				: boardState(_board),
				action(_action),
				parent(_parent),
				player(_player)
			{
				boardState.Move(*action, _player);
				winner = boardState.Winner();
			}
			int numSimulations = 1; // init with non zero to prevent div by zero
			int numWins = 0;
			const Game::Action* action;
			Game::CubeState player;
			Game::Board<5> boardState;

			Node* parent = nullptr;
			Node* childs = nullptr;
			int numChilds;
			Game::GameResult winner;
		};
	public:
		using BasicBot::BasicBot;

		Game::Turn Step(const Game::Board<5>& _state)
		{
			m_allocator.reset();
			m_numSimulations = 1;

			Node root;
			root.boardState = _state;
			// first real move is from the player
			root.player = GetOther(m_symbol);
			auto& [nodes, count] = Expand(root);
			root.childs = nodes;
			root.numChilds = count;
			
			auto begin = std::chrono::steady_clock::now();
			do{
				// descend to a leaf
				Node* n = &root;
				unsigned depth = 0;
				do {
					n = &Select(*n);
					++depth;
				} while (n->childs && depth < MaxDepth);

				// expand, choose random child node
				if (depth < MaxDepth)
				{
					auto&[nodes, count] = Expand(*n);
					n->childs = nodes;
					n->numChilds = count;

					n = &nodes[Utils::g_random.Uniform(0, count - 1)];
				}

				//play out
				using namespace Game;
				GameResult result = Playout(*n);
				CubeState winner;
				if (result == GameResult::Cross) winner = CubeState::Cross;
				else if (result == GameResult::Circle) winner = CubeState::Circle;
				else winner = CubeState::Blank;

				//propagate result back
				while(n->parent)
				{
					if (n->player == winner) ++n->numWins;
					++n->numSimulations;
					n = n->parent;
				}

				++m_numSimulations;
			} while ((std::chrono::steady_clock::now() - begin) < std::chrono::milliseconds(TurnTime));
			std::cout << "explored paths: " << m_numSimulations << std::endl;

			Node* it = std::max_element(root.childs, root.childs + root.numChilds, [](const Node& lhs, const Node& rhs)
			{
				return lhs.numSimulations < rhs.numSimulations;
			});
			return Game::Turn(*it->action, m_symbol);
		}

	private:
		// builds all legal moves
		std::pair<Node*, int> Expand(Node& _node)
		{
			using namespace Game;
			Node* nodes = static_cast<Node*> (m_allocator.alloc(sizeof(Node) * Board<5>::ACTIONS.size() - 2));
			unsigned ind = 0;

			for (const Action& a : Board<5>::ACTIONS)
			{
				if (_node.boardState.Get(a.srcX, a.srcY) == _node.player)
					continue;

				new (nodes + ind) (Node)(_node.boardState, &a, &_node,
					GetOther(_node.player));
				++ind;
			}
			
			return std::make_pair(nodes, ind);
		}

		// returns the node that should be expanded
		Node& Select(Node& _parent)
		{
			float maxVal = 0.f;
			Node* n;

			for (int i = 0; i < _parent.numChilds; ++i)
			{
				Node& c = _parent.childs[i];
				float v = static_cast<float>(c.numWins) / c.numSimulations;
				v += 1.4f * std::sqrt(static_cast<float>(m_numSimulations) / c.numSimulations);
				if (v > maxVal)
				{
					maxVal = v;
					n = &c;
				}
			}

			return *n;
		}

		Game::GameResult Playout(Node& _node)
		{
			using namespace Game;

			auto board = _node.boardState;

			GameResult result;
			do {
				CubeState currentPlayer = _node.player;
				
				// pick random action
				const Action* act;
				do {
					int ind = Utils::g_random.Uniform(0, static_cast<int>(Board<5>::ACTIONS.size()-1));
					act = &Board<5>::ACTIONS[ind];
				} while (board.Get(act->srcX, act->srcY) == currentPlayer);
				currentPlayer = currentPlayer == CubeState::Cross ? CubeState::Circle : CubeState::Cross;
				board.Move(*act, currentPlayer);
				result = board.Winner();

			} while (result == GameResult::None);

			return result;
		}

		int m_numSimulations;
		Utils::StackAllocator<32768> m_allocator;
	};
}