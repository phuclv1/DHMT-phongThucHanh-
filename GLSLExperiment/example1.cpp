#pragma region
//Chương trình vẽ 1 hình lập phương đơn vị theo mô hình lập trình OpenGL hiện đại

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/

typedef vec4 point4;
typedef vec4 color4;
using namespace std;
#pragma endregion Thư viện và định nghĩa
// remember to prototype
#pragma region
void generateGeometry( void );
void initGPUBuffers( void );
void shaderSetup( void );
void display( void );
void keyboard( unsigned char key, int x, int y );
#pragma endregion Prototype

#pragma region
vec4 black = color4(0.0, 0.0, 0.0, 1.0);
vec4 red = color4(1.0, 0.0, 0.0, 1.0);
vec4 yellow = color4(1.0, 1.0, 0.0, 1.0);
vec4 green = color4(0.0, 1.0, 0.0, 1.0);
vec4 blue = color4(0.0, 0.0, 1.0, 1.0);
vec4 magenta = color4(1.0, 0.0, 1.0, 1.0);
vec4 pink = color4(1.0, 0.9, 1.0, 1.0);
vec4 cyan = color4(0.0, 1.0, 1.0, 1.0);
vec4 grey = color4(0.25, 0.25, 0.25, 1.0);
vec4 silver = color4(0.75, 0.75, 0.75, 1.0);
vec4 white = color4(1.0, 1.0, 1.0, 1.0);
vec4 redDark = color4(0.65, 0, 0, 1.0);
vec4 yellowLight = color4(1.0, 1.0, 0.75, 1.0);
#pragma endregion Màu mẫu 

