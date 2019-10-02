#pragma once
class ConstantBuffer:Noncopyable
{
public:
	ConstantBuffer();
	~ConstantBuffer();
	/*!
	*@brief ConstantBufferを作成。
	*@param[in]	pInitData		初期データ。
	*@param[in]	bufferSize		バッファサイズ。
	*/
	bool Create(const void* pInitData, int bufferSize);
	/*!
	 * @brief	ID3D11Buffer*を取得。
	 */
	ID3D11Buffer*& GetBody()
	{
		return m_gpuBuffer;
	}
	/*!
	*@brief	開放。
	*@details
	* この関数はデストラクタからもコールされています。
	* 明示的なタイミングで開放したい場合に呼び出してください。
	*/
	void Release();
private:
	ID3D11Buffer* m_gpuBuffer = nullptr;
};

