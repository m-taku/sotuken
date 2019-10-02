#pragma once
class ConstantBuffer:Noncopyable
{
public:
	ConstantBuffer();
	~ConstantBuffer();
	/*!
	*@brief ConstantBuffer���쐬�B
	*@param[in]	pInitData		�����f�[�^�B
	*@param[in]	bufferSize		�o�b�t�@�T�C�Y�B
	*/
	bool Create(const void* pInitData, int bufferSize);
	/*!
	 * @brief	ID3D11Buffer*���擾�B
	 */
	ID3D11Buffer*& GetBody()
	{
		return m_gpuBuffer;
	}
	/*!
	*@brief	�J���B
	*@details
	* ���̊֐��̓f�X�g���N�^������R�[������Ă��܂��B
	* �����I�ȃ^�C�~���O�ŊJ���������ꍇ�ɌĂяo���Ă��������B
	*/
	void Release();
private:
	ID3D11Buffer* m_gpuBuffer = nullptr;
};