#pragma region
mat4 model, projection, view;
// Số các đỉnh của các tam giác
const int NumPoints = 36; 
const int soDinhHLP = 36;
const int soDinhHinhTru = 144;
const int soDinhCau = 360;
int Index = 0;
point4 points[soDinhHLP + soDinhHinhTru + soDinhCau ]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[soDinhHLP + soDinhHinhTru  + soDinhCau ]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
vec3 normal[soDinhHLP + soDinhHinhTru + soDinhCau ];
point4 DinhHLP[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 MauHLP[8];
point4 DinhHtru[26];
color4 MauHtru[26];
point4 DinhHinhCau[62];
color4 MauHcau[62];
GLfloat but[3] = { -0.02 ,-0.005,-0.02 };
float QuayCamera[5] = { 0,0,0,0,0 };
int on = 0;
GLuint program,  loc_vNormal, model_loc, projection_loc, view_loc;
#pragma endregion Các biến tính toán trong bộ nhớ
GLfloat dr = M_PI / 180;


#pragma region


void TaoDinhHLP()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	DinhHLP[0] = point4(-0.5, -0.5, 0.5, 1.0);
	DinhHLP[1] = point4(-0.5, 0.5, 0.5, 1.0);
	DinhHLP[2] = point4(0.5, 0.5, 0.5, 1.0);
	DinhHLP[3] = point4(0.5, -0.5, 0.5, 1.0);
	DinhHLP[4] = point4(-0.5, -0.5, -0.5, 1.0);
	DinhHLP[5] = point4(-0.5, 0.5, -0.5, 1.0);
	DinhHLP[6] = point4(0.5, 0.5, -0.5, 1.0);
	DinhHLP[7] = point4(0.5, -0.5, -0.5, 1.0);


	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	MauHLP[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	MauHLP[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	MauHLP[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	MauHLP[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	MauHLP[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	MauHLP[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	MauHLP[6] = color4(1.0, 1.0, 1.0, 1.0); // white
	MauHLP[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}


void MauDinhHLP(int a, int b, int c, int d)  /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	colors[Index] = MauHLP[a]; points[Index] = DinhHLP[a]; Index++;
	colors[Index] = MauHLP[b]; points[Index] = DinhHLP[b]; Index++;
	colors[Index] = MauHLP[c]; points[Index] = DinhHLP[c]; Index++;
	colors[Index] = MauHLP[a]; points[Index] = DinhHLP[a]; Index++;
	colors[Index] = MauHLP[c]; points[Index] = DinhHLP[c]; Index++;
	colors[Index] = MauHLP[d]; points[Index] = DinhHLP[d]; Index++;
}
void HinhLapPhuong(void)  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/
{
	MauDinhHLP(1, 0, 3, 2);
	MauDinhHLP(2, 3, 7, 6);
	MauDinhHLP(3, 0, 4, 7);
	MauDinhHLP(6, 5, 1, 2);
	MauDinhHLP(4, 5, 6, 7);
	MauDinhHLP(5, 4, 0, 1);
}
#pragma endregion HLP

#pragma region
void TaoDinhHtru()
{
	//hình trụ mặt trên
	int j = 0;
	for (int i = 0; i <= 11; i++) {
		DinhHtru[i] = point4(cos(j * dr), 1, -sin(j * dr), 1.0);
		j = j + 30;
	}
	DinhHtru[12] = point4(0, 1, 0, 1.0);
	//mặt dưới
	j = 0;
	for (int i = 13; i <= 24; i++) {
		DinhHtru[i] = point4(cos(j * dr), -1, -sin(j * dr), 1.0);
		j = j + 30;
	}
	DinhHtru[25] = point4(0, -1, 0, 1.0);
	/*
	for (int k = 0; k < 144; k++) {

		 	
		MauHtru[k] = color4(0.0, 0.0, 0.0, 1.0);	 k++; // black		
		MauHtru[k] = color4(1.0, 0.0, 0.0, 1.0);	 k++; // red
		MauHtru[k] = color4(1.0, 1.0, 0.0, 1.0);	 k++; // yellow
		MauHtru[k] = color4(0.0, 1.0, 0.0, 1.0);	 k++;// green
		MauHtru[k] = color4(0.0, 0.0, 1.0, 1.0);	 k++;// blue
		MauHtru[k] = color4(1.0, 0.0, 1.0, 1.0);	 k++;// magenta
		MauHtru[k] = color4(1.0, 1.0, 1.0, 1.0);	 k++;// white
		MauHtru[k] = color4(0.0, 1.0, 1.0, 1.0);	 k++;// cyan
	}*/
}
void MBHTru(int a, int b, int c, int d) {
	colors[Index] = MauHtru[a]; points[Index] = DinhHtru[a]; Index++;
	colors[Index] = MauHtru[b]; points[Index] = DinhHtru[b]; Index++;
	colors[Index] = MauHtru[c]; points[Index] = DinhHtru[c]; Index++;
	colors[Index] = MauHtru[a]; points[Index] = DinhHtru[a]; Index++;
	colors[Index] = MauHtru[c]; points[Index] = DinhHtru[c]; Index++;
	colors[Index] = MauHtru[d]; points[Index] = DinhHtru[d]; Index++;

}
void MTHTru(int a[]) {
	
	for (int i = 2; i <= 12; i++) {
		 colors[Index] = MauHtru[a[0]]; points[Index] = DinhHtru[a[0]]; Index++;
		 colors[Index] = MauHtru[a[i-1]]; points[Index] = DinhHtru[a[i - 1]]; Index++;
		 colors[Index] = MauHtru[a[i]]; points[Index] = DinhHtru[a[i]]; Index++;
		if (i == 12) {
			colors[Index] = MauHtru[a[0]]; points[Index] = DinhHtru[a[0]]; Index++;
			colors[Index] = MauHtru[a[12]]; points[Index] = DinhHtru[a[12]]; Index++;
			colors[Index] = MauHtru[a[1]]; points[Index] = DinhHtru[a[1]]; Index++;
		}
	}

}
void HinhTru() {
	int a[] = { 12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	int b[] = { 25, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24 };
	//mặt tròn
	MTHTru(a);
	MTHTru(b);
	//thân hình trụ
	for (int i = 1; i <= 11; i++) {
		MBHTru(a[i], a[i + 1], b[i + 1], b[i]);
	}
	MBHTru(a[12], a[1], b[1], b[12]);
}
#pragma endregion H Trụ

#pragma region
void TaoDinhHinhCau() {
	float ratioSphere = 1;
	//hình tròn qua tâm
	int j = 0;
	for (int i = 0; i <= 11; i++) {
		DinhHinhCau[i] = point4(cos(j * dr), 0, sin(j * dr), 1.0);
		j = j + 30;
	}
	//hình tròn cách tâm sin 30 độ
	j = 0;
	ratioSphere = cos(30 * dr);
	for (int i = 12; i <= 23; i++) {
		DinhHinhCau[i] = point4(cos(j * dr) * ratioSphere, sin(30 * dr), sin(j * dr) * ratioSphere, 1.0);
		j = j + 30;
	}
	//hình tròn cách tâm sin 60 độ
	j = 0;
	ratioSphere = cos(60 * dr);
	for (int i = 24; i <= 35; i++) {
		DinhHinhCau[i] = point4(cos(j * dr) * ratioSphere, sin(60 * dr), sin(j * dr) * ratioSphere, 1.0);
		j = j + 30;
	}
	//hình tròn cách tâm - sin 30 độ
	j = 0;
	ratioSphere = cos(30 * dr);
	for (int i = 36; i <= 47; i++) {
		DinhHinhCau[i] = point4(cos(j * dr) * ratioSphere, -sin(30 * dr), sin(j * dr) * ratioSphere, 1.0);
		j = j + 30;
	}
	//hình tròn cách tâm - sin 60 độ
	j = 0;
	ratioSphere = cos(60 * dr);
	for (int i = 48; i <= 59; i++) {
		DinhHinhCau[i] = point4(cos(j * dr) * ratioSphere, -sin(60 * dr), sin(j * dr) * ratioSphere, 1.0);
		j = j + 30;
	}
	//đỉnh trên
	DinhHinhCau[60] = point4(0.0, sin(60 * dr), 0.0, 1.0);
	//đỉnh dưới
	DinhHinhCau[61] = point4(0.0, -sin(60 * dr), 0.0, 1.0);

	for (int k = 0; k < 62; k++) {


		MauHcau[k] = color4(0.0, 0.0, 0.0, 1.0);	 k++; // black		
		MauHcau[k] = color4(1.0, 0.0, 0.0, 1.0);	 k++; // red
		MauHcau[k] = color4(1.0, 1.0, 0.0, 1.0);	 k++; // yellow
		MauHcau[k] = color4(0.0, 1.0, 0.0, 1.0);	 k++;// green
		MauHcau[k] = color4(0.0, 0.0, 1.0, 1.0);	 k++;// blue
		MauHcau[k] = color4(1.0, 0.0, 1.0, 1.0);	 k++;// magenta
		MauHcau[k] = color4(1.0, 1.0, 1.0, 1.0);	 k++;// white
		MauHcau[k] = color4(0.0, 1.0, 1.0, 1.0);	 k++;// cyan
	}
}

void MBHinhCau(int a, int b, int c, int d) {
	
	colors[Index] = MauHcau[a]; points[Index] = DinhHinhCau[a]; Index++;
	colors[Index] = MauHcau[b]; points[Index] = DinhHinhCau[b]; Index++;
	colors[Index] = MauHcau[c]; points[Index] = DinhHinhCau[c]; Index++;

	colors[Index] = MauHcau[a]; points[Index] = DinhHinhCau[a]; Index++;
	colors[Index] = MauHcau[c]; points[Index] = DinhHinhCau[c]; Index++;
	colors[Index] = MauHcau[d]; points[Index] = DinhHinhCau[d]; Index++;
}
void MTHinhCau(int a[], int pole) {
	
	for (int i = 1; i <= 11; i++) {
		colors[Index] = MauHcau[pole]; points[Index] = DinhHinhCau[pole]; Index++;
		colors[Index] = MauHcau[a[i-1]]; points[Index] = DinhHinhCau[a[i - 1]]; Index++;
		colors[Index] = MauHcau[a[i]]; points[Index] = DinhHinhCau[a[i]]; Index++;
		if (i == 11) {
			colors[Index] = MauHcau[pole]; points[Index] = DinhHinhCau[pole]; Index++;
			colors[Index] = MauHcau[a[11]]; points[Index] = DinhHinhCau[a[11]]; Index++;
			colors[Index] = MauHcau[a[0]]; points[Index] = DinhHinhCau[a[0]]; Index++;
		}
	}
}
void HinhCau() {
	int a[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	int b[] = { 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23 };
	int c[] = { 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };
	int d[] = { 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47 };
	int e[] = { 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59 };
	//mặt cầu trên tâm
	
	for (int i = 0; i <= 10; i++) {
		MBHinhCau(a[i], a[i + 1], b[i + 1], b[i]);
	}
	MBHinhCau(a[11], a[0], b[0], b[11]);
	for (int i = 0; i <= 10; i++) {
		MBHinhCau(b[i], b[i + 1], c[i + 1], c[i]);
	}
	MBHinhCau(b[11], b[0], c[0], c[11]);
	//mặt cầu dưới tâm
	
	for (int i = 0; i <= 10; i++) {
		MBHinhCau(a[i], a[i + 1], d[i + 1], d[i]);
	}

		MBHinhCau(a[11], a[0], d[0], d[11]);
	for (int i = 0; i <= 10; i++) {
		MBHinhCau(d[i], d[i + 1], e[i + 1], e[i]);
	}
	MBHinhCau(d[11], d[0], e[0], e[11]);
	
	//mặt đỉnh trên
	MTHinhCau(c, 60);
	//mặt đỉnh dưới
	MTHinhCau(e, 61);
}
#pragma endregion H Cầu

void generateGeometry(void)
{
	TaoDinhHLP();
	HinhLapPhuong();
	cout << Index << endl;
	TaoDinhHtru();
	HinhTru();
	cout << Index << endl;
	TaoDinhHinhCau();
	HinhCau();
	cout << Index << endl;

}

#pragma region
void initGPUBuffers( void )
{
	// Tạo một VAO - vertex array object
	GLuint vao;
    glGenVertexArrays( 1, &vao );     
    glBindVertexArray( vao );

    // Tạo và khởi tạo một buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors), NULL, GL_STATIC_DRAW );

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors); 
	
}


void shaderSetup( void )
{
	// Nạp các shader và sử dụng chương trình shader
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );   // hàm InitShader khai báo trong Angel.h
    glUseProgram( program );

    // Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
    GLuint loc_vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( loc_vPosition );
    glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));



	model_loc = glGetUniformLocation(program, "model");
	view_loc = glGetUniformLocation(program, "view");
	projection_loc = glGetUniformLocation(program, "projection");

	glEnable(GL_DEPTH_TEST);
    glClearColor( 1.0, 1.0, 1.0, 1.0 );        /* Thiết lập màu trắng là màu xóa màn hình*/
}
#pragma endregion Tạo bộ nhớ đệm và khởi tạo shader

#pragma region
void VeHinhLapPhuong(mat4 instance, mat4 matrix)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * matrix * instance);
	glDrawArrays(GL_TRIANGLES, 0, soDinhHLP);    /*Vẽ các tam giác*/
}
void VeHinhTru(mat4 instance, mat4 matrix)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * matrix * instance);
	glDrawArrays(GL_TRIANGLES, soDinhHLP, soDinhHinhTru);    /*Vẽ các tam giác*/
}
void VeHinhCau(mat4 instance, mat4 matrix) {
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model * matrix * instance);
	glDrawArrays(GL_TRIANGLES, soDinhHLP + soDinhHinhTru, soDinhCau);    /*Vẽ các tam giác*/
}

