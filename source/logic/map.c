#include "map.h"
#include "../render/tiles.h"
#include "../render/model.h"

#include "../libs/nxjson/nxjson.h"
#include "../libs/cglm/cglm.h"

#include "stdio.h"
#include "stdlib.h"

void InitMap(map* inputMap, char pathToMapFile[]){

	// Opening map file 
	FILE* mapFile = fopen(pathToMapFile, "rb");
	fseek(mapFile, 0, SEEK_END);
	long int fsize = ftell(mapFile);
	fseek(mapFile, 0, SEEK_SET);
	char *mapFileContent= calloc(1, fsize + 1);
	fread(mapFileContent, fsize, 1, mapFile);
	fclose(mapFile);
	
	// Parsing json
	const nx_json* map = nx_json_parse(mapFileContent, 0);
	
	if (map){

		// Unpacking tiles
		const nx_json* tilesArray= nx_json_get(map, "tiles");
 		inputMap->tileCount = tilesArray->children.length;
		inputMap->tilesArray = calloc(1, sizeof(tiles) * tilesArray->children.length );

		for (int i = 0; i < tilesArray->children.length; i++){
			const nx_json* currentTiles = nx_json_item(tilesArray, i);
			const char* texturePath = nx_json_get(currentTiles, "texturePath")->text_value;
			const nx_json* shape = nx_json_get(currentTiles, "shape");

			bool random = nx_json_get(currentTiles, "randomTiles")->num.u_value;

			size_t ySize = shape->children.length;
			size_t xSize = nx_json_item(shape, 0)->children.length;
			int *tileShape = calloc(1, sizeof(int) * xSize * ySize); // Becouse i cant into 2D arrays
			for (int y = 0; y < ySize; y++){
				const nx_json* xShapeRow = nx_json_item(shape, y);
				for (int x = 0; x < xSize; x++){
					if ( nx_json_item(xShapeRow, x)->num.u_value != 0){
						tileShape[y*xSize + x] = nx_json_item(xShapeRow, x)->num.u_value;
					}
					else{
						tileShape[y*xSize + x] = 0;
					}
				}

			}
			InitTiles(&(inputMap->tilesArray[i]),random, tileShape, ySize, xSize, texturePath);
			free(tileShape);
		}

		// Unpacking models
		const nx_json* modelArray= nx_json_get(map, "models");
 		inputMap->modelCount = modelArray->children.length;
		inputMap->modelArray = calloc(1, sizeof(tiles) * modelArray->children.length );

		for (int i = 0; i < modelArray->children.length; i++){
			const nx_json* currentModel = nx_json_item(modelArray, i);
			const char* texturePath = NULL;//nx_json_get(currentModel, "texturePath")->text_value;
			const char* modelPath = nx_json_get(currentModel, "modelPath")->text_value;

			InitModel(&(inputMap->modelArray[i]), modelPath, texturePath);
		}
	}
}

void DrawMap(map *inputMap, mat4 *projection, mat4 *view, double currentTime){
	for (int i = 0; i < inputMap->tileCount; i++){
		DrawTiles(&(inputMap->tilesArray[i]), projection, view, currentTime);
	}
}
