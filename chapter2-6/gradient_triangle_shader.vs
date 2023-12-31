#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

// 向片段着色器输出一个颜色
out vec3 ourColor;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	ourColor = aColor;
};