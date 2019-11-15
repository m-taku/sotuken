#pragma once
class Carving : public IGameObject
{
public:
	Carving();
	~Carving();
	/// <summary>
	/// è‚É“ü‚ê‚é‰Â”\«‚Ì‚ ‚é‘fŞ‚ğ’Ç‰Á‚·‚é
	/// Šm—¦‚ª‘S‚Ä‘«‚µ‚Ä100‚É‚È‚é‚æ‚¤‚É‚·‚é
	/// </summary>
	/// <param name="name">‘fŞ‚Ì–¼‘O</param>
	/// <param name="value">‚»‚Ì‘fŞ‚ğè‚É“ü‚ê‚éŠm—¦</param>
	void AddItem(std::string name, int value);
	/// <summary>
	/// ‘fŞ‚ğè‚É“ü‚ê‚é
	/// </summary>
	/// <returns>‘fŞ–¼</returns>
	std::string GetItem();
private:
	std::map<std::string, int> m_item;					//‘fŞ
};

