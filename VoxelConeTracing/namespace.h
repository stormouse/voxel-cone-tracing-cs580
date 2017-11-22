#pragma once
#ifndef NAMESPACE_H_
#define NAMESPACE_H_

namespace {
	const char * voxelVisualizationShaderVS = "Shaders/voxel_visualization.vert";
	const char * voxelVisualizationShaderFS = "Shaders/voxel_visualization.frag";
	const char * worldPositionShaderVS = "Shaders/world_position.vert";
	const char * worldPositionShaderFS = "Shaders/world_position.frag";

	const char * modelMatrixName = "M";
	const char * viewMatrixName = "V";
	const char * projectionMatrixName = "P";
	const char * positionName = "position";
	const char * normalName = "normal";
	const char * colorName = "color";
	const char * texCoordName = "texCoord";
	const char * indicesName = "indices";
	const char * cameraPostionName = "cameraPosition";

	const char * textureFrontName = "textureFront";
	const char * textureBackName = "textureBack";
	const char * texture3DName = "texture3D";

	const char * diffuseColorName = "diffuseColor";
	const char * specularColorName = "specularColor";
	const char * emissivityName = "emissivity";
	const char * transparencyName = "transparency";
	const char * refractiveIndexName = "refractiveIndex";
	const char * specularReflectanceName = "specularReflectivity";
	const char * diffuseReflectanceName = "diffuseReflectivity";
	const char * specularDiffusionName = "specularDiffusion";
}


#endif // !NAMESPACE_H_
