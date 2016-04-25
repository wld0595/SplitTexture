/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code
#include <stdio.h>
#include <stdlib.h>

#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "Vertex.h"
#include "Texture.h"
#include "FBO.h"

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)



static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

static const char gVertexShader[]=
	"attribute vec4 aPosition;\n"
	"attribute vec2 aTexCoord;\n"
	"varying vec2 vTexCoord;\n"
	"void main() {\n"
	" gl_Position=aPosition;\n"
	" vTexCoord=aTexCoord;\n"
	"}\n";

static const char gFragmentShader[] =
	"precision mediump float;\n"
	"varying vec2 vTexCoord;\n"
	"uniform sampler2D sTexture;\n"
	"void main() {\n"
	"gl_FragColor = texture2D(sTexture,vTexCoord);\n"
	"}\n";

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

GLuint gProgram;//==//the handle of shader program 
GLuint gvPositionHandle;//===//the handle of vertex in vertex shader program
GLuint gTexHandle;//===//the handle of texture(UV) in vertex shader program
GLuint gTexIndex;//===//the handle of texture in fragment shader program

//===//Declaration the object of Vertex class and Texture class and Fbo class
Vertex CurrentVertex;
Texture CurrentTexture;
Fbo CurrentFbo;

//===//The vertex data of window's frame  buffer
static VertexData g_VertexBufferData[]={
	{-1.0,1.0,0.0, 0.0,1.0},
	{1.0,1.0,0.0,  1.0,1.0},
	{-1.0,-1.0,0.0,  0.0,0.0},
	{1.0,-1.0,0.0,   1.0,0.0},
};

//===//The handle of vertex data
GLuint g_uiVertexBuffer;

//===//The width and height of window
GLint g_iWindowWidth=0;
GLint g_iWindowHeight=0;

bool setupGraphics(int w, int h) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);

	//===//acquire the width and height of window
	g_iWindowWidth=w;
	g_iWindowHeight=h;

	//===//Set the vertex data of Fbo
	CurrentVertex.SetVertexData();
	//===//VBO
	CurrentVertex.CreateVBO();

	//===//Set the handle of window's frame  buffer and fill data
	glGenBuffers(1,&g_uiVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,g_uiVertexBuffer);
	if (glIsBuffer(g_uiVertexBuffer)==GL_FALSE)
	{
		fprintf(stderr,"Failed to generate or bind the vertex buffer.Please try again.\n");
		getchar();
		exit(EXIT_FAILURE);
	}
	glBufferData(GL_ARRAY_BUFFER,(sizeof(VertexData)*4),g_VertexBufferData,GL_STATIC_DRAW);

	//===//Load texture
	//CurrentTexture.LoadTex("/sdcard/l2.jpg");
	CurrentTexture.LoadTex("/sdcard/text.jpg");
	
	//===//Set the size of FBO
	CurrentFbo.SetWidth(CurrentTexture.GetTexWidth());
	CurrentFbo.SetHeight(CurrentTexture.GetTexHeight());
	//===//Create a FBO
	CurrentFbo.GenFBO();

	//===//Load shader
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }

	gvPositionHandle = glGetAttribLocation(gProgram, "aPosition");
	checkGlError("glGetAttribLocation");
	LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
		gvPositionHandle);

	gTexHandle = glGetAttribLocation(gProgram, "aTexCoord");
	checkGlError("glGetAttribLocation");
	LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
		gTexHandle);

	gTexIndex =glGetUniformLocation(gProgram, "sTexture");
	checkGlError("glGetAttribLocation");
	LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
		gTexIndex);
	

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	checkGlError("glClearColor");

    return true;
}

void renderFrame() {
    
	glBindFramebuffer(GL_FRAMEBUFFER, CurrentFbo.GetFrameBufferID());

	glClear( GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

	glViewport(0, 0, CurrentTexture.GetTexWidth(), CurrentTexture.GetTexHeight());

	glUseProgram(gProgram);
	checkGlError("glUseProgram");
	
	glActiveTexture(GL_TEXTURE0);
	checkGlError("glActiveTexture");
	glBindTexture(GL_TEXTURE_2D, CurrentTexture.GetTextureID());
	checkGlError("glBindTexture");
	glUniform1i(gTexIndex, 0);

	glEnableVertexAttribArray(gvPositionHandle);
	checkGlError("glEnableVertexAttribArray");
	glBindBuffer(GL_ARRAY_BUFFER,CurrentVertex.GetVertexBuffer());
	glVertexAttribPointer(gvPositionHandle, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)0);
	checkGlError("glVertexAttribPointer");

	glEnableVertexAttribArray(gTexHandle);
	checkGlError("glEnableVertexAttribArray");
	glVertexAttribPointer(gTexHandle, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)12);
	checkGlError("glVertexAttribPointer");

	//Draw a triangle
	for (GLint iCnt=0;iCnt<5*9*4;iCnt+=4)
	{
		glDrawArrays(GL_TRIANGLE_STRIP,iCnt,4);
		checkGlError("glDrawArrays");
	}

	

	//==============//
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear( GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, g_iWindowWidth, g_iWindowHeight);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, CurrentFbo.GetFrameTextureID());
	glUniform1i(gTexIndex,0);

	glBindBuffer(GL_ARRAY_BUFFER,g_uiVertexBuffer);
	glVertexAttribPointer(gvPositionHandle,3,GL_FLOAT,GL_FALSE,20,(void *)0);

	glVertexAttribPointer(gTexHandle,2,GL_FLOAT,GL_FALSE,20,(void *)12);//offset site very important!!!

	glDrawArrays(GL_TRIANGLE_STRIP,0,4);

	//Disable the attribute array
	glDisableVertexAttribArray(gvPositionHandle);
	glDisableVertexAttribArray(gTexHandle);

}

extern "C" {
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_android_gl2jni_GL2JNILib_step(JNIEnv * env, jobject obj)
{
    renderFrame();
}


