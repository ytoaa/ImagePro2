
// ImagePro2View.cpp: CImagePro2View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ImagePro2.h"
#endif

#include "ImagePro2Doc.h"
#include "ImagePro2View.h"
#include "CinputDig.h"
#include <Vfw.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#define AVI_FILE	16
#endif


// CImagePro2View

IMPLEMENT_DYNCREATE(CImagePro2View, CScrollView)

BEGIN_MESSAGE_MAP(CImagePro2View, CScrollView)
	ON_COMMAND(ID_MENUTEST, &CImagePro2View::OnMenutest)
	ON_COMMAND(ID_PIXELADD, &CImagePro2View::OnPixeladd)
	ON_COMMAND(ID_PIXELSUB, &CImagePro2View::OnPixelsub)
	ON_COMMAND(ID_PIXELDIV, &CImagePro2View::OnPixeldiv)
	ON_COMMAND(ID_PIXELMUL, &CImagePro2View::OnPixelmul)
	ON_COMMAND(ID_PIXEL_HISTOEQ, &CImagePro2View::OnPixelHistoeq)
	ON_COMMAND(ID_PIXELSTR, &CImagePro2View::OnPixelstr)
	ON_COMMAND(ID_PIXELBINARIZATION, &CImagePro2View::OnPixelbinarization)
	ON_COMMAND(ID_PIXELTWOIMAGEADD, &CImagePro2View::OnPixeltwoimageadd)
	ON_COMMAND(ID_PIXELIMAGESUB, &CImagePro2View::OnPixelimagesub)
	ON_COMMAND(ID_REGION_SHARPENING, &CImagePro2View::OnRegionSharpening)
	ON_COMMAND(ID_REGION_SMOOTHING, &CImagePro2View::OnRegionSmoothing)
	ON_COMMAND(ID_REGION_EMBOSSING, &CImagePro2View::OnRegionEmbossing)
	ON_COMMAND(ID_Region_Prewit, &CImagePro2View::OnRegionPrewit)
	ON_COMMAND(ID_Region_Roberts, &CImagePro2View::OnRegionRoberts)
	ON_COMMAND(ID_Region_Sobel, &CImagePro2View::OnRegionSobel)
	
ON_COMMAND(ID_REGION_EVERAGE_FLITERING, &CImagePro2View::OnRegionEverageFlitering)
ON_COMMAND(ID_REGION_MEDIANFILTERING, &CImagePro2View::OnRegionMedianfiltering)
ON_COMMAND(ID_GEOMETRY_ZOOMIN_PIXEL_COPY, &CImagePro2View::OnGeometryZoominPixelCopy)
ON_COMMAND(ID_MOPOLOGYCOLORTOGRAY, &CImagePro2View::OnMopologycolortogray)
ON_COMMAND(ID_MOPOLOGYBINARIZATION, &CImagePro2View::OnMopologybinarization)
ON_COMMAND(ID_MOPOLOGYEROSION, &CImagePro2View::OnMopologyerosion)
ON_COMMAND(ID_MOPOLOGYDILATION, &CImagePro2View::OnMopologydilation)
ON_COMMAND(ID_MOPOLOGYOPENNING, &CImagePro2View::OnMopologyopenning)
ON_COMMAND(ID_MOPOLOGYCLOSING, &CImagePro2View::OnMopologyclosing)
ON_COMMAND(ID_GEOMETRY_ZOOMIN_BINUERINTERPOLATION, &CImagePro2View::OnGeometryZoominBinuerinterpolation)
ON_COMMAND(ID_GEOMETRY_ZOOMOUT_SUBSAMPLING, &CImagePro2View::OnGeometryZoomoutSubsampling)
ON_COMMAND(ID_GEOMETRY_ZOOMOUT_MEAN_SUB, &CImagePro2View::OnGeometryZoomoutMeanSub)
ON_COMMAND(ID_GEOMETRY_ZOOMOUT_AVG, &CImagePro2View::OnGeometryZoomoutAvg)
ON_COMMAND(ID_GEOMETRY_ROTATION, &CImagePro2View::OnGeometryRotation)
ON_COMMAND(ID_GEOMETRY_MIRROR, &CImagePro2View::OnGeometryMirror)
ON_COMMAND(ID_GEOMETRY_FLIP, &CImagePro2View::OnGeometryFlip)
ON_COMMAND(ID_GEOMETRY_WARPING, &CImagePro2View::OnGeometryWarping)
ON_COMMAND(ID_AVI_VIEW, &CImagePro2View::OnAviView)
END_MESSAGE_MAP()

// CImagePro2View 생성/소멸

CImagePro2View::CImagePro2View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CImagePro2View::~CImagePro2View()
{
}

BOOL CImagePro2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CImagePro2View 그리기

void CImagePro2View::OnDraw(CDC* pDC)
{
	CImagePro2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	int x, y;
	if (viewMode == AVI_FILE) {
		PAVIFILE pavi;
		AVIFILEINFO fi;
		int stm;
		PAVISTREAM pstm = NULL;
		AVISTREAMINFO si;
		PGETFRAME pfrm = NULL;
		int frame;
		LPBITMAPINFOHEADER pbmpih;
		unsigned char* image;

		int x, y;

		AVIFileInit();
		AVIFileOpen(&pavi, AVIFileName, OF_READ | OF_SHARE_DENY_NONE, NULL);
		AVIFileInfo(pavi, &fi, sizeof(AVIFILEINFO));

		for (stm = 0; stm < fi.dwStreams; stm++)
		{
			AVIFileGetStream(pavi, &pstm, 0, stm);
			AVIStreamInfo(pstm, &si, sizeof(si));
			if (si.fccType == streamtypeVIDEO)
			{
				pfrm = AVIStreamGetFrameOpen(pstm, NULL);
				for (frame = 0; frame < si.dwLength; frame++)
				{
					pbmpih = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pfrm, frame);
					if (!pbmpih)	continue;

					image = (unsigned char*)((LPSTR)pbmpih + (WORD)pbmpih->biSize);

					/*
					for(y=0;y<fi.dwHeight;y++)
						for (x = 0; x < fi.dwWidth; x++)
						{
							pDC->SetPixel(x, fi.dwHeight-1-y,
								RGB(image[(y * fi.dwWidth + x) * 3 + 2],
									image[(y * fi.dwWidth + x) * 3 + 1],
									image[(y * fi.dwWidth + x) * 3 + 0]));
						}
					*/
					pDC->SetStretchBltMode(COLORONCOLOR);
					::SetDIBitsToDevice(pDC->GetSafeHdc(),
						0, 0, fi.dwWidth, fi.dwHeight,
						0, 0, 0, fi.dwWidth,
						image, (BITMAPINFO*)pbmpih, DIB_RGB_COLORS);
					Sleep(30);
				}

			}
		}
		AVIStreamGetFrameClose(pfrm);
		AVIStreamRelease(pstm);
		AVIFileRelease(pavi);
		AVIFileExit();

	}
	if (pDoc->inputimg != NULL)
	{
		if (pDoc->depth == 1) {
			for (int y = 0; y < pDoc->ImageHeight; y++)
				for (int x = 0; x < pDoc->ImageWidth; x++)
					pDC->SetPixel(x, y, RGB(pDoc->inputimg[y][x], pDoc->inputimg[y][x], pDoc->inputimg[y][x]));
		}
		else {
			for (int y = 0; y < pDoc->ImageHeight; y++)
				for (int x = 0; x < pDoc->ImageWidth; x++)
					pDC->SetPixel(x, y, RGB(pDoc->inputimg[y][3 * x + 0], pDoc->inputimg[y][3*x+1], pDoc->inputimg[y][3 * x + 2]));
		}
	}
	if (pDoc->resultimg != NULL) {
		if (pDoc->depth == 1) {
			for (int y = 0; y < pDoc->ImageHeight; y++)
				for (int x = 0; x < pDoc->ImageWidth; x++)
					pDC->SetPixel(pDoc->ImageWidth + 20 + x, y, RGB(pDoc->resultimg[y][x], pDoc->resultimg[y][x], pDoc->resultimg[y][x]));
		}
		else
		{
			for (int y = 0; y < pDoc->ImageHeight; y++)
				for (int x = 0; x < pDoc->ImageWidth; x++)
					pDC->SetPixel(pDoc->ImageWidth + 20 + x, y, RGB(pDoc->resultimg[y][3 * x + 0], pDoc->resultimg[y][3 * x + 1], pDoc->resultimg[y][3 * x + 2]));
		}
	}
	if (pDoc->inputimg2 != NULL) {
		if (pDoc->depth == 1) {
			for (int y = 0; y < pDoc->ImageHeight; y++)
				for (int x = 0; x < pDoc->ImageWidth; x++)
					pDC->SetPixel(2 * (pDoc->ImageWidth + 20) + x, y, RGB(pDoc->inputimg2[y][x], pDoc->inputimg2[y][x], pDoc->inputimg2[y][x]));
		}
		else {
			for (int y = 0; y < pDoc->ImageHeight; y++)
				for (int x = 0; x < pDoc->ImageWidth; x++)
					pDC->SetPixel(2 * (pDoc->ImageWidth + 20) + x, y, RGB(pDoc->inputimg2[y][3 * x + 0], pDoc->inputimg2[y][3 * x + 1], pDoc->inputimg2[y][3 * x + 2]));
		}
	}
	if (pDoc->gresultimg != NULL) {
		if (pDoc->depth == 1) {
			for (int y = 0; y < pDoc->gImageHeight; y++)
				for (int x = 0; x < pDoc->gImageWidth; x++)
					pDC->SetPixel( x, pDoc->ImageHeight+20+y, RGB(pDoc->gresultimg[y][x], pDoc->gresultimg[y][x], pDoc->gresultimg[y][x]));
		}
		else {
			for (int y = 0; y < pDoc->gImageHeight; y++)
				for (int x = 0; x < pDoc->gImageWidth; x++)
					pDC->SetPixel( x, pDoc->ImageHeight + 20+y, RGB(pDoc->gresultimg[y][3 * x + 0], pDoc->gresultimg[y][3 * x + 1], pDoc->gresultimg[y][3 * x + 2]));
		}
	}
	
}
   

