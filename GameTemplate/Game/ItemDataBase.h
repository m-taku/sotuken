#pragma once
//アイテムテーブルの構造体
struct ITEMINFO {
	std::string Name;					//名前
	int ID;								//ID
	std::string FilePath;				//ファイルパス
	int Type;							//アイテムのタイプ
};
//ストックテーブルの構造体
struct ITEMSTOCK {
	int ID;								//ID
	std::string Name;					//名前
	int Stocks;							//アイテム数
};
class ItemDataBase : public IGameObject
{
public:
	ItemDataBase();
	~ItemDataBase();
	/// <summary>
	/// 引数で渡されたIDのファイルパスを取得する
	/// </summary>
	/// <param name="id">アイテムID</param>
	/// <returns>ファイルパス</returns>
	std::string GetFilePath(int id);
	/// <summary>
	/// 引数で渡されたIDのアイテム名を返す
	/// </summary>
	/// <param name="id">アイテムID</param>
	/// <returns>アイテム名</returns>
	std::string GetItemName(int id);
	/// <summary>
	/// 第一引数と同じIDのアイテムの所持数を第二引数分増やす
	/// </summary>
	/// <param name="id">アイテムID</param>
	/// <param name="stockSize">増やす数</param>
	void SetItemStock(int id, int stockSize);
	/// <summary>
	/// 引数で渡されたアイテム数を返す
	/// </summary>
	/// <param name="id">アイテムID</param>
	/// <returns>アイテム数</returns>
	int GetItemStocks(int id);
private:
	char *m_errMsg = 0;						//エラーメッセージ
	std::list<ITEMINFO> m_ItemList;			//アイテムテーブルデータ
	std::list<ITEMSTOCK> m_StockList;		//ストックテーブルデータ
};