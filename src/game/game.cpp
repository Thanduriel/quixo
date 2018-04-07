#include "board.hpp"

namespace Game {

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
	// 
	constexpr std::array<uint32_t, 5> SHIFT_MASKS_X_LEFT =
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
	};

	bool Board::IsLegal(const Action& _action) const
	{
		return (_action.srcX != _action.dstX && _action.srcY == _action.dstY
			|| _action.srcX == _action.dstX && _action.srcY != _action.dstY)
			&& (_action.srcX == 0 || _action.srcX == MAX_INDEX
				|| _action.srcY == 0 || _action.srcY == MAX_INDEX);
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
			//extract
			line = SHIFT_MASKS_X_LEFT[_srcY] & _board;
			// shift line to make space
			line <<= 1;
			// set new cube
			line |= _newState << (_srcY * 5);
			//clear line
			_board &= ~WIN_MASKS[_srcY];
		}
		else if (_srcX < _destX)
		{
			line = SHIFT_MASKS_X_RIGHT[_srcY] & _board;
			line >>= 1;
			// right most bit of the line
			line |= _newState << (_srcY * 5 + 4);
			_board &= ~WIN_MASKS[_srcY];
		}
		else if (_srcY > _destY)
		{
			line = SHIFT_MASKS_Y_LEFT[_srcX] & _board;
			line <<= 5;
			line |= _newState << _srcX;
			_board &= ~WIN_MASKS[_srcX + 5];
		}
		else if (_srcY < _destY)
		{
			line = SHIFT_MASKS_Y_RIGHT[_srcX] & _board;
			line >>= 5;
			// new cube is in last line
			line |= _newState << (_srcX + 20);
			_board &= ~WIN_MASKS[_srcX + 5];
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
}