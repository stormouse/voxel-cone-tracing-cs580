#include "Object.h"

Object::Object(string path) {
	this->obj_path = path;
	
	this->CheckObjectPath();
	if (this->objectType != Quad) {
		this->LoadObject();
	}
	else {
		this->createQuad();
	}
	this->TransformObject();
	this->AssignMaterial();
	this->BindObjectToBuffer();
}

Object::~Object() {
	if (this->has_material) {
		for (auto * mat : materials) {
			delete mat;
			mat = NULL;
		}
	}
	
}

void Object::CheckObjectPath() {
	//convert string to lowercase
	transform(obj_path.begin(), obj_path.end(), obj_path.begin(), ::tolower);
	
	if (this->obj_path.find("buddha") != string::npos) {
		this->objectType = Buddha;
	}
	else if (this->obj_path.find("bunny") != string::npos) {
		this->objectType = Bunny;
	}
	else if (this->obj_path.find("cornell") != string::npos) {
		this->objectType = Cornell;
	}
	else if (this->obj_path.find("dragon") != string::npos) {
		this->objectType = Dragon;
	}
	else if (this->obj_path.find("susanne") != string::npos) {
		this->objectType = Susanne;
	}
	else if (this->obj_path.find("quad") != string::npos) {
		this->objectType = Quad;
	}
	else if (this->obj_path.find("teapot") != string::npos) {
		this->objectType = Teapot;
	}
	else {
		this->objectType = None;
	}
}

void Object::createQuad() {
	Mesh mesh;
	VertexData vertex;

	vertex.position = { -1, -1, 1 };
	vertex.texCoord = { 0, 1 };
	vertex.normal = { 0, 0, 1 };
	mesh.vertexData.push_back(vertex);

	vertex.position = { 1, -1, 1 };
	vertex.texCoord = { 0, 0 };
	vertex.normal = { 0, 0, 1 };
	mesh.vertexData.push_back(vertex);

	vertex.position = { 1, 1, 1 };
	vertex.texCoord = { 1, 1 };
	vertex.normal = { 0, 0, 1 };
	mesh.vertexData.push_back(vertex);

	vertex.position = { -1, 1, 1 };
	vertex.texCoord = { 1 , 0 };
	vertex.normal = { 0, 0, 1 };
	mesh.vertexData.push_back(vertex);

	mesh.indices = { 0, 1, 2, 0, 2, 3 };
	this->meshes.push_back(mesh);
	this->mesh_size = meshes.size();
	std::cout << std::setprecision(4) << " - Creating Quad Successfully." << std::endl;
}

void Object::LoadObject() {
#if __UTILITY_LOG_LOADING_TIME
	double logTimestamp = glfwGetTime();
	double took;
	std::cout << "Loading obj '" << this->obj_path << "'..." << std::endl;
#endif

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	if (!tinyobj::LoadObj(shapes, materials, err, this->obj_path.c_str()) || shapes.size() == 0) {
#if __UTILITY_LOG_LOADING_TIME
		std::cerr << "Failed to load object with path '" << this->obj_path << "'. Error message:" << std::endl << err << std::endl;
#endif
		return;
	}

#if __UTILITY_LOG_LOADING_TIME
	took = glfwGetTime() - logTimestamp;
	std::cout << std::setprecision(4) << " - Parsing '" << this->obj_path << "' took " << took << " seconds (by tinyobjloader)." << std::endl;
	logTimestamp = glfwGetTime();
#endif

	// Load all shapes.
	for (const auto & shape : shapes) {

		// Create a new mesh.
		Mesh newMesh;
		auto & vertexData = newMesh.vertexData;
		auto & indices = newMesh.indices;

		indices.reserve(shape.mesh.indices.size());

		// Push back all indices.
		for (const auto index : shape.mesh.indices) {
			indices.push_back(index);
		}

		vertexData.reserve(shape.mesh.positions.size());

		// Positions.
		for (unsigned int i = 0, j = 0; i < shape.mesh.positions.size(); i += 3, ++j) {
			if (j >= vertexData.size()) {
				vertexData.push_back(VertexData());
			}
			vertexData[j].position.x = shape.mesh.positions[i + 0];
			vertexData[j].position.y = shape.mesh.positions[i + 1];
			vertexData[j].position.z = shape.mesh.positions[i + 2];
		}

		// Normals.
		for (unsigned int i = 0, j = 0; i < shape.mesh.normals.size(); i += 3, ++j) {
			if (j >= vertexData.size()) {
				vertexData.push_back(VertexData());
			}
			vertexData[j].normal.x = shape.mesh.normals[i + 0];
			vertexData[j].normal.y = shape.mesh.normals[i + 1];
			vertexData[j].normal.z = shape.mesh.normals[i + 2];
		}

		// Texture coordinates.
		for (unsigned int i = 0, j = 0; i < shape.mesh.texcoords.size(); i += 2, ++j) {
			if (j >= vertexData.size()) {
				vertexData.push_back(VertexData());
			}
			vertexData[j].texCoord.x = shape.mesh.texcoords[i + 0];
			vertexData[j].texCoord.y = shape.mesh.texcoords[i + 1];
		}
		this->meshes.push_back(newMesh);
	}

	this->mesh_size = meshes.size();

#if __UTILITY_LOG_LOADING_TIME
	took = glfwGetTime() - logTimestamp;
	std::cout << std::setprecision(4) << " - Loading '" << this->obj_path << "' took " << took << " seconds." << std::endl;
#endif
}

