#pragma once
/// <summary>
///　クラスのコピー禁止用のクラス（継承用です）
/// </summary>
struct  Noncopyable{
	Noncopyable() = default;
	Noncopyable(const Noncopyable&) = delete;
	Noncopyable& operator=(const Noncopyable&) = delete;
};

