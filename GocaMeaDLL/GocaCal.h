/*
	数据处理
*/
#ifndef __GocaCal_H__
#define __GocaCal_H__

#ifdef __cplusplus
extern "C" {
#endif

	double* GocaInterpolation(double arrX[], int points, double calcX[], double calcY[], int calcPoints);
	double* GocaGetMag(double freq[], double mag[], int points, int mode, int band, int CalorMea);
	double* GocaWrapToggle(double input[], int points, int toggle);
	double* GocaCalc(double freq[], double value[], int points, double calcX[], double calcY[], int mode, int CalorMea);
	double* GocaGetPhase(double freq[], double phase[], int points, int mode, int band, int CalorMea);
	double* readArrayFile(const char* filename, int points);
	void writeArrayFile(const char* filename, double* arr, int points);
#ifdef __cplusplus
}
#endif

#endif  

