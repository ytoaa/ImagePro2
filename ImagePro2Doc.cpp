
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
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	inputImg = NULL;
	inputImg2 = NULL;
	resultImg = NULL;
	gResultImg = NULL;

}

CImagePro2Doc::~CImagePro2Doc()
{
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


void CImagePro2Doc::LoadImageFile(CArchive& ar)
{
	int i, maxVaule;
	char type[16], buf[256];
	CFile* fp = ar.GetFile();
	CString fname = fp->GetFilePath();
	const char* fileExt = strrchr(fname, '.');

	if (fileExt == nullptr) {
		AfxMessageBox("파일 확장자를 찾을 수 없습니다.");
		return;
	}

	if (strcmp(fileExt, ".bmp") == 0 || strcmp(fileExt, ".BMP") == 0)
	{
		BITMAPFILEHEADER bf;
		BITMAPINFOHEADER bi;

		fp->Read(&bf, sizeof(BITMAPFILEHEADER));
		fp->Read(&bi, sizeof(BITMAPINFOHEADER));

		imageWidth = bi.biWidth;
		imageHeight = bi.biHeight;
		depth = bi.biBitCount / 8;

		if (depth != 1 && depth != 3 && depth != 4) {
			AfxMessageBox("지원하지 않는 BMP 파일 형식입니다.");
			return;
		}
		if (depth == 4) {
			depth = 3;  // 4-byte BMP images are typically RGBA, convert to RGB
		}

		// BMP 이미지 데이터는 아래에서 위로 저장되므로 뒤집어서 읽어야 함
		inputImg = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
		resultImg = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));

		int rowSize = ((imageWidth * depth + 3) / 4) * 4;  // Adjust for padding

		for (i = 0; i < imageHeight; i++) {
			inputImg[i] = (unsigned char*)malloc(rowSize);
			resultImg[i] = (unsigned char*)malloc(rowSize);
			fp->Seek(bf.bfOffBits + (imageHeight - i - 1) * rowSize, CFile::begin);
			fp->Read(inputImg[i], rowSize);

			// BGR to RGB
			for (int j = 0; j < imageWidth * depth; j += depth) {
				unsigned char temp = inputImg[i][j];
				inputImg[i][j] = inputImg[i][j + 2];
				inputImg[i][j + 2] = temp;
			}
		}
	}
	else if (strcmp(fileExt, ".ppm") == 0 || strcmp(fileExt, ".PPM") == 0 || strcmp(fileExt, ".PGM") == 0 || strcmp(fileExt, ".pgm") == 0)
	{
		ar.ReadString(type, 15);
		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d %d", &imageWidth, &imageHeight);

		do {
			ar.ReadString(buf, 255);
		} while (buf[0] == '#');
		sscanf_s(buf, "%d", &maxVaule);

		if (strcmp(type, "P5") == 0) depth = 1;
		else depth = 3;
	}
	else if (strcmp(fileExt, ".raw") == 0 || strcmp(fileExt, ".RAW") == 0)
	{
		if (fp->GetLength() != 256 * 256) {
			AfxMessageBox("256x256 크기의 파일만 사용 가능합니다.");
			return;
		}
		imageWidth = 256;
		imageHeight = 256;
		depth = 1;
	}
	else
	{
		// 다른 파일 유형에 대한 처리 코드
		AfxMessageBox("지원하지 않는 파일 형식입니다.");
		return;
	}
	if (inputImg == nullptr) {
		inputImg = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));
		resultImg = (unsigned char**)malloc(imageHeight * sizeof(unsigned char*));

		for (i = 0; i < imageHeight; i++) {
			inputImg[i] = (unsigned char*)malloc(imageWidth * depth);
			resultImg[i] = (unsigned char*)malloc(imageWidth * depth);
		}
	}

	// 나머지 코드는 여기에 두면 중복 코드를 줄일 수 있습니다.

	if (strcmp(fileExt, ".bmp") != 0) {
		// BMP 파일이 아닌 경우에만 실행
		for (i = 0; i < imageHeight; i++)
			ar.Read(inputImg[i], imageWidth * depth);
	}
}




void CImagePro2Doc::LoadTwoImages()
{
	CFile file;
	CFileDialog dlg(TRUE);

	AfxMessageBox("Select the First Image");

	if (dlg.DoModal() == IDOK) {
		file.Open(dlg.GetPathName(), CFile::modeRead);
		file.Read(inputImg, 256 * 256);
		file.Close();
		// TODO: 여기에 구현 코드 추가.
	}
}


void CImagePro2Doc::PixelAdd()
{
	int value;

	for(int y =0; y<256; y++)
		for (int x = 0; x < 256; x++) {
			value = inputImg[y][x] + 100;
			if (value > 255) resultImg[y][x] = 255;
			else resultImg[y][x] = value;
		}
	// TODO: 여기에 구현 코드 추가.
}


