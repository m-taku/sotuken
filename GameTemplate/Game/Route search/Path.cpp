#include "stdafx.h"
#include "Path.h"
#include"Navimake.h"

Path::Path()
{
}


Path::~Path()
{

}
bool Path::Course(CVector3 sturt, CVector3 end)
{
	m_stop = false;
	m_pathdata = FindGO<Navimake>("Navimake");
	if (m_pathdata == nullptr)
	{
		return false;
	}
	m_nowNo = 0;
	m_coursepasu.clear();
	//もらったポジションを使ってパス番号を取得
	int startNo = m_pathdata->FindPos_No(sturt);
	int endNo = m_pathdata->FindPos_No(end);
	//スタートのパス番号を使って最初のデータを作る
	PasDate* Date = new PasDate;
	Date->No = startNo;
	//オープンノードとクローズノードを制作
	std::vector<PasDate*> open;
	std::vector<PasDate*> close;
	//オープンにスタートのデータを積む
	open.push_back(Date);
	auto NextPas = open[0];
	float CurrentCost = 0.0f;
	//データの番号が最後の番号になるまで続ける
	while (NextPas->No != endNo)
	{
		if (m_stop)
		{
			for (auto ereas : open) {
				delete ereas;
			}
			for (auto ereas : close) {
				delete ereas;
			}
			return false;
		}
		//リンクデータを探す
		auto LincPas = m_pathdata->FindLinc(*NextPas, endNo, CurrentCost);
		//探し終わったのでクローズする。
		open.erase(
			std::remove(open.begin(), open.end(), NextPas),
			open.end());
		close.push_back(NextPas);
		//リンクの数だけ
		for (int j = 0; j < 3; j++)
		{
			//もしもリンクデータがあれば
			if (-1 != LincPas[j]->No)
			{
				//オープンに積む
				open.push_back(LincPas[j]);
				for (int i = 0; i < open.size(); i++) {
					if (open[i] == LincPas[j])
					{
						break;
					}
					//オープンの中に同じ番号のパス番号があれば
					if (open[i]->No == LincPas[j]->No) {
						//データを比較するしてコストの低いほうを残す
						if (open[i]->to_DrstinCost <= LincPas[j]->to_DrstinCost)
						{
							delete LincPas[j];
							open.erase(
								std::remove(open.begin(), open.end(), LincPas[j]),
								open.end());
							break;
						}
						else
						{
							delete open[i];
							open.erase(
								std::remove(open.begin(), open.end(), open[i]),
								open.end());
							break;

						}
					}
				}
				//クローズにも同じパス番号があるか検索する
				for (int i = 0; i < close.size(); i++) {
					if (close[i]->No == LincPas[j]->No) {
						if (close[i]->to_DrstinCost <= LincPas[j]->to_DrstinCost) {
							delete  LincPas[j];
							open.erase(
								std::remove(open.begin(), open.end(), LincPas[j]),
								open.end());
							break;
						}
						else {
							delete close[i];
							close.erase(
								std::remove(close.begin(), close.end(), close[i]),
								open.end());
							break;
						}
					}
				}
				//データ検索の終わり
			}
			//リンクデータがない場合データを消去する
			else
			{
				delete LincPas[j];
			}
		}
		//オープンがゼロはおかしいので強制終了する
		if (open.size() <= 0)
		{
			break;
		}
		float costnV2 = FLT_MAX;
		//オープンの中から最小のコストを検索するし
		//見つかったデータを次のオープンするデータにする
		for (auto i : open)
		{
			if ((i->MoveCost + i->to_DrstinCost) <= costnV2) {
				costnV2 = i->MoveCost + i->to_DrstinCost;
				NextPas = i;
			}
		}
		//そのリンクまでの最小コストを移動コストとする
		CurrentCost = NextPas->MoveCost;
	}
	if (open.size() <= 0)
	{
		//オープンがないのは不具合なので終了する
		m_coursepasu.push_back(startNo);
	}
	else
	{
		//エンドからスタートに向かってリンクを進めていく
		while (NextPas->No != startNo) {
			m_coursepasu.push_back(NextPas->No);
			NextPas = NextPas->ParentDate;
		}
		m_coursepasu.push_back(NextPas->No);
		//ここままだと逆の番号なので
		//リバースしてスタートからにする
		std::reverse(m_coursepasu.begin(), m_coursepasu.end());
		if (m_coursepasu.size() > 2)
		{
			//ここでスムージング処理
			Smoothing(&m_coursepasu);
		}
		//デバックしたければする
		//m_pathdata->DebugVector(m_coursepasu);
	}
	//すべて終わったのでデータを消去する
	for (auto ereas : open) {
		delete ereas;
	}
	for (auto ereas : close) {
		delete ereas;
	}

	return true;
}
void Path::Smoothing(std::vector<int>* pasu)
{
	//スムーズスタート時の番号
	std::vector<int> pa;
	int start = (*pasu)[0];
	pa.push_back(start);
	int Next;
	int wait = 0;
	//1つ飛ばした2番目から検索を開始にする。
	for (int i = 1; i < pasu->size(); i++) 
	{
		Next = (*pasu)[i];
		if (m_pathdata->CollisionTest(start, Next))
		{
			if (wait == 0) {
				start = (*pasu)[i - 1];
				wait++;
				pa.push_back(start);
			}
		}
		else
		{
			if (wait != 0) {
				pa.push_back(Next);
				wait = 0;
			}
		}
	}
	pa.push_back(Next);
	*pasu = pa;
}
const CVector3 Path::PathPos()
{
	if (m_coursepasu.size()-1 > m_nowNo) {
		return m_pathdata->FindNo_Pos(m_coursepasu[m_nowNo++]);
	}
	else
	{
		return m_pathdata->FindNo_Pos(m_coursepasu[m_nowNo]);
	}
}