void CImagePro2View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy =2048;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CImagePro2View 진단

#ifdef _DEBUG
void CImagePro2View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImagePro2View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImagePro2Doc* CImagePro2View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImagePro2Doc)));
	return (CImagePro2Doc*)m_pDocument;
}
#endif //_DEBUG


// CImagePro2View 메시지 처리기


void CImagePro2View::OnMenutest()
{
	AfxMessageBox("안녕하세요 메뉴출력입니다");
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CImagePro2View::OnPixeladd()
{
	CImagePro2Doc* pDoc = GetDocument();
	if (pDoc->inputimg == NULL)return;

	int x, y,value;

	for(y=0;y<pDoc->ImageHeight;y++)
		for (x = 0; x < pDoc->ImageWidth; x++)
			{
			if (pDoc->depth == 1) {
				value = pDoc->inputimg[y][x] + 100;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][x] = value;
			}
			else {
				value = pDoc->inputimg[y][3*x+0] + 100;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3*x+0] = value;

				value = pDoc->inputimg[y][3 * x + 1] + 100;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 1] = value;

				value = pDoc->inputimg[y][3 * x + 2] + 100;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 2] = value;
			}
		}
	Invalidate();

	
}


void CImagePro2View::OnPixelsub()
{
	CImagePro2Doc* pDoc = GetDocument();
	if (pDoc->inputimg == NULL)return;

	int x, y, value;

	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x < pDoc->ImageWidth; x++)
		{
			if (pDoc->depth == 1) {
				value = pDoc->inputimg[y][x] - 100;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][x] = value;
			}
			else {
				value = pDoc->inputimg[y][3 * x + 0] - 100;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 0] = value;

				value = pDoc->inputimg[y][3 * x + 1] - 100;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 1] = value;

				value = pDoc->inputimg[y][3 * x + 2] - 100;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 2] = value;
			}
		}
	Invalidate();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CImagePro2View::OnPixeldiv()
{
	CImagePro2Doc* pDoc = GetDocument();

	int x, y, value;

	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x < pDoc->ImageWidth; x++)
		{
			if (pDoc->depth == 1) {
				value = pDoc->inputimg[y][x] / 1.2;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][x] = value;
			}
			else {
				value = pDoc->inputimg[y][3 * x + 0] / 1.2;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 0] = value;

				value = pDoc->inputimg[y][3 * x + 1] / 1.2;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 1] = value;

				value = pDoc->inputimg[y][3 * x + 2] / 1.2;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 2] = value;
			}
		}
	Invalidate();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CImagePro2View::OnPixelmul()
{
	CImagePro2Doc* pDoc = GetDocument();

	int x, y, value;

	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x < pDoc->ImageWidth; x++)
		{
			if (pDoc->depth == 1) {
				value = pDoc->inputimg[y][x] * 1.2;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][x] = value;
			}
			else {
				value = pDoc->inputimg[y][3 * x + 0] * 1.2;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 0] = value;

				value = pDoc->inputimg[y][3 * x + 1] * 1.2;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 1] = value;

				value = pDoc->inputimg[y][3 * x + 2] * 1.2;
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 2] = value;
			}
		}
	Invalidate();


}


void CImagePro2View::OnPixelHistoeq()
{
	CImagePro2Doc* pDoc = GetDocument();
	int x, y, k;
	int hist[256]{}, sum[256]{};
	int rhist[256]{}, rsum[256]{}, ghist[256]{}, gsum[256]{}, bhist[256]{}, bsum[256]{};
	int acc_hist = 0;
	int r_acc_hist = 0, g_acc_hist = 0, b_acc_hist = 0;
	float N = (float)(pDoc->ImageHeight * pDoc->ImageWidth);	//영상의 전체 픽셀 수

	//명암값 빈도수 조사
	for (y = 0; y < pDoc->ImageHeight; y++) {
		for (x = 0; x < pDoc->ImageWidth; x++) {
			if (pDoc->depth == 1) {	 //흑백 이미지
				hist[pDoc->inputimg[y][x]]++;
			}
			else {	 //컬러 이미지
				rhist[pDoc->inputimg[y][x * 3 + 0]]++;
				ghist[pDoc->inputimg[y][x * 3 + 1]]++;
				bhist[pDoc->inputimg[y][x * 3 + 2]]++;
			}
		}
	}

	//누적된 히스토그램 합 계산
	for (k = 0; k < 256; k++) {
		if (pDoc->depth == 1) {	 //흑백 이미지
			acc_hist += hist[k];
			sum[k] = acc_hist;
		}
		else {	 //컬러 이미지
			r_acc_hist += rhist[k];
			rsum[k] = r_acc_hist;

			g_acc_hist += ghist[k];
			gsum[k] = g_acc_hist;

			b_acc_hist += bhist[k];
			bsum[k] = b_acc_hist;
		}
	}

	for (y = 0; y < pDoc->ImageHeight; y++) {
		for (x = 0; x < pDoc->ImageWidth; x++) {
			if (pDoc->depth == 1) {	 //흑백 이미지
				k = pDoc->inputimg[y][x];
				pDoc->resultimg[y][x] = (unsigned char)(sum[k] / N * 255);
			}
			else {	 //컬러 이미지
				k = pDoc->inputimg[y][x * 3 + 0];
				pDoc->resultimg[y][x * 3 + 0] = (unsigned char)(rsum[k] / N * 255);

				k = pDoc->inputimg[y][x * 3 + 1];
				pDoc->resultimg[y][x * 3 + 1] = (unsigned char)(gsum[k] / N * 255);

				k = pDoc->inputimg[y][x * 3 + 2];
				pDoc->resultimg[y][x * 3 + 2] = (unsigned char)(bsum[k] / N * 255);
			}
		}
	}

	Invalidate();
}

