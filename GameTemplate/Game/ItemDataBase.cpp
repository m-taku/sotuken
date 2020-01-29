#include "stdafx.h"
#include "ItemDataBase.h"
#include <fstream>
#include <sqlite3.h>

#pragma comment( lib, "sqlite3.lib" )

//抽出結果が返るコールバック関数
int ItemCallback(void* iteminfo, int size, char** data, char **ColName) {
	// 引数はリスト
	std::list<ITEMINFO> *pItemList = (std::list< ITEMINFO >*)iteminfo;
	ITEMINFO item;
	item.ID = atoi(data[0]);	// ID
	item.Name = data[1];		// Name
	item.FilePath = data[2];	// FilePath
	item.Type = atoi(data[3]);	// Type
	pItemList->push_back(item);
	return 0;
}

//抽出結果が返るコールバック関数
int StockCallback(void* iteminfo, int size, char** data, char **ColName) {
	// 引数はリスト
	std::list<ITEMSTOCK> *pItemList = (std::list< ITEMSTOCK >*)iteminfo;
	ITEMSTOCK stock;
	stock.ID = atoi(data[0]);			// ID
	stock.Name = data[1];				// Name
	stock.Stocks = atoi(data[2]);		// Stock
	pItemList->push_back(stock);
	return 0;
}

ItemDataBase::ItemDataBase()
{
	// データベースファイルを新規生成
	sqlite3 *ItemDB = 0;
	int rc = sqlite3_open("Item.db", &ItemDB);
	if (rc != SQLITE_OK) {
		//失敗

	}
	// アイテムテーブルとストックテーブルの作成
	// アイテムテーブル生成SQL
	char ItemTable_Create_SQL[] = "CREATE TABLE ItemTable ("
		"               ID      INTEGER PRIMARY KEY,"
		"               Name    TEXT    NOT NULL   ,"
		"               FilePath    TEXT    NOT NULL   ,"
		"               Type   INTEGER NOT NULL   )";


	// ストックテーブル生成SQL
	char StockTable_Create_SQL[] = "CREATE TABLE StockTable ("
		"              ID      INTEGER PRIMARY KEY,"
		"               Name    TEXT    NOT NULL   ,"
		"              Stock   INTEGER NOT NULL   )";
	// 生成
	sqlite3_exec(ItemDB, ItemTable_Create_SQL, 0, 0, &m_errMsg);
	sqlite3_exec(ItemDB, StockTable_Create_SQL, 0, 0, &m_errMsg);
	
	// データベースにアイテムを登録
	char Insert_Item_SQL[] = " INSERT INTO ItemTable ( ID, Name, FilePath, Type)"
		"             values( %d, '%s', '%s', %d )          ";
	
	// 新規追加
	char Insert_Stock_SQL[] = "INSERT INTO StockTable( ID,Name, Stock) "
		"             values( %d,'%s', %d)                       ";
	
	std::fstream ifs("Item.csv");
	if (!ifs.is_open()) {
		sqlite3_close(ItemDB);
		//失敗

	}
	char dummy[100], zName[24], zFilePath[256], InsertItemSQL[512], InsertStockSQL[512];
	int iID, iType;
	ifs.getline(dummy, 100); // コメント無視
	ifs.getline(dummy, 100); // 列名無視
	while (1)
	{
		ifs >> iID >> zName >> zFilePath >> iType;

		if (ifs.eof())
			break;

		// データベースに登録
		sprintf_s(InsertItemSQL, Insert_Item_SQL, iID, zName, zFilePath, iType);
		sqlite3_exec(ItemDB, InsertItemSQL, 0, 0, &m_errMsg);

		// データベースに登録
		sprintf_s(InsertStockSQL, Insert_Stock_SQL, iID, zName, 0);
		sqlite3_exec(ItemDB, InsertStockSQL, 0, 0, &m_errMsg);
	}
	
	//SQLを実行
	sqlite3_exec(ItemDB, "SELECT ID,Name,FilePath,Type FROM ItemTable", ItemCallback, &m_ItemList, &m_errMsg);

	// データベースを閉じる
	sqlite3_close(ItemDB);
}

ItemDataBase::~ItemDataBase()
{
}

std::string ItemDataBase::GetFilePath(int id)
{
	std::string filepath = "エラー";

	for (auto itr = m_ItemList.begin(); itr != m_ItemList.end(); ++itr)
	{
		if (itr->ID == id)
		{
			filepath = itr->FilePath;
			break;
		}
	}

	return filepath;

}

std::string ItemDataBase::GetItemName(int id)
{
	std::string itemName = "エラー";

	for (auto itr = m_ItemList.begin(); itr != m_ItemList.end(); ++itr)
	{
		if (itr->ID == id)
		{
			itemName = itr->Name;
			break;
		}
	}

	return itemName;
}

void ItemDataBase::SetItemStock(int id, int stockSize)
{
	// データベースファイルを新規生成
	sqlite3 *ItemDB = 0;
	int rc = sqlite3_open("Item.db", &ItemDB);
	if (rc != SQLITE_OK) {
		//失敗
	}

	// 個数変更
	char update_stock_SQL[512];
	sprintf_s(update_stock_SQL, "UPDATE StockTable SET Stock = Stock +%d WHERE StockTable.ID = %d", stockSize, id);

	//更新
	sqlite3_exec(ItemDB, update_stock_SQL, 0, 0, &m_errMsg);

	//SQLを実行
	sqlite3_exec(ItemDB, "SELECT ID,Name,Stock FROM StockTable", StockCallback, &m_StockList, &m_errMsg);

	// データベースを閉じる
	sqlite3_close(ItemDB);
}

int ItemDataBase::GetItemStocks(int id)
{
	int itemStocks = -1;

	for (auto itr = m_StockList.begin(); itr != m_StockList.end(); ++itr)
	{
		if (itr->ID == id)
		{
			itemStocks = itr->Stocks;
			break;
		}
	}

	return itemStocks;
}