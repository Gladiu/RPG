#define CGLTF_IMPLEMENTATION
#include "../libs/cgltf/cgltf.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include "model.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/cglm/cglm.h"

void InitModel(model *inputModel, const char modelPath[], const char texturePath[]){



	// Loading Model from file
	cgltf_options options = {0};
	cgltf_data* data = NULL;
	cgltf_result result = cgltf_parse_file(&options, modelPath, &data);
	cgltf_load_buffers(&options, data, modelPath);

	inputModel->meshCount = data->meshes_count;
	inputModel->nodeCount = data->nodes_count;
	inputModel->meshArray = calloc(1, sizeof(primitive*) * inputModel->meshCount);
	for (int currentMeshIndex = 0; currentMeshIndex < data->meshes_count; currentMeshIndex++){
		(inputModel->meshArray)[currentMeshIndex] = calloc(1, sizeof(primitive) * data->meshes[currentMeshIndex].primitives_count);
	}

	
	// Enabling and binding buffers buffers

	for ( int currentMeshIndex = 0; currentMeshIndex < data->meshes_count; currentMeshIndex++){
		cgltf_mesh *currentMesh = &data->meshes[currentMeshIndex];

		for (int currentPrimitiveIndex = 0; currentPrimitiveIndex < currentMesh->primitives_count; currentPrimitiveIndex++){
			cgltf_primitive *currentPrimitive = &currentMesh->primitives[currentPrimitiveIndex];

			glGenVertexArrays(1, &inputModel->meshArray[currentMeshIndex][currentPrimitiveIndex].VAO);
			glGenBuffers(1, &inputModel->meshArray[currentMeshIndex][currentPrimitiveIndex].VBO);
			glGenBuffers(1, &inputModel->meshArray[currentMeshIndex][currentPrimitiveIndex].EBO);
			glBindVertexArray(inputModel->meshArray[currentMeshIndex][currentPrimitiveIndex].VAO);

			vec3 *position = NULL;
			vec2 *texCoord = NULL;
			vec3 *normal = NULL;
			size_t vertexCount = 0;

			for (int currentAttributeIndex = 0; currentAttributeIndex < currentPrimitive->attributes_count; currentAttributeIndex++){
				cgltf_attribute *currentAttribute = &currentPrimitive->attributes[currentAttributeIndex];

				if ( currentAttribute->type == cgltf_attribute_type_position){
					 position = calloc(1, currentAttribute->data->count * 3 * sizeof(float));
					 size_t floatsToUnpack = cgltf_accessor_unpack_floats(currentAttribute->data, NULL, 0);
					 cgltf_accessor_unpack_floats(currentAttribute->data, (cgltf_float*)position, floatsToUnpack);
					 vertexCount = floatsToUnpack/3;
				}

				if ( currentAttribute->type == cgltf_attribute_type_texcoord){
					 texCoord = calloc(1, currentAttribute->data->count * 2 * sizeof(float));
					 size_t floatsToUnpack = cgltf_accessor_unpack_floats(currentAttribute->data, NULL, 0);
					 cgltf_accessor_unpack_floats(currentAttribute->data, (cgltf_float*)texCoord, floatsToUnpack);
				}

				if ( currentAttribute->type == cgltf_attribute_type_normal){
					 normal = calloc(1, currentAttribute->data->count * 3 * sizeof(float));
					 size_t floatsToUnpack = cgltf_accessor_unpack_floats(currentAttribute->data, NULL, 0);
					 cgltf_accessor_unpack_floats(currentAttribute->data, (cgltf_float*)normal, floatsToUnpack);
				}
			}

			// Procesing all the data to fit it into one VBO
			
			float *VBOData = calloc(1, sizeof(float)*(3+2+3)*vertexCount);

			for ( int i = 0; i < vertexCount; i++){
				VBOData[i*8 + 0] = (position[i])[0];
				VBOData[i*8 + 1] = (position[i])[1];
				VBOData[i*8 + 2] = (position[i])[2];
				VBOData[i*8 + 3] = (texCoord[i])[0];
				VBOData[i*8 + 4] = (texCoord[i])[1];
				VBOData[i*8 + 5] = (normal[i])[0];
				VBOData[i*8 + 6] = (normal[i])[1];
				VBOData[i*8 + 7] = (normal[i])[2];
			}
			inputModel->meshArray[currentMeshIndex][currentPrimitiveIndex].indiceCount = currentPrimitive->indices->count;
			unsigned int* indices = calloc(1,currentPrimitive->indices->count *sizeof(unsigned int));

			for (int currentIndiceIndex = 0; currentIndiceIndex  < currentPrimitive->indices->count; currentIndiceIndex++){
				indices[currentIndiceIndex] = cgltf_accessor_read_index(currentPrimitive->indices,currentIndiceIndex);
			}
			
			glBindBuffer(GL_ARRAY_BUFFER, inputModel->meshArray[currentMeshIndex][currentPrimitiveIndex].VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(3+2+3)*vertexCount, VBOData, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inputModel->meshArray[currentMeshIndex][currentPrimitiveIndex].EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, currentPrimitive->indices->count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(0));
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3*sizeof(float)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(5*sizeof(float)));
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			free(VBOData);
			free(position);
			free(texCoord);
			free(normal);
			free(indices);

		}
		// Reading nodes from file to get instance of meshes
		for (int currentNodeIndex = 0; currentNodeIndex < data->nodes_count; currentNodeIndex++){
			cgltf_node *currentNode = &data->nodes[currentNodeIndex];

			if (currentNode->mesh == currentMesh){
				inputModel->nodeArray[currentNodeIndex].meshIndex = currentMeshIndex;

				if (currentNode->has_matrix){
					glm_mat4_copy((vec4*)(currentNode->matrix), inputModel->nodeArray[currentNodeIndex].model);
				}
				else{
					mat4 T;
					glm_mat4_identity(T);
					mat4 R;
					glm_mat4_identity(R);
					mat4 S;
					glm_mat4_identity(S);

					if (currentNode->has_translation){
						glm_translate_to(T, currentNode->translation, T);
					}

					if (currentNode->has_rotation){
						glm_euler_xyz(currentNode->rotation, T);
					}

					if (currentNode->has_scale){
						glm_scale(S, currentNode->scale);
					}
					mat4 temp;
					glm_mat4_mul(T, R, temp);
					glm_mat4_mul(temp,S, inputModel->nodeArray[currentNodeIndex].model);
				}
			}
		}
	}

	// Disabling buffers
	glBindVertexArray(0);



	// Creating Shaders
	
	FILE *f = fopen("../source/render/generic.frag", "rb");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *genericFragmentShader = calloc(1, fsize + 1);
	fread(genericFragmentShader , fsize, 1, f);
	fclose(f);

	GLint succes;
	GLchar infoLog[512];
	GLuint fragment;
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, (const GLchar**)&genericFragmentShader, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &succes);
	if(!succes)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		fprintf(stderr, "FATAL. COULDNT COMPILE FRAGMENT SHADER %s\n", infoLog);
		exit(EXIT_FAILURE);
	}

	f = fopen("../source/render/generic.vert", "rb");
	fseek(f, 0, SEEK_END);
	fsize = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *genericVertexShader = calloc(1, fsize + 1);
	fread(genericVertexShader , fsize, 1, f);
	fclose(f);

	GLuint vertex;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, (const GLchar**)&genericVertexShader, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &succes);
	if(!succes)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		fprintf(stderr, "FATAL. COULDNT COMPILE VERTEX SHADER %s\n", infoLog);
		exit(EXIT_FAILURE);
	}
	
	inputModel->shaderProgram = glCreateProgram();
	glAttachShader(inputModel->shaderProgram, vertex);
	glAttachShader(inputModel->shaderProgram, fragment);
	glLinkProgram(inputModel->shaderProgram);
	glGetProgramiv(inputModel->shaderProgram, GL_LINK_STATUS, &succes);
	if(!succes)
	{
		glGetProgramInfoLog(inputModel->shaderProgram, 512, NULL, infoLog);
		fprintf(stderr, "FATAL. COULDNT LINK SHADER PROGRAM %s\n", infoLog);
		exit(EXIT_FAILURE);
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);


	// TODO Texture support
	if ( 0 ) {
		int textureWidth, textureHeight;
		unsigned char *image = SOIL_load_image(texturePath, &textureWidth, &textureHeight, 0, SOIL_LOAD_RGBA);
		glGenTextures(1, &inputModel->tex0);
		glBindTexture(GL_TEXTURE_2D, inputModel->tex0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);
 	}
	free(genericFragmentShader);
	free(genericVertexShader);
	cgltf_free(data);
}

