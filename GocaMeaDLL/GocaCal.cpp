#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>

#include "calData.h"
#include "GocaMea.h"
#include "GocaCal.h"


/*
	描述：插值算法
	参数：
	arrX：求插值结果所需的横坐标
	points：插值点数，最多取 arrX 的长度
	calcX：校准数据横坐标
	calcY：校准数据纵坐标
	calcPoints：校准数据点数，最多取校准数据的数组长度，量度范围原则上要大于 arrX。
*/
double* GocaInterpolation(double arrX[], int points, double calcX[], double calcY[], int calcPoints) {
	if (points <= 0) { return { 0 }; }
	// 创建一个新数组用于存储结果
	double* newArrY = (double*)malloc(sizeof(double) * points);
	int j = 0, i = 0;
	// 对 arrX 作线性插值
	for (; i < points; i++) {
		// 先判断 arrX 所处插值区间
		for (; j < calcPoints; j++) {
			// 找到正确位置并计算
			// y = y0 + (x-x0) * (y1-y0) / (x1-x0)
			if (calcX[j] <= arrX[i] && calcX[j + 1] >= arrX[i]) {
				newArrY[i] = calcY[j] + (arrX[i] - calcX[j]) * (calcY[j + 1] - calcY[j]) / (calcX[j + 1] - calcX[j]);
				break;
			}
		}
	}
	return newArrY;
}


/*
	描述：获取各模式、光源下的幅度测量值
	参数：
	freq: 频率，初测数据横坐标
	Mag: 相位响应，初测数据纵坐标
	points: 数组长度/需要计算的点数
	mode: EO(1), OE(2)
	band: CBAND(1) C波段, OBAND(2) O波段
	CalorMea：仅对 OE 模式有用。OECAL(1)校准，OEMEA(2)测量
			  先用 OECAL 将校准时测量值代入进行计算
			  再用 OEMEA 将校准后测量值代入进行计算
*/
double* GocaGetMag(double freq[], double mag[], int points, int mode, int band, int CalorMea) {

	// 根据波段调整校准数据
	double* calcFreq = calPDfreq;
	double* calcMag;

	//OE 测量需要读取校准数据
	if (mode == OE && CalorMea == OEMEA) {
		if (band == OBAND) {
			calcMag = readArrayFile("calMagData_O.txt", points);
		}
		else {
			calcMag = readArrayFile("calMagData_C.txt", points);
		}
	}
	// OE 校准和 EO 测量使用的都是标准数据
	else if (band == OBAND) {
		calcMag = calPDAndLineMag_O;
	}
	else {
		// 默认C波段
		calcMag = calPDAndLineMag_C;
	}
	return GocaCalc(freq, mag, points, calcFreq, calcMag, mode, CalorMea);
}

/*
	描述：获取各模式、光源下的相位测量值
	参数：
	freq: 频率，初测数据横坐标
	phase: 相位响应，初测数据纵坐标
	points: 数组长度/需要计算的点数
	mode: EO(1), OE(2)
	band: CBAND(1) C波段, OBAND(2) O波段
	CalorMea：仅对 OE 模式有用。OECAL(1)校准，OEMEA(2)测量
			  先用 OECAL 将校准时测量值代入进行计算
			  再用 OEMEA 将校准后测量值代入进行计算
*/
double* GocaGetPhase(double freq[], double phase[], int points, int mode, int band, int CalorMea) {

	// 输入频率 和 校准频率 需要 unwrap 后再代入计算
	double* unwrapPhase = GocaWrapToggle(phase, points, UNWRAP);

	// 根据波段调整校准数据
	double* calcFreq = calPDfreq;
	double* calcPhase;

	// OE 测量需要读取用户校准数据
	if (mode == OE && CalorMea == OEMEA) {

		if (band == OBAND) {
			calcPhase = readArrayFile("calPhaseData_O.txt", points);
		}
		else {
			calcPhase = readArrayFile("calPhaseData_C.txt", points);
		}
		calcPhase = GocaWrapToggle(calcPhase, points, UNWRAP);
	}
	else if (band == OBAND) {
		// O波段
		calcPhase = calPDAndLinePhase_O;
		calcPhase = GocaWrapToggle(calcPhase, 10001, UNWRAP);
	}
	else {
		// 默认C波段
		calcPhase = calPDAndLinePhase_C;
		calcPhase = GocaWrapToggle(calcPhase, 10001, UNWRAP);
	}

	double* result = GocaCalc(freq, unwrapPhase, points, calcFreq, calcPhase, mode, CalorMea);

	free(unwrapPhase);
	free(calcPhase);
	// 重新wrap
	return GocaWrapToggle(result, points, WRAP);;
}

