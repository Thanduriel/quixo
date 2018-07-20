#include "board.hpp"

namespace Game {

	// all possible 5-lines on the board
	constexpr std::array<uint32_t, 12> WIN_MASKS =
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
	// masks for different (partial) lines to perform moves on them
/*	constexpr std::array<uint32_t, 5> SHIFT_MASKS_X_LEFT =
	{
		0b00000000000000000000000000001111,
		0b00000000000000000000000111100000,
		0b00000000000000000011110000000000,
		0b00000000000001111000000000000000,
		0b00000000111100000000000000000000,
	};
	constexpr std::array<uint32_t, 5> SHIFT_MASKS_X_RIGHT =
	{
		0b00000000000000000000000000011110,
		0b00000000000000000000000111000000,
		0b00000000000000000111100000000000,
		0b00000000000011110000000000000000,
		0b00000001111000000000000000000000,
	};

	constexpr std::array<uint32_t, 5> SHIFT_MASKS_Y_LEFT =
	{
		0b00000000000000001000010000100001,
		0b00000000000000010000100001000010,
		0b00000000000000100001000010000100,
		0b00000000000001000010000100001000,
		0b00000000000010000100001000010000
	};

	constexpr std::array<uint32_t, 5> SHIFT_MASKS_Y_RIGHT =
	{
		0b00000000000100001000010000100000,
		0b00000000001000010000100001000000,
		0b00000000010000100001000010000000,
		0b00000000100001000010000100000000,
		0b00000001000010000100001000000000
	};*/

	// ********************************************* //
	constexpr std::array<uint32_t, 5> SHIFT_MASKS_X_LEFT =
	{
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000001,
		0b00000000000000000000000000000011,
		0b00000000000000000000000000000111,
		0b00000000000000000000000000001111
	};

	constexpr std::array<uint32_t, 5> LINE_MASKS_X_LEFT =
	{
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000011,
		0b00000000000000000000000000000111,
		0b00000000000000000000000000001111,
		0b00000000000000000000000000011111
	};

	// ********************************************* //
	constexpr std::array<uint32_t, 5> SHIFT_MASKS_X_RIGHT =
	{
		0b00000000000000000000000000000000,
		0b00000000000000000000000000010000,
		0b00000000000000000000000000011000,
		0b00000000000000000000000000011100,
		0b00000000000000000000000000011110
	};

	constexpr std::array<uint32_t, 5> LINE_MASKS_X_RIGHT =
	{
		0b00000000000000000000000000000000,
		0b00000000000000000000000000011000,
		0b00000000000000000000000000011100,
		0b00000000000000000000000000011110,
		0b00000000000000000000000000011111
	};

	// ********************************************* //
	constexpr std::array<uint32_t, 5> SHIFT_MASKS_Y_LEFT =
	{
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000001,
		0b00000000000000000000000000100001,
		0b00000000000000000000010000100001,
		0b00000000000000001000010000100001
	};
	
	constexpr std::array<uint32_t, 5> LINE_MASKS_Y_LEFT =
	{
		0b00000000000000000000000000000000,
		0b00000000000000000000000000100001,
		0b00000000000000000000010000100001,
		0b00000000000000001000010000100001,
		0b00000000000100001000010000100001
	};

	// ********************************************* //
	constexpr std::array<uint32_t, 5> SHIFT_MASKS_Y_RIGHT =
	{
		0b00000000000000000000000000000000,
		0b00000000000100000000000000000000,
		0b00000000000100001000000000000000,
		0b00000000000100001000010000000000,
		0b00000000000100001000010000100000
	};

	constexpr std::array<uint32_t, 5> LINE_MASKS_Y_RIGHT =
	{
		0b00000000000000000000000000000000,
		0b00000000000100001000000000000000,
		0b00000000000100001000010000000000,
		0b00000000000100001000010000100000,
		0b00000000000100001000010000100001
	};

	bool Board::IsOuter(int x, int y) const
	{
		return x * y ==  0 || x == MAX_INDEX || y == MAX_INDEX;
	}

