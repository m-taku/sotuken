/*!
 * @brief	���w�֌W�F�X�B
 */

#pragma once


class CMath {
public:
	static const float PI;
	/*!
	 * @brief	degree����radian�ɕϊ��B
	 */
	static float DegToRad(float deg)
	{
		return deg * (PI / 180.0f);
	}
	/*!
	 * @brief	radian����degree�ɕϊ��B
	 */
	static float RadToDeg(float rad)
	{
		return rad / (PI / 180.0f);
	}

	static float Acos(float dotres)
	{
		return acosf(min(1.0f, max(-1.0f, dotres)));
	}
	/*!
	 *@brief	t0��t1�Ԃ���`�⊮�B
	 *@details
	 *  ret = t0 + (t1-t0) + rate;
	 *@param[in]	t		��ԗ�	0.0�`1.0
	 */
	static inline float Lerp(float rate, float t0, float t1)
	{
		return t0 + (t1 - t0)*rate;
	}
};