/*
	描述：不同模式下测量的具体实现方式
	参数：
	freq: 频率，初测数据横坐标
	value: 响应值，初测数据纵坐标
	points: 数组长度/需要计算的点数
	calcX：校准数据横坐标
	calcY：校准数据纵坐标
	mode: EO(1), OE(2)
	band: CBAND(1) C波段, OBAND(2) O波段
	CalorMea：仅对 OE 模式有用。OECAL(1)校准，OEMEA(2)测量
			  先用 OECAL 将校准时测量值代入进行计算
			  再用 OEMEA 将校准后测量值代入进行计算
*/
double* GocaCalc(double freq[], double value[], int points, double calcX[], double calcY[], int mode, int CalorMea) {
	//-------------------- 判断不同测量模式 ---------------------------
	double* result = { 0 };
	//-------------------------- EO -----------------------------
	if (mode == EO) {
		// 计算插值
		result = GocaInterpolation(freq, points, calcX, calcY, 10001);
		// 测量值与插值相减
		for (int i = 0; i < points; i++) {
			result[i] = value[i] - result[i];
		}
	}
	//----------------------- OE CAL --------------------------------
	else if (mode == OE && CalorMea == OECAL) {
		// 计算插值
		result = GocaInterpolation(freq, points, calcX, calcY, 10001);
		// 测量值与插值相减
		for (int i = 0; i < points; i++) {
			result[i] = value[i] - result[i];
		}
	}
	//----------------------- OE MEA --------------------------------
	else if (mode == OE && CalorMea == OEMEA) {
		result = (double*)malloc(sizeof(double) * points);
		// 第二次测量数据 - 校准数据（第一次校准所得数据）
		for (int i = 0; i < points; i++) {
			result[i] = value[i] - calcY[i];
		}
	}
	return result;
}

/*
	描述：对函数进行解卷绕/卷绕
	参数：
	input：输入函数值
	points：需要进行运算的点数，最大为 input 的数组长度
	toggle：切换选项。输入 UNWRAP(1) 进行解卷绕，输入 WRAP(2) 进行卷绕
*/
double* GocaWrapToggle(double input[], int points, int toggle) {

	if (points <= 1) {
		return { 0 };
	}

	int PI = 180,
		cycleNum = 0,
		cycle = 0;

	double* result = (double*)malloc(sizeof(double) * points);

	// 根据相位响应曲线的趋势，判断补偿周期的正负
	if (input[0] > input[1]) {
		cycle = -2 * PI;
	}
	else {
		cycle = 2 * PI;
	}

	// 第一个值不变

	result[0] = input[0];

	// 判断是 unwrap 还是 wrap 运算
	// unwrap 解卷绕
	if (toggle == UNWRAP) {

		// 若第 2 个点就跳变，说明之前判断的 cycle 是反的。
		if (fabs(input[0] - input[1]) > PI) {
			cycle = -cycle;
		}

		for (int i = 1; i < points; i++) {
			// 相邻点正负符号变化，且差距大于 180 度，说明发生了跳变，补偿周期 +1
			if (input[i - 1] * input[i]<0 && fabs(input[i - 1] - input[i])>PI) {
				cycleNum++;
			}
			// 每个值都补偿一次周期
			result[i] = input[i] + double(cycle) * cycleNum;
		}
	}
	// wrap 卷绕
	else if (toggle == WRAP) {
		int cycle_1 = 0;
		int cycle_2 = 0;

		for (int i = 0; i < points; i++) {
			// 周期为（2n+1）* pi
			if (fabs(input[i]) > PI) {
				cycleNum = abs(int((input[i] - cycle / 2) / 360)) + 1;
				result[i] = input[i] - double(cycle) * cycleNum;
			}
			else {
				result[i] = input[i];
			}
		}
	}

	return result;
}

double* readArrayFile(const char* filename, int points) {
	double* arr = (double*)malloc(sizeof(double) * points);
	FILE* fp;
	fopen_s(&fp, filename, "rb");
	if (fp != 0) {
		for (int i = 0; i < points; i++) {
			fscanf_s(fp, "%lf\n", &arr[i]);
		}
	}
	fclose(fp);
	return arr;
}

void writeArrayFile(const char* filename, double* arr, int points) {
	remove(filename);
	FILE* fp;
	fopen_s(&fp, filename, "wb");
	for (int i = 0; i < points; i++) {
		fprintf(fp, "%lf\n", arr[i]);
	}
	fclose(fp);
	SetFileAttributesA(filename, FILE_ATTRIBUTE_HIDDEN);
}