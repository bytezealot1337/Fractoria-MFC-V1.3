// GlobalFunction.h


double Round(double dbVal, int nPlaces);
CString RemoveZeros(CString str);
void OnProgress();

UINT SetMarqueeOn(LPVOID Param);

UINT OnMandelbrot(LPVOID Param);
UINT OnMandelbrotQuick(LPVOID Param);
UINT OnJulia(LPVOID Param);
UINT OnJuliaQuick(LPVOID Param);
UINT OnNewton(LPVOID Param);
UINT OnNewtonRoot(LPVOID Param);
UINT OnNova(LPVOID Param);
UINT OnNovaSpecial(LPVOID Param);
UINT OnGlynn(LPVOID Param);
UINT OnPhoenix(LPVOID Param);
UINT OnCosTanZ(LPVOID Param);

UINT OnPreviewMandelbrot(LPVOID Param);
UINT OnPreviewMandelbrotQuick(LPVOID Param);
UINT OnPreviewJulia(LPVOID Param);
UINT OnPreviewJuliaQuick(LPVOID Param);
UINT OnPreviewNewton(LPVOID Param);
UINT OnPreviewNewtonRoot(LPVOID Param);
UINT OnPreviewNova(LPVOID Param);
UINT OnPreviewNovaSpecial(LPVOID Param);
UINT OnPreviewGlynn(LPVOID Param);
UINT OnPreviewPhoenix(LPVOID Param);
UINT OnPreviewCosTanZ(LPVOID Param);

BOOL WndToBmp(CDC *pDC, CString szFile);
HANDLE DDBToDIB(CBitmap &bitmap, DWORD dwCompression, CPalette *pPal);
UINT OnAnimationThread(LPVOID Param);

void OnAnimationRender();

void OnAnimationMandelbrot();
void OnAnimationMandelbrotQuick();
void OnAnimationJulia();
void OnAnimationJuliaQuick();
void OnAnimationNewton();
void OnAnimationNewtonRoot();
void OnAnimationNova();
void OnAnimationNovaSpecial();
void OnAnimationGlynn();
void OnAnimationPhoenix();
void OnAnimationCosTanZ();

void UpdateMainTabData();

