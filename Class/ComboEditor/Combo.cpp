#include "Combo.h"

void Combo::Init()
{
	
}

void Combo::Update()
{

}

bool Combo::GetConditions()
{
	// 条件配列の中身が無ければ条件を満たしているとする
	if (conditions_.size() == 0) { return true; }

	// 配列分ループする
	for (int i = 0; i < conditions_.size(); i++) {
		// １つでも条件を達成していなければ早期リターン
		if (!conditions_[i].CheckCondition()) { return false; }
	}

	// ここまで到達した場合条件を満たしているとする
	return true;
}
