#define DLL_EXPORT

#include <Windows.h>
#include <stdio.h>
#include "GocaCal.h"
#include "GocaMea.h"

// EO 测量 幅度响应
void EOMeaMag(double* freq, double* mag, int points, int band, double* output) {
	double* result = GocaGetMag(freq, mag, points, EO, band, 0);
	for (int i = 0; i < points; i++) {
		output[i] = result[i];
	}
}


// EO 测量 相位响应
void EOMeaPhase(double* freq, double* phase, int points, int band, double* output) {
	double* result = GocaGetPhase(freq, phase, points, EO, band, 0);
	for (int i = 0; i < points; i++) {
		output[i] = result[i];
	}
}

// OE校准 幅度响应
void OECalMag(double* freq, double* mag, int points, int band) {
	double* result = GocaGetMag(freq, mag, points, OE, band, OECAL);
	// 保存数据
	if (band == OBAND) {
		writeArrayFile("calMagData_O.txt", result, points);
	}
	else {
		writeArrayFile("calMagData_C.txt", result, points);
	}
}

// OE校准 相位响应
void OECalPhase(double* freq, double* phase, int points, int band) {
	double* result = GocaGetPhase(freq, phase, points, OE, band, OECAL);
	// 保存数据
	if (band == OBAND) {
		writeArrayFile("calPhaseData_O.txt", result, points);
	}
	else {
		writeArrayFile("calPhaseData_C.txt", result, points);
	}
}

// OE测量 幅度响应
void OEMeaMag(double* freq, double* mag, int points, int band, double* output) {
	double* result = GocaGetMag(freq, mag, points, OE, band, OEMEA);
	for (int i = 0; i < points; i++) {
		output[i] = result[i];
	}
}

// OE测量 相位响应
void OEMeaPhase(double* freq, double* phase, int points, int band, double* output) {
	double* result = GocaGetPhase(freq, phase, points, OE, band, OEMEA);
	for (int i = 0; i < points; i++) {
		output[i] = result[i];
	}
}


