
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define TWO_IMAGES 1
#define THREE_IMAGES 2
#define TWO_IMAGES_SCALED 4

// CImagePro2View

IMPLEMENT_DYNCREATE(CImagePro2View, CScrollView)

BEGIN_MESSAGE_MAP(CImagePro2View, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImagePro2View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_clear, &CImagePro2View::Onclear)
	ON_COMMAND(ID_durty, &CImagePro2View::Ondurty)
	ON_COMMAND(ID_embo, &CImagePro2View::Onembo)
	ON_COMMAND(ID_REGION_SOBEL2, &CImagePro2View::OnRegionSobel2)
	ON_COMMAND(ID_REGION_MEDIAN, &CImagePro2View::OnRegionMedian)
	ON_COMMAND(ID_EROSION, &CImagePro2View::OnErosion)
	ON_COMMAND(ID_DILATION, &CImagePro2View::OnDilation)
	ON_COMMAND(ID_OPENING, &CImagePro2View::OnOpening)
	ON_COMMAND(ID_CLOSING, &CImagePro2View::OnClosing)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_PIXEL_COPY, &CImagePro2View::OnGeometryZoominPixelCopy)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_INTERPOLATION, &CImagePro2View::OnGeometryZoominInterpolation)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_SUBSAMPLING, &CImagePro2View::OnGeometryZoominSubsampling)
	ON_COMMAND(ID_GEOMETRY_ZOOMIN_AVG, &CImagePro2View::OnGeometryZoominAvg)
	ON_COMMAND(ID_GEOMETRY_ROTATE, &CImagePro2View::OnGeometryRotate)
	ON_COMMAND(ID_GEOMETRY_MIRROR, &CImagePro2View::OnGeometryMirror)
	ON_COMMAND(ID_GEOMETRY_FLIP, &CImagePro2View::OnGeometryFlip)
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
	if (pDoc->inputImg == NULL) return;
	if (pDoc->depth == 1) {
		for (int y = 0; y < pDoc->imageHeight; y++)
			for (int x = 0; x < pDoc->imageWidth; x++)
				pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][x],
					pDoc->inputImg[y][x], pDoc->inputImg[y][x]));
		if (viewMode == THREE_IMAGES) {
			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->inputImg2[y][x],
							pDoc->inputImg2[y][x],
							pDoc->inputImg2[y][x]));
			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth * 2 + 60, y,
						RGB(pDoc->resultImg[y][x],
							pDoc->resultImg[y][x],
							pDoc->resultImg[y][x]));
		}
		else if (viewMode == TWO_IMAGES_SCALED) {
			for (int y = 0; y < pDoc->gImageHeight; y++)
				for (int x = 0; x < pDoc->gImageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth+30, y,
						RGB(pDoc->gResultImg[y][x],
							pDoc->gResultImg[y][x],
							pDoc->gResultImg[y][x]));
		}
		else {
			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->resultImg[y][x],
							pDoc->resultImg[y][x],
							pDoc->resultImg[y][x]));
		}
	}
	else if (pDoc->depth == 3) {
		for (int y = 0; y < pDoc->imageHeight; y++)
			for (int x = 0; x < pDoc->imageWidth; x++)
				pDC->SetPixel(x, y, RGB(pDoc->inputImg[y][3 * x],
					pDoc->inputImg[y][3 * x + 1], pDoc->inputImg[y][3 * x + 2]));
		if (viewMode == THREE_IMAGES) {
			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->inputImg2[y][3 * x],
							pDoc->inputImg2[y][3 * x + 1],
							pDoc->inputImg2[y][3 * x + 2]));
			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth * 2 + 60, y,
						RGB(pDoc->resultImg[y][3 * x],
							pDoc->resultImg[y][3 * x + 1],
							pDoc->resultImg[y][3 * x + 2]));
		}
		else {
			for (int y = 0; y < pDoc->imageHeight; y++)
				for (int x = 0; x < pDoc->imageWidth; x++)
					pDC->SetPixel(x + pDoc->imageWidth + 30, y,
						RGB(pDoc->resultImg[y][3 * x],
							pDoc->resultImg[y][3 * x + 1],
							pDoc->resultImg[y][3 * x + 2]));
		}
	}
	/*if (!pDoc)
		return;*/

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}

void CImagePro2View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = 2048;
	sizeTotal.cy = 1024;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CImagePro2View 인쇄


void CImagePro2View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImagePro2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CImagePro2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CImagePro2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CImagePro2View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImagePro2View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
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


