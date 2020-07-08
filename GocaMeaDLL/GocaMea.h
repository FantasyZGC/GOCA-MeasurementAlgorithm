
#ifndef __GocaMea_H__
#define __GocaMea_H__

#ifdef DLL_EXPORT
#define DLL_EXPORT _declspec(dllexport)
#else
#define DLL_EXPORT _declspec(dllimport)
#endif // DLL_EXPORT

#ifdef __cplusplus
extern "C" {
#endif
	/*
		API
	*/
	// EO测量
	DLL_EXPORT void EOMeaMag(double* freq, double* mag, int points, int band, double* output);
	DLL_EXPORT void EOMeaPhase(double* freq, double* phase, int points, int band, double* output);

	// OE校准
	DLL_EXPORT void OECalMag(double* freq, double* mag, int points, int band);
	DLL_EXPORT void OECalPhase(double* freq, double* phase, int points, int band);

	// OE测量
	DLL_EXPORT void OEMeaMag(double* freq, double* mag, int points, int band, double* output);
	DLL_EXPORT void OEMeaPhase(double* freq, double* phase, int points, int band, double* output);

	const int EO = 1;
	const int OE = 2;
	const int CBAND = 1;
	const int OBAND = 2;
	const int OECAL = 1;
	const int OEMEA = 2;
	const int UNWRAP = 1;
	const int WRAP = 2;

#ifdef __cplusplus
}
#endif

#endif  

