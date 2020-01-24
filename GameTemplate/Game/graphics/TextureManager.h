#pragma once
class TextureManager : public Noncopyable
{
public:
	TextureManager();
	~TextureManager();
	/*!
	*@brief	���f�������[�h�B
	*@param[in]	filePath	�t�@�C���p�X�B
	*@param[in]	skeleton	�X�P���g���B
	*/
	ID3D11ShaderResourceView* Load(const wchar_t* filePath);
	/*!
	*@brief	���f���f�[�^��S�J���B
	*/
	void Release();
private:
	//�t�@�C���p�X���L�[�ADirectXModel�̃C���X�^���X��l�Ƃ���}�b�v�B
	//�����݂����Ȃ��̂ł��B
	std::map<
		std::wstring,					//wstring���L�[�B 
		ID3D11ShaderResourceView*   	//���ꂪ�l�B
	> m_directX;
};

//g_skinModelDataManager��extern�錾�B
//extern�錾�͎��Ԃł͂Ȃ��̂Œ��ӁI
//�R���p�C���ɂǂ����ɂ��邩��g���Ă˂Ƌ�������́B
extern TextureManager g_TextureManager;