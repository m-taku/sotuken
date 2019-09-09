#pragma once
/// <summary>
/// フォントクラス
/// </summary>
class GameFont :public Noncopyable
{
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	GameFont();
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~GameFont();
	/// <summary>
	/// フォントを設定。
	/// </summary>
	/// <param name="font">
	/// DirectX::SpriteFontを使用して変更。（DirectX::SpriteFont*）
	/// </param>
	void SetFont(DirectX::SpriteFont* font)
	{
		//if (m_spriteFont != g_graphicsEngine->GetSpriteFont()) {
		//	delete m_spriteFont;
		//}
		//m_spriteFont = font;
	}
	/// <summary>
	/// 描画開始。
	/// </summary>
	void BeginDraw();
	/// <summary>
	/// 描画終了
	/// </summary>
	/// <remarks>
	/// そのままだと透過などの設定が変更されてままなので忘れずに呼び出してください。
	/// </remarks>
	void EndDraw();
	/// <summary>
	/// フォントの描画。
	/// </summary>
	/// <param name="text">
	/// 表示したいテキスト。
	/// </param>
	/// <param name="position">
	/// フォントを表示する座標。
	/// 座標系は、画面の左上が(-FRAME_BUFFER_W/2、FRAME_BUFFER_H/2)
	/// 画面の右下が(FRAME_BUFFER_W/2、-FRAME_BUFFER_H/2)となっている。
	/// </param>
	/// <param name="color">
	/// 文字の色
	/// </param>
	/// <param name="rotation">
	/// 文字の回転
	/// </param>
	/// <param name="scale">
	/// 大きさ
	/// </param>
	/// <param name="pivot">
	/// 2D上でどこを起点にするか
	///	0.5, 0.5で画像の中心が基点。
	///	0.0, 0.0で画像の左下。
	///	1.0, 1.0で画像の右上。
	/// UnityのuGUIに準拠。
	/// </param>
	void Draw(
		wchar_t const* text,
		const CVector2& position,
		const CVector4& color = CVector4::White(),
		float rotation = 0.0f,
		float scale = 1.0f,
		CVector2 pivot = { 0.5f, 0.5f }
	);
private:
	DirectX::SpriteBatch*	m_spriteBatch = nullptr;	//スプライトバッチ。
	DirectX::SpriteFont*	m_spriteFont = nullptr;		//スプライトフォント。
	CMatrix m_scaleMat = CMatrix::Identity();				//拡大行列
};

