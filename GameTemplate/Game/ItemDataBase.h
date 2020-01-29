#pragma once
struct ITEMINFO {
	std::string Name;					//名前
	int ID;								//ID
	std::string FilePath;				//ファイルパス
	int Type;							//アイテムのタイプ
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
	/// 引数で渡されたIDのファイルパスを取得する
	/// </summary>
	/// <param name="id">ID</param>
	/// <returns>ファイルパス</returns>
	std::string GetFilePath(int id);
	std::string GetItemName(int id);
	void SetItemStock(int id, int stockSize);
	int GetItemStocks(int id);
private:
	char *m_errMsg = 0;						//エラーメッセージ
	std::list<ITEMINFO> m_ItemList;			//データ
	std::list<ITEMSTOCK> m_StockList;
};