void CImagePro2View::OnPixelstr()
{
CImagePro2Doc* pDoc = GetDocument();
//변수 선언
int min = 256, max = -1;
int rmin = 256, rmax = -1, gmin = 256, gmax = -1, bmin = 256, bmax = -1;
int x, y;
int p;

//최대, 최소 값 검색
for (y = 0; y < pDoc->ImageHeight; y++) {
	for (x = 0; x < pDoc->ImageWidth; x++) {
		if (pDoc->depth == 1) {	 //흑백 이미지
			if (pDoc->inputimg[y][x] < min) min = pDoc->inputimg[y][x];
			if (pDoc->inputimg[y][x] > max) max = pDoc->inputimg[y][x];
		}
		else {	 //컬러 이미지
			if (pDoc->inputimg[y][x * 3 + 0] < rmin) rmin = pDoc->inputimg[y][x * 3 + 0];
			if (pDoc->inputimg[y][x * 3 + 0] > rmax) rmax = pDoc->inputimg[y][x * 3 + 0];

			if (pDoc->inputimg[y][x * 3 + 1] < gmin) gmin = pDoc->inputimg[y][x * 3 + 1];
			if (pDoc->inputimg[y][x * 3 + 1] > gmax) gmax = pDoc->inputimg[y][x * 3 + 1];

			if (pDoc->inputimg[y][x * 3 + 2] < bmin) bmin = pDoc->inputimg[y][x * 3 + 2];
			if (pDoc->inputimg[y][x * 3 + 2] > bmax) bmax = pDoc->inputimg[y][x * 3 + 2];
		}
	}
}

//P' = (float)(P-min)/(max-min)*255
for (y = 0; y < pDoc->ImageHeight; y++) {
	for (x = 0; x < pDoc->ImageWidth; x++) {
		if (pDoc->depth == 1) {	 //흑백 이미지
			p = pDoc->inputimg[y][x];
			pDoc->resultimg[y][x] = (float)(p - min) / (max - min) * 255;
		}
		else {	 //컬러 이미지
			p = pDoc->inputimg[y][x * 3 + 0];
			pDoc->resultimg[y][x * 3 + 0] = (float)(p - rmin) / (rmax - rmin) * 255;

			p = pDoc->inputimg[y][x * 3 + 1];
			pDoc->resultimg[y][x * 3 + 1] = (float)(p - gmin) / (gmax - gmin) * 255;

			p = pDoc->inputimg[y][x * 3 + 2];
			pDoc->resultimg[y][x * 3 + 2] = (float)(p - bmin) / (bmax - bmin) * 255;
		}
	}
}

Invalidate();
}


void CImagePro2View::OnPixelbinarization()
{
	CImagePro2Doc* pDoc = GetDocument();

	int i, x, y;
	int threshold = 100;
	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x <pDoc->ImageWidth; x++)
		{
			if (pDoc->inputimg[y][x] > threshold) pDoc->resultimg[y][x] = 255;
			else                                                    pDoc->resultimg[y][x] = 0;
		}
	 Invalidate(); 
}


void CImagePro2View::OnPixeltwoimageadd()
{
	CImagePro2Doc* pDoc = GetDocument();
 
	
	
	CFileDialog dlg(true);
	CFile file;

	if (dlg.DoModal() == IDOK) {

		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		pDoc->LoadSecondImageFile(ar);
		file.Close();

		int x, y;
		for (y = 0; y < pDoc->ImageHeight; y++) {
			for (x = 0; x < pDoc->ImageWidth; x++) {
				if (pDoc->depth == 1) {	 //흑백 이미지
					pDoc->resultimg[y][x] = 0.5 * pDoc->inputimg[y][x] + 0.5 * pDoc->inputimg2[y][x];
				}
				else {	 //컬러 이미지
					pDoc->resultimg[y][x * 3 + 0] = 0.5 * pDoc->inputimg[y][x * 3 + 0] + 0.5 * pDoc->inputimg2[y][x * 3 + 0];
					pDoc->resultimg[y][x * 3 + 1] = 0.5 * pDoc->inputimg[y][x * 3 + 1] + 0.5 * pDoc->inputimg2[y][x * 3 + 1];
					pDoc->resultimg[y][x * 3 + 2] = 0.5 * pDoc->inputimg[y][x * 3 + 2] + 0.5 * pDoc->inputimg2[y][x * 3 + 2];
				}
			}
		}

		Invalidate();
	}
}


void CImagePro2View::LoadTwoImage()
{
	CImagePro2Doc* pDoc = GetDocument();
	CFileDialog dlg(TRUE);

	if (dlg.DoModal() == IDOK) {
		CFile file;
		file.Open(dlg.GetPathName(), CFile::modeRead); //  파일 열기
		CArchive ar(&file, CArchive::load);
		pDoc->LoadSecondImageFile(ar);

		file.Close();
	}
	
}


void CImagePro2View::OnPixelimagesub()
{
	CImagePro2Doc* pDoc = GetDocument();
	CFileDialog dlg(true);
	CFile file;

	if (dlg.DoModal() == IDOK) {

		file.Open(dlg.GetPathName(), CFile::modeRead);
		CArchive ar(&file, CArchive::load);
		pDoc->LoadSecondImageFile(ar);
		file.Close();

		int x, y;
		for (y = 0; y < pDoc->ImageHeight; y++) {
			for (x = 0; x < pDoc->ImageWidth; x++) {
				if (pDoc->depth == 1) {	 //흑백 이미지
					pDoc->resultimg[y][x] = abs(pDoc->inputimg[y][x] - pDoc->inputimg2[y][x]);
				}
				else {	 //컬러 이미지
					pDoc->resultimg[y][x * 3 + 0] = abs(pDoc->inputimg[y][x * 3 + 0] - pDoc->inputimg2[y][x * 3 + 0]);
					pDoc->resultimg[y][x * 3 + 1] = abs(pDoc->inputimg[y][x * 3 + 1] - pDoc->inputimg2[y][x * 3 + 1]);
					pDoc->resultimg[y][x * 3 + 2] = abs(pDoc->inputimg[y][x * 3 + 2] - pDoc->inputimg2[y][x * 3 + 2]);
				}
			}
		}

		Invalidate();
	}
}

void CImagePro2View::convolve(unsigned char** inimg, unsigned char** outimg, int cols, int rows, float mask[][3], int bias, int depth)
{
	int x, y;
	int i, j;
	int sum,rsum,gsum,bsum;

	for (y = 1; y < rows-1; y++)
		for (x = 1; x < cols-1; x++) 
		{
			if (depth == 1) {


				sum = 0;
				for (j = 0; j < 3; j++)
					for (i = 0; i < 3; i++)
					{
						sum += (inimg[y + j - 1][x + i - 1] * mask[j][i]);
					}
				sum += bias;
				if (sum > 255)sum = 255;
				else if (sum < 0)sum = 0;

				outimg[y][x] = sum;
			}
			else {
				rsum = 0; gsum = 0; bsum = 0;
				for (j = 0; j < 3; j++)
					for (i = 0; i < 3; i++)
					{
						rsum += (inimg[y + j - 1][3*(x + i - 1)+0] * mask[j][i]);
						gsum += (inimg[y + j - 1][3 * (x + i - 1) + 1] * mask[j][i]);
						bsum += (inimg[y + j - 1][3 * (x + i - 1) + 2] * mask[j][i]);

					}
				rsum += bias;
				gsum += bias;
				bsum += bias;

				if (rsum > 255)rsum = 255;
				else if (rsum < 0)rsum = 0;
				if (gsum > 255)gsum = 255;
				else if (gsum < 0)gsum = 0;
				if (bsum > 255)bsum = 255;
				else if (bsum < 0)bsum = 0;


				outimg[y][3 * x + 0] = rsum;
				outimg[y][3 * x + 1] = gsum;
				outimg[y][3 * x + 2] = bsum;
			}

			

		}
}

