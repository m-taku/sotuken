#pragma once
//�A�C�e���e�[�u���̍\����
struct ITEMINFO {
	std::string Name;					//���O
	int ID;								//ID
	std::string FilePath;				//�t�@�C���p�X
	int Type;							//�A�C�e���̃^�C�v
};
//�X�g�b�N�e�[�u���̍\����
struct ITEMSTOCK {
	int ID;								//ID
	std::string Name;					//���O
	int Stocks;							//�A�C�e����
};
class ItemDataBase : public IGameObject
{
public:
	ItemDataBase();
	~ItemDataBase();
	/// <summary>
	/// �����œn���ꂽID�̃t�@�C���p�X���擾����
	/// </summary>
	/// <param name="id">�A�C�e��ID</param>
	/// <returns>�t�@�C���p�X</returns>
	std::string GetFilePath(int id);
	/// <summary>
	/// �����œn���ꂽID�̃A�C�e������Ԃ�
	/// </summary>
	/// <param name="id">�A�C�e��ID</param>
	/// <returns>�A�C�e����</returns>
	std::string GetItemName(int id);
	/// <summary>
	/// �������Ɠ���ID�̃A�C�e���̏�����������������₷
	/// </summary>
	/// <param name="id">�A�C�e��ID</param>
	/// <param name="stockSize">���₷��</param>
	void SetItemStock(int id, int stockSize);
	/// <summary>
	/// �����œn���ꂽ�A�C�e������Ԃ�
	/// </summary>
	/// <param name="id">�A�C�e��ID</param>
	/// <returns>�A�C�e����</returns>
	int GetItemStocks(int id);
private:
	char *m_errMsg = 0;						//�G���[���b�Z�[�W
	std::list<ITEMINFO> m_ItemList;			//�A�C�e���e�[�u���f�[�^
	std::list<ITEMSTOCK> m_StockList;		//�X�g�b�N�e�[�u���f�[�^
};