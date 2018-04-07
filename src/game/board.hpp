#pragma once

#include <cassert>
#include <array>
#include <string>

namespace Game {

	enum CubeState
	{
		Blank,
		Cross,
		Circle,
		COUNT
	};

	enum struct GameResult
	{
		Cross,
		Circle,
		Draw,
		None,
		COUNT
	};
	constexpr std::array<char*, static_cast<int>(GameResult::COUNT)> GAME_RESULT =
	{ {
		"Cross",
		"Circle",
		"Draw",
		"None"
	} };

	constexpr char* STATE_CHARS = "+xo";

	struct Action
	{
		Action() = default;
		constexpr Action(int _srcX, int _srcY, int _dstX, int _dstY)
			: srcX(_srcX),
			srcY(_srcY),
			dstX(_dstX),
			dstY(_dstY)
		{}
		constexpr Action& operator=(const Action& _oth)
		{
			srcX = _oth.srcX;
			srcY = _oth.srcY;
			dstX = _oth.dstX;
			dstY = _oth.dstY;

			return *this;
		}

		int srcX;
		int srcY;
		int dstX;
		int dstY;
	//	CubeState state;
	};

	struct Turn
	{
		Turn(const Action& _action, CubeState _newState) : action(_action), state(_newState) {}
		Action action;
		CubeState state;
	};

#define MakeActions(x,y) \
		if (x != 0) { (*this)[ind] = Action{ x,y,0,y }; ++ind; }		\
		if (x != MAX_INDEX) { (*this)[ind] = Action{ x,y,MAX_INDEX,y}; ++ind; }	\
		if (y != 0) { (*this)[ind] = Action{ x,y,x,0 }; ++ind; }				\
		if (y != MAX_INDEX) { (*this)[ind] = Action{ x,y,x,MAX_INDEX }; ++ind; }\

	// all possible actions
	template<unsigned BoardSize>
	class ActionCollection : public std::array<Action, (BoardSize-2) * 4 * 3 + 4 * 2>
	{
		constexpr static int MAX_INDEX = BoardSize - 1;
	public:
		constexpr ActionCollection()
		{
			unsigned ind = 0;

			// x borders
			for (int i = 0; i < BoardSize; ++i)
			{
				MakeActions(i, 0);
				MakeActions(i, MAX_INDEX);
			}
			// y borders without corners
			for (int i = 1; i < BoardSize-1; ++i)
			{
				MakeActions(0, i);
				MakeActions(MAX_INDEX, i);
			}
		}

	private:
	};

	template<unsigned BoardSize>
	class Board
	{
	public:
		constexpr static int MAX_INDEX = BoardSize - 1;
		constexpr static ActionCollection<BoardSize> ACTIONS{};

		Board() : m_board{} {}

		bool IsLegal(const Action& _action) const
		{
			return (_action.srcX != _action.dstX && _action.srcY == _action.dstY  
				|| _action.srcX == _action.dstX && _action.srcY != _action.dstY)
				&& (_action.srcX == 0 || _action.srcX == MAX_INDEX
				|| _action.srcY == 0 || _action.srcY == MAX_INDEX);
		}

		constexpr static std::array<uint32_t, 12> WIN_MASKS =
		{
			0b00000000000000000000000000011111,
			0b00000000000000000000001111100000,
			0b00000000000000000111110000000000,
			0b00000000000011111000000000000000,
			0b00000001111100000000000000000000,

			0b00000000000100001000010000100001,
			0b00000000001000010000100001000010,
			0b00000000010000100001000010000100,
			0b00000000100001000010000100001000,
			0b00000001000010000100001000010000,

			0b00000001000001000001000001000001,
			0b00000000000100010001000100010000,
		};

		GameResult Winner() const
		{
			unsigned crosses = 0;
			unsigned circles = 0;
			for (int x = 0; x < BoardSize; ++x)
				for (int y = 0; y < BoardSize; ++y)
				{
					crosses |= m_board[x][y] == CubeState::Cross;
					crosses <<= 1;
					circles |= m_board[x][y] == CubeState::Circle;
					circles <<= 1;
				}

			int crossLines = 0;
			int circleLines = 0;

			auto isNotZero = [](uint32_t x) {return ~(~x & (x + ~0)) >> 31; };
			for (uint32_t mask : WIN_MASKS)
			{
				crossLines += isNotZero(mask & crosses);
				circleLines += isNotZero(mask & circles);
			}
			

			if (crossLines > circleLines) return GameResult::Cross;
			else if (circleLines > crossLines) return GameResult::Circle;
			else if (circleLines) return GameResult::Draw;

			return GameResult::None;
		}

		void Move(const Action& _action, CubeState _newState)
		{
			assert(IsLegal(_action));
			Move(_action.srcX, _action.srcY, _action.dstX, _action.dstY, _newState);
		}

		void Move(int _srcX, int _srcY, int _destX, int _destY, CubeState _state)
		{
			if(_srcX  != _destX )
			{
				const int sgn = _srcX < _destX ? 1 : -1;
				for (int ix = _srcX; ix != _destX; ix += sgn)
					m_board[ix][_srcY] = m_board[ix + sgn][_srcY];
			}
			else
			{
				const int sgn = _srcY < _destY ? 1 : -1;
				for (int iy = _srcY; iy != _destY; iy += sgn)
					m_board[_srcX][iy] = m_board[_srcX][iy+sgn];
			}           
			m_board[_destX][_destY] = _state;
		}

		template<typename StreamT>
		void Print(StreamT& _stream) const
		{
			for (int y = 0; y < BoardSize; ++y)
			{
				for (int x = 0; x < BoardSize; ++x)
					_stream << STATE_CHARS[static_cast<int>(m_board[x][y])];
				_stream << "\n";
			}
		}

		CubeState Get(unsigned _idx, unsigned _idy) const { return m_board[_idx][_idy]; }
	private:
		std::array< std::array<CubeState,BoardSize>, BoardSize > m_board;
	};
}