void CImagePro2View::OnRegionSharpening()
{
	CImagePro2Doc* pDoc = GetDocument();
	float kernel[3][3] = { {0,-1,0},
						  {-1,5,-1},
						  {0,-1,0} };

	convolve(pDoc->inputimg, pDoc->resultimg, pDoc->ImageWidth, pDoc->ImageHeight, kernel, 0, pDoc->depth);
	
	Invalidate();
}

void CImagePro2View::OnRegionSmoothing()
{
	CImagePro2Doc* pDoc = GetDocument();
	float kernel[3][3] = {
		{1 / 9.0f, 1 / 9.0f, 1 / 9.0f},
		{1 / 9.0f, 1 / 9.0f, 1 / 9.0f},
		{1 / 9.0f, 1 / 9.0f, 1 / 9.0f}
	};

	convolve(pDoc->inputimg, pDoc->resultimg, pDoc->ImageWidth, pDoc->ImageHeight, kernel, 0, pDoc->depth);

	Invalidate();
}



void CImagePro2View::OnRegionEmbossing()
{
	CImagePro2Doc* pDoc = GetDocument();
	float kernel[3][3] = {
		{-1, 0, 0},
		{0, 0, 0},
		{0, 0, 1}
	};

	convolve(pDoc->inputimg, pDoc->resultimg, pDoc->ImageWidth, pDoc->ImageHeight, kernel, 128, pDoc->depth);

	Invalidate();
}


void CImagePro2View::OnRegionPrewit()
{
	CImagePro2Doc* pDoc = GetDocument();
	int i, x, y, value;
	int rvalue, gvalue, bvalue;
	float kernel_h[3][3] = {
		{-1, -1, -1},
		{0, 0, 0},
		{1, 1, 1}
	};
	float kernel_v[3][3] = {
		{-1, 0, 1},
		{-1, 0, 1},
		{-1, 0, 1}
	};
	unsigned char ** Er, ** Ec;
	Er = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));

	for (i = 0; i < pDoc->ImageHeight; i++) {
		Er[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);


}
		

	convolve(pDoc->inputimg,Er,  pDoc->ImageWidth, pDoc->ImageHeight, kernel_h, 128, pDoc->depth);
	convolve(pDoc->inputimg,Ec,  pDoc->ImageWidth, pDoc->ImageHeight, kernel_v,128 , pDoc->depth);

	for (y = 0; y < pDoc->ImageHeight; y++) 
		for (x = 0; x < pDoc->ImageWidth; x++) {
			if (pDoc->depth == 1) {
				value = sqrt((Er[y][x] - 128) * (Er[y][x] - 128) + (Ec[y][x] - 128) * (Ec[y][x] - 128));
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][x] = (unsigned char)value;
			}
			else {
				rvalue = sqrt((Er[y][3 * x + 0] - 128) * (Er[y][3 * x + 0] - 128) + (Ec[y][3 * x + 0] - 128) * (Ec[y][3 * x + 0] - 128));;
				gvalue = sqrt((Er[y][3 * x + 1] - 128) * (Er[y][3 * x + 1] - 128) + (Ec[y][3 * x + 1] - 128) * (Ec[y][3 * x + 1] - 128));
				bvalue = sqrt((Er[y][3 * x + 2] - 128) * (Er[y][3 * x + 2] - 128) + (Ec[y][3 * x + 2] - 128) * (Ec[y][3 * x + 2] - 128));

				value = sqrt(rvalue * rvalue + gvalue * gvalue + bvalue * bvalue);

				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 0] = value;
				pDoc->resultimg[y][3 * x + 1] = value;
				pDoc->resultimg[y][3 * x + 2] = value;

				
			}
		}
	//메모리삭제
	for (i = 0; i < pDoc->ImageHeight; i++)
	{
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	Invalidate();

}



void CImagePro2View::OnRegionRoberts()
{
	CImagePro2Doc* pDoc = GetDocument();
	int i, x, y, value;
	int rvalue, gvalue, bvalue;
	float kernel_h[3][3] = {
		{-1, 0, 0},
		{0, 1, 0},
		{0, 0, 0}
	};
	float kernel_v[3][3] = {
		{0, 0, -1},
		{0, 1, 0},
		{0, 0,0}
	};
	unsigned char** Er, ** Ec;
	Er = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));

	for (i = 0; i < pDoc->ImageHeight; i++) {
		Er[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);


	}


	convolve(pDoc->inputimg, Er, pDoc->ImageWidth, pDoc->ImageHeight, kernel_h, 128, pDoc->depth);
	convolve(pDoc->inputimg, Ec, pDoc->ImageWidth, pDoc->ImageHeight, kernel_v, 128, pDoc->depth);

	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x < pDoc->ImageWidth; x++) {
			if (pDoc->depth == 1) {
				value = sqrt((Er[y][x] - 128) * (Er[y][x] - 128) + (Ec[y][x] - 128) * (Ec[y][x] - 128));
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][x] = (unsigned char)value;
			}
			else {
				rvalue = sqrt((Er[y][3 * x + 0] - 128) * (Er[y][3 * x + 0] - 128) + (Ec[y][3 * x + 0] - 128) * (Ec[y][3 * x + 0] - 128));;
				gvalue = sqrt((Er[y][3 * x + 1] - 128) * (Er[y][3 * x + 1] - 128) + (Ec[y][3 * x + 1] - 128) * (Ec[y][3 * x + 1] - 128));
				bvalue = sqrt((Er[y][3 * x + 2] - 128) * (Er[y][3 * x + 2] - 128) + (Ec[y][3 * x + 2] - 128) * (Ec[y][3 * x + 2] - 128));

				value = sqrt(rvalue * rvalue + gvalue * gvalue + bvalue * bvalue);

				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 0] = value;
				pDoc->resultimg[y][3 * x + 1] = value;
				pDoc->resultimg[y][3 * x + 2] = value;


			}
		}
	//메모리삭제
	for (i = 0; i < pDoc->ImageHeight; i++)
	{
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	Invalidate();
}


void CImagePro2View::OnRegionSobel()
{
	CImagePro2Doc* pDoc = GetDocument();
	int i, x, y, value;
	int rvalue, gvalue, bvalue;
	float kernel_h[3][3] = {
		{-1, -2, -1},
		{0, 0, 0},
		{1, 2, 1}
	};
	float kernel_v[3][3] = {
		{1, 0, -1},
		{2, 0, -2},
		{1, 0, -1}
	};
	unsigned char** Er, ** Ec;
	Er = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));
	Ec = (unsigned char**)malloc(pDoc->ImageHeight * sizeof(unsigned char*));

	for (i = 0; i < pDoc->ImageHeight; i++) {
		Er[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);
		Ec[i] = (unsigned char*)malloc(pDoc->ImageWidth * pDoc->depth);


	}


	convolve(pDoc->inputimg, Er, pDoc->ImageWidth, pDoc->ImageHeight, kernel_h, 128, pDoc->depth);
	convolve(pDoc->inputimg, Ec, pDoc->ImageWidth, pDoc->ImageHeight, kernel_v, 128, pDoc->depth);

	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x < pDoc->ImageWidth; x++) {
			if (pDoc->depth == 1) {
				value = sqrt((Er[y][x] - 128) * (Er[y][x] - 128) + (Ec[y][x] - 128) * (Ec[y][x] - 128));
				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][x] = (unsigned char)value;
			}
			else {
				rvalue = sqrt((Er[y][3 * x + 0] - 128) * (Er[y][3 * x + 0] - 128) + (Ec[y][3 * x + 0] - 128) * (Ec[y][3 * x + 0] - 128));;
				gvalue = sqrt((Er[y][3 * x + 1] - 128) * (Er[y][3 * x + 1] - 128) + (Ec[y][3 * x + 1] - 128) * (Ec[y][3 * x + 1] - 128));
				bvalue = sqrt((Er[y][3 * x + 2] - 128) * (Er[y][3 * x + 2] - 128) + (Ec[y][3 * x + 2] - 128) * (Ec[y][3 * x + 2] - 128));

				value = sqrt(rvalue * rvalue + gvalue * gvalue + bvalue * bvalue);

				if (value > 255)value = 255;
				else if (value < 0)value = 0;
				pDoc->resultimg[y][3 * x + 0] = value;
				pDoc->resultimg[y][3 * x + 1] = value;
				pDoc->resultimg[y][3 * x + 2] = value;


			}
		}
	//메모리삭제
	for (i = 0; i < pDoc->ImageHeight; i++)
	{
		free(Er[i]);
		free(Ec[i]);
	}
	free(Er);
	free(Ec);

	Invalidate();
}
void CImagePro2View::OnRegionEverageFlitering()
{
	CImagePro2Doc* pDoc = GetDocument();
	int x, y, i, j;
	int sum;
	int pixelcount;
	int xpos, ypos;
	int rsum, gsum, bsum;


	for (y = 0; y < pDoc->ImageHeight; y++)
		for (x = 0; x < pDoc->ImageWidth; x++)
		{
			sum = 0;	pixelcount = 0; rsum = gsum = bsum=0;
			for (j = -2; j <= 2; j++)
				for (i = -2; i <= 2; i++)
				{
					xpos = x + i;
					ypos = y + j;
					if (xpos >= 0 && xpos <= pDoc->ImageWidth - 1 && ypos >= 0 && ypos <= pDoc->ImageHeight - 1)
					{
						if(pDoc->depth==1)
						sum += pDoc->inputimg[y + j][x + i];
						else {
							rsum += pDoc->inputimg[y + j][3 * (x + i) + 0];
							gsum += pDoc->inputimg[y + j][3 * (x +i) +1 ];
							bsum += pDoc->inputimg[y + j][3 * (x + i) + 2];
						}
						pixelcount++;
					}
				}
			if(pDoc->depth==1)pDoc->resultimg[y][x] = sum / pixelcount;
			else 
			{
				pDoc->resultimg[y][3 * x + 0] = rsum / pixelcount;
				pDoc->resultimg[y][3 * x + 1 ]= gsum / pixelcount;
				pDoc->resultimg[y][3 * x + 2 ]= bsum / pixelcount;
			}
			
		}
	Invalidate();
}