void DrawModel(model* inputModel, mat4 *projection, mat4 *view)
{
	for ( int currentNodeIndex = 0; currentNodeIndex < inputModel->nodeCount; currentNodeIndex++){
		node *currentNode = &inputModel->nodeArray[currentNodeIndex];

		for ( int currentPrimitiveIndex = 0; currentPrimitiveIndex < inputModel->primitiveCount[currentNode->meshIndex]; currentPrimitiveIndex++){
			glUseProgram(inputModel->shaderProgram);
			
			mat4 totalModel;
			glm_mat4_mul(inputModel->model, currentNode->model, totalModel);
			GLint modelLoc = glGetUniformLocation(inputModel->shaderProgram, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)totalModel );

			GLint viewLoc = glGetUniformLocation(inputModel->shaderProgram, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)view);

			GLint projectionLoc = glGetUniformLocation(inputModel->shaderProgram, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float*)projection);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, inputModel->tex0);
			glUniform1i(glGetUniformLocation(inputModel->shaderProgram,"inputTexture0"), 0);


			GLint totalAnimationFramesLoc = glGetUniformLocation(inputModel->shaderProgram, "totalAnimationFrames");
			glUniform1f(totalAnimationFramesLoc, 8.0f); // TODO change to be variable
			

			glBindVertexArray(inputModel->meshArray[currentNode->meshIndex]);

			glDrawElements(GL_TRIANGLES, inputModel->meshArray[meshIndex][primitiveIndex].indiceCount, GL_UNSIGNED_INT, 0);

			glBindVertexArray(0);
		}
	}
}
