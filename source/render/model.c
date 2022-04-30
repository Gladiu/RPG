#define CGLTF_IMPLEMENTATION
#include "../libs/cgltf/cgltf.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "model.h"
#include "image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libs/cglm/cglm.h"

void InitModel(model *inputModel, const char modelPath[]){



	// Loading Model from file
	cgltf_options options = {0};
	cgltf_data* data = NULL;
	cgltf_result result = cgltf_parse_file(&options, modelPath, &data);
	cgltf_load_buffers(&options, data, modelPath);

	glm_mat4_identity(inputModel->model);

	inputModel->meshCount = data->meshes_count;
	inputModel->meshArray = calloc(1, sizeof(primitive*) * inputModel->meshCount);
	for (int meshIndex = 0; meshIndex < data->meshes_count; meshIndex++){
		inputModel->meshArray[meshIndex] = calloc(1, sizeof(primitive) * data->meshes[meshIndex].primitives_count);
	}

	inputModel->primitiveCount = calloc(1, sizeof(int) * inputModel->meshCount);

	inputModel->nodeCount = data->nodes_count;
	inputModel->nodeArray = calloc(1, sizeof(node) * inputModel->nodeCount);

	
	// Enabling and binding buffers buffers

	for ( int meshIndex = 0; meshIndex < data->meshes_count; meshIndex++){
		cgltf_mesh *mesh = &data->meshes[meshIndex];
		inputModel->primitiveCount[meshIndex] = mesh->primitives_count;

		for (int primitiveIndex = 0; primitiveIndex < mesh->primitives_count; primitiveIndex++){
			cgltf_primitive *primitive = &mesh->primitives[primitiveIndex];

			glGenVertexArrays(1, &inputModel->meshArray[meshIndex][primitiveIndex].VAO);
			glGenBuffers(1, &inputModel->meshArray[meshIndex][primitiveIndex].VBO);
			glGenBuffers(1, &inputModel->meshArray[meshIndex][primitiveIndex].EBO);
			glBindVertexArray(inputModel->meshArray[meshIndex][primitiveIndex].VAO);

			vec3 *position = NULL;
			vec2 *texCoord = NULL;
			vec3 *normal = NULL;
			size_t vertexCount = 0;

			for (int AttributeIndex = 0; AttributeIndex < primitive->attributes_count; AttributeIndex++){
				cgltf_attribute *Attribute = &primitive->attributes[AttributeIndex];

				if ( Attribute->type == cgltf_attribute_type_position){
					 position = calloc(1, Attribute->data->count * 3 * sizeof(float));
					 size_t floatsToUnpack = cgltf_accessor_unpack_floats(Attribute->data, NULL, 0);
					 cgltf_accessor_unpack_floats(Attribute->data, (cgltf_float*)position, floatsToUnpack);
					 vertexCount = floatsToUnpack/3;
				}

				if ( Attribute->type == cgltf_attribute_type_texcoord){
					 texCoord = calloc(1, Attribute->data->count * 2 * sizeof(float));
					 size_t floatsToUnpack = cgltf_accessor_unpack_floats(Attribute->data, NULL, 0);
					 cgltf_accessor_unpack_floats(Attribute->data, (cgltf_float*)texCoord, floatsToUnpack);
				}

				if ( Attribute->type == cgltf_attribute_type_normal){
					 normal = calloc(1, Attribute->data->count * 3 * sizeof(float));
					 size_t floatsToUnpack = cgltf_accessor_unpack_floats(Attribute->data, NULL, 0);
					 cgltf_accessor_unpack_floats(Attribute->data, (cgltf_float*)normal, floatsToUnpack);
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
			inputModel->meshArray[meshIndex][primitiveIndex].indiceCount = primitive->indices->count;
			unsigned int* indices = calloc(1,primitive->indices->count *sizeof(unsigned int));

			for (int IndiceIndex = 0; IndiceIndex  < primitive->indices->count; IndiceIndex++){
				indices[IndiceIndex] = cgltf_accessor_read_index(primitive->indices,IndiceIndex);
			}
			
			glBindBuffer(GL_ARRAY_BUFFER, inputModel->meshArray[meshIndex][primitiveIndex].VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(3+2+3)*vertexCount, VBOData, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inputModel->meshArray[meshIndex][primitiveIndex].EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, primitive->indices->count * sizeof(unsigned int), indices, GL_STATIC_DRAW);

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

		// Checking what material the mesh has assigned 
		for (int materialIndex = 0; materialIndex < data->materials_count; materialIndex++){
			if(primitive->material == &data->materials[materialIndex]){
				inputModel->meshArray[meshIndex][primitiveIndex].materialIndex = materialIndex;
			}
		}

		}
		// Reading nodes from file to get instance of meshes
		for (int nodeIndex = 0; nodeIndex < data->nodes_count; nodeIndex++){
			cgltf_node *currentNode = &data->nodes[nodeIndex];

			if (currentNode->mesh == mesh){
				inputModel->nodeArray[nodeIndex].meshIndex = meshIndex;

				if (currentNode->has_matrix){
					glm_mat4_copy((vec4*)(currentNode->matrix), inputModel->nodeArray[nodeIndex].model);
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
					glm_mat4_mul(temp,S, inputModel->nodeArray[nodeIndex].model);
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
	
	inputModel->materialCount = data->materials_count;
	inputModel->materialArray = calloc(1,sizeof(material)*inputModel->materialCount);
	for (int materialIndex = 0; materialIndex < data->materials_count; materialIndex++){
		cgltf_material *material = &data->materials[materialIndex];
		cgltf_texture *texture = material->pbr_metallic_roughness.base_color_texture.texture;
		if (material->has_pbr_metallic_roughness){
			int textureWidth = 0;
			int textureHeight = 0;
			int channels = 0;
			int forcedChannels = 4; // From stbi header

			char *modelFolder = "../source/deccer-cubes/";
			char *fullPath = malloc(strlen(texture->image->uri)+strlen(modelFolder)+1);
			sprintf(fullPath, "%s%s\0", modelFolder,texture->image->uri);
			FILE *textureFile = fopen(fullPath, "rb");

			unsigned char *textureData = LoadImageFromFile(textureFile, &textureWidth, &textureHeight, 0, 4); // 4 is from stbi docs for rgba

			fclose(textureFile);
			free(fullPath);
			glGenTextures(1, &inputModel->materialArray[materialIndex].diffuseIndex);
			glBindTexture(GL_TEXTURE_2D, inputModel->materialArray[materialIndex].diffuseIndex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture->sampler->wrap_s);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture->sampler->wrap_t);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture->sampler->min_filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture->sampler->mag_filter);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			FreeImage(textureData);
		}
	}
	free(genericFragmentShader);
	free(genericVertexShader);
	cgltf_free(data);
}

void DrawModel(model* inputModel, mat4 *projection, mat4 *view)
{
	for ( int nodeIndex = 0; nodeIndex < inputModel->nodeCount; nodeIndex++){
		node *renderedNode = &inputModel->nodeArray[nodeIndex];
		int primitiveCount = inputModel->primitiveCount[renderedNode->meshIndex];

		for ( int primitiveIndex = 0; primitiveIndex < primitiveCount; primitiveIndex++){
			primitive *renderedPrimitive = &(inputModel->meshArray[renderedNode->meshIndex][primitiveIndex]);
			//material *renderedMaterial = &()
			glUseProgram(inputModel->shaderProgram);
			
			mat4 totalModel;
			glm_mat4_mul(inputModel->model, renderedNode->model, totalModel);
			GLint modelLoc = glGetUniformLocation(inputModel->shaderProgram, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)totalModel );

			GLint viewLoc = glGetUniformLocation(inputModel->shaderProgram, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)view);

			GLint projectionLoc = glGetUniformLocation(inputModel->shaderProgram, "projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float*)projection);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, inputModel->materialArray[renderedPrimitive->materialIndex].diffuseIndex);
			glUniform1i(glGetUniformLocation(inputModel->shaderProgram,"inputTexture0"), 0);


			GLint totalAnimationFramesLoc = glGetUniformLocation(inputModel->shaderProgram, "totalAnimationFrames");
			glUniform1f(totalAnimationFramesLoc, 8.0f); // TODO change to be variable
			

			glBindVertexArray(renderedPrimitive->VAO);

			glDrawElements(GL_TRIANGLES, renderedPrimitive->indiceCount, GL_UNSIGNED_INT, 0);

			glBindVertexArray(0);
		}
	}
}