void CImagePro2View::OnRegionMedianfiltering()
{
	CImagePro2Doc* pDoc = GetDocument();
	int x, y, i, j, temp;

	if (pDoc->depth == 1) {
		// 흑백 이미지의 경우
		for (y = 1; y < pDoc->ImageHeight - 1; y++)
			for (x = 1; x < pDoc->ImageWidth - 1; x++) {
				int n[9] = {
					pDoc->inputimg[y - 1][x - 1], pDoc->inputimg[y - 1][x], pDoc->inputimg[y - 1][x + 1],
					pDoc->inputimg[y][x - 1], pDoc->inputimg[y][x], pDoc->inputimg[y][x + 1],
					pDoc->inputimg[y + 1][x - 1], pDoc->inputimg[y + 1][x], pDoc->inputimg[y + 1][x + 1]
				};

				// 버블 정렬을 사용하여 중앙값 계산
				for (i = 8; i > 0; i--)
					for (j = 0; j < 8; j++)
						if (n[j] > n[j + 1]) {
							temp = n[j + 1];
							n[j + 1] = n[j];
							n[j] = temp;
						}

				// 중앙값을 결과 이미지에 저장
				pDoc->resultimg[y][x] = n[4];
			}
	}
	else {
		// 컬러 이미지의 경우
		for (y = 1; y < pDoc->ImageHeight - 1; y++)
			for (x = 1; x < pDoc->ImageWidth - 1; x++) {
				// 각 채널에 대해 주변 픽셀 값을 저장
				int n_r[9] = {
					pDoc->inputimg[y - 1][3 * (x - 1) + 0], pDoc->inputimg[y - 1][3 * x + 0], pDoc->inputimg[y - 1][3 * (x + 1) + 0],
					pDoc->inputimg[y][3 * (x - 1) + 0], pDoc->inputimg[y][3 * x + 0], pDoc->inputimg[y][3 * (x + 1) + 0],
					pDoc->inputimg[y + 1][3 * (x - 1) + 0], pDoc->inputimg[y + 1][3 * x + 0], pDoc->inputimg[y + 1][3 * (x + 1) + 0]
				};

				int n_g[9] = {
					pDoc->inputimg[y - 1][3 * (x - 1) + 1], pDoc->inputimg[y - 1][3 * x + 1], pDoc->inputimg[y - 1][3 * (x + 1) + 1],
					pDoc->inputimg[y][3 * (x - 1) + 1], pDoc->inputimg[y][3 * x + 1], pDoc->inputimg[y][3 * (x + 1) + 1],
					pDoc->inputimg[y + 1][3 * (x - 1) + 1], pDoc->inputimg[y + 1][3 * x + 1], pDoc->inputimg[y + 1][3 * (x + 1) + 1]
				};

				int n_b[9] = {
					pDoc->inputimg[y - 1][3 * (x - 1) + 2], pDoc->inputimg[y - 1][3 * x + 2], pDoc->inputimg[y - 1][3 * (x + 1) + 2],
					pDoc->inputimg[y][3 * (x - 1) + 2], pDoc->inputimg[y][3 * x + 2], pDoc->inputimg[y][3 * (x + 1) + 2],
					pDoc->inputimg[y + 1][3 * (x - 1) + 2], pDoc->inputimg[y + 1][3 * x + 2], pDoc->inputimg[y + 1][3 * (x + 1) + 2]
				};

				// 각 채널에 대해 버블 정렬을 사용하여 중앙값 계산
				for (i = 8; i > 0; i--)
					for (j = 0; j < 8; j++) {
						if (n_r[j] > n_r[j + 1]) {
							temp = n_r[j + 1];
							n_r[j + 1] = n_r[j];
							n_r[j] = temp;
						}
						if (n_g[j] > n_g[j + 1]) {
							temp = n_g[j + 1];
							n_g[j + 1] = n_g[j];
							n_g[j] = temp;
						}
						if (n_b[j] > n_b[j + 1]) {
							temp = n_b[j + 1];
							n_b[j + 1] = n_b[j];
							n_b[j] = temp;
						}
					}

				// 중앙값을 결과 이미지에 저장
				pDoc->resultimg[y][3 * x + 0] = n_r[4];
				pDoc->resultimg[y][3 * x + 1] = n_g[4];
				pDoc->resultimg[y][3 * x + 2] = n_b[4];
			}
	}

	Invalidate();
}


void CImagePro2View::OnMopologycolortogray()
{
	CImagePro2Doc* pDoc = GetDocument();
	if (pDoc->depth == 1) return;

	int x, y;
	int gray;

	for (y = 1; y < pDoc->ImageHeight - 1; y++) {
		for (x = 1; x < pDoc->ImageWidth - 1; x++) {
			gray = ((pDoc->inputimg[y][x * 3 + 0] + pDoc->inputimg[y][x * 3 + 1] + pDoc->inputimg[y][x * 3 + 0]) / 3);
			pDoc->inputimg[y][x * 3 + 0] = gray;
			pDoc->inputimg[y][x * 3 + 1] = gray;
			pDoc->inputimg[y][x * 3 + 2] = gray;
		}
	}

	Invalidate();
}


void CImagePro2View::OnMopologybinarization()
{
	CImagePro2Doc* pDoc = GetDocument();
	int x, y;
	int th = 100;

	for (y = 1; y < pDoc->ImageHeight - 1; y++) {
		for (x = 1; x < pDoc->ImageWidth - 1; x++) {
			if (pDoc->depth == 1) {
				if (pDoc->inputimg[y][x] > th)
					pDoc->inputimg[y][x] = 255;
				else
					pDoc->inputimg[y][x] = 0;
			}
			else {
				if (pDoc->inputimg[y][x * 3 + 0] > th) {
					pDoc->inputimg[y][x * 3 + 0] = 255;
					pDoc->inputimg[y][x * 3 + 1] = 255;
					pDoc->inputimg[y][x * 3 + 2] = 255;
				}
				else {
					pDoc->inputimg[y][x * 3 + 0] = 0;
					pDoc->inputimg[y][x * 3 + 1] = 0;
					pDoc->inputimg[y][x * 3 + 2] = 0;
				}
			}
		}
	}

	Invalidate();
}


