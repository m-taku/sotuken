#pragma once
/// <summary>
///�@�N���X�̃R�s�[�֎~�p�̃N���X�i�p���p�ł��j
/// </summary>
struct  Noncopyable{
	Noncopyable() = default;
	Noncopyable(const Noncopyable&) = delete;
	Noncopyable& operator=(const Noncopyable&) = delete;
};

