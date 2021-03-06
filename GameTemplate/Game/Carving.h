#pragma once
class Carving : public IGameObject
{
public:
	Carving();
	~Carving();
	/// <summary>
	/// 手に入れる可能性のある素材を追加する
	/// 確率が全て足して100になるようにする
	/// </summary>
	/// <param name="name">素材の名前</param>
	/// <param name="value">その素材を手に入れる確率</param>
	void AddItem(std::string name, int value);
	/// <summary>
	/// 素材を手に入れる
	/// </summary>
	/// <returns>素材名</returns>
	std::string GetItem();
private:
	std::map<std::string, int> m_item;					//素材
};