void CImagePro2Doc::PixelTwoImageAdd()
{
	int vaule;

	LoadTwoImages();

	for (int y = 0; y < 256; y++)
		for (int x = 0; x < 256; x++) {
			vaule = inputImg[y][x] + inputImg2[y][x];
			if (vaule > 255) resultImg[y][x] = 255;
			else resultImg[y][x] = vaule;
		}
	// TODO: 여기에 구현 코드 추가.
}

#include <math.h>
void CImagePro2Doc::RegionSobel()
{
	int i, x, y, sum;
	float mask1[3][3] = { 1,0,-1,2,0,-2,1,0,-1 };
	float mask2[3][3] = { -1,-2,-1,0,0,0,1,2,1 };
	unsigned char** Er, ** Ec;

	Er = (unsigned char**)malloc((imageHeight) * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc((imageHeight) * sizeof(unsigned char*));
	// TODO: 여기에 구현 코드 추가.

	for (i = 0; i < imageHeight; i++) {
		Er[i] = (unsigned char*)malloc(imageWidth * depth);
		Ec[i] = (unsigned char*)malloc(imageWidth * depth);
	}
	Convolve(inputImg, Er, imageWidth, imageHeight, mask1, 0, depth);
	Convolve(inputImg, Ec, imageWidth, imageHeight, mask2, 0, depth);

	for (y=0;y<imageHeight;y++)
		for (x = 0; x < imageWidth * depth; x++) {
			sum = sqrt(Er[y][x] * Er[y][x] + Ec[y][x] * Ec[y][x]);
			if (sum > 255) sum = 255;
			if (sum < 0) sum = 0;
			resultImg[y][x] = (unsigned char)sum;
		}
}



void CImagePro2Doc::Convolve(unsigned char** inputImg, unsigned char** resultImg, int cols, int rows, float mask[][3] , int bias, int depth)
{
	// TODO: 여기에 구현 코드 추가.
	int x, y;
	int i, j;
	int red, green,blue;
	int sum;
	unsigned char** tmpImg;

	tmpImg = (unsigned char**)malloc((imageHeight + 2) * sizeof(unsigned char*));

	for (i = 0; i < imageHeight + 2;i++)
		tmpImg[i] = (unsigned char*)malloc((imageWidth + 2) * depth);

	for (y = 0; y < imageHeight + 2; y++)
		for (x = 0; x < (imageWidth + 2) * depth; x++)
			tmpImg[y][x] = 0;

	for (y = 1; y < imageHeight + 1; y++)
		for (x = 1; x < imageWidth + 1; x++)
			if (depth == 1) tmpImg[y][x] = inputImg[y - 1][x - 1];
			else if (depth == 3) {
				tmpImg[y][3 * x] = inputImg[y - 1][3 * (x - 1)];
				tmpImg[y][3 * x + 1] = inputImg[y - 1][3 * (x - 1) + 1];
				tmpImg[y][3 * x + 2] = inputImg[y - 1][3 * (x - 1) + 2];
			}
	for (y=0;y<imageHeight;y++) 
		for (x = 0; x < imageWidth; x++) {
			if (depth == 1) {
				sum = 0;
				for (i = 0; i < 3; i++)
					for (j = 0; j < 3; j++)
						sum += (int)(tmpImg[y + i][x + j] * mask[i][j]);
				if (sum > 255) sum = 255;
				if (sum < 0) sum = 0;
				resultImg[y][x] = (unsigned char) sum;
			}
			else if (depth == 3) {
				red = 0;
				green = 0;
				blue = 0;

				for (i=0;i<3;i++)
					for (j = 0; j < 3; j++) {
						red += (int)(tmpImg[y + i][3 * (x + j)] * mask[i][j]);
						green += (int)(tmpImg[y + i][3 * (x + j)+1] * mask[i][j]);
						blue += (int)(tmpImg[y + i][3 * (x + j)+2] * mask[i][j]);
					}
				if (red > 255) red = 255;
				if (red < 0) red = 0;
				if (green > 255) green = 255;
				if (green < 0) green = 0;
				if (blue > 255) blue = 255;
				if (blue < 0) blue = 0;

				resultImg[y][3 * x] = (unsigned char) red;
				resultImg[y][3 * x+1] = (unsigned char) green;
				resultImg[y][3 * x+2] = (unsigned char) blue;
			}
		}
	for (i = 0; i < imageHeight + 2; i++) free(tmpImg[i]);
	free(tmpImg);

		// TODO: 여기에 구현 코드 추가.
}


void CImagePro2Doc::RegionMedian() {
	int i, j, temp, x, y;
	int n[9];

	for (y = 1; y < imageHeight - 1; y++) {
		for (x = 1; x < imageWidth - 1; x++) {
			if (depth == 1) {
				n[0] = inputImg[y - 1][x - 1];
				n[1] = inputImg[y - 1][x];
				n[2] = inputImg[y - 1][x + 1];
				n[3] = inputImg[y][x - 1];
				n[4] = inputImg[y][x];
				n[5] = inputImg[y][x + 1];
				n[6] = inputImg[y + 1][x - 1];
				n[7] = inputImg[y + 1][x];
				n[8] = inputImg[y + 1][x + 1];

				for (i = 8; i > 0; i--) {
					for (j = 0; j < i; j++) {
						if (n[j] > n[j + 1]) {
							temp = n[j + 1];
							n[j + 1] = n[j];
							n[j] = temp;
						}
					}
				}

				resultImg[y][x] = n[4];
			}
			else if (depth == 3) { // RGB 처리 추가
				int r[9], g[9], b[9];
				for (int k = 0; k < 9; k++) {
					int row = k / 3;
					int col = k % 3;
					r[k] = inputImg[y - 1 + row][3 * (x - 1 + col)];
					g[k] = inputImg[y - 1 + row][3 * (x - 1 + col) + 1];
					b[k] = inputImg[y - 1 + row][3 * (x - 1 + col) + 2];
				}

				for (i = 8; i > 0; i--) {
					for (j = 0; j < i; j++) {
						if (r[j] > r[j + 1]) {
							temp = r[j + 1];
							r[j + 1] = r[j];
							r[j] = temp;
						}
						if (g[j] > g[j + 1]) {
							temp = g[j + 1];
							g[j + 1] = g[j];
							g[j] = temp;
						}
						if (b[j] > b[j + 1]) {
							temp = b[j + 1];
							b[j + 1] = b[j];
							b[j] = temp;
						}
					}
				}

				resultImg[y][3 * x] = r[4];
				resultImg[y][3 * x + 1] = g[4];
				resultImg[y][3 * x + 2] = b[4];
			}
		}
	}

}




void CImagePro2Doc::Erosion()
{
	// TODO: 여기에 구현 코드 추가.
	int x, y, min;
	for (y=1;y<imageHeight-1;y++)
		for (x = 1; x < imageWidth - 1; x++) {
			min = 255;
			if (inputImg[y - 1][x - 1] < min) min = inputImg[y - 1][x - 1];
			if (inputImg[y - 1][x] < min) min = inputImg[y - 1][x];
			if (inputImg[y][x - 1] < min) min = inputImg[y][x - 1];
			if (inputImg[y - 1][x + 1] < min) min = inputImg[y - 1][x + 1];
			if (inputImg[y + 1][x - 1] < min) min = inputImg[y + 1][x - 1];
			if (inputImg[y][x + 1] < min) min = inputImg[y][x + 1];
			if (inputImg[y + 1][x] < min) min = inputImg[y + 1][x];
			if (inputImg[y + 1][x + 1] < min) min = inputImg[y + 1][x + 1];
			if (inputImg[y][x] < min) min = inputImg[y][x];

			resultImg[y][x] = min;
		}
}


void CImagePro2Doc::Dilation()
{
	int x, y, max;
	for (y = 1; y < imageHeight - 1; y++)
		for (x = 1; x < imageWidth - 1; x++) {
			max = 0;
			if (inputImg[y - 1][x - 1] > max) max = inputImg[y - 1][x - 1];
			if (inputImg[y - 1][x] > max) max = inputImg[y - 1][x];
			if (inputImg[y][x - 1] > max) max = inputImg[y][x - 1];
			if (inputImg[y - 1][x + 1] > max) max = inputImg[y - 1][x + 1];
			if (inputImg[y + 1][x - 1] > max) max = inputImg[y + 1][x - 1];
			if (inputImg[y][x + 1] > max) max = inputImg[y][x + 1];
			if (inputImg[y + 1][x] > max) max = inputImg[y + 1][x];
			if (inputImg[y + 1][x + 1] > max) max = inputImg[y + 1][x + 1];
			if (inputImg[y][x] > max) max = inputImg[y][x];

			resultImg[y][x] = max;
		}
	// TODO: 여기에 구현 코드 추가.
}


void CImagePro2Doc::Opening()
{
	Erosion();
	CopyResultToInput();
	Erosion();

	CopyResultToInput();
	Erosion();

	CopyResultToInput();
	Dilation();

	CopyResultToInput();
	Dilation();

	CopyResultToInput();
	Dilation();
	// TODO: 여기에 구현 코드 추가.
}


void CImagePro2Doc::CopyResultToInput()
{
	// TODO: 여기에 구현 코드 추가.
	int x, y;
	for (y = 0; y < imageHeight; y++)
		for (x = 0; x < imageWidth; x++)
			inputImg[y][x] = resultImg[y][x];
}


void CImagePro2Doc::Closing()
{
	// TODO: 여기에 구현 코드 추가.
	Dilation();
	CopyResultToInput();
	Dilation();

	CopyResultToInput();
	Dilation();

	CopyResultToInput();
	Erosion();

	CopyResultToInput();
	Erosion();

	CopyResultToInput();
	Erosion();
}


void CImagePro2Doc::GeometryZoominPixelCopy()
{	

	// TODO: 여기에 구현 코드 추가.
}