	bool Board::IsLegal(const Action& _action) const
	{
		for (auto& action : ACTIONS)
			if (action == _action) return true;
		return false;
	/*	return (_action.srcX != _action.dstX && _action.srcY == _action.dstY
			|| _action.srcX == _action.dstX && _action.srcY != _action.dstY)
			&& IsOuter(_action.srcX, _action.srcY) && IsOuter(_action.dstX, _action.dstY);
			*/
	}

	GameResult Board::Winner() const
	{
		int crossLines = 0;
		int circleLines = 0;

		for (uint32_t mask : WIN_MASKS)
		{
			crossLines += !((mask & m_crosses) ^ mask);
			circleLines += !((mask & m_circles) ^ mask);
		}


		if (crossLines > circleLines) return GameResult::Cross;
		else if (circleLines > crossLines) return GameResult::Circle;
		else if (circleLines) return GameResult::Draw;

		return GameResult::None;
	}

	void Board::Move(const Action& _action, CubeState _newState)
	{
		assert(IsLegal(_action));
		Move(_action.srcX, _action.srcY, _action.dstX, _action.dstY, m_crosses,
			_newState == CubeState::Cross ? 1u : 0u);
		Move(_action.srcX, _action.srcY, _action.dstX, _action.dstY, m_circles,
			_newState == CubeState::Circle ? 1u : 0u);
	}

	void Board::Move(int _srcX, int _srcY, int _destX, int _destY, uint32_t& _board, uint32_t _newState)
	{
		uint32_t line;

		if (_srcX > _destX)
		{
			const unsigned lineShift = _srcY * 5u;
			const int ind = _srcX - _destX;
			const uint32_t mask = SHIFT_MASKS_X_LEFT[ind] << lineShift;
			//extract
			line = mask & _board;
			// shift line to make space
			line <<= 1;
			// set new cube
			line |= _newState << lineShift;
			//clear line
			_board &= ~(LINE_MASKS_X_LEFT[ind] << lineShift);
		}
		else if (_srcX < _destX)
		{
			const unsigned lineShift = _srcY * 5u;
			const int ind = _destX - _srcX;
			const uint32_t mask = SHIFT_MASKS_X_RIGHT[ind] << lineShift;
			line = mask & _board;
			line >>= 1;
			// right most bit of the line
			line |= _newState << (lineShift + 4);
			_board &= ~(LINE_MASKS_X_RIGHT[ind] << lineShift);
		}
		else if (_srcY > _destY)
		{
			const int ind = _srcY - _destY;
			const uint32_t mask = SHIFT_MASKS_Y_LEFT[ind] << _srcX;
			line = mask & _board;
			line <<= 5;
			line |= _newState << _srcX;
			_board &= ~(LINE_MASKS_Y_LEFT[ind] << _srcX);
		}
		else if (_srcY < _destY)
		{
			const int ind = _destY - _srcY;
			const uint32_t mask = SHIFT_MASKS_Y_RIGHT[ind] << _srcX;
			line = mask & _board;
			line >>= 5;
			// new cube is in last line
			line |= _newState << (_srcX + 20);
			_board &= ~(LINE_MASKS_Y_RIGHT[ind] << _srcX);
		}
		//set line
		_board |= line;
	}

	CubeState Board::Get(unsigned _idx, unsigned _idy) const
	{
		const uint32_t mask = (1 << _idx) << (_idy * BoardSize);

		// CubeState::Circle == 2
		// assuming a valid board state at most one can be true
		return static_cast<CubeState>(IsNotZero(mask & m_crosses) + (IsNotZero(mask & m_circles) << 1));
	}

	void Board::Set(unsigned _idx, unsigned _idy, CubeState _newState)
	{
		const uint32_t mask = (1 << _idx) << (_idy * BoardSize);

		if (_newState == CubeState::Cross)
		{
			m_crosses |= mask;
			m_circles &= ~mask;
		}
		else if (_newState == CubeState::Circle)
		{
			m_crosses &= ~mask;
			m_circles |= mask;
		}
		else
		{
			m_crosses &= ~mask;
			m_circles &= ~mask;
		}
	}

	int Board::GetNumSymbols(CubeState _symbol) const
	{
		uint32_t bits = _symbol == CubeState::Cross ? m_crosses : m_circles;
		int count = 0;
		while (bits)
		{
			if (bits & 0x1) ++count;
			bits >>= 1;
		}

		return count;
	}
}