#pragma endregion Hàm sẵn

#pragma region 
float DEGtoRAD(float k) {
	 return k * dr;
}



vec4 eye = vec4(0, 0,8, 1);
vec4 at = vec4(0, 0,- 1, 1)+eye;
vec4 up = vec4(0, 1, 0, 1);
// Các tham số cho projection
GLfloat l = -0.01, r = 0.01;
GLfloat b = -0.01, t = 0.01;
GLfloat zn = 0.03, zf = 10;

#pragma endregion khai cáo eye 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///																											  ///
///									PHẦN THIẾT KẾ MÔ HÌNH PHÒNG THỰC HÀNH								      ///
///																											  ///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma region
GLfloat door[2] = { 0,0 }, theta_model[3] = { 0,0,0 }, namcua[] = { 0,0 }, mangchieu = 0;

mat4 cuatrai;
mat4 magchieu1;
mat4 magchieu2;
mat4 cuaphai;
mat4 instance;
mat4 tempModel;
mat4 banGV;

float yUpMan2 = 0.04;
float yTateMan2 = -10;
float xTateMan2 = 0;
float yXoayMan1 = 0;
float zXoayMan1 = 0;
float gocXoayThanhTreoX = 0;
float gocXoayThanhTreoY = 0;
vec4 dis = grey;
float displayScreen = 0, displayAppCloseButton = 0;
float move1, move2;
float chuotCoordinate[3], appCoordinate[3], closeCoordinate[3];
float appScaleX = 0.04;
float appScaleY = 0.04;
bool screenOn = false, appOn = false;
#pragma endregion Phúc khai Báo
#pragma region 
void kinhnho(GLfloat k) {// các tấm kính tạo RightWall
	instance = Translate(3.01f, 0.45f, k) * Scale(0.01f, 1.9f, 0.9f);
	VeHinhLapPhuong(instance, mat4());
}
void vachnho1(GLfloat h) {// dùng làm tấm ngăn giữa các tấm kính tạo RightWall

	instance = Translate(3.01f, 0.45f, h) * Scale(0.02f, 1.9f, 0.1f);
	VeHinhLapPhuong(instance, mat4());
}
void kinhto(GLfloat k) {// các tấm kính tạo LeftWall

	instance = Translate(-3.01f, 0.0f, k) * Scale(0.01f, 2.8f, 0.9f);
	VeHinhLapPhuong(instance, mat4());
}
void vachnho2(GLfloat h) {// dùng làm tấm ngăn giữa các tấm kính tạo LeftWall

	instance = Translate(-3.01f, 0.0f, h) * Scale(0.02f, 2.8f, 0.1f);
	VeHinhLapPhuong(instance, mat4());
}
void TuongBaoQuanh() {
	//Floor
	
	instance = Translate(0.0f, -1.51f, 0.0f) * Scale(6.04f, 0.02f, 8.04f);
	VeHinhLapPhuong(instance, mat4());
	//Ceiling
	
	instance = Translate(0.0f, 1.51f, 0.0f) * Scale(6.04f, 0.02f, 8.04f);
	VeHinhLapPhuong(instance, mat4());
	//BackWall

	instance = Translate(0.0f, 0.0f, 4.01f) * Scale(6.0f, 3.0f, 0.02f);
	VeHinhLapPhuong(instance, mat4());
	//RightWall
	
	instance = Translate(3.01f, -1.0f, 3.45f) * Scale(0.02f, 1.0f, 1.1f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(3.01f, 0.55f, 2.1f) * Scale(0.02f, 0.1f, 1.6f);
	VeHinhLapPhuong(instance, mat4());

	instance = Translate(3.01f, 1.45f, 0.0f) * Scale(0.02f, 0.1f, 8.0f);
	VeHinhLapPhuong(instance, mat4());
	
	instance = Translate(3.01f, 1.0f, 2.1f) * Scale(0.01f, 0.8f, 1.6f);
	VeHinhLapPhuong(instance, mat4());
	
	instance = Translate(3.01f, -1.0f, -1.35f) * Scale(0.02f, 1.0f, 5.3f);
	VeHinhLapPhuong(instance, mat4());
	vachnho1(3.95);
	kinhnho(3.45);
	vachnho1(2.95);
	vachnho1(1.25);
	kinhnho(0.75);
	vachnho1(0.25);
	kinhnho(-0.25);
	vachnho1(-0.75);
	kinhnho(-1.25);
	vachnho1(-1.75);
	kinhnho(-2.25);
	vachnho1(-2.75);
	kinhnho(-3.25);
	
	instance = Translate(3.01f, 0.45f, -3.85f) * Scale(0.02f, 1.9f, 0.3f);
	VeHinhLapPhuong(instance, mat4());
	//LeftWall
	
	instance = Translate(-3.01f, 1.45f, 0.0f) * Scale(0.02f, 0.1f, 8.0f);
	VeHinhLapPhuong(instance, mat4());
	
	instance = Translate(-3.01f, -1.45f, 0.0f) * Scale(0.02f, 0.1f, 8.0f);
	VeHinhLapPhuong(instance, mat4());

	vachnho2(3.95);
	kinhto(3.45);
	vachnho2(2.95);
	instance = Translate(-3.01f, -1.0f, 2.45) * Scale(0.01f, 1.05f, 0.9f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(-3.01f, 1.2f, 2.45) * Scale(0.01f, 0.40f, 0.9f);
	VeHinhLapPhuong(instance, mat4());
	
	vachnho2(1.95);
	kinhto(1.45);
	vachnho2(0.95);
	kinhto(0.45);
	vachnho2(-0.05);
	kinhto(-0.55);
	vachnho2(-1.05);
	kinhto(-1.55);
	vachnho2(-2.05);
	kinhto(-2.55);
	vachnho2(-3.05);
	kinhto(-3.55);

	//FrontWall

	/*
	material_diffuse = ConvertRGB(004, 200, 194);
	FillColor(material_diffuse);
	instance = Translate(0.0f, 0.0f, -4.01f) * Scale(6.0f, 3.0f, 0.02f);
	VeHinhLapPhuong(instance, mat4());
	*/

}
GLfloat a[] = { 0,0,0,0,0,0 };
void Cuaravao() {
	
	instance = Translate(-0.375f, 0.0f, 0.0f) * Scale(0.05f, 1.9f, 0.02f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(0.375f, 0.0f, 0.0f) * Scale(0.05f, 1.9f, 0.02f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(0.0f, 0.975f, 0.0f) * Scale(0.8f, 0.05f, 0.02f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(0.0f, -0.975f, 0.0f) * Scale(0.8f, 0.05f, 0.02f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(0.0f, 0.0f, 0.0f) * Scale(0.8f, 0.1f, 0.02f);
	VeHinhLapPhuong(instance, mat4());
	
	instance = Translate(0.0f, -0.5f, 0.0f) * Scale(0.7f, 0.9f, 0.005f);
	VeHinhLapPhuong(instance, mat4());
	
	instance = Translate(0.0f, 0.5f, 0.0f) * Scale(0.7f, 0.9f, 0.005f);
	VeHinhLapPhuong(instance, mat4());
}
void taynamcua() {
	
	instance = Translate(0.0f, 0.0f, 0.1f) * RotateZ(90) * Scale(0.02f, 0.1f, 0.02f);
	VeHinhTru(instance, mat4());
	
	instance = Translate(0.1f, 0.0f, 0.1f) * Scale(0.021f, 0.021f, 0.021f);
	VeHinhCau(instance, mat4());
	instance = Translate(-0.1f, 0.0f, 0.1f) * Scale(0.021f, 0.021f, 0.021f);
	VeHinhCau(instance, mat4());


	instance = Translate(0.0f, 0.0f, -0.1f) * RotateZ(90) * Scale(0.02f, 0.1f, 0.02f);
	VeHinhTru(instance, mat4());

	instance = Translate(0.1f, 0.0f, -0.1f) * Scale(0.021f, 0.021f, 0.021f);
	VeHinhCau(instance, mat4());
	instance = Translate(-0.1f, 0.0f, -0.1f) * Scale(0.021f, 0.021f, 0.021f);
	VeHinhCau(instance, mat4());


	instance = Translate(0.1f, 0.0f, 0.0f) * RotateX(90) * Scale(0.02f, 0.1f, 0.02f);
	VeHinhTru(instance, mat4());
}


void cuon_cua_bang_chieu() {
	
	instance = Translate(1.3f, 0.0f, 0.0f) * Scale(0.01f, 0.20f, 0.2f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(-1.3f, 0.0f, 0.0f) * Scale(0.01f, 0.2f, 0.2f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(0.0f, 0.0f, -0.1f) * RotateZ(90) * Scale(0.1f, 1.3f, 0.1f);
	VeHinhTru(instance, mat4());

	instance = Translate(0.0f, 0.0f, -0.1f) * RotateZ(90) * Scale(0.03f, 1.32f, 0.03f);
	VeHinhTru(instance, mat4());

	instance = Translate(0.0f, -0.1, 0.0f) * Scale(2.5f, 0.2f, 0.01f);
	VeHinhLapPhuong(instance, mat4());
}
void bang_chieu(GLfloat k) {

	instance = Translate(0.0f, -1.5, 0.0f) * Scale(2.5f, 0.2, 0.01f);
	VeHinhLapPhuong(instance, mat4());
	if (k <= -2.2) {

		instance = Translate(0.0f, 4.3, -0.1f) * Scale(2.5f, 0.1f, 0.0101f);
		VeHinhLapPhuong(instance, mat4());
	};
}
void cuon_chieu_duoi() {
	instance = Translate(0.0f, -0.2f, 0.0f) * RotateZ(90) * Scale(0.06f, 1.3f, 0.06f);
	VeHinhTru(instance, mat4());

	instance = Translate(0.0f, -0.12f, 0.0f) * Scale(2.5f, 0.05f, 0.0101f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(1.3f, -0.2f, 0.0f) * RotateZ(90) * Scale(0.08f, 0.02f, 0.07f);
	VeHinhTru(instance, mat4());
	instance = Translate(-1.3f, -0.2f, 0.0f) * RotateZ(90) * Scale(0.08f, 0.02f, 0.07f);
	VeHinhTru(instance, mat4());

	instance = Translate(0.0f, -0.1, 0.0f) * Scale(2.5f, 0.2f, 0.01f);
	VeHinhLapPhuong(instance, mat4());
	// dây kéo

	instance = Translate(0.0f, -0.3f, 0.0f) * Scale(0.012f, 0.2f, 0.02f);
	VeHinhLapPhuong(instance, mat4());
	instance = Translate(0.0f, -0.4f, 0.0f) * Scale(0.03f, 0.03f, 0.03f);
	VeHinhCau(instance, mat4());
}
void Man() {
	// khung màng hình 
	instance = Translate(0.295, 0.26, -0.01) * Scale(0.01, 0.3, 0.01);

	VeHinhLapPhuong(instance, mat4());

	instance = Translate(-0.295, 0.26, -0.01) * Scale(0.01, 0.3, 0.01);

	VeHinhLapPhuong(instance, mat4());

	instance = Translate(0, 0.405, -0.01) * Scale(0.58, 0.01, 0.01);

	VeHinhLapPhuong(instance, mat4());

	instance = Translate(0, 0.115, -0.01) * Scale(0.58, 0.01, 0.01);

	VeHinhLapPhuong(instance, mat4());

	instance = Translate(0, 0.26, 0) * Scale(0.6, 0.3, 0.01);

	VeHinhLapPhuong(instance, mat4());

	// màng hình 
	instance = Translate(0, 0.26, -0.01) * Scale(0.58, 0.28, 0.01);

	VeHinhLapPhuong(instance, mat4());
	//con chỏ chuột 
	instance = Translate(0.22 - move1, 0.36 - move2, -0.01) * Scale(0.01, 0.01, displayScreen);

	VeHinhLapPhuong(instance, mat4());
	// app trên màng hình 
	instance = Translate(0, 0.3, -0.01) * Scale(appScaleX, appScaleY, displayScreen - 0.005);

	VeHinhLapPhuong(instance, mat4());
	instance = Translate(-0.13, 0.35, -0.01) * Scale(0.04, 0.04, displayAppCloseButton);

	VeHinhLapPhuong(instance, mat4());
}
void GiaTreo() {

	instance = Translate(0, 0.26, 0.025) * Scale(0.24, 0.16, 0.04);

	VeHinhLapPhuong(instance, mat4());

	instance = Translate(0, 0, 0.025) * RotateY(gocXoayThanhTreoY) * RotateX(gocXoayThanhTreoX) * Translate(0, 0.12, 0) * Scale(0.1, 0.24, 0.02);

	VeHinhLapPhuong(instance, mat4());

	instance = Translate(0, 0.01, 0) * Scale(0.24, 0.02, 0.16);

	VeHinhLapPhuong(instance, mat4());
}
void ManHinh() {
	Man();
	GiaTreo();
}
void Chuot() {
	instance = Translate(0.25 - move1 * 0.25, 0.02, -0.2 - move2 * 0.25) * Scale(0.08, 0.04, 0.12);

	VeHinhLapPhuong(instance, mat4());
}

void ban_gv() {
	// chân bàn
	
	instance = Translate(0.58f, -0.42f, 0.28f) * Scale(0.04f, 0.8f, 0.04f);
	VeHinhLapPhuong(instance, mat4());

	instance = Translate(-0.58f, -0.42f, 0.28f) * Scale(0.04f, 0.8f, 0.04f);
	VeHinhLapPhuong(instance, mat4());

	instance = Translate(-0.58f, -0.42f, -0.28f) * Scale(0.04f, 0.8f, 0.04f);
	VeHinhLapPhuong(instance, mat4());

	instance = Translate(0.58f, -0.42f, -0.28f) * Scale(0.04f, 0.8f, 0.04f);
	VeHinhLapPhuong(instance, mat4());
	// mặt bàn
	instance = Scale(1.2f, 0.04f, 0.6f);
	VeHinhLapPhuong(instance, mat4());
	// vách bên 
	instance = Translate(0.0f, -0.32f, -0.28f) * Scale(1.12f, 0.6f, 0.02f);
	VeHinhLapPhuong(instance, mat4());

	instance = Translate(-0.58f, -0.32f, 0.0f) * Scale(0.02f, 0.6f, 0.52f);
	VeHinhLapPhuong(instance, mat4());

	instance = Translate(0.58f, -0.32f, 0.0f) * Scale(0.02f, 0.6f, 0.52f);
	VeHinhLapPhuong(instance, mat4());
	// ngăn bàn 
	
	instance = Translate(0.0f, -0.15f, 0.0f) * Scale(1.12f, 0.02f, 0.4f);
	VeHinhLapPhuong(instance, mat4());
}
void bo_ban_gv() {
	ManHinh();
	Chuot();
	model = RotateY(180) * Translate(0, -0.02, 0);
	ban_gv();
}
void LeVanPhuc() {
	tempModel = RotateY(theta_model[0]) * RotateX(theta_model[1]) * RotateZ(theta_model[2])*RotateY(180);
	model = tempModel;
	TuongBaoQuanh();
	
	model = tempModel * Translate(-2.0f, -0.62f, 3.0f);
	ban_gv();
	model = tempModel * RotateY(180) * Translate(2.0f, -0.6f, -3.0f);
	ManHinh();
	model = tempModel * RotateY(180) * Translate(2.0f, -0.6f, -3.0f);
	Chuot();
	model = tempModel * Translate(0.0f, 1.26f, 3.8f);
	cuon_cua_bang_chieu();
	model = model * magchieu2 * Translate(0.0f, mangchieu, 0.0f);
	cuon_chieu_duoi();
	model = model * magchieu1 * Translate(0.0f, 7 * mangchieu, 0.0f) * Scale(1, mangchieu * 5, 0.01f);
	bang_chieu(mangchieu);
	model = tempModel * cuatrai * Translate(3.01f, -0.5f, 2.9f) * RotateY(door[0]) * Translate(0.4f, 0.0f, 0.0f);
	Cuaravao();
	model = model * cuatrai * Translate(0.32f, 0.0f, 0.0f) * RotateZ(namcua[0]) * Translate(-0.1f, 0.0f, 0.0f);
	taynamcua();
	model = tempModel * cuaphai * Translate(3.01f, -0.5f, 1.3f) * RotateY(door[1]) * Translate(0.4f, 0.0f, 0.0f);
	Cuaravao();
	model = model * cuaphai * Translate(0.32f, 0.0f, 0.0f) * RotateZ(namcua[1]) * Translate(-0.1f, 0.0f, 0.0f) * RotateX(180);
	taynamcua();
}

#pragma endregion Lê Văn Phúc 



mat4 modelh;
mat4 model_ghe;
mat4 model_cua;
mat4 model_bang;
mat4 model_chieu;
mat4 model_caudao;
float cs=0;
#pragma region 
void line_bang(GLfloat k, GLfloat m, GLfloat n) {

	instance = RotateZ(n) * Translate(0.0, m, 0.0f) * Scale(k, 0.01f, 0.011f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_bang * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void bang() {
	// mặt bảng

	instance = Scale(4.0f, 2.0f, 0.01f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_bang * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	// viền bảng 

	instance = Translate(0.0f, 1.05f, 0.0f) * Scale(4.2f, 0.1f, 0.02f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_bang * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0.0f, -1.05f, 0.0f) * Scale(4.2f, 0.1f, 0.02f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_bang * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);


	instance = Translate(2.05f, 0.0f, 0.0f) * Scale(0.1f, 2.0f, 0.02f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_bang * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(-2.05f, 0.0f, 0.0f) * Scale(0.1f, 2.0f, 0.02f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_bang * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	

	

	// đường kẻ bản 
	line_bang(4, 0.9, 0);
	line_bang(4, 0.8, 0);
	line_bang(4, 0.7, 0);
	line_bang(4, 0.6, 0);
	line_bang(4, 0.5, 0);
	line_bang(4, 0.4, 0);
	line_bang(4, 0.3, 0);
	line_bang(4, 0.2, 0);
	line_bang(4, 0.1, 0);
	line_bang(4, 0.0, 0);
	line_bang(4, -0.9, 0);
	line_bang(4, -0.8, 0);
	line_bang(4, -0.7, 0);
	line_bang(4, -0.6, 0);
	line_bang(4, -0.5, 0);
	line_bang(4, -0.4, 0);
	line_bang(4, -0.3, 0);
	line_bang(4, -0.2, 0);
	line_bang(4, -0.1, 0);

	line_bang(2, 1.9, 90);
	line_bang(2, 1.8, 90);
	line_bang(2, 1.7, 90);
	line_bang(2, 1.6, 90);
	line_bang(2, 1.5, 90);
	line_bang(2, 1.4, 90);
	line_bang(2, 1.3, 90);
	line_bang(2, 1.2, 90);
	line_bang(2, 1.1, 90);
	line_bang(2, 1.0, 90);
	line_bang(2, 0.9, 90);
	line_bang(2, 0.8, 90);
	line_bang(2, 0.7, 90);
	line_bang(2, 0.6, 90);
	line_bang(2, 0.5, 90);
	line_bang(2, 0.4, 90);
	line_bang(2, 0.3, 90);
	line_bang(2, 0.2, 90);
	line_bang(2, 0.1, 90);
	line_bang(2, 0.0, 90);
	line_bang(2, -0.9, 90);
	line_bang(2, -0.8, 90);
	line_bang(2, -0.7, 90);
	line_bang(2, -0.6, 90);
	line_bang(2, -0.5, 90);
	line_bang(2, -0.4, 90);
	line_bang(2, -0.3, 90);
	line_bang(2, -0.2, 90);
	line_bang(2, -0.1, 90);
	line_bang(2, -1.9, 90);
	line_bang(2, -1.8, 90);
	line_bang(2, -1.7, 90);
	line_bang(2, -1.6, 90);
	line_bang(2, -1.5, 90);
	line_bang(2, -1.4, 90);
	line_bang(2, -1.3, 90);
	line_bang(2, -1.2, 90);
	line_bang(2, -1.1, 90);
	line_bang(2, -1.0, 90);
}
void Tru() {
	mat4 instance = Scale(0.2, 0.50, 0.2);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chieu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void MayChieu() {
	mat4 instance = Translate(0, 0.4, 0) * Scale(0.5, 0.30, 0.5);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chieu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void Nut(GLfloat x, GLfloat y, GLfloat z) {
	mat4 instance = Translate(x, y, z) * Scale(0.03, 0.03, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chieu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void AllNut() {
	Nut(-0.09, 0.4, -0.25);
	Nut(-0.13, 0.4, -0.25);
	Nut(-0.17, 0.4, -0.25);
}

void OKinh() {
	mat4 instance = Translate(0.1, 0.4, -0.25) * Scale(0.1, 0.1, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_chieu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void FullMayChieu() {
	Tru();
	MayChieu();
	AllNut();
	OKinh();
}

void MatSau()
{
	mat4 instance = Scale(0.3, 0.4, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_caudao * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MatTrenDuoi(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 instance = Translate(x, y, z) * Scale(0.3, 0.02, 0.25);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_caudao * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MatTraiPhai(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 instance = Translate(x, y, z) * Scale(0.02, 0.4, 0.25);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_caudao * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void MatTruoc()
{
	mat4 instance = Scale(0.3, 0.4, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_caudao * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void De()
{
	mat4 instance = Translate(0, 0, 0.08) * Scale(0.08, 0.15, 0.14);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_caudao * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void ThanhTruot(mat4 model_part2)
{
	mat4 instance = Translate(0, 0, -0) * Scale(0.03, 0.02, 0.07);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_part2 * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

void HopCauDaoIm()
{
	MatSau();
	MatTraiPhai(0.14, 0, 0.105);
	MatTraiPhai(-0.14, 0, 0.105);
	MatTrenDuoi(0, 0.21, 0.105);
	MatTrenDuoi(0, -0.21, 0.105);

}

GLfloat aa = 0, bb = 0;

void PhamHuyHoang() {
	model_chieu = tempModel * Translate(0,1.2,1.2) * RotateX(180) ;
	FullMayChieu();
	model_bang = tempModel * Translate(0.0f, 0.0f, 4.0f);
	bang();

	model_caudao = tempModel * Translate(-2.5, 0.3, 4.0) * RotateY(180);
	HopCauDaoIm();
	De();
	mat4 model_part2 = tempModel  * Translate(-2.5, 0.3, 3.8) * RotateX(aa);
	ThanhTruot(model_part2);
	model_caudao = model_caudao * Translate(-0.14, 0, 0.23) * RotateY(bb) * Translate(0.14, 0, 0);
	MatTruoc();
}
#pragma endregion PhamHuyHoang



#pragma region
void ChanDon(float x, float y, float z)
{
	instance = Translate(x, y, z) * Scale(0.02, 0.4, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_ghe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
void Chan()
{
	ChanDon(-0.14, -0.2, -0.14);
	ChanDon(-0.14, -0.2, 0.15);
	ChanDon(0.14, -0.2, -0.14);
	ChanDon(0.14, -0.2, 0.15);
}
void MatGhe()
{
	instance = Scale(0.3, 0.02, 0.3);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_ghe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void LungGhe()
{
	instance = Translate(0, 0.2, 0.15) * Scale(0.3, 0.4, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_ghe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void Canh(float x, float y, float z, float a, float b, float c)
{
	instance = Translate(x, y, z) * Scale(a, b, c);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_ghe * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void CanhTrai()
{
	Canh(-0.14, 0.1, -0.14, 0.02, 0.2, 0.02);//1
	Canh(-0.14, 0.2, 0, 0.02, 0.02, 0.3);//3
}
void CanhPhai()
{
	Canh(0.14, 0.1, -0.14, 0.02, 0.2, 0.02);//1

	Canh(0.14, 0.2, 0, 0.02, 0.02, 0.3);//3
}
void Ghe()
{
	MatGhe();
	LungGhe();
	Chan();
	CanhTrai();
	CanhPhai();
}
void khungcuaso() {
	instance = Translate(-0.43, 0, 0) * Scale(0.04, 1.48, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE,  model_cua * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0.43, 0, 0) * Scale(0.04, 1.48, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_cua * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0, 0.72, 0) * Scale(0.82, 0.04, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE,  model_cua * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

	instance = Translate(0, -0.72, 0) * Scale(0.82, 0.04, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE,  model_cua * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void canhcuaso() {
	 instance = Scale(0.82, 1.4, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE,  model_cua * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void TranVanDu() {
	 model_ghe = tempModel * Translate(2.2, -1.1, -1.9)* RotateY(90) ;
	Ghe();
	model_ghe = tempModel * Translate(2.2, -1.1, -2.7) * RotateY(90);
	Ghe();
	model_ghe = tempModel * Translate(0.8, -1.1, -1.9) * RotateY(-90);
	Ghe();
	model_ghe = tempModel * Translate(0.8, -1.1, -2.7) * RotateY(-90);
	Ghe();

	model_ghe = tempModel * Translate(-2.2, -1.1, -1.9) * RotateY(-90);
	Ghe();
	model_ghe = tempModel * Translate(-2.2, -1.1, -2.7) * RotateY(-90);
	Ghe();
	model_ghe = tempModel * Translate(-0.8, -1.1, -1.9) * RotateY(90);
	Ghe();
	model_ghe = tempModel * Translate(-0.8, -1.1, -2.7) * RotateY(90);
	Ghe();
	
	model_ghe = tempModel * Translate(2.2, -1.1, -0.6) * RotateY(90);
	Ghe();
	model_ghe = tempModel * Translate(2.2, -1.1, 0.2) * RotateY(90);
	Ghe();
	model_ghe = tempModel * Translate(0.8, -1.1, -0.6) * RotateY(-90);
	Ghe();
	model_ghe = tempModel * Translate(0.8, -1.1, 0.2) * RotateY(-90);
	Ghe();

	model_ghe = tempModel * Translate(-2.2, -1.1, -0.6) * RotateY(-90);
	Ghe();
	model_ghe = tempModel * Translate(-2.2, -1.1, 0.2) * RotateY(-90);
	Ghe();
	model_ghe = tempModel * Translate(-0.8, -1.1, -0.6) * RotateY(90);
	Ghe();
	model_ghe = tempModel * Translate(-0.8, -1.1, 0.2) * RotateY(90);
	Ghe();

	model_cua = tempModel * Translate(-3.01, 0.25, 2.45) * RotateY(90);
	khungcuaso();
	model_cua = model_cua *  Translate(0, 0.65, 0) * RotateX(cs) * Translate(0,-0.65,0);
	canhcuaso();
}

#pragma endregion TranVanDu 

#pragma region
mat4 model_ban;
GLfloat xx = 0, yy = 0, zz = 0, p = 0,nn=0;
void matBan()
{
	mat4 instance = Scale(0.8, 0.02, 0.4);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE,  model_ban * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void chanBan(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 instance = Translate(x, y, z) * Scale(0.04, 0.6, 0.04);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE,  model_ban * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}

void thanBan()
{
	matBan();
	chanBan(-0.37, -0.31, 0.17);
	chanBan(-0.37, -0.31, -0.17);
	chanBan(0.37, -0.31, 0.17);
	chanBan(0.37, -0.31, -0.17);
}

void khungbp(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 instance = Translate(x, y, z) * Scale(0.4, 0.02, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE,  model_ban * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void nganKeo()
{
	mat4 instance = Scale(0.6, 0.02, 0.3);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE,  model_ban * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);    /*Vẽ các tam giác*/
}
void ngankeohoanchinh()
{
	nganKeo();
	khungbp(0.0, 0.028, -0.1);
}
void mhmaytinh()
{
	mat4 instance = Scale(0.44, 0.25, 0.03);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_ban * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void thande(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 instance = Translate(x, y, z) * Scale(0.02, 0.06, 0.02);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_ban * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void chande(GLfloat x, GLfloat y, GLfloat z)
{
	mat4 instance = Translate(x, y, z) * Scale(0.18, 0.01, 0.1);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE,  model_ban * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void mthoanchinh()
{
	mhmaytinh();
	thande(0.0, -0.15, 0.0);
	chande(0.0, -0.2, 0.0);
}
void ban1( GLfloat x , GLfloat y, GLfloat z)
{
	GLfloat aa = DegreesToRadians * p;
	model_ban = tempModel* Translate(x , y, z+0.4) * RotateY(aa-90);
	thanBan();
	model_ban = model_ban * Translate(0.0, 0.0, 0.0) * Translate(0.0, 0.25, 0.0);
	mthoanchinh();
	model_ban = model_ban * Translate(0.0, 0.0, nn) * Translate(0.0, -0.4, 0.0);
	ngankeohoanchinh();

}
void ban2(GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat aa = DegreesToRadians * p;
	model_ban = tempModel * Translate(x - 0.4, y, z + 0.4) * RotateY(aa +90);
	thanBan();
	model_ban = model_ban * Translate(0.0, 0.0, 0.0) * Translate(0.0, 0.25, 0.0);
	mthoanchinh();
	model_ban = model_ban * Translate(0.0, 0.0, nn) * Translate(0.0, -0.4, 0.0);
	ngankeohoanchinh();

}
void ban3(GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat aa = DegreesToRadians * p;
	model_ban = tempModel * Translate(x , y, z-0.40) * RotateY(aa-90);
	thanBan();
	model_ban = model_ban * Translate(0.0, 0.0, 0.0) * Translate(0.0, 0.25, 0.0);
	mthoanchinh();
	model_ban = model_ban * Translate(0.0, 0.0, nn) * Translate(0.0, -0.4, 0.0);
	ngankeohoanchinh();

}
void ban4(GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat aa =  DegreesToRadians * p;
	model_ban = tempModel * Translate(x-0.4 , y, z -0.4) * RotateY(aa +90);
	thanBan();
	model_ban = model_ban * Translate(0.0, 0.0, 0.0) * Translate(0.0, 0.25, 0.0);
	mthoanchinh();
	model_ban = model_ban * Translate(0.0, 0.0, nn) * Translate(0.0, -0.4, 0.0);
	ngankeohoanchinh();

}

void heban(GLfloat x, GLfloat y, GLfloat z) {
	ban1(x, y, z);
	ban2(x, y, z);
	ban3(x, y, z);
	ban4(x, y, z);
}
void HoangThang() {
	heban(1.7, -0.9, -2.3);
	heban(1.7, -0.9, -0.2);
	heban(-1.3, -0.9, -0.2);
	heban(-1.3, -0.9, -2.3);
}
#pragma endregion HoangThang

#pragma region
mat4 model_view, model_tu, model_dh,c1,c2,c3,c4,c5,c6;
GLuint model_view_loc;
GLfloat theta[] = { 0, 0, 0 ,0,0,0,0,0,0,0};
GLfloat thetao[] = { 0, 0, 0 };
void matdieuhoa() {
	//matsau
	instance = Scale(1, 0.4f, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_dh * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//mattrai
	instance = Translate(-0.5f, 0, 0.15) * Scale(0.01f, 0.4f, 0.3f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_dh * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//matphai
	instance = Translate(0.5f, 0, 0.15) * Scale(0.01f, 0.4f, 0.3f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_dh * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//mattren
	instance = Translate(0, 0.2, 0.15) * Scale(1, 0.01, 0.3);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_dh * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//matduoi
	instance = Translate(0, -0.2, 0.15) * Scale(1, 0.01, 0.3);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_dh * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//mattruoc1
	instance = Translate(0, 0.05, 0.3) * Scale(1, 0.3, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_dh * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void canh() {
	instance = Translate(0, -0.04, 0) * Scale(1, 0.08, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_dh * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}
void NguyenMinhQuoc() {
	model_dh = tempModel * Translate(-3, 1.2, 0) * RotateY(90);
	matdieuhoa();
	model_dh = model_dh * Translate(0, -0.1, 0.3) * RotateX(theta[9]);
	canh();
}
#pragma endregion NguyenMinhQuoc

void tu() {
	//matsau
	instance = Scale(1, 1.6f, 0.04f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//matbentrai
	instance = Translate(-0.48f, 0, 0.25) * Scale(0.04f, 1.6f, 0.5f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//matbenphai
	instance = Translate(0.48f, 0, 0.25) * Scale(0.04f, 1.6f, 0.5f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//canhtren
	instance = Translate(0, 0.8f, 0.25f) * Scale(1, 0.01, 0.5f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//canhduoi
	instance = Translate(0, -0.8f, 0.25f) * Scale(1, 0.01, 0.5f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//Canh giua
	instance = Translate(0, 0, 0.25f) * Scale(0.06f, 1.6f, 0.5f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	//Canh ngang
	instance = Translate(0, 0.78f, 0.25f) * Scale(1, 0.04f, 0.5f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	instance = Translate(0, -0.78f, 0.25f) * Scale(1, 0.04f, 0.5f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	instance = Translate(0, 0.27f, 0.25f) * Scale(1, 0.04f, 0.5f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
	instance = Translate(0, -0.27f, 0.25f) * Scale(1, 0.04f, 0.5f);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}


void canhtu1() {
	instance = Translate(-0.23, 0, -0.02) * Scale(0.5 - 0.02, 0.53 - 0.02, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}
void canhtu2() {
	instance = Translate(-0.23, 0, -0.02) * Scale(0.5 - 0.02, 0.53 - 0.02, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}
void canhtu3() {
	instance = Translate(-0.23, 0, -0.02) * Scale(0.5 - 0.02, 0.53 - 0.02, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}
void canhtu4() {
	instance = Translate(-0.23, 0, -0.02) * Scale(0.5 - 0.02, 0.53 - 0.02, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}
void canhtu5() {
	instance = Translate(-0.23, 0, -0.02) * Scale(0.5 - 0.02, 0.53 - 0.02, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);

}
void canhtu6() {
	instance = Translate(-0.23, 0, -0.02) * Scale(0.5 - 0.02, 0.53 - 0.02, 0.01);
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, model_tu * instance);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}


void tuhoanthien() {
	model_tu = RotateY(theta[0]);
	model_tu = tempModel * Translate(3 - 0.05, -0.7, 0) * RotateY(270);
	tu();
	model_tu = model_tu * Translate(0, 0, 0.52);
	canhtu1();
	model_tu = model_tu * Translate(0.5, -0.5 - 0.02, 0);
	canhtu2();
	model_tu = model_tu * Translate(-0.5, 0, 0);
	canhtu3();
	model_tu = model_tu * Translate(0.5, 0.5, 0);
	canhtu4();
	model_tu = model_tu * Translate(0, 0.5 + 0.04, 0);
	canhtu5();
	model_tu = model_tu * Translate(-0.5, 0, 0) * RotateY(theta[8]);
	canhtu6();


}





mat4 test;
void vieww() {
	
	mat4 v = RotateX(QuayCamera[3])* RotateY(QuayCamera[4])* Translate(QuayCamera[0], QuayCamera[1], QuayCamera[2]) * LookAt(eye, at, up);
	//mat4 v = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, v);
	mat4 p = Frustum(l, r, b, t, zn, zf);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, p);
	glViewport(0, 0, 1000, 1000);
	glutPostRedisplay();
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	vieww();
	//model = RotateY(90);
	//model= RotateY(theta[0]);
	LeVanPhuc();
	TranVanDu();
	PhamHuyHoang();
	NguyenMinhQuoc();
	HoangThang();
	tuhoanthien();
	
	
	
	glutSwapBuffers();
}



#pragma region


void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '0':// LeVanPhuc
		theta_model[0] += 5;
		glutPostRedisplay();
		break;
	case '-':
		theta_model[1] += 5;
		glutPostRedisplay();
		break;
	case '=':
		theta_model[2] += 5;
		glutPostRedisplay();
		break;
	case '1':
		door[0] += 5;
		if (door[0] >= 90) door[0] = 90;
		glutPostRedisplay();
		break;
	case '2':
		door[0] -= 5;
		if (door[0] <= -75) door[0] = -75;
		glutPostRedisplay();
		break;
	case '3':
		door[1] -= 5;
		if (door[1] <= -90) door[1] = -90;
		glutPostRedisplay();
		break;
	case '4':
		door[1] += 5;
		if (door[1] >= 75) door[1] = 75;
		glutPostRedisplay();
		break;

	case 'q':
		mangchieu -= 0.2;
		if (mangchieu <= -2.2) mangchieu = -2.2;
		glutPostRedisplay();
		break;
	case 'e':
		mangchieu += 0.2;
		if (mangchieu >= 0) mangchieu = 0;
		glutPostRedisplay();
		break;
	case '5':
		namcua[0] += 5;
		if (namcua[0] >= 40) namcua[0] = 40;
		glutPostRedisplay();
		break;
	case '6':
		namcua[0] -= 5;
		if (namcua[0] <= 0) namcua[0] = 0;
		glutPostRedisplay();
		break;
	case '7':
		namcua[1] += 5;
		if (namcua[1] >= 40) namcua[1] = 40;
		glutPostRedisplay();
		break;
	case '8':
		namcua[1] -= 5;
		if (namcua[1] <= 0) namcua[1] = 0;
		glutPostRedisplay();
		break;
	case 'w':
		QuayCamera[2] += 10 * 0.1;
		break;
	case 's':
		QuayCamera[2] -= 10 * 0.1;
		break;
	case 'd': 
		QuayCamera[0] -= 10 * 0.1;
		break;
	case 'a':
		QuayCamera[0] += 10 * 0.1;
		break;
	case 'x':
		QuayCamera[4] += 10 * 0.1;
		break;
	case 'z':
		QuayCamera[4] -= 10 * 0.1;
		break;
	case 't':
			QuayCamera[3] += 10*0.1;
			
			if (QuayCamera[3] >= 90) QuayCamera[3] = 90;
			break;
		case 'r':
			QuayCamera[3] -= 10*0.1;
			if (QuayCamera[3] <= -90) QuayCamera[3] = -90;
			break;
		// TranVanDu
		case 'n':
			cs -= 5;
			if (cs < 0) cs = 0;
			glutPostRedisplay();
			break;
		case 'm':
			cs += 5;
			if (cs > 30) cs = 30;
			glutPostRedisplay();
			break;

		//NguyenMinhQuoc
			//bat dieu hoa
		case '(':
			// một số lệnh 
			theta[1] += 5;
			if (theta[9] > 0) theta[9] = 0;
			glutPostRedisplay();
			break;
		case '9':
			// một số lệnh 
			theta[9] -= 5;
			if (theta[9] < -60) theta[9] = -60;
			glutPostRedisplay();
			break;
		//dong mo tu
		case 'v':
			theta[8] -= 5;
			if (theta[8] < 360) theta[8] -= 360;
			glutPostRedisplay();
			break;
		case 'b':
			theta[8] += 5;
			if (theta[8] > 360) theta[8] -= 360;
			glutPostRedisplay();
			break;
		//Bat tat cau dao
		case 'u':
			aa = -15;
			if (aa >= -15) {
				aa = aa - 30;
			};
			glutPostRedisplay();
			break;
		case 'U':
			aa = 15;
			if (aa <= 15) {
				aa = aa + 30;
			};
			glutPostRedisplay();
			break;
		//Dong mo tu
		case 'o':
			if (bb >= -180) {
				bb = bb - 5;
			};
			glutPostRedisplay();
			break;
		case 'c':
			if (bb <= 0) {
				bb = bb + 5;
			};
			glutPostRedisplay();
			break;
		
	
		//Hoang Thang
		
	}

}
#pragma endregion Keyboard  

int main(int argc, char** argv)
{
	// main function: program starts here
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1000, 900);
	glutInitWindowPosition(300, 0);
	glutCreateWindow("Drawing a Cube");

	glewInit();

	generateGeometry();
	initGPUBuffers();
	shaderSetup();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);


	glutMainLoop();
}
