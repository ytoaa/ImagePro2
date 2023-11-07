﻿
// ImagePro2Doc.h: CImagePro2Doc 클래스의 인터페이스
//


#pragma once


class CImagePro2Doc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CImagePro2Doc() noexcept;
	DECLARE_DYNCREATE(CImagePro2Doc)

// 특성입니다.
public:
	unsigned char** inputImg;
	unsigned char** inputImg2;
	unsigned char** resultImg;
	int imageWidth;
	int imageHeight;
	int depth;

	int gImageWidth;
	int gImageHeight;
	unsigned char** gResultImg;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CImagePro2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	void LoadImageFile(CArchive& ar);
	void LoadTwoImages();
	void PixelAdd();
	void PixelTwoImageAdd();
	void RegionSobel();
	void Convolve(unsigned char** inputImg, unsigned char** resultImg, int cols, int rows, float mask[][3] , int bias, int depth);
	void RegionMedian();
	void Erosion();
	void Dilation();
	void Opening();
	void CopyResultToInput();
	void Closing();
	void GeometryZoominPixelCopy();
};
