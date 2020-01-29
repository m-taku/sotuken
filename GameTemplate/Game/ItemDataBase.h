#pragma once
struct ITEMINFO {
	std::string Name;					//���O
	int ID;								//ID
	std::string FilePath;				//�t�@�C���p�X
	int Type;							//�A�C�e���̃^�C�v
};
struct ITEMSTOCK {
	int ID;
	std::string Name;
	int Stocks;
};
class ItemDataBase : public IGameObject
{
public:
	ItemDataBase();
	~ItemDataBase();
	/// <summary>
	/// �����œn���ꂽID�̃t�@�C���p�X���擾����
	/// </summary>
	/// <param name="id">ID</param>
	/// <returns>�t�@�C���p�X</returns>
	std::string GetFilePath(int id);
	std::string GetItemName(int id);
	void SetItemStock(int id, int stockSize);
	int GetItemStocks(int id);
private:
	char *m_errMsg = 0;						//�G���[���b�Z�[�W
	std::list<ITEMINFO> m_ItemList;			//�f�[�^
	std::list<ITEMSTOCK> m_StockList;
};