#pragma once
namespace smEngine {
	/*!
	 * @brief	�X�v���C�g�B
	 */
	class Sprite : Noncopyable {
	public:
		static const CVector2	DEFAULT_PIVOT;					//!<�s�{�b�g�B
		/*!
		 * @brief	�R���X�g���N�^�B
		 */
		Sprite();
		/*!
		 * @brief	�f�X�g���N�^�B
		 */
		~Sprite();
		/*!
		 * @brief	�������B
		 *@param[in]	tex		�e�N�X�`���B
		 *@param[in]	w		���B
		 *@param[in]	h		�����B
		 */
		void Init(ShaderResourceView& tex, float w, float h);
		/*!
		 * @brief	�e�N�X�`����ݒ�B
		 *@param[in]	tex		�e�N�X�`���B
		 */
		void SetTexture(ShaderResourceView& tex)
		{
			m_textureSRV = &tex;
		}
		/*!
		* @brief	��Z�J���[��ݒ�B
		*@param[in]	mulColor	��Z�J���[�B
		*/
		void SetMulColor(const CVector4& mulColor)
		{
			m_mulColor = mulColor;
		}
		/*!
		*@brief	�X�V
		*@param[in]	trans		���s�ړ��B
		*@param[in]	rot			��]�B
		*@param[in]	scale		�g��B
		*@param[in] pivot		��_�B
		*						0.5, 0.5�ŉ摜�̒��S����_�B
		*						0.0, 0.0�ŉ摜�̍����B
		*						1.0, 1.0�ŉ摜�̉E��B
		*						Unity��uGUI�ɏ����B
		*/
		void Update(const CVector3& trans, const CQuaternion& rot, const CVector3& scale, const CVector2& pivot = DEFAULT_PIVOT);
		/*!
		*@brief	�`��
		*@param[in]	viewMatrix		�r���[�s��B
		*@param[in]	projMatrix		�v���W�F�N�V�����s��B
		*/
		void Draw(ID3D11DeviceContext* DeviceContext, const CMatrix& viewMatrix, const CMatrix& projMatrix);

	private:
		struct SSpriteCB {
			CMatrix WVP;		//���[���h�r���[�v���W�F�N�V�����s��B
			CVector4 mulColor;	//��Z�J���[�B
		};
		CVector3				m_position = CVector3::Zero();	//!<���W�B
		CQuaternion				m_rotation = CQuaternion::Identity();		//!<��]
		CVector3				m_scale = CVector3::One();
		CMatrix					m_world = CMatrix::Identity();	//!<���[���h�s��B
		Shader					m_ps;							//!<�s�N�Z���V�F�[�_�[�B
		Shader					m_vs;							//!<���_�V�F�[�_�[�B
		CVector4				m_mulColor = CVector4::White();	//!<��Z�J���[�B
		Primitive				m_primitive;					//!<�v���~�e�B�u�B
		ShaderResourceView*	m_textureSRV = nullptr;			//!<�e�N�X�`���B
		ConstantBuffer			m_cb;
		bool					m_isInited = false;				//!<�������t���O�B
		CVector2				m_size = CVector2::Zero();		//!<�T�C�Y�B
	};
}

