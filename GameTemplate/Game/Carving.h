#pragma once
class Carving : public IGameObject
{
public:
	Carving();
	~Carving();
	/// <summary>
	/// ��ɓ����\���̂���f�ނ�ǉ�����
	/// �m�����S�đ�����100�ɂȂ�悤�ɂ���
	/// </summary>
	/// <param name="name">�f�ނ̖��O</param>
	/// <param name="value">���̑f�ނ���ɓ����m��</param>
	void AddItem(std::string name, int value);
	/// <summary>
	/// �f�ނ���ɓ����
	/// </summary>
	/// <returns>�f�ޖ�</returns>
	std::string GetItem();
private:
	std::map<std::string, int> m_item;					//�f��
};