void Object::BindObjectToBuffer() {
	for (int i = 0; i < this->mesh_size; i++) {
		meshes[i].BindMeshToBuffer();
	}
}

/*Transforming Object*/
void Object::TransformObject() {
	switch (this->objectType) {
	case Buddha:
		TransformBuddha();
		break;
	case Bunny:
		TransformBunny();
		break;
	case Cornell:
		TransformCornell();
		break;
	case Dragon:
		TransformDragon();
		break;
	case Susanne:
		TransformSusanne();
		break;
	case Teapot:
		TransformTeapot();
		break;
	default:
		break;
	}
}

void Object::TransformBuddha() {
	for (int i = 0; i < this->mesh_size; i++) {
		meshes[i].transform.scale = glm::vec3(1.8f);
		meshes[i].transform.rotation = glm::vec3(0, 2.4, 0);
		meshes[i].transform.position = glm::vec3(0, -0.13, 0.05);
		meshes[i].transform.updateTransformMatrix();
	}
}

void Object::TransformBunny() {
	for (int i = 0; i < this->mesh_size; i++) {
		meshes[i].transform.scale = glm::vec3(0.31f);
		meshes[i].transform.rotation = glm::vec3(0, 0.4, 0);
		meshes[i].transform.position = glm::vec3(0.44, -0.62, 0);
		meshes[i].transform.updateTransformMatrix();
	}
}

void Object::TransformCornell() {
	for (int i = 0; i < this->mesh_size; i++) {
		meshes[i].transform.scale = glm::vec3(0.995f);
		meshes[i].transform.position -= glm::vec3(0, 0, 0);
		meshes[i].transform.updateTransformMatrix();
	}
	this->mesh_size = meshes.size() - 2;
}

void Object::TransformDragon() {
	for (int i = 0; i < this->mesh_size; i++) {
		meshes[i].transform.scale = glm::vec3(1.3f);
		meshes[i].transform.rotation = glm::vec3(0, 2.1, 0);
		meshes[i].transform.position = glm::vec3(-0.28, -0.52, 0);
		meshes[i].transform.updateTransformMatrix();
	}
}

void Object::TransformSusanne() {
	for (int i = 0; i < this->mesh_size; i++) {
		meshes[i].transform.scale = glm::vec3(0.23f);
		meshes[i].transform.rotation = glm::vec3(0, 0, 0);
		meshes[i].transform.position = glm::vec3(-0.44, -0.45, -0.2);
		meshes[i].transform.updateTransformMatrix();
	}
}

