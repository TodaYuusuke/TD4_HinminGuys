#pragma once
// 当たり判定マスク用のビット値
#define BanNone 0b0		// 0000000000000000
#define BanMove 0b1		// 0000000000000001
#define BanParry 0b1 << 1	// 0000000000000010
#define BanAttack 0b1 << 2	// 0000000000000100
#define BanEvasion 0b1 << 3	// 0000000000001000
#define BanSheath 0b1 << 4	// 0000000000010000
#define BanLockOn 0b1 << 5	// 0000000000100000
#define BanALL (0b1 << 16) - 0b1	// 1111111111111111

// 
#define InputNone 0b0		// 0000000000000000
#define InputMove 0b1		// 0000000000000001
#define InputParry 0b1 << 1	// 0000000000000010
#define InputAttack 0b1 << 2	// 0000000000000100
#define InputEvasion 0b1 << 3	// 0000000000001000
#define InputSheath 0b1 << 4	// 0000000000010000
#define InputLockOn 0b1 << 5	// 0000000000100000
#define InputALL (0b1 << 16) - 0b1	// 1111111111111111

inline int GetSetBitPosition(int value) {
    int position = 0;
    while ((value & 1) == 0) {
        value >>= 1;
        position++;
    }
    return position;
}
inline bool IsBitSame(int banInput, int currentInput, int bitPosition) {
    /*if (((banInput >> bitPosition) & 1) == ((currentInput >> bitPosition) & 1)) {
        return true;
    }
    if (((banInput >> bitPosition) & 0) == ((currentInput >> bitPosition) & 1)) {
        return false;
    }
    return true;*/
    return ((banInput >> bitPosition) & 1) == ((currentInput >> bitPosition) & 1);
}

//namespace BanInput {
//	namespace Mask {
//
//	};
//	namespace 
//};