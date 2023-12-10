
// ImagePro2Doc.cpp: CImagePro2Doc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImagePro2.h"
#endif

#include "ImagePro2Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImagePro2Doc

IMPLEMENT_DYNCREATE(CImagePro2Doc, CDocument)

BEGIN_MESSAGE_MAP(CImagePro2Doc, CDocument)
END_MESSAGE_MAP()


// CImagePro2Doc 생성/소멸

CImagePro2Doc::CImagePro2Doc() noexcept
{
	inputimg = NULL;
	inputimg2 = NULL;
	resultimg = NULL;

	gresultimg = NULL;
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CImagePro2Doc::~CImagePro2Doc()
{
	int i;
	if (inputimg != NULL)
	{
		for (i = 0; i < ImageHeight; i++)
			free(inputimg[i]);
		free(inputimg);
	}
	if (resultimg != NULL)
	{
		for (i = 0; i < ImageHeight; i++)
			free(resultimg[i]);
		free(resultimg);
	}
	if (inputimg2 != NULL)
	{
		for (i = 0; i < ImageHeight; i++)
			free(inputimg2[i]);
		free(inputimg2);
	}
	if (gresultimg != NULL)
	{
		for (i = 0; i < gImageHeight; i++)
			free(gresultimg[i]);
		free(gresultimg);
	}

}

BOOL CImagePro2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CImagePro2Doc serialization

void CImagePro2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		LoadImageFile(ar);

		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CImagePro2Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CImagePro2Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImagePro2Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImagePro2Doc 진단

#ifdef _DEBUG
void CImagePro2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImagePro2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImagePro2Doc 명령
int CImagePro2Doc::LoadImageFile(CArchive& ar)
{
	{
		int maxValue, i;
		char type[16], buf[256];
		CFile* fp = ar.GetFile();
		CString fname = fp->GetFilePath();
		if (!strcmp(strchr(fname, '.'), ".bmp") || !strcmp(strchr(fname, '.'), ".BMP")) {
			BITMAPFILEHEADER bf;
			BITMAPINFOHEADER bi;

			fp->Read(&bf, sizeof(BITMAPFILEHEADER));
			fp->Read(&bi, sizeof(BITMAPINFOHEADER));

			ImageWidth = bi.biWidth;
			ImageHeight = bi.biHeight;
			depth = bi.biBitCount / 8;

			if (depth != 1 && depth != 3 && depth != 4) {
				AfxMessageBox("지원하지 않는 BMP 파일 형식입니다.");
				return 0;
			}
			if (depth == 4) {
				depth = 3;  // 4-byte BMP images are typically RGBA, convert to RGB
			}

			// BMP 이미지 데이터는 아래에서 위로 저장되므로 뒤집어서 읽어야 함
			inputimg = (unsigned char**)malloc(ImageHeight * sizeof(unsigned char*));
			resultimg = (unsigned char**)malloc(ImageHeight * sizeof(unsigned char*));

			int rowSize = ((ImageWidth * depth + 3) / 4) * 4;  // Adjust for padding

			for (int i = 0; i < ImageHeight; i++) {
				inputimg[i] = (unsigned char*)malloc(rowSize);
				resultimg[i] = (unsigned char*)malloc(rowSize);
				fp->Seek(bf.bfOffBits + (ImageHeight - i - 1) * rowSize, CFile::begin);
				fp->Read(inputimg[i], rowSize);

				// BGR to RGB
				for (int j = 0; j < ImageWidth * depth; j += depth) {
					unsigned char temp = inputimg[i][j];
					inputimg[i][j] = inputimg[i][j + 2];
					inputimg[i][j + 2] = temp;
				}
			}
		}
		//strcmp(strchr(fname, '.'), ".ppm");	// == 0 => 확장자가 ppm
		else {
			if (!strcmp(strchr(fname, '.'), ".ppm") || !strcmp(strchr(fname, '.'), ".PPM") ||
				!strcmp(strchr(fname, '.'), ".pgm") || !strcmp(strchr(fname, '.'), ".PGM")) {

				ar.ReadString(type, 15);

				do {
					ar.ReadString(buf, 255);
				} while (buf[0] == '#');
				sscanf(buf, "%d %d", &ImageWidth, &ImageHeight);

				do {
					ar.ReadString(buf, 255);
				} while (buf[0] == '#');
				sscanf(buf, "%d", &maxValue);

				if (!strcmp(type, "P5")) depth = 1;
				else depth = 3;
			}
			else if (!strcmp(strchr(fname, '.'), ".raw") || !strcmp(strchr(fname, '.'), ".RAW")) {
				ImageWidth = 256;
				ImageHeight = 256;
				depth = 1;
			}

			//메모리 할당
			inputimg = (unsigned char**)malloc(ImageHeight * sizeof(unsigned char*));
			resultimg = (unsigned char**)malloc(ImageHeight * sizeof(unsigned char*));
			for (i = 0; i < ImageHeight; i++) {
				inputimg[i] = (unsigned char*)malloc(ImageWidth * depth);
				resultimg[i] = (unsigned char*)malloc(ImageWidth * depth);
			}

			//파일에서 읽어서 저장
			for (i = 0; i < ImageHeight; i++)
				ar.Read(inputimg[i], ImageWidth * depth);
		}
		return 0;
	}
}


void CImagePro2Doc::LoadSecondImageFile(CArchive& ar)
{
	int w, h, d;
	int maxValue, i;
	char type[16], buf[256];
	CFile* fp = ar.GetFile();
	CString fname = fp->GetFilePath();

	if (!strcmp(strchr(fname, '.'), ".ppm") || !strcmp(strchr(fname, '.'), ".PPM") ||
		!strcmp(strchr(fname, '.'), ".pgm") || !strcmp(strchr(fname, '.'), ".PGM")) {

		ar.ReadString(type, 15);

		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf(buf, "%d %d", &w, &h);

		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf(buf, "%d", &maxValue);

		if (!strcmp(type, "P5")) d = 1;
		else d = 3;
	}
	else if (!strcmp(strchr(fname, '.'), ".raw") || !strcmp(strchr(fname, '.'), ".RAW")) {
		w = 256;
		h = 256;
		d = 1;
	}

	if (ImageWidth != w || ImageHeight != h || depth != d) {
		AfxMessageBox("두번째 파일의 width,height,depth가 다르면 읽을 수 없습니다.");
		return;
	}

	//메모리 할당
	inputimg2 = (unsigned char**)malloc(ImageHeight * sizeof(unsigned char*));
	for (i = 0; i < ImageHeight; i++) {
		inputimg2[i] = (unsigned char*)malloc(ImageWidth * depth);
	}

	//파일에서 읽어서 저장
	for (i = 0; i < ImageHeight; i++)
		ar.Read(inputimg2[i], ImageWidth * depth);

	return;
}