void CImagePro2View::OnMopologyerosion()
{
	CImagePro2Doc* pDoc = GetDocument();
	int x, y;
	int min;

	for (y = 1; y < pDoc->ImageHeight - 1; y++) {
		for (x = 1; x < pDoc->ImageWidth - 1; x++) {
			if (pDoc->depth == 1) {	//흑백
				min = 255;
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 3; i++) {
						if (min > pDoc->inputimg[y + j - 1][x + i - 1])
							min = pDoc->inputimg[y + j - 1][x + i - 1];
					}
				}
				pDoc->resultimg[y][x] = min;
			}
			else {	//컬러
				min = 255;
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 3; i++) {
						if (min > pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 0])
							min = pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 0];
					}
				}
				pDoc->resultimg[y][x * 3 + 0] = min;

				min = 255;
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 3; i++) {
						if (min > pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 1])
							min = pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 1];
					}
				}
				pDoc->resultimg[y][x * 3 + 1] = min;

				min = 255;
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 3; i++) {
						if (min > pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 2])
							min = pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 2];
					}
				}
				pDoc->resultimg[y][x * 3 + 2] = min;
			}
		}
	}

	Invalidate();
}


void CImagePro2View::OnMopologydilation()
{
	CImagePro2Doc* pDoc = GetDocument();
	int x, y;
	int max;

	for (y = 1; y < pDoc->ImageHeight - 1; y++) {
		for (x = 1; x < pDoc->ImageWidth - 1; x++) {
			if (pDoc->depth == 1) {	//흑백
				max = 0;
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 3; i++) {
						if (max < pDoc->inputimg[y + j - 1][x + i - 1])
							max = pDoc->inputimg[y + j - 1][x + i - 1];
					}
				}
				pDoc->resultimg[y][x] = max;
			}
			else {	//컬러
				max = 0;
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 3; i++) {
						if (max < pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 0])
							max = pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 0];
					}
				}
				pDoc->resultimg[y][x * 3 + 0] = max;

				max = 0;
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 3; i++) {
						if (max < pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 1])
							max = pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 1];
					}
				}
				pDoc->resultimg[y][x * 3 + 1] = max;

				max = 0;
				for (int j = 0; j < 3; j++) {
					for (int i = 0; i < 3; i++) {
						if (max < pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 2])
							max = pDoc->inputimg[y + j - 1][(x + i - 1) * 3 + 2];
					}
				}
				pDoc->resultimg[y][x * 3 + 2] = max;
			}
		}
	}

	Invalidate();
}
void CImagePro2View::CopyResultToInput()
{
	CImagePro2Doc* pDoc = GetDocument();
	int x, y;

	for (y = 1; y < pDoc->ImageHeight - 1; y++) {
		for (x = 1; x < pDoc->ImageWidth * pDoc->depth; x++) {
			pDoc->inputimg[y][x] = pDoc->resultimg[y][x];
		}
	}
}

void CImagePro2View::OnMopologyopenning()
{
	OnMopologyerosion();
	CopyResultToInput();
	OnMopologyerosion();
	CopyResultToInput();
	OnMopologyerosion();
	CopyResultToInput();

	OnMopologydilation();
	CopyResultToInput();
	OnMopologydilation();
	CopyResultToInput();
	OnMopologydilation();
	CopyResultToInput();
}


void CImagePro2View::OnMopologyclosing()
{
	OnMopologydilation();
	CopyResultToInput();
	OnMopologydilation();
	CopyResultToInput();
	OnMopologydilation();
	CopyResultToInput();

	OnMopologyerosion();
	CopyResultToInput();
	OnMopologyerosion();
	CopyResultToInput();
	OnMopologyerosion();
	CopyResultToInput();
}
void CImagePro2View::OnGeometryZoominPixelCopy()
{
	CImagePro2Doc* pDoc = GetDocument();
	int x, y, i, j;

	int xscale = 3;
	int yscale = 2;

	if (pDoc->gresultimg != NULL) {
		for (i = 0; i < pDoc->gImageHeight; i++) free(pDoc->gresultimg[i]);
		free(pDoc->gresultimg);
	}

	pDoc->gImageWidth = pDoc->ImageWidth * xscale;
	pDoc->gImageHeight = pDoc->ImageHeight * yscale;

	//메모리 할당
	pDoc->gresultimg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->gImageHeight; i++) {
		pDoc->gresultimg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
	}

	/*
	//전방향사상
	for (y = 0; y < pDoc->imageHeight; y++) {
		for (x = 0; x < pDoc->imageWidth; x++) {
			for (j = 0; j < yscale; j++) {
				for (i = 0; i < xscale; i++) {
					if (pDoc->depth == 1) {
						pDoc->gResultImage[yscale * y + j][xscale * x + i] = pDoc->inputImage[y][x];
					}
					else {
						pDoc->gResultImage[yscale * y + j][(xscale * x + i) * 3 + 0] = pDoc->inputImage[y][x * 3 + 0];
						pDoc->gResultImage[yscale * y + j][(xscale * x + i) * 3 + 1] = pDoc->inputImage[y][x * 3 + 1];
						pDoc->gResultImage[yscale * y + j][(xscale * x + i) * 3 + 2] = pDoc->inputImage[y][x * 3 + 2];
					}
				}
			}
		}
	}
	*/

	//역방향 사상
	for (y = 0; y < pDoc->gImageHeight; y++) {
		for (x = 0; x < pDoc->gImageWidth; x++) {
			if (pDoc->depth == 1) {
				pDoc->gresultimg[y][x] = pDoc->inputimg[y / yscale][x / xscale];
			}
			else {
				pDoc->gresultimg[y][x * 3 + 0] = pDoc->inputimg[y / yscale][(x / xscale) * 3 + 0];
				pDoc->gresultimg[y][x * 3 + 1] = pDoc->inputimg[y / yscale][(x / xscale) * 3 + 1];
				pDoc->gresultimg[y][x * 3 + 2] = pDoc->inputimg[y / yscale][(x / xscale) * 3 + 2];
			}
		}
	}

	Invalidate();

}




