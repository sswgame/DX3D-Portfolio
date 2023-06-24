#pragma once

class CTexture;

#include "ListUI.h"

class ParamUI
{
public:
	static int KeyCount;

public:
	static int   Param_Int(const string& _strName, const int* _pInOut);
	static float Param_Float(const string& _strName, const float* _pInOut);
	static Vec2  Param_Vec2(const string& _strName, const Vec2* _pInOut);
	static Vec4  Param_Vec4(const string& _strName, const Vec4* _pInOut);

	static std::string Param_String(const string& _strName, const char* _pIn); // ���ڿ� ǥ�� ( �б� ���� )
	static void        Param_String_ReadOnly(const std::string& _strName, const char* _pIn);

	// ��ȯ�� : ����ƮUI Ȱ��ȭ ���� �� true �ƴϸ� false
	static bool Param_Tex(const string& _strName, CTexture* _pCurTex, UI* _pInst = nullptr, DBCLKED _pFunc = nullptr);

	static int  Param_Int_DropDown(const string& _strName, const int* _pInOut, const std::vector<std::string>& vecList);
	static int  Param_Int_CheckBox(const string& _strName, const int* _pInOut);
	static Vec4 Param_Vec4_ColorPicker(const std::string& _strName, const Vec4* _pInOut);
};
