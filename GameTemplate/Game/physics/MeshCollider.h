#pragma once
#include "Physics/ICollider.h"


class SkinModel;

/*!
 * @brief	���b�V���R���C�_�[�B
 */
class MeshCollider : public ICollider
{
	typedef std::vector<CVector3>					VertexBuffer;		//���_�o�b�t�@�B
	typedef std::vector<unsigned int>				IndexBuffer;		//�C���f�b�N�X�o�b�t�@�B
	typedef std::unique_ptr<VertexBuffer>			VertexBufferPtr;
	typedef std::unique_ptr<IndexBuffer>			IndexBufferPtr;
	
public:
	/*!
	 * @brief	�R���X�g���N�^�B
	 */
	MeshCollider();
	/*!
	 * @brief	�f�X�g���N�^�B
	 */
	~MeshCollider();
	/*!
	 * @brief	CSkinModel���烁�b�V���R���C�_�[�𐶐��B
	 *@param[in]	model		�X�L�����f���B
	 */
	void CreateFromSkinModel(const SkinModel& model, const CMatrix* offsetMatrix);
	void CreateFromSkinModel(const SkinModel& model, const CMatrix* offsetMatrix, CVector3 scale);
	btCollisionShape* GetBody() const override
	{
		return m_meshShape.get();
	}
	const  VertexBuffer& Getvertex(int No) const
	{
		return *m_vertexBufferArray[No].get();
	}
	const  IndexBuffer& GetIndex(int No) const
	{
		return *m_indexBufferArray[No].get();
	}
	int Getok()
	{
		return m_vertexBufferArray.size();
	}
private:
	std::vector<VertexBufferPtr>					m_vertexBufferArray;		//���_�o�b�t�@�̔z��B
	std::vector<IndexBufferPtr>						m_indexBufferArray;		//�C���f�b�N�X�o�b�t�@�̔z��B
	std::unique_ptr<btBvhTriangleMeshShape>			m_meshShape;				//���b�V���`��B
	std::unique_ptr<btTriangleIndexVertexArray>		m_stridingMeshInterface;
};

