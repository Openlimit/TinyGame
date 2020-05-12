#include "DeferredRenderer.h"
#include <iostream>

DeferredRenderer::DeferredRenderer(GLuint width, GLuint height) :Renderer(width, height)
{
	glGenFramebuffers(1, &this->G_Buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, this->G_Buffer);

	//注意如果使用RGB，则不能启动Blend
	// - 位置颜色缓冲
	this->gPosition = new Texture2D();
	this->gPosition->Internal_Format = GL_RGB16F;
	this->gPosition->Image_Format = GL_RGB;
	this->gPosition->Filter_Min = this->gPosition->Filter_Max = GL_NEAREST;
	this->gPosition->GenerateFloat(width, height, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->gPosition->ID, 0);

	// - 法线颜色缓冲
	this->gNormal = new Texture2D();
	this->gNormal->Internal_Format = GL_RGB16F;
	this->gNormal->Image_Format = GL_RGB;
	this->gNormal->Filter_Min = this->gNormal->Filter_Max = GL_NEAREST;
	this->gNormal->GenerateFloat(width, height, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->gNormal->ID, 0);

	// - 颜色 + 镜面颜色缓冲
	this->gAlbedoSpec = new Texture2D();
	this->gAlbedoSpec->Internal_Format = GL_RGBA;
	this->gAlbedoSpec->Image_Format = GL_RGBA;
	this->gAlbedoSpec->Filter_Min = this->gAlbedoSpec->Filter_Max = GL_NEAREST;
	this->gAlbedoSpec->Generate(width, height, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, this->gAlbedoSpec->ID, 0);

	// - MRT(Multiple Render Targets)
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// Depth RenderBuffer
	glGenRenderbuffers(1, &this->rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, this->rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// - Finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	this->initRenderData();
}

DeferredRenderer::~DeferredRenderer()
{
	glDeleteBuffers(1, &this->VBO);
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteRenderbuffers(1, &this->rboDepth);
	glDeleteFramebuffers(1, &this->G_Buffer);

	if (this->gPosition != nullptr)
		delete this->gPosition;
	if (this->gNormal != nullptr)
		delete this->gNormal;
	if (this->gAlbedoSpec != nullptr)
		delete this->gAlbedoSpec;
}

void DeferredRenderer::initRenderData()
{
	// Configure VAO/VBO
	GLfloat vertices[] = {
		// Pos        // Tex
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void DeferredRenderer::Init(Scene* scene)
{

}

void DeferredRenderer::Draw(Scene *scene)
{
	//Geometric Pass
	glBindFramebuffer(GL_FRAMEBUFFER, this->G_Buffer);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto iter : scene->shaderObjectlist)
	{
		auto shaderID = iter.first;
		auto objectList = iter.second;

		glUseProgram(shaderID);
		for (auto obj : objectList)
		{
			glm::mat4 model = obj->transform.getTransform();
			obj->material->defferedGeoShader->SetMatrix4("model", model);
			obj->material->updateDefferedGeoShader();
			for (int j = 0; j < obj->material->textures.size(); j++)
			{
				glActiveTexture(GL_TEXTURE0 + j);
				obj->material->textures[j]->Bind();
			}

			glBindVertexArray(obj->VAO);
			//glDrawElements(GL_TRIANGLES, obj->mesh->indices.size(), GL_UNSIGNED_INT, 0);
			glDrawArrays(GL_TRIANGLES, 0, obj->mesh->vertices.size());
			glBindVertexArray(0);
		}
	}

	//Shading Pass
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	if (postProcessor != nullptr)
		postProcessor->BeginRender();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	auto firstObject = scene->renderObjects.begin();
	firstObject->second->material->defferedShadingShader->Use();
	firstObject->second->material->updateDefferedShadingShader();

	glActiveTexture(GL_TEXTURE0);
	this->gPosition->Bind();
	glActiveTexture(GL_TEXTURE1);
	this->gNormal->Bind();
	glActiveTexture(GL_TEXTURE2);
	this->gAlbedoSpec->Bind();

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	if (postProcessor != nullptr)
		postProcessor->Render(this->gPosition);
}