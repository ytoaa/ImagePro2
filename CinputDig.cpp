// CinputDig.cpp: 구현 파일
//

#include "pch.h"
#include "ImagePro2.h"
#include "afxdialogex.h"
#include "CinputDig.h"


// CinputDig 대화 상자

IMPLEMENT_DYNAMIC(CinputDig, CDialogEx)

CinputDig::CinputDig(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, minputdig(0)
{

}

CinputDig::~CinputDig()
{
}

void CinputDig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITANGLE, minputdig);
}


BEGIN_MESSAGE_MAP(CinputDig, CDialogEx)
END_MESSAGE_MAP()


// CinputDig 메시지 처리기
