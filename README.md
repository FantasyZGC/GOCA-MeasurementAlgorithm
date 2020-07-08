## GOCA API 手册——数据处理

本篇文档将为您介绍 GOCA 提供的测量与数据处理函数，这些函数能方便有效地处理测量数据，并将它转换为您需要的结果。

### 总览

<table>
    <tr>
        <th width="60px">编号</th>
        <th>函数名</th>
        <th>简介</th>
    </tr>
    <tr>
        <td>1</td>
        <td>EOMeaMag</td>
        <td>EO 测量。输入仪器测量值，根据波段的不同，计算并返回幅度响应。</td>
    </tr>
    <tr>
        <td>2</td>
        <td>EOMeaPhase</td>
        <td>EO 测量。输入仪器测量值，根据波段的不同，计算并返回相位响应。</td>
    </tr>
    <tr>
        <td>3</td>
        <td>OECalMag</td>
        <td>OE 校准。输入仪器测量值，根据波段的不同，进行幅度响应校准。</td>
    </tr>
    <tr>
        <td>4</td>
        <td>OECalPhase</td>
        <td>OE 校准。输入仪器测量值，根据波段的不同，进行相位响应校准。</td>
    </tr>
     <tr>
        <td>5</td>
        <td>OEMeaMag</td>
        <td>OE 测量。输入仪器测量值，根据波段的不同，计算并返回幅度响应。需要先使用 OECalMag。</td>
    </tr>
     <tr>
        <td>6</td>
        <td>OEMeaPhase</td>
        <td>OE 测量。输入仪器测量值，根据波段的不同，计算并返回相位响应。需要先使用 OECalPhase。</td>
    </tr>
</table>



### 详细

#### void EOMeaMag(double* freq, double mag[], int points, int band,double* output)

##### 简介：

+ EO 测量。输入仪器测量值，根据波段的不同，计算并返回幅度响应。

##### 返回值：

+ void。

##### 参数：

+ double* freq ：EO 测量时使用的频率数组，作为最终测量结果的横坐标。
+ double* mag：EO 测量时通过仪器得到的幅度响应，用于计算，得到最终的幅度响应。
+ int points：测量点数，最大可输入值为 freq  的数组长度。
+ int band：波段，输入 CBAND 或 1，按 C 波段计算；输入 OBAND 或 2，按 O 波段计算。
+ double* output：计算结果，用于接收返回的幅度响应。

##### 编程示例。

```c
/*
	EO 测量幅度响应
	fre: 测量所用频率数组
	mag: 仪器初测得到的幅度响应
	points: fre 与 mag 的数组长度
	CBAND: 等价于常量 1，表示该次测量波段为 C 波段
	result: 用于接收计算结果
*/
double* result = (double*)malloc(sizeof(double) * points);
EOMeaMag(fre, mag, points, CBAND, result);
```



#### void EOMeaPhase(double* freq, double* phase, int points, int band, double* output)

##### 简介：

+ EO 测量。输入仪器测量值，根据波段的不同，计算并返回相位响应。

##### 返回值：

+ void。

##### 参数：

+ double* phase：EO 测量时通过仪器得到的相位响应，用于计算，得到最终的相位响应。
+ double* output：计算结果，用于接收返回的相位响应。
+ 其他：同 EOMeaMag。

##### 编程示例：

```c
/*
	EO 测量相位响应
	fre: 测量所用频率数组
	phase: 仪器初测得到的相位响应
	points: fre 与 phase 的数组长度
	CBAND: 等价于常量 1，表示该次测量波段为 C 波段
	result: 用于接收计算结果
*/
double* result = (double*)malloc(sizeof(double) * points);
EOMeaMag(fre, phase, points, CBAND, result);
```



#### void OECalMag(double* freq, double* mag, int points, int band)

##### 简介：

+ OE 校准。输入仪器测量值，根据波段的不同，进行幅度响应校准。

##### 返回值：

+ void。

##### 参数：

+ double* freq ：OE 测量时使用的频率数组，作为最终测量结果的横坐标。
+ double* mag：OE 测量时通过仪器得到的幅度响应，用于校准。
+ int points：测量点数，最大可输入值为 freq  的数组长度。
+ int band：波段，输入 CBAND 或 1，按 C 波段计算；输入 OBAND 或 2，按 O 波段计算。

##### 编程示例：

```c
/*	
	OE 校准幅度响应
	fre: 测量所用频率数组
	mag: 仪器初测得到的幅度响应
	points: fre 与 mag 的数组长度
	OBAND: 等价于常量 2，表示该次测量波段为 O 波段
	不需要接收结果
*/
OECalMag(freq, mag, points, OBAND);
```



#### void OECalPhase(double* freq, double* phase, int points, int band)

##### 简介：

+ OE 校准。输入仪器测量值，根据波段的不同，进行相位响应校准。

##### 返回值：

+ void。

##### 参数：

+ double* phase：OE 测量时通过仪器得到的相位响应，用于校准。

+ 其他：同 OECalMag。

##### 编程示例：

```c
/*	
	OE 校准相位响应
	fre: 测量所用频率数组
	phase: 仪器初测得到的相位响应
	points: fre 与 mag 的数组长度
	OBAND: 等价于常量 2，表示该次测量波段为 O 波段
	不需要接收结果
*/
OECalMag(freq, mag, points, OBAND);
```



#### void OEMeaMag(double* freq, double* mag, int points, int band, double* output);

##### 简介：

+ OE 测量。输入仪器测量值，根据波段的不同，计算并返回幅度响应。需要先使用 OECalMag。

##### 返回值：

+ void。

##### 参数：

+ double* freq ：OE 测量时使用的频率数组，作为最终测量结果的横坐标。
+ double* mag：OE 测量时通过仪器得到的幅度响应，用于计算，得到最终的幅度响应。
+ int points：测量点数，最大可输入值为 freq 的数组长度。
+ int band：波段，输入 CBAND 或 1，按 C 波段计算；输入 OBAND 或 2，按 O 波段计算。
+ double* output：计算结果，用于接收返回的幅度响应。

##### 编程示例：

```c
/*	
	OE 测量幅度响应
	fre: 测量所用频率数组
	mag: 仪器初测得到的相位响应
	points: fre 与 mag 的数组长度
	CBAND: 等价于常量 1，表示该次测量波段为 C 波段
	result: 用于接收计算结果
	测量前需要先校准
*/

OECalMag(freq, mag, points, CBAND);// 先进行校准
double* result = (double*)malloc(sizeof(double) * points);
OEMeaMag(fre, phase, points, CBAND, result);
```



#### void OEMeaPhase(double* freq, double* phase, int points, int band, double* output);

##### 简介：

+ OE 测量。输入仪器测量值，根据波段的不同，计算并返回相位响应。需要先使用 OECalPhase。

##### 返回值：

+ void。

##### 参数：

+ double* phase：OE 测量时通过仪器得到的相位响应，用于计算，得到最终的相位响应。
+ double* output：计算结果，用于接收返回的相位响应。
+ 其他：同 OEMeaMag。

##### 编程示例：

```c
/*	
	OE 测量相位响应
	fre: 测量所用频率数组
	phase: 仪器初测得到的相位响应
	points: fre 与 phase 的数组长度
	CBAND: 等价于常量 1，表示该次测量波段为 C 波段
	result: 用于接收计算结果
	测量前需要先校准
*/

OECalPhase(freq, phase, points, CBAND);// 先进行校准
double* result = (double*)malloc(sizeof(double) * points);
OEMeaPhase(fre, phase, points, CBAND, result);
```

