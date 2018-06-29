#pragma once

#include <cassert>
#include <array>
#include <string>

namespace Game {

	enum CubeState
	{
		Blank = 0,
		Cross = 1,
		Circle = 2,
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
	constexpr std::array<const char*, static_cast<int>(GameResult::COUNT)> GAME_RESULT =
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

	class Board
	{
	public:
		constexpr static int BoardSize = 5;
		constexpr static int MAX_INDEX = BoardSize - 1;
		constexpr static ActionCollection<BoardSize> ACTIONS{};

		Board() : m_crosses(0), m_circles(0) {}

		bool IsLegal(const Action& _action) const;

		GameResult Winner() const;

		void Move(const Action& _action, CubeState _newState);

		void Move(int _srcX, int _srcY, int _destX, int _destY, uint32_t& _board, uint32_t _newState);

		template<typename StreamT>
		void Print(StreamT& _stream) const
		{
			for (int y = 0; y < BoardSize; ++y)
			{
				for (int x = 0; x < BoardSize; ++x)
					_stream << STATE_CHARS[static_cast<int>(Get(x,y))];
				_stream << "\n";
			}
		}

		CubeState Get(unsigned _idx, unsigned _idy) const;

		// does not work for blanks!
		int GetNumSymbols(CubeState _symbol) const;

		std::pair<uint32_t, uint32_t> Get() { return { m_crosses,m_circles }; }
	private:
		uint32_t m_crosses;
		uint32_t m_circles;


		constexpr uint32_t IsNotZero(uint32_t x) const {return ~(~x & (x + ~0)) >> 31; };
	};
}