void CImagePro2View::OnGeometryZoominBinuerinterpolation()
{
	CImagePro2Doc* pDoc = GetDocument();
	int x, y, i;

	float xscale = 3;
	float yscale = 3;
	float src_x, src_y;
	float alpha, beta;

	int Ax, Ay, Bx, By,Cx,Cy,Dx,Dy;
	int E, F;


	if (pDoc->gresultimg != NULL) {
		for (i = 0; i < pDoc->gImageHeight; i++) free(pDoc->gresultimg[i]);
		free(pDoc->gresultimg);
	}

	pDoc->gImageWidth = pDoc->ImageWidth * xscale ;
	pDoc->gImageHeight = pDoc->ImageHeight * yscale ;

	//메모리 할당
	pDoc->gresultimg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->gImageHeight; i++) {
		pDoc->gresultimg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
	}
	//역방향 
	for (y = 0; y < pDoc->gImageHeight; y++) {
		for (x = 0; x < pDoc->gImageWidth; x++) 
		{
			// 픽셀좌표연산
			src_x = x / (float)xscale;
			src_y = y / (float)yscale;
			alpha = src_x - x / xscale;
			beta = src_y - y / yscale;
			Ax = src_x;
			Ay = src_y;
			Bx = Ax + 1;
			By = Ay;
			Cx = Ax;
			Cy = Ay + 1;
			Dx = Ax + 1;
			Dy = Ay + 1;

			if (Bx > pDoc->ImageWidth - 1)Bx = pDoc->ImageWidth - 1;
			if (Cy > pDoc->ImageHeight - 1)Cy = pDoc->ImageHeight - 1;
			if (Dx > pDoc->ImageWidth - 1)Dx = pDoc->ImageWidth - 1;
			if (Dy > pDoc->ImageHeight - 1)Dy = pDoc->ImageHeight - 1;




			if (pDoc->depth == 1)
			{

				E = (1 - alpha) * pDoc->inputimg[Ay][Ax] + alpha * pDoc->inputimg[By][Bx];
				F = (1 - alpha) * pDoc->inputimg[Cy][Cx] + alpha * pDoc->inputimg[Dy][Dx];

				pDoc->gresultimg[y][x] = (1 - beta) * E + beta * F;
			}
			else {
				E = (1 - alpha) * pDoc->inputimg[Ay][3*Ax+0] + alpha * pDoc->inputimg[By][3*Bx+0];
				F = (1 - alpha) * pDoc->inputimg[Cy][3*Cx+0] + alpha * pDoc->inputimg[Dy][3*Dx+0];

				pDoc->gresultimg[y][3*x+0] = (1 - beta) * E + beta * F;

				E = (1 - alpha) * pDoc->inputimg[Ay][3 * Ax + 1] + alpha * pDoc->inputimg[By][3 * Bx + 1];
				F = (1 - alpha) * pDoc->inputimg[Cy][3 * Cx + 1] + alpha * pDoc->inputimg[Dy][3 * Dx + 1];

				pDoc->gresultimg[y][3 * x+1] = (1 - beta) * E + beta * F;

				E = (1 - alpha) * pDoc->inputimg[Ay][3 * Ax + 2] + alpha * pDoc->inputimg[By][3 * Bx + 2];
				F = (1 - alpha) * pDoc->inputimg[Cy][3 * Cx + 2] + alpha * pDoc->inputimg[Dy][3 * Dx + 2];

				pDoc->gresultimg[y][3 * x+2] = (1 - beta) * E + beta * F;
			}
		}
	}

	Invalidate();
}


void CImagePro2View::OnGeometryZoomoutSubsampling()
{
	CImagePro2Doc* pDoc = GetDocument();
	int x, y, i;

	int xscale = 3; // 1/3
	int yscale = 2; // 1/2

	if (pDoc->gresultimg != NULL) {
		for (i = 0; i < pDoc->gImageHeight; i++) free(pDoc->gresultimg[i]);
		free(pDoc->gresultimg);
	}
	pDoc->gImageWidth = (pDoc->ImageWidth) / xscale;
	pDoc->gImageHeight = (pDoc->ImageHeight) / yscale;


	//메모리 할당
	pDoc->gresultimg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
	for (i = 0; i < pDoc->gImageHeight; i++) {
		pDoc->gresultimg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
	}


	for (y = 0; y < pDoc->gImageHeight; y++) {
		for (x = 0; x < pDoc->gImageWidth; x++)
		{
			if (pDoc->depth == 1)
				pDoc->gresultimg[y][x] = pDoc->inputimg[y * yscale][x * xscale];
			else {
				pDoc->gresultimg[y][3 * x + 0] = pDoc->inputimg[y * yscale][3 * (x * xscale) + 0];
				pDoc->gresultimg[y][3 * x + 1] = pDoc->inputimg[y * yscale][3 * (x * xscale) + 1];
				pDoc->gresultimg[y][3 * x + 2] = pDoc->inputimg[y * yscale][3 * (x * xscale) + 2];

			}
		}
		Invalidate();

	}
}


	void CImagePro2View::OnGeometryZoomoutMeanSub()
	{
		OnRegionSmoothing();
		CopyResultToInput();
		OnGeometryZoomoutSubsampling();
	}


	void CImagePro2View::OnGeometryZoomoutAvg()
	{
		CImagePro2Doc* pDoc = GetDocument();
		int x, y, i, j;
		int src_x, src_y;
		int sum;
		int rsum, gsum, bsum;

		int xscale = 3;
		int yscale = 2;

		if (pDoc->gresultimg != NULL) {
			for (i = 0; i < pDoc->gImageHeight; i++) free(pDoc->gresultimg[i]);
			free(pDoc->gresultimg);
		}

		pDoc->gImageWidth = pDoc->ImageWidth / xscale;
		pDoc->gImageHeight = pDoc->ImageHeight / yscale;
		//메모리 할당
		pDoc->gresultimg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
		for (i = 0; i < pDoc->gImageHeight; i++) {
			pDoc->gresultimg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
		}

		//전방향 사상
		for (y = 0; y < pDoc->ImageHeight - yscale; y += yscale) {
			for (x = 0; x < pDoc->ImageWidth - xscale; x += xscale) {
				if (pDoc->depth == 1) {
					sum = 0;
					for (j = 0; j < yscale; j++) {
						for (i = 0; i < xscale; i++) {
							src_x = x + i;
							src_y = y + j;
							sum += pDoc->inputimg[src_y][src_x];
						}
					}
					pDoc->gresultimg[y / yscale][x / xscale] = sum / (xscale * yscale);
				}
				else {
					rsum = gsum = bsum = 0;
					for (j = 0; j < yscale; j++) {
						for (i = 0; i < xscale; i++) {
							src_x = x + i;
							src_y = y + j;
							rsum += pDoc->inputimg[src_y][3 * src_x + 0];
							gsum += pDoc->inputimg[src_y][3 * src_x + 1];
							bsum += pDoc->inputimg[src_y][3 * src_x + 2];
						}
					}
					pDoc->gresultimg[y / yscale][3 * (x / xscale)  + 0] = rsum / (xscale * yscale);
					pDoc->gresultimg[y / yscale][3 * (x / xscale)  + 1] = gsum / (xscale * yscale);
					pDoc->gresultimg[y / yscale][3 * (x / xscale)  + 2] = bsum / (xscale * yscale);
				}
			}
		}

		Invalidate();
	
	}