void CImagePro2View::Onclear()
{
	CImagePro2Doc* pDoc = GetDocument();
	float kernel[3][3] = { {0,-1,0},{-1,5,-1},{0,-1,0 } };
	convolve(pDoc->inputImg, pDoc->resultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);
	Invalidate();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}
void CImagePro2View::Ondurty()
{
	CImagePro2Doc* pDoc = GetDocument();
	float kernel[3][3] = { {1 / 9.f, 1 / 9.f, 1 / 9.f},
						  {1 / 9.f, 1 / 9.f, 1 / 9.f},
						  {1 / 9.f, 1 / 9.f, 1 / 9.f} };
	convolve(pDoc->inputImg, pDoc->resultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, 0, pDoc->depth);
	Invalidate();
}


void CImagePro2View::Onembo()
{
	CImagePro2Doc* pDoc = GetDocument();
	float kernel[3][3] = { {-2,-1,0},
						   {-1, 1,1},
						   {0 , 1 ,2} };
	convolve(pDoc->inputImg, pDoc->resultImg, pDoc->imageWidth, pDoc->imageHeight, kernel, -128, pDoc->depth);
	Invalidate();
}


void CImagePro2View::convolve(unsigned char** inimg, unsigned char** outimg, int cols, int rows, float mask[][3], int bias, int depth)
{
	// TODO: 여기에 구현 코드 추가.
	int x, y;
	int i, j;
	int sum, rsum, gsum, bsum;

	for (y = 1; y < rows - 1; y++)
		for (x = 1; x < cols - 1; x++)
		{
			if (depth == 1)
			{
				sum = 0;
				for (j = 0; j < 3; j++)
				{
					for (i = 0; i < 3; i++)
					{
						sum += (inimg[y + j - 1][x + i - 1] * mask[j][i]);
					}
					sum += bias;
					if (sum > 255) sum = 255;
					else if (sum < 0) sum = 0;
					outimg[y][x] = sum;
				}

			}
			else
			{
				rsum = 0; gsum = 0; bsum = 0;
				for (j = 0; j < 3; j++)
					for (i = 0; i < 3; j++)
					{
						rsum += (inimg[y + j - 1][3 * (x + i - 1) + 0] * mask[j][i]);
						gsum += (inimg[y + j - 1][3 * (x + i - 1) + 1] * mask[j][i]);
						bsum += (inimg[y + j - 1][3 * (x + i - 1) + 2] * mask[j][i]);
					}
				rsum += bias;
				gsum += bias;
				bsum += bias;
				if (rsum > 255) rsum = 255;
				else if (rsum < 0) rsum = 0;
				if (gsum > 255) gsum = 255;
				else if (gsum < 0) gsum = 0;
				if (bsum > 255) bsum = 255;
				else if (bsum < 0) bsum = 0;

				outimg[y][3 * x + 0] = rsum;
				outimg[y][3 * x + 1] = gsum;
				outimg[y][3 * x + 2] = bsum;
			}

		}
}





void CImagePro2View::OnRegionSobel2()
{
	CImagePro2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->RegionSobel();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CImagePro2View::OnRegionMedian()
{
	CImagePro2Doc* pDoc = GetDocument();;
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->RegionMedian();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}




void CImagePro2View::OnErosion()
{
	CImagePro2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->Erosion();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
 	// TODO: 여기에 구현 코드 추가.
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}




void CImagePro2View::OnDilation()
{
	CImagePro2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->Dilation();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CImagePro2View::OnOpening()
{
	CImagePro2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->inputImg == NULL) return;
	pDoc->Opening();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CImagePro2View::OnClosing()
{
	CImagePro2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->inputImg == NULL) return;
	pDoc->Closing();
	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CImagePro2View::OnGeometryZoominPixelCopy()
{
	CImagePro2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryZoominPixelCopy();

	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CImagePro2View::OnGeometryZoominInterpolation()
{
	CImagePro2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryZoominInterpolation();

	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CImagePro2View::OnGeometryZoominSubsampling()
{
	CImagePro2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryZoominSubsampling();

	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CImagePro2View::OnGeometryZoominAvg()
{
	CImagePro2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryZoominAvg();

	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CImagePro2View::OnGeometryRotate()
{
	CImagePro2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryRotate();

	viewMode = TWO_IMAGES_SCALED;
	Invalidate(FALSE);
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CImagePro2View::OnGeometryMirror()
{
	CImagePro2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryMirror();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CImagePro2View::OnGeometryFlip()
{
	CImagePro2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc->inputImg == NULL) return;
	pDoc->GeometryFlip();

	viewMode = TWO_IMAGES;
	Invalidate(FALSE);
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}