void Object::TransformTeapot() {
	for (int i = 0; i < this->mesh_size; i++) {
		meshes[i].transform.scale = glm::vec3(0.05f);
		meshes[i].transform.rotation = glm::vec3(0, 0, 0);
		meshes[i].transform.position = glm::vec3(0, 0.15, -0.36); 
		meshes[i].transform.updateTransformMatrix();
	}
}

/*Assigning Material to Object*/
void Object::AssignMaterial() {
	switch (this->objectType) {
	case Buddha:
		BuddhaMaterial();
		this->has_material = true;
		break;
	case Bunny:
		BunnyMaterial();
		this->has_material = true;
		break;
	case Cornell:
		CornellMaterial();
		this->has_material = true;
		break;
	case Dragon:
		DragonMaterial();
		this->has_material = true;
		break;
	case Susanne:
		SusanneMaterial();
		this->has_material = true;
		break;
	case Teapot:
		TeapotMaterial();
		this->has_material = true;
		break;
	default:
		this->has_material = false;
		break;
	}
}

void Object::BuddhaMaterial() {
	for (int i = 0; i < this->mesh_size; i++) {
		Material *material = Material::White();
		material->specularColor = glm::vec3(0.99, 0.61, 0.43);
		material->diffuseColor = material->specularColor;
		material->emissivity = 0.0f;
		material->transparency = 1.0f;
		material->refractiveIndex = 1.2f;
		material->specularReflectivity = 1.0f;
		material->diffuseReflectivity = 0.0f;
		material->specularDiffusion = 1.9f;
		materials.push_back(material);
	}
}

void Object::BunnyMaterial() {
	for (int i = 0; i < this->mesh_size; i++) {
		Material *material = Material::White();
		material->specularColor = glm::vec3(0.7, 0.8, 0.7);
		material->diffuseColor = material->specularColor;
		material->emissivity = 0.0f;
		material->specularReflectivity = 0.6f;
		material->diffuseReflectivity = 0.4f;
		material->specularDiffusion = 9.4f;
		materials.push_back(material);
	}
}

void Object::CornellMaterial() {
	
	materials.push_back(Material::Green()); // Green wall.
	materials.push_back(Material::White()); // Floor.
	materials.push_back(Material::White()); // Roof.
	materials.push_back(Material::Red()); // Red wall.
	materials.push_back(Material::White()); // White wall.
	materials.push_back(Material::White()); // Left box.
	materials.push_back(Material::White()); // Right box.

}

void Object::DragonMaterial() {
	for (int i = 0; i < this->mesh_size; i++) {
		Material *material = Material::White();
		material->specularColor = glm::vec3(1.0, 0.8, 0.6);
		material->diffuseColor = material->specularColor;
		material->emissivity = 0.0f;
		material->specularReflectivity = 1.0f;
		material->diffuseReflectivity = 0.0f;
		material->specularDiffusion = 2.2f;
		materials.push_back(material);
	}
}

void Object::SusanneMaterial() {
	for (int i = 0; i < this->mesh_size; i++) {
		Material *material = Material::White();
		material->specularColor = glm::vec3(0.2, 0.8, 1.0);
		material->diffuseColor = material->specularColor;
		material->emissivity = 0.0f;
		material->specularReflectivity = 1.0f;
		material->diffuseReflectivity = 0.0f;
		material->specularDiffusion = 3.2f;
		material->transparency = 1.0f;
		materials.push_back(material);
	}
}

void Object::TeapotMaterial() {
	for (int i = 0; i < this->mesh_size; i++) {
		Material *material = Material::White();
		material->specularColor = glm::vec3(1.0, 0.8, 0.6);
		material->diffuseColor = material->specularColor;
		material->emissivity = 0.0f;
		material->specularReflectivity = 1.0f;
		material->diffuseReflectivity = 0.0f;
		material->specularDiffusion = 2.2f;
		materials.push_back(material);
	}
}

/*Uploading rendering settings to shader*/
void Object::Render(GLuint program) {
	for (int i = 0; i < this->mesh_size; i++) {
		if (this->has_material) {
			materials[i]->BindMaterialToProgram(program);
		}		
		meshes[i].BindMeshToProgram(program);
		
	}
}