#define PI 3.14159
	void CImagePro2View::OnGeometryRotation()
	{
		CImagePro2Doc* pDoc = GetDocument();
		CinputDig angleinput;
		int x, y, i, j,angle;

		//int angle = 30; //degree
		float radian;
		int Cx, Cy, Oy;
		int xdiff, ydiff;
		int x_source, y_source;
		if (pDoc->gresultimg != NULL) {
			for (i = 0; i < pDoc->gImageHeight; i++) free(pDoc->gresultimg[i]);
			free(pDoc->gresultimg);
		}
		//angleinput.DoModal();
		if (angleinput.DoModal() != IDOK) {
			// User pressed Cancel or closed the dialog, return without processing
			angle = 120;
			//return;
		}
		else 
			angle = angleinput.minputdig;
		radian = PI / 180 * angle;
		pDoc->gImageWidth = pDoc->ImageHeight * fabs(cos(PI / 2 - radian)) + pDoc->ImageWidth * fabs(cos(radian));
		pDoc->gImageHeight = pDoc->ImageHeight * fabs(cos(radian)) + pDoc->ImageWidth * fabs(cos(PI / 2 - radian));

		//메모리 할당
		pDoc->gresultimg = (unsigned char**)malloc(pDoc->gImageHeight * sizeof(unsigned char*));
		for (i = 0; i < pDoc->gImageHeight; i++) {
			pDoc->gresultimg[i] = (unsigned char*)malloc(pDoc->gImageWidth * pDoc->depth);
		}
		Cx = pDoc->ImageWidth / 2;
		Cy = pDoc->ImageHeight / 2;
		Oy = pDoc->ImageHeight - 1;

		xdiff = (pDoc->gImageWidth - pDoc->ImageWidth) / 2;
		ydiff = (pDoc->gImageHeight - pDoc->ImageHeight) / 2;

		
			for (y = -ydiff; y < pDoc->gImageHeight - ydiff; y++)
				for (x = -xdiff; x < pDoc->gImageWidth - xdiff; x++) {
					x_source = (int)(((Oy - y) - Cy) * sin(radian) + (x - Cx) * cos(radian) + Cx);
					y_source = (int)(((Oy - y) - Cy) * cos(radian) - (x - Cx) * sin(radian) + Cy);

					y_source = Oy - y_source;
					if (pDoc->depth == 1) {
						if (x_source < 0 || x_source > pDoc->ImageWidth - 1 ||
							y_source < 0 || y_source > pDoc->ImageHeight - 1)
						{
							pDoc->gresultimg[y + ydiff][x + xdiff] = 255;
						}
						else {
							pDoc->gresultimg[y + ydiff][x + xdiff] = pDoc->inputimg[y_source][x_source];
						}
					}
					else {
						if (x_source < 0 || x_source > pDoc->ImageWidth - 1 ||
							y_source < 0 || y_source > pDoc->ImageHeight - 1) {
							pDoc->gresultimg[y + ydiff][3 * (x + xdiff) + 0] = 255;
							pDoc->gresultimg[y + ydiff][3 * (x + xdiff) + 1] = 255;
							pDoc->gresultimg[y + ydiff][3 * (x + xdiff) + 2] = 255;
						}
						else {
							pDoc->gresultimg[y + ydiff][3 * (x + xdiff) + 0] = pDoc->inputimg[y_source][3 * x_source + 0];
							pDoc->gresultimg[y + ydiff][3 * (x + xdiff) + 1] = pDoc->inputimg[y_source][3 * x_source + 1];
							pDoc->gresultimg[y + ydiff][3 * (x + xdiff) + 2] = pDoc->inputimg[y_source][3 * x_source + 2];
						}
					}
		}
		
		Invalidate();
	}

	


	void CImagePro2View::OnGeometryMirror()
	{
		CImagePro2Doc* pDoc = GetDocument();
		int y, x;

		for (y = 0; y < pDoc->ImageHeight; y++)
			for (x = 0; x < pDoc->ImageWidth; x++)
				if (pDoc->depth == 1) {
					pDoc->resultimg[y][x] = pDoc->inputimg[y][pDoc->ImageWidth - 1 - x];
				}
				else {
					pDoc->resultimg[y][3 * x + 0] = pDoc->inputimg[y][3 * (pDoc->ImageWidth - 1 - x) + 0];
					pDoc->resultimg[y][3 * x + 1] = pDoc->inputimg[y][3 * (pDoc->ImageWidth - 1 - x) + 1];
					pDoc->resultimg[y][3 * x + 2] = pDoc->inputimg[y][3 * (pDoc->ImageWidth - 1 - x) + 2];
				}
		// TODO: 여기에 명령 처리기 코드를 추가합니다.
		Invalidate();
	}


	void CImagePro2View::OnGeometryFlip()
	{
		CImagePro2Doc* pDoc = GetDocument();
		int y, x;

		for (y = 0; y < pDoc->ImageHeight; y++)
			for (x = 0; x < pDoc->ImageWidth; x++)
				if (pDoc->depth == 1) {
					pDoc->resultimg[pDoc->ImageHeight - 1 - y][x] = pDoc->inputimg[y][x];
				}
				else {
					pDoc->resultimg[pDoc->ImageHeight - 1 - y][3 * x + 0] = pDoc->inputimg[y][3 * x + 0];
					pDoc->resultimg[pDoc->ImageHeight - 1 - y][3 * x + 1] = pDoc->inputimg[y][3 * x + 1];
					pDoc->resultimg[pDoc->ImageHeight - 1 - y][3 * x + 2] = pDoc->inputimg[y][3 * x + 2];
				}
		// TODO: 여기에 명령 처리기 코드를 추가합니다.
		Invalidate();
	}

	typedef struct
	{
		int Px;
		int Py;
		int Qx;
		int Qy;
	} control_line;

	void CImagePro2View::OnGeometryWarping()
	{
		CImagePro2Doc* pDoc = GetDocument();
		control_line source_lines[5] = { {0,0,pDoc->ImageWidth - 1,0},{pDoc->ImageWidth - 1,0,pDoc->ImageWidth - 1,pDoc->ImageHeight - 1},{pDoc->ImageWidth - 1,pDoc->ImageHeight - 1,0,pDoc->ImageHeight - 1},{0,pDoc->ImageHeight - 1,0,0},{10,30,50,90} };
		control_line dest_lines[5] = { {0,0,pDoc->ImageWidth - 1,0},{pDoc->ImageWidth - 1,0,pDoc->ImageWidth - 1,pDoc->ImageHeight - 1},{pDoc->ImageWidth - 1,pDoc->ImageHeight - 1,0,pDoc->ImageHeight - 1},{0,pDoc->ImageHeight - 1,0,0},{200,90,10,5} };
		// TODO: 여기에 명령 처리기 코드를 추가합니다.
		double u;
		double h;
		double d;
		double tx, ty;
		double xp, yp;

		double weight;
		double totalWeight;
		double a = 0.001;
		double b = 2.0;
		double p = 0.75;

		int x1, x2, y1, y2;
		int src_x1, src_y1, src_x2, src_y2;
		double src_line_length, dest_line_length;

		int num_lines = 5;
		int line;
		int x, y;
		int source_x, source_y;
		int last_row, last_col;
		last_row = pDoc->ImageHeight - 1;
		last_col = pDoc->ImageWidth - 1;

		for (y = 0; y < pDoc->ImageHeight; y++)
		{
			for (x = 0; x < pDoc->ImageWidth; x++)
			{
				totalWeight = 0.0;
				tx = 0.0;
				ty = 0.0;

				for (line = 0; line < num_lines; line++)
				{
					x1 = dest_lines[line].Px;
					y1 = dest_lines[line].Py;
					x2 = dest_lines[line].Qx;
					y2 = dest_lines[line].Qy;

					dest_line_length = sqrt((double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));

					u = (double)((x - x1) * (x2 - x1) + (y - y1) * (y2 - y1)) /
						(double)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
					h = (double)((y - y1) * (x2 - x1) - (x - x1) * (y2 - y1)) / dest_line_length;

					if (u < 0) d = sqrt((double)((x - x1) * (x - x1) + (y - y1) * (y - y1)));
					else if (u > 1) d = sqrt((double)((x - x2) * (x - x2) + (y - y2) * (y - y2)));
					else d = fabs(h);

					src_x1 = source_lines[line].Px;
					src_y1 = source_lines[line].Py;
					src_x2 = source_lines[line].Qx;
					src_y2 = source_lines[line].Qy;
					src_line_length = sqrt((double)((src_x2 - src_x1) * (src_x2 - src_x1) + (src_y2 - src_y1) * (src_y2 - src_y1)));
					xp = src_x1 + u * (src_x2 - src_x1) - h * (src_y2 - src_y1) / src_line_length;
					yp = src_y1 + u * (src_y2 - src_y1) + h * (src_x2 - src_x1) / src_line_length;

					weight = pow((pow((double)(dest_line_length), p) / (a + d)), b);

					tx += (xp - x) * weight;
					ty += (yp - y) * weight;
					totalWeight += weight;
				}
				source_x = x + (int)(tx / totalWeight + 0.5);
				source_y = y + (int)(ty / totalWeight + 0.5);

				if (source_x < 0) source_x = 0;
				if (source_x > last_col) source_x = last_col;
				if (source_y < 0) source_y = 0;
				if (source_y > last_row) source_y = last_row;

				
				if (pDoc->depth == 1) {
					pDoc->resultimg[y][x] = pDoc->inputimg[source_y][source_x];
				}
				else {
					pDoc->resultimg[y][3 * x + 0] = pDoc->inputimg[source_y][3 * source_x + 0];
					pDoc->resultimg[y][3 * x + 1] = pDoc->inputimg[source_y][3 * source_x + 1];
					pDoc->resultimg[y][3 * x + 2] = pDoc->inputimg[source_y][3 * source_x + 2];
				}
			}
		}
		Invalidate();
	}


	void CImagePro2View::OnAviView()
	{
		CFile file;
		CFileDialog dlg(TRUE);

		if (dlg.DoModal() == IDOK) {
			strcpy_s(AVIFileName, dlg.GetPathName());
			viewMode = AVI_FILE;
		}
		Invalidate(FALSE);
		// TODO: 여기에 명령 처리기 코드를 추가합니다.
	}
