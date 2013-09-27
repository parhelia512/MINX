#include "Texture2D.h"
using namespace MINX::Graphics;
Texture2D::Texture2D(char* fileLoc, GLuint shaderProgram, GLuint textures[], int texID)
{
	float tempVertices[] = {
	-01.0f,  01.0f,		 1.0f, 1.0f, 1.0f,		0.0f, 1.0f, // Top-left
     01.0f,  01.0f,		 1.0f, 1.0f, 1.0f,		1.0f, 1.0f,// Top-right
     01.0f, -01.0f,		 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,// Bottom-right

     01.0f, -01.0f,		 1.0f, 1.0f, 1.0f,		1.0f, 0.0f,// Bottom-right
    -01.0f, -01.0f,		 1.0f, 1.0f, 1.0f,		0.0f, 0.0f,// Bottom-left
    -01.0f,  01.0f,		 1.0f, 1.0f, 1.0f,		0.0f, 1.0f// Top-left
	};

	for(int i =0 ; i < sizeof(vertices) / sizeof(float); i++)
	{
		vertices[i] =  tempVertices[i];
	}

	glGenVertexArrays(1,&vertexArray);
	glBindVertexArray(vertexArray);
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,  sizeof(vertices), vertices, GL_STATIC_DRAW);


	
	GLint posAttrib = glGetAttribLocation( shaderProgram, "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), 0 );

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib,3,GL_FLOAT,GL_FALSE,7*sizeof(float),(void*)(2*sizeof(float)));

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7*sizeof(float), (void*)(5*sizeof(float)));
		this->shaderProgram = shaderProgram;
	
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, (textures)[texID]);
	//glBindTexture(GL_TEXTURE_2D, tex);
	//float color[] = {1.0f, 0.0f, 0.0f, 1.0f};
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color
	// IMAGAE LOADING
		FIBITMAP* bitmap = FreeImage_Load(
			FreeImage_GetFileType(fileLoc, 0),
			fileLoc);
		FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);

		int nWidth = FreeImage_GetWidth(pImage);
		int nHeight = FreeImage_GetHeight(pImage);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
			0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

		FreeImage_Unload(pImage);
	//END IMAGE LOADING);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glGenerateMipmap(GL_TEXTURE_2D);

	 uniTrans = glGetUniformLocation( shaderProgram, "trans" );
	 uniTint = glGetUniformLocation(shaderProgram, "tint");

	 this->texID = texID;
	 this->textures = textures;
	//*/
}
Texture2D::~Texture2D()
{
	glDeleteBuffers( 1, &vertexBuffer );
	glDeleteVertexArrays( 1, &vertexArray );
}
void Texture2D::Draw(int x, int y)
{
	glm::mat4 trans;
	trans = glm::translate(trans, glm::vec3(x,y,0));
	trans = fitToScreen(trans);
	glUniformMatrix4fv( uniTrans, 1, GL_FALSE, glm::value_ptr( trans ) );
	//glUniform3f(uniTint, .75,0,.75);
	this->Draw();
}
void Texture2D::Draw(int x, int y, float scaleX, float scaleY)
{
	glm::mat4 trans;
	trans = glm::scale(trans, glm::vec3(scaleX, scaleY, 1));
	trans = glm::translate(trans, glm::vec3(x,y,0));
	trans = fitToScreen(trans);
	glUniformMatrix4fv( uniTrans, 1, GL_FALSE, glm::value_ptr( trans ) );
	//glUniform3f(uniTint, .75,0,.75);
	this->Draw();
}
void Texture2D::Draw(int x, int y, float rotationAngle)
{
	glm::mat4 trans;
	trans = glm::rotate(trans, rotationAngle,glm::vec3(1,1,0));
	trans = glm::translate(trans, glm::vec3(x,y,0));
	trans = fitToScreen(trans);
	glUniformMatrix4fv( uniTrans, 1, GL_FALSE, glm::value_ptr( trans ) );
	//glUniform3f(uniTint, .75,0,.75);
	this->Draw();
}
void Texture2D::Draw(int x, int y, float scaleX, float scaleY, float rotationAngle)
{
	glm::mat4 trans;
	trans = glm::scale(trans, glm::vec3(scaleX, scaleY, 1));
	trans = glm::rotate(trans, rotationAngle,glm::vec3(1,1,0));
	trans = glm::translate(trans, glm::vec3(x,y,0));
	trans = fitToScreen(trans);
	glUniformMatrix4fv( uniTrans, 1, GL_FALSE, glm::value_ptr( trans ) );
	//glUniform3f(uniTint, .75,0,.75);
	this->Draw();
}

void Texture2D::Draw(int x, int y, float scaleX, float scaleY, float rotationAngle, Color* tintColor)
{
	glm::mat4 trans;

	trans = glm::scale(trans, glm::vec3(scaleX, scaleY, 1));
	trans = glm::rotate(trans, rotationAngle,glm::vec3(0,0,1));
	trans = glm::translate(trans, glm::vec3(x,y,0));
	trans = fitToScreen(trans);
	glUniformMatrix4fv( uniTrans, 1, GL_FALSE, glm::value_ptr( trans ) );
	glUniform3f(uniTint, tintColor->R/255.0f,tintColor->G/255.0f,tintColor->B/255.0f);
	this->Draw();
}

void Texture2D::Draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, (textures)[texID]);
	glBindVertexArray(vertexArray);
	glDrawArrays(GL_TRIANGLES,0,6);
}
glm::mat4 Texture2D::fitToScreen(glm::mat4 trans)
{
	//glm::scale(trans, glm::vec3(1.0f/GameWindow::width, 1.0f/GameWindow::height, 1.0f));
	return glm::translate(trans, glm::vec3(-GameWindow::width/2, -GameWindow::height/2 ,1.0f));
}