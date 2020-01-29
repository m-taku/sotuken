#include "stdafx.h"
#include "ItemDataBase.h"
#include <fstream>
#include <sqlite3.h>

#pragma comment( lib, "sqlite3.lib" )

//���o���ʂ��Ԃ�R�[���o�b�N�֐�
int ItemCallback(void* iteminfo, int size, char** data, char **ColName) {
	// �����̓��X�g
	std::list<ITEMINFO> *pItemList = (std::list< ITEMINFO >*)iteminfo;
	ITEMINFO item;
	item.ID = atoi(data[0]);	// ID
	item.Name = data[1];		// Name
	item.FilePath = data[2];	// FilePath
	item.Type = atoi(data[3]);	// Type
	pItemList->push_back(item);
	return 0;
}

//���o���ʂ��Ԃ�R�[���o�b�N�֐�
int StockCallback(void* iteminfo, int size, char** data, char **ColName) {
	// �����̓��X�g
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
	// �f�[�^�x�[�X�t�@�C����V�K����
	sqlite3 *ItemDB = 0;
	int rc = sqlite3_open("Item.db", &ItemDB);
	if (rc != SQLITE_OK) {
		//���s

	}
	// �A�C�e���e�[�u���ƃX�g�b�N�e�[�u���̍쐬
	// �A�C�e���e�[�u������SQL
	char ItemTable_Create_SQL[] = "CREATE TABLE ItemTable ("
		"               ID      INTEGER PRIMARY KEY,"
		"               Name    TEXT    NOT NULL   ,"
		"               FilePath    TEXT    NOT NULL   ,"
		"               Type   INTEGER NOT NULL   )";


	// �X�g�b�N�e�[�u������SQL
	char StockTable_Create_SQL[] = "CREATE TABLE StockTable ("
		"              ID      INTEGER PRIMARY KEY,"
		"               Name    TEXT    NOT NULL   ,"
		"              Stock   INTEGER NOT NULL   )";
	// ����
	sqlite3_exec(ItemDB, ItemTable_Create_SQL, 0, 0, &m_errMsg);
	sqlite3_exec(ItemDB, StockTable_Create_SQL, 0, 0, &m_errMsg);
	
	// �f�[�^�x�[�X�ɃA�C�e����o�^
	char Insert_Item_SQL[] = " INSERT INTO ItemTable ( ID, Name, FilePath, Type)"
		"             values( %d, '%s', '%s', %d )          ";
	
	// �V�K�ǉ�
	char Insert_Stock_SQL[] = "INSERT INTO StockTable( ID,Name, Stock) "
		"             values( %d,'%s', %d)                       ";
	
	std::fstream ifs("Item.csv");
	if (!ifs.is_open()) {
		sqlite3_close(ItemDB);
		//���s

	}
	char dummy[100], zName[24], zFilePath[256], InsertItemSQL[512], InsertStockSQL[512];
	int iID, iType;
	ifs.getline(dummy, 100); // �R�����g����
	ifs.getline(dummy, 100); // �񖼖���
	while (1)
	{
		ifs >> iID >> zName >> zFilePath >> iType;

		if (ifs.eof())
			break;

		// �f�[�^�x�[�X�ɓo�^
		sprintf_s(InsertItemSQL, Insert_Item_SQL, iID, zName, zFilePath, iType);
		sqlite3_exec(ItemDB, InsertItemSQL, 0, 0, &m_errMsg);

		// �f�[�^�x�[�X�ɓo�^
		sprintf_s(InsertStockSQL, Insert_Stock_SQL, iID, zName, 0);
		sqlite3_exec(ItemDB, InsertStockSQL, 0, 0, &m_errMsg);
	}
	
	//SQL�����s
	sqlite3_exec(ItemDB, "SELECT ID,Name,FilePath,Type FROM ItemTable", ItemCallback, &m_ItemList, &m_errMsg);

	// �f�[�^�x�[�X�����
	sqlite3_close(ItemDB);
}

ItemDataBase::~ItemDataBase()
{
}

std::string ItemDataBase::GetFilePath(int id)
{
	std::string filepath = "�G���[";

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
	std::string itemName = "�G���[";

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
	// �f�[�^�x�[�X�t�@�C����V�K����
	sqlite3 *ItemDB = 0;
	int rc = sqlite3_open("Item.db", &ItemDB);
	if (rc != SQLITE_OK) {
		//���s
	}

	// ���ύX
	char update_stock_SQL[512];
	sprintf_s(update_stock_SQL, "UPDATE StockTable SET Stock = Stock +%d WHERE StockTable.ID = %d", stockSize, id);

	//�X�V
	sqlite3_exec(ItemDB, update_stock_SQL, 0, 0, &m_errMsg);

	//SQL�����s
	sqlite3_exec(ItemDB, "SELECT ID,Name,Stock FROM StockTable", StockCallback, &m_StockList, &m_errMsg);

	// �f�[�^�x�[�X�����
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