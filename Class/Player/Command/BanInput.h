#pragma once
// 当たり判定マスク用のビット値
#define BanNone 0b0		// 0000000000000000
#define BanMove 0b1		// 0000000000000001
#define BanParry 0b1 << 1	// 0000000000000010
#define BanAttack 0b1 << 2	// 0000000000000100
#define BanEvasion 0b1 << 3	// 0000000000001000
#define BanSheath 0b1 << 4	// 0000000000010000
#define BanLockOn 0b1 << 5	// 0000000000100000
#define BanALL (0b1 << 4) - 0b1	// 0000000000011111(ロックオン以外)

inline int GetSetBitPosition(int value) {
    int position = 0;
    while ((value & 1) == 0) {
        value >>= 1;
        position++;
    }
    return position;
}
inline bool IsBitSame(int banInput, int currentInput, int bitPosition) {
    return ((banInput >> bitPosition) & 1) == ((currentInput >> bitPosition) & 1);
}

/// <summary>
/// 入力禁止を解除
/// </summary>
/// <param name="currentBanInput">現在の入力禁止状態</param>
/// <param name="eraceBanInput">解除したい入力禁止</param>
/// <returns></returns>
inline int EraceBanInput(const int& currentBanInput, const int& eraceBanInput) {
    return currentBanInput & ~(1 << GetSetBitPosition(eraceBanInput));
}
/// <summary>
/// 入力禁止を開始
/// </summary>
/// <param name="currentBanInput"></param>
/// <param name="banInput"></param>
/// <returns></returns>
inline int StartBanInput(const int& currentBanInput, const int& banInput) {
    return currentBanInput | banInput;
}