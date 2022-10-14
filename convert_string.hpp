#pragma once
#include <string>

// 共通設計として、別にutf-8文字コードをShift-JIS文字コードに変換する・またその逆をするというわけではない
// あくまで保持するデータそのものは同じで、諸々の関数で受け取って処理する際にu8stringが使えないことへの対策
// utf-8なら、半角文字に関してはASCIIと同じ文字コードのため、変換してもShift-JIS的に解釈できない文字が含まれるようになるだけで多分問題ないはず
// ただ、formatで全角文字に置換しようとかするとダメそう　そのあたりは気を付けるように
// マジで……諸々の関数がu8stringに対応したらそれで解決する話なんですが？？？？？　C++？？？？？？　おい

	// stringにu8stringを代入する
std::string operator<<=(std::string& s, const std::u8string& u8);

// u8stringにstringを代入する
std::u8string operator<<=(std::u8string& u8, const std::string& s);

namespace ext
{

	// u8stringをstringに変換したものを返す
	std::string convert(const std::u8string& u8);

	// stringをu8stringに変換したものを返す
	std::u8string convert(const std::string& s);

	// u8stringの文字をconst char*として返す（標準のc_str関数はconst char8_t*）
	const char* tochar(const std::u8string& u